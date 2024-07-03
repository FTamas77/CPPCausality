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

    // Function to compute the pseudoinverse of a matrix
    static Eigen::MatrixXd pseudoinverse(const Eigen::MatrixXd& X);
};

#endif // STATISTIC_H

