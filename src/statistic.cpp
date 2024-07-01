#include "statistic.h"
#include "dataset.h"
#include <vector>
#include <memory>
#include <numeric>
#include <cmath>
#include <boost/math/distributions/students_t.hpp>

/*
double Statistic::performStatisticalTest(const std::shared_ptr<Column> data_i,
                                         const std::shared_ptr<Column> data_j,
                                         const std::vector<std::shared_ptr<Column>> data_conditioningSet)
{
    // Compute the means of the two samples
    double mean1 = std::accumulate(data_i->begin(), data_i->end(), 0.0) / data_i->size();
    double mean2 = std::accumulate(data_j->begin(), data_j->end(), 0.0) / data_j->size();

    // Compute the variances of the two samples
    double variance1 = std::inner_product(data_i->begin(), data_i->end(), data_i->begin(), 0.0) / data_i->size() - mean1 * mean1;
    double variance2 = std::inner_product(data_j->begin(), data_j->end(), data_j->begin(), 0.0) / data_j->size() - mean2 * mean2;

    // Compute the pooled standard deviation
    double pooledStdDev = std::sqrt((variance1 / data_i->size() + variance2 / data_j->size()) / 2);

    // Compute the t statistic
    double t = (mean1 - mean2) / (pooledStdDev * std::sqrt(1.0 / data_i->size() + 1.0 / data_j->size()));

    // Compute the degrees of freedom
    double degreesOfFreedom = data_i->size() + data_j->size() - 2;

    // Create a Students t distribution
    boost::math::students_t dist(degreesOfFreedom);

    // Compute the p-value
    double p = 2 * (1 - boost::math::cdf(dist, std::abs(t)));

    return p;
}
*/

double Statistic::performStatisticalTest(const Dataset &data, int i, int j, const std::set<int> &conditioningSet)
{
    // This function should implement the statistical test using the indices of the columns.
    // Retrieve column data based on indices and perform the test.
    std::shared_ptr<Column> data_i = data.getColumn(i);
    std::shared_ptr<Column> data_j = data.getColumn(j);

    std::vector<std::shared_ptr<Column>> data_conditioningSet;
    for (int k : conditioningSet)
    {
        std::shared_ptr<Column> column_k = data.getColumn(k);
        if (column_k)
        {
            data_conditioningSet.push_back(column_k);
        }
        else
        {
            throw std::runtime_error("Invalid column data");
        }
    }

    // Perform the actual statistical test with data_i, data_j, and data_conditioningSet
    // This is a placeholder. Replace it with actual test logic.
    return 0.05; // Placeholder p-value
}