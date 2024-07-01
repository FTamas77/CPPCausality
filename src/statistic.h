#include "dataset.h"
#include <vector>
#include <set>
#include <memory>

class Statistic
{
public:
    static double performStatisticalTest(const Dataset &data, int i, int j, const std::set<int> &conditioningSet);

    /*static double performStatisticalTest(const std::shared_ptr<Column> data_i,
                                                    const std::shared_ptr<Column> data_j,
                                                    const std::vector<std::shared_ptr<Column>> data_conditioningSet);*/
};