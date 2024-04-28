#include "statistic.h"
#include "dataset.h"
#include <vector>

double Statistic::performStatisticalTest(const std::shared_ptr<Column> data_i,
                                         const std::shared_ptr<Column> data_j,
                                         const std::vector<std::shared_ptr<Column>> data_conditioningSet)
{
    // Implement your statistical test here
    // This is just a placeholder
    double p_value = 0.0;

    // ...

    return p_value;
}