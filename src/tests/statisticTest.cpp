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
    bool expected_independence;
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

    // Check if p_value indicates independence or dependence
    if (testCase.expected_independence) {
        EXPECT_GE(p_value, 0.05);
    }
    else {
        EXPECT_LT(p_value, 0.05);
    }
}

INSTANTIATE_TEST_SUITE_P(
    StatisticTest,
    StatisticTest,
    ::testing::Values(
        // Test case 1: Basic example with a small dataset and a conditioning set
        TestCase{ { { 1, 2.1, 3.2, 4.3, 5.4 }, { 2, 4.1, 6.2, 8.3, 10.4 }, { 1.5, 2.1, 1.6, 2.2, 1.7 } }, 0, 1, { 2 }, false },
        // Test case 2: Another small dataset with no conditioning set
        TestCase{ { { 1, 2.01, 3.02 }, { 2, 4.01, 6.02 }, { 1.1, 2.02, 3.04 } }, 0, 1, {}, true },
        // Test case 3: Testing with a constant column
        TestCase{ { { 1, 1, 1, 1, 1 }, { 2, 3, 4, 5, 6 }, { 1.1, 2.1, 3.1, 4.1, 5.1 } }, 0, 1, {}, true },
        // Test case 4: Moderate dataset with conditioning set
        TestCase{ { { 1, 2, 3, 4, 5 }, { 2, 4, 6, 8, 10 }, { 3, 4, 5, 6, 7 } }, 0, 1, { 2 }, true },
        // Test case 5: Corrected correlation without conditioning set
        TestCase{ { { 1, 2, 3, 4, 5 }, { 2, 4, 6, 8, 10 }, { 3, 4, 5, 6, 7 } }, 0, 1, {}, false },
        // Test case 6: Perfect correlation with conditioning set
        TestCase{ { { 1, 2, 3, 4, 5 }, { 1, 2, 3, 4, 5 }, { 1, 2, 3, 4, 5 } }, 0, 1, { 2 }, false },
        // Test case 7: Removed constant column from conditioning set
        TestCase{ { { 1, 2, 3, 4, 5 }, { 5, 4, 3, 2, 1 }, { 1, 2, 3, 4, 5 } }, 0, 1, {}, false },
        // Test case 8: No constant columns
        TestCase{ { { 1, 2, 3, 4, 5 }, { 2, 4, 6, 8, 10 }, { 3, 4, 5, 6, 7 } }, 0, 1, { 2 }, false },
        // Test case 9: Different columns with corrected conditioning set
        TestCase{ { { 1, 2, 3, 4, 5 }, { 2, 4, 6, 8, 10 }, { 1, 2, 3, 4, 5 } }, 0, 2, { 1 }, false },
        // Test case 10: Fixed data for large dataset
        TestCase{
            {
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 },  // Variable 0
                { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 },  // Variable 1
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 },  // Variable 6
                { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 },  // Variable 8
                { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },  // Variable 9
                { 1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38 },  // Variable 10
                { 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0 },  // Variable 11
                { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 },  // Variable 15
                { 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48 },  // Variable 16
                { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 },  // Variable 17
                { 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }   // Variable 18
            },
            0, 1, { 6, 8, 9, 10, 11, 15, 16, 17, 18 }, false }
    )
);
