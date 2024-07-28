#include "statistic.h"
#include "dataset.h"
#include <boost/math/distributions/students_t.hpp>
#include <Eigen/Dense>
#include <Eigen/QR>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>

using namespace Eigen;
using namespace std;

double Statistic::testConditionalIndependence(const shared_ptr<const Dataset>& data, int i, int j, const set<int>& conditioningSet) {
    // Retrieve data columns for the variables i and j
    shared_ptr<Column> data_i = data->getColumn(i);
    shared_ptr<Column> data_j = data->getColumn(j);

    // Check if data columns are valid
    if (!data_i || !data_j) {
        throw runtime_error("Invalid column data.");
    }

    // Convert data columns to std::vector for easier manipulation
    vector<double> col_i(data_i->begin(), data_i->end());
    vector<double> col_j(data_j->begin(), data_j->end());

    // Check if either variable is constant
    if (all_of(col_i.begin(), col_i.end(), [&](double val) { return val == col_i[0]; }) ||
        all_of(col_j.begin(), col_j.end(), [&](double val) { return val == col_j[0]; })) {
        cerr << "One of the variables is constant: " << (all_of(col_i.begin(), col_i.end(), [&](double val) { return val == col_i[0]; }) ? "i" : "j") << endl;
        return 1.0; // Return a high p-value indicating non-independence
    }

    size_t num_rows = col_i.size();
    size_t num_conditioning_cols = conditioningSet.size();

    // Handle the case where there are no conditioning variables
    if (num_conditioning_cols == 0) {
        // Map std::vector to Eigen's VectorXd
        VectorXd vec_i = Eigen::Map<VectorXd>(col_i.data(), col_i.size());
        VectorXd vec_j = Eigen::Map<VectorXd>(col_j.data(), col_j.size());

        // Calculate means and standard deviations
        double mean_i = vec_i.mean();
        double mean_j = vec_j.mean();
        double stdev_i = sqrt((vec_i.array() - mean_i).square().sum() / (vec_i.size() - 1));
        double stdev_j = sqrt((vec_j.array() - mean_j).square().sum() / (vec_j.size() - 1));

        // Ensure standard deviations are not zero to avoid division by zero
        if (stdev_i == 0 || stdev_j == 0) {
            return 1.0; // Return a high p-value indicating non-independence
        }

        // Calculate the correlation
        double corr = (vec_i.dot(vec_j) - num_rows * mean_i * mean_j) / ((num_rows - 1) * stdev_i * stdev_j);

        // Check if correlation is NaN or Inf
        if (std::isnan(corr) || std::isinf(corr)) {
            return 1.0; // Return a high p-value indicating non-independence
        }

        // Calculate the t-statistic
        double t_statistic = corr * sqrt((num_rows - 2) / (1 - corr * corr));

        // Check for NaN or Inf in t-statistic
        if (std::isnan(t_statistic) || std::isinf(t_statistic)) {
            return 1.0; // Return a high p-value indicating non-independence
        }

        // Calculate the p-value using Student's t-distribution
        boost::math::students_t dist(num_rows - 2);
        double p_value = 2 * boost::math::cdf(boost::math::complement(dist, abs(t_statistic)));

        // Ensure p_value is within valid range
        if (std::isinf(p_value) || std::isnan(p_value)) {
            return 1.0; // Return a high p-value indicating non-independence
        }

        return p_value;
    }

    // Ensure there are enough rows to perform the analysis
    if (num_rows <= num_conditioning_cols + 2) {
        throw runtime_error("Not enough rows to form a valid X matrix.");
    }

    // Create the design matrix for the conditioning set
    MatrixXd designMatrix(num_rows, num_conditioning_cols + 2);
    designMatrix.col(0) = Eigen::Map<VectorXd>(col_i.data(), col_i.size());
    designMatrix.col(1) = Eigen::Map<VectorXd>(col_j.data(), col_j.size());

    size_t colIndex = 2;
    for (int k : conditioningSet) {
        shared_ptr<Column> column_k = data->getColumn(k);
        if (!column_k) {
            throw runtime_error("Invalid column data.");
        }
        vector<double> col_k(column_k->begin(), column_k->end());

        // Check if the column is constant
        if (all_of(col_k.begin(), col_k.end(), [&](double val) { return val == col_k[0]; })) {
            cerr << "Conditioning set contains a constant column: " << k << endl;
            return 1.0; // Return a high p-value indicating non-independence
        }

        designMatrix.col(colIndex) = Eigen::Map<VectorXd>(col_k.data(), col_k.size());
        colIndex++;
    }

    // Extract X and y matrices
    MatrixXd X = designMatrix.block(0, 2, num_rows, num_conditioning_cols);
    VectorXd y_i = designMatrix.col(0);
    VectorXd y_j = designMatrix.col(1);

    // Ensure X and y_i, y_j are valid
    if (X.rows() == 0 || X.cols() == 0 || y_i.size() == 0 || y_j.size() == 0) {
        throw runtime_error("One or more matrices/vectors are empty.");
    }

    // Compute beta coefficients using QR decomposition
    VectorXd beta_i = X.colPivHouseholderQr().solve(y_i);
    VectorXd beta_j = X.colPivHouseholderQr().solve(y_j);

    // Compute residuals
    VectorXd residuals_i = y_i - X * beta_i;
    VectorXd residuals_j = y_j - X * beta_j;

    // Calculate the correlation between residuals
    double residual_corr = residuals_i.dot(residuals_j) / (sqrt(residuals_i.squaredNorm()) * sqrt(residuals_j.squaredNorm()));

    // Check for near ±1 correlations
    if (abs(residual_corr) >= 1.0 - numeric_limits<double>::epsilon()) {
        cerr << "Residuals correlation too close to ±1: " << residual_corr << endl;
        return 1.0; // Return a high p-value indicating non-independence
    }

    // Convert sizes to integers for calculation
    int n = static_cast<int>(num_rows);  // Explicit conversion
    int k = static_cast<int>(num_conditioning_cols);  // Explicit conversion

    // Calculate the t-statistic
    double t_statistic = residual_corr * sqrt((n - k - 2) / (1 - residual_corr * residual_corr));

    // Check for NaN or Inf in t-statistic
    if (std::isnan(t_statistic) || std::isinf(t_statistic)) {
        return 1.0; // Return a high p-value indicating non-independence
    }

    // Calculate the p-value using Student's t-distribution
    boost::math::students_t dist(n - k - 2);
    double p_value = 2 * boost::math::cdf(boost::math::complement(dist, abs(t_statistic)));

    // Ensure p_value is within valid range
    if (std::isinf(p_value) || std::isnan(p_value)) {
        cerr << "Invalid p-value computed: " << p_value << endl;
        return 1.0; // Return a high p-value indicating non-independence
    }

    return p_value;
}
