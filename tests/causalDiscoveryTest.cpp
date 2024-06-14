#include <gtest/gtest.h>
#include "graph.h"
#include "dataset.h"
#include "dataLoader.h"
#include "causalDiscovery.h"
#include <stdexcept>
#include <memory>

class CausalDiscoveryTest : public ::testing::Test
{
protected:
    // You can remove any or all of the following functions if its body is empty.

    CausalDiscoveryTest()
    {
        // You can do set-up work for each test here.
    }

    ~CausalDiscoveryTest() override
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Class members declared here can be used by all tests in the test suite for CausalDiscoveryTest.
};

TEST_F(CausalDiscoveryTest, TestFCI)
{
    Dataset data;
    DataLoader loader;

    // Load the data from the CSV file
    try
    {
        loader.loadCSV("./data.csv", data);
    }
    catch (const std::exception& e)
    {
        FAIL() << "Failed to load data: " << e.what();
    }

    // Set your significance level
    double alpha = 0.05;

    // Run the FCI algorithm
    CausalDiscovery fci;
    auto result = fci.FCI(data, alpha);

    result->printGraph();

    // Check the output
    // This will depend on what output you expect from your FCI algorithm
    // Replace with your actual expected output
    std::unique_ptr<Graph> expectedOutput = std::make_unique<Graph>();
    // Set up the expected graph structure here...

    ASSERT_EQ(*result, *expectedOutput);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
