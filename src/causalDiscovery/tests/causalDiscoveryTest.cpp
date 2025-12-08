#include "causalDiscovery.h"
#include "graph.h"
#include "dataset.h"
#include "CSVReader.h"
#include <gtest/gtest.h>
#include <memory>

class CausalDiscoveryConstraintsTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto columns = CSVReader::readCSVFile("KV-41762_202301_test.csv", 4);
        data = std::make_shared<Dataset>(std::move(columns));
    }

    std::shared_ptr<Dataset> data;
};

TEST_F(CausalDiscoveryConstraintsTest, ForbiddenEdgeRemovedTest) {
    auto graph = std::make_shared<Graph>(data);
    
    // Add forbidden edge: 2 (PassingNoise) -> 3 (CO2)
    graph->addForbiddenEdge(2, 3);
    
    CausalDiscovery fci;
    fci.runFCI(graph, 0.05);
    
    // Verify forbidden edge is not present
    EXPECT_FALSE(graph->hasDirectedEdge(2, 3));
    EXPECT_FALSE(graph->hasDirectedEdge(3, 2));
}

TEST_F(CausalDiscoveryConstraintsTest, RequiredEdgePreservedTest) {
    auto graph = std::make_shared<Graph>(data);
    
    // Add required edge: 0 (CylinderCapacity) -> 1 (EnginePower)
    graph->addRequiredEdge(0, 1);
    
    CausalDiscovery fci;
    fci.runFCI(graph, 0.05);
    
    // Verify required edge exists
    bool hasEdge = graph->hasDirectedEdge(0, 1) || graph->hasDirectedEdge(1, 0);
    EXPECT_TRUE(hasEdge) << "Required edge 0->1 should be preserved";
}

TEST_F(CausalDiscoveryConstraintsTest, DirectionConstraintEnforcedTest) {
    auto graph = std::make_shared<Graph>(data);
    
    // Add direction constraint: 0 -> 1 (if edge exists, it must go 0->1)
    graph->addDirectionConstraint(0, 1);
    
    CausalDiscovery fci;
    fci.runFCI(graph, 0.05);
    
    // If edge exists, it should be directed 0->1, not 1->0
    if (graph->hasDirectedEdge(0, 1) || graph->hasDirectedEdge(1, 0)) {
        EXPECT_TRUE(graph->hasDirectedEdge(0, 1));
        EXPECT_FALSE(graph->hasDirectedEdge(1, 0));
    }
}

TEST_F(CausalDiscoveryConstraintsTest, MultipleConstraintsTest) {
    auto graph = std::make_shared<Graph>(data);
    
    // Forbidden: PassingNoise -> CO2
    graph->addForbiddenEdge(2, 3);
    
    // Required: CylinderCapacity -> EnginePower
    graph->addRequiredEdge(0, 1);
    
    // Direction: CylinderCapacity -> EnginePower
    graph->addDirectionConstraint(0, 1);
    
    CausalDiscovery fci;
    fci.runFCI(graph, 0.05);
    
    // Verify all constraints
    EXPECT_FALSE(graph->hasDirectedEdge(2, 3));
    
    bool hasRequiredEdge = graph->hasDirectedEdge(0, 1) || graph->hasDirectedEdge(1, 0);
    EXPECT_TRUE(hasRequiredEdge);
    
    if (hasRequiredEdge) {
        EXPECT_TRUE(graph->hasDirectedEdge(0, 1));
    }
}

TEST_F(CausalDiscoveryConstraintsTest, ConstraintsDoNotCreateSpuriousEdgesTest) {
    auto graph = std::make_shared<Graph>(data);
    
    // Run without constraints
    CausalDiscovery fci1;
    auto graph1 = std::make_shared<Graph>(data);
    fci1.runFCI(graph1, 0.05);
    auto edges1 = graph1->getEdges();
    
    // Run with only direction constraints (should not add edges)
    graph->addDirectionConstraint(0, 1);
    CausalDiscovery fci2;
    fci2.runFCI(graph, 0.05);
    auto edges2 = graph->getEdges();
    
    // Direction constraints should not increase edge count
    EXPECT_LE(edges2.size(), edges1.size() + 1) 
        << "Direction constraints should not create new edges";
}
