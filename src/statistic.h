#include "dataset.h"
#include <vector>
#include <set>
#include <memory>

class Statistic
{
public:
    static double performStatisticalTest(const std::shared_ptr<Column> data_i,
                                                    const std::shared_ptr<Column> data_j,
                                                    const std::vector<std::shared_ptr<Column>> data_conditioningSet);
};