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
    auto [col_i, col_j] = retrieveAndValidateData(data, i, j);
    size_t num_rows = col_i.size();
    size_t num_conditioning_cols = conditioningSet.size();

    if (num_conditioning_cols == 0) {
        return handleNoConditioning(col_i, col_j);
    }

    if (num_rows <= num_conditioning_cols + 2) {
        throw runtime_error("Not enough rows to form a valid X matrix.");
    }

    return handleConditioning(data, i, j, conditioningSet, col_i, col_j, num_rows, num_conditioning_cols);
}

// Retrieve data columns and validate them
pair<vector<double>, vector<double>> Statistic::retrieveAndValidateData(const shared_ptr<const Dataset>& data, int i, int j) {
    shared_ptr<Column> data_i = data->getColumn(i);
    shared_ptr<Column> data_j = data->getColumn(j);

    if (!data_i || !data_j) {
        throw runtime_error("Invalid column data.");
    }

    vector<double> col_i(data_i->begin(), data_i->end());
    vector<double> col_j(data_j->begin(), data_j->end());

    if (isConstant(col_i) || isConstant(col_j)) {
        cerr << "One of the variables is constant: " << (isConstant(col_i) ? "i" : "j") << endl;
    }

    return { col_i, col_j };
}

// Check if a vector is constant
bool Statistic::isConstant(const vector<double>& vec) {
    return all_of(vec.begin(), vec.end(), [&](double val) { return val == vec[0]; });
}

// Handle cases with no conditioning variables
double Statistic::handleNoConditioning(const vector<double>& col_i, const vector<double>& col_j) {
    size_t num_rows = col_i.size();
    VectorXd vec_i = Eigen::Map<const VectorXd>(col_i.data(), col_i.size());
    VectorXd vec_j = Eigen::Map<const VectorXd>(col_j.data(), col_j.size());

    double mean_i = vec_i.mean();
    double mean_j = vec_j.mean();
    double stdev_i = sqrt((vec_i.array() - mean_i).square().sum() / (vec_i.size() - 1));
    double stdev_j = sqrt((vec_j.array() - mean_j).square().sum() / (vec_j.size() - 1));

    if (stdev_i == 0 || stdev_j == 0) {
        return 1.0;
    }

    double corr = (vec_i.dot(vec_j) - num_rows * mean_i * mean_j) / ((num_rows - 1) * stdev_i * stdev_j);
    if (std::isnan(corr) || std::isinf(corr)) {
        return 1.0;
    }

    double t_statistic = corr * sqrt((num_rows - 2) / (1 - corr * corr));
    if (std::isnan(t_statistic) || std::isinf(t_statistic)) {
        return 1.0;
    }

    return computePValue(t_statistic, num_rows, 0);
}

// Handle cases with conditioning variables
double Statistic::handleConditioning(const shared_ptr<const Dataset>& data, int i, int j, const set<int>& conditioningSet, const vector<double>& col_i, const vector<double>& col_j, size_t num_rows, size_t num_conditioning_cols) {
    MatrixXd designMatrix(num_rows, num_conditioning_cols + 2);
    designMatrix.col(0) = Eigen::Map<const VectorXd>(col_i.data(), col_i.size());
    designMatrix.col(1) = Eigen::Map<const VectorXd>(col_j.data(), col_j.size());

    size_t colIndex = 2;
    for (int k : conditioningSet) {
        shared_ptr<Column> column_k = data->getColumn(k);
        if (!column_k) {
            throw runtime_error("Invalid column data.");
        }
        vector<double> col_k(column_k->begin(), column_k->end());

        if (isConstant(col_k)) {
            cerr << "Conditioning set contains a constant column: " << k << endl;
            return 1.0;
        }

        designMatrix.col(colIndex) = Eigen::Map<const VectorXd>(col_k.data(), col_k.size());
        colIndex++;
    }

    MatrixXd X = designMatrix.block(0, 2, num_rows, num_conditioning_cols);
    VectorXd y_i = designMatrix.col(0);
    VectorXd y_j = designMatrix.col(1);

    if (X.rows() == 0 || X.cols() == 0 || y_i.size() == 0 || y_j.size() == 0) {
        throw runtime_error("One or more matrices/vectors are empty.");
    }

    double residual_corr = computeResidualCorrelation(X, y_i, y_j);
    if (abs(residual_corr) >= 1.0 - numeric_limits<double>::epsilon()) {
        cerr << "Residuals correlation too close to ±1: " << residual_corr << endl;
        return 1.0;
    }

    double t_statistic = computeTStatistic(residual_corr, num_rows, num_conditioning_cols);
    if (std::isnan(t_statistic) || std::isinf(t_statistic)) {
        return 1.0;
    }

    return computePValue(t_statistic, num_rows, num_conditioning_cols);
}

// Compute the correlation between residuals
double Statistic::computeResidualCorrelation(const MatrixXd& X, const VectorXd& y_i, const VectorXd& y_j) {
    VectorXd beta_i = X.colPivHouseholderQr().solve(y_i);
    VectorXd beta_j = X.colPivHouseholderQr().solve(y_j);

    VectorXd residuals_i = y_i - X * beta_i;
    VectorXd residuals_j = y_j - X * beta_j;

    return residuals_i.dot(residuals_j) / (sqrt(residuals_i.squaredNorm()) * sqrt(residuals_j.squaredNorm()));
}

// Compute the t-statistic
double Statistic::computeTStatistic(double correlation, size_t num_rows, size_t num_conditioning_cols) {
    return correlation * sqrt((num_rows - num_conditioning_cols - 2) / (1 - correlation * correlation));
}

// Compute the p-value from the t-statistic
double Statistic::computePValue(double t_statistic, size_t num_rows, size_t num_conditioning_cols) {
    boost::math::students_t dist(num_rows - num_conditioning_cols - 2);
    double p_value = 2 * boost::math::cdf(boost::math::complement(dist, abs(t_statistic)));

    if (std::isinf(p_value) || std::isnan(p_value)) {
        cerr << "Invalid p-value computed: " << p_value << endl;
        return 1.0;
    }

    return p_value;
}
