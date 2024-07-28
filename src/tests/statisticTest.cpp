#include "statistic.h"
#include "dataset.h"
#include <gtest/gtest.h>
#include <vector>
#include <set>
#include <memory>

using namespace std;

struct TestCase {
    vector<vector<double>> data;
    int i;
    int j;
    set<int> conditioningSet;
};

class StatisticTest : public ::testing::TestWithParam<TestCase> {
protected:
    shared_ptr<Dataset> createDataset(const vector<vector<double>>& data) {
        vector<Column> columns;
        for (const auto& col : data) {
            columns.push_back(col);
        }
        return make_shared<Dataset>(std::move(columns));
    }
};

TEST_P(StatisticTest, ConditionalIndependenceTests) {
    auto testCase = GetParam();
    auto data = createDataset(testCase.data);
    Statistic statistic;

    double p_value = statistic.testConditionalIndependence(data, testCase.i, testCase.j, testCase.conditioningSet);

    // Output p_value for debugging purposes
    std::cout << "p_value: " << p_value << std::endl;

    // General checks for valid p_value range
    ASSERT_GE(p_value, 0.0);
    ASSERT_LE(p_value, 1.0);

    // Example specific assertions for illustrative purposes
    // Adjust these expectations based on realistic behavior of your function
    if (testCase.i == 0 && testCase.j == 1) {
        if (testCase.conditioningSet.empty()) {
            if (testCase.data[0] == vector<double>{1, 2.01, 3.02}) {
                EXPECT_GT(p_value, 0.05); // Adjust this based on expected output
            }
        }
    }
}

INSTANTIATE_TEST_SUITE_P(
    StatisticTest,
    StatisticTest,
    ::testing::Values(
        TestCase{ { { 1, 2.1, 3.2, 4.3, 5.4 }, { 2, 4.1, 6.2, 8.3, 10.4 }, { 1.5, 2.1, 1.6, 2.2, 1.7 } }, 0, 1, { 2 } },
        TestCase{ { { 1, 2.01, 3.02 }, { 2, 4.01, 6.02 }, { 1.1, 2.02, 3.04 } }, 0, 1, {} },
        TestCase{ { { 1, 1, 1, 1, 1 }, { 2, 3, 4, 5, 6 }, { 1.1, 2.1, 3.1, 4.1, 5.1 } }, 0, 1, {} },
        TestCase{ { { 1, 2, 3, 4, 5 }, { 2, 4, 6, 8, 10 }, { 3, 4, 5, 6, 7 } }, 0, 1, { 2 } },
        TestCase{ { { 1, 2, 3, 4, 5 }, { 5, 4, 3, 2, 1 }, { 1, 2, 3, 4, 5 } }, 0, 1, { 2 } }
    )
);
