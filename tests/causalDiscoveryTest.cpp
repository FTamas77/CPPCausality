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
    auto data = std::make_shared<Dataset>(std::initializer_list<Column>{
        {1, 2, 3, 4, 5},   // Variable 0
        { 2, 3, 4, 5, 6 },   // Variable 1 (correlated with Variable 0)
        { 1, 1, 2, 2, 3 },   // Variable 2 (correlated with Variable 3)
        { 2, 2, 3, 3, 4 },   // Variable 3 (correlated with Variable 2)
        { 5, 4, 3, 2, 1 }    // Variable 4 (negatively correlated with Variable 0)
    });


    auto graph = std::make_shared<Graph>(data);

    // Significance level
    double alpha = 0.05;

    CausalDiscovery fci;
    fci.runFCI(graph, alpha);

    auto expectedGraph = std::make_shared<Graph>(data);
    expectedGraph->addUndirectedEdge(0, 1);
    expectedGraph->addUndirectedEdge(2, 3);
    expectedGraph->addUndirectedEdge(0, 4);

    ASSERT_EQ(graph, expectedGraph) << "FCI algorithm should produce the expected graph structure";

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
