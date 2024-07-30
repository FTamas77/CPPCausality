#ifndef STATISTIC_H
#define STATISTIC_H

#include "dataset.h"
#include <memory>
#include <set>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <Eigen/Dense>

class Statistic {
public:
    static double testConditionalIndependence(const std::shared_ptr<const Dataset>& data, int i, int j, const std::set<int>& conditioningSet);

private:
    template <typename M, typename V>
    static V solve(const M& mat, const V& vec);

    static Eigen::MatrixXd pseudoinverse(const Eigen::MatrixXd& X);

    static std::pair<std::vector<double>, std::vector<double>> retrieveAndValidateData(const std::shared_ptr<const Dataset>& data, int i, int j);

    static bool isConstant(const std::vector<double>& vec);

    static double handleNoConditioning(const std::vector<double>& col_i, const std::vector<double>& col_j);

    static double handleConditioning(const std::shared_ptr<const Dataset>& data, int i, int j, const std::set<int>& conditioningSet, const std::vector<double>& col_i, const std::vector<double>& col_j, size_t num_rows, size_t num_conditioning_cols);

    static double computeResidualCorrelation(const Eigen::MatrixXd& X, const Eigen::VectorXd& y_i, const Eigen::VectorXd& y_j);

    static double computeTStatistic(double correlation, size_t num_rows, size_t num_conditioning_cols);

    static double computePValue(double t_statistic, size_t num_rows, size_t num_conditioning_cols);
};

#endif // STATISTIC_H
