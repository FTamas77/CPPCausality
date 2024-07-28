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
    CausalDiscoveryTest()
    {
    }

    ~CausalDiscoveryTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(CausalDiscoveryTest, SmokeTestFCIWithLargerDataset)
{
    auto data = std::make_shared<Dataset>(std::vector<Column>{
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20},  // Variable 0
        { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 },  // Variable 1 (moderately correlated with Variable 0)
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 },  // Variable 6 (similar pattern to Variable 0)
        { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 },  // Variable 8 (moderate correlation with Variable 0)
        { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },  // Variable 9 (similar pattern to Variable 8)
        { 1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38 },  // Variable 10 (linear pattern, less correlated with Variables 0 and 1)
        { 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0 },  // Variable 11 (inverse pattern with Variable 10)
        { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 },  // Variable 15 (linear pattern similar to Variables 0 and 1)
        { 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48 },  // Variable 16 (linear pattern with step size)
        { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 },  // Variable 17 (similar to Variable 6)
        { 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },  // Variable 18 (inverse pattern similar to Variable 7)
        { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 }  // Variable 19 (evenly spaced)
    });

    auto graph = std::make_shared<Graph>(data);

    // Significance level
    double alpha = 0.05;

    CausalDiscovery fci;
    fci.runFCI(graph, alpha);

    auto expectedGraph = std::make_shared<Graph>(data);
    expectedGraph->addDirectedEdge(0, 1);  // Variable 0 influences Variable 1
    expectedGraph->addUndirectedEdge(0, 6);  // Variable 0 and Variable 6 are similar
    expectedGraph->addDirectedEdge(0, 8);  // Variable 0 influences Variable 8
    expectedGraph->addUndirectedEdge(8, 9);  // Variable 8 and Variable 9 are similar
    expectedGraph->addDirectedEdge(10, 11);  // Variable 10 influences Variable 11 inversely
    expectedGraph->addUndirectedEdge(0, 15);  // Variable 0 and Variable 15 are similar
    expectedGraph->addDirectedEdge(10, 16);  // Variable 10 influences Variable 16
    expectedGraph->addDirectedEdge(0, 17);  // Variable 0 influences Variable 17
    expectedGraph->addDirectedEdge(7, 18);  // Variable 7 influences Variable 18 inversely
    expectedGraph->addDirectedEdge(10, 19);  // Variable 10 influences Variable 19


    ASSERT_EQ(graph, expectedGraph) << "FCI algorithm should produce the expected graph structure";
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
