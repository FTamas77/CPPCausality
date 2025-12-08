#include "graph.h"
#include "dataset.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

class GraphConstraintsTest : public ::testing::Test {
protected:
    std::shared_ptr<Dataset> createTestDataset() {
        std::vector<std::vector<double>> columns = {
            {1.0, 2.0, 3.0, 4.0},
            {2.0, 3.0, 4.0, 5.0},
            {3.0, 4.0, 5.0, 6.0},
            {4.0, 5.0, 6.0, 7.0}
        };
        return std::make_shared<Dataset>(std::move(columns));
    }
};

TEST_F(GraphConstraintsTest, ForbiddenEdgeTest) {
    auto dataset = createTestDataset();
    auto graph = std::make_shared<Graph>(dataset);

    graph->addForbiddenEdge(0, 1);
    graph->addForbiddenEdge(2, 3);

    EXPECT_TRUE(graph->isForbiddenEdge(0, 1));
    EXPECT_TRUE(graph->isForbiddenEdge(2, 3));
    EXPECT_FALSE(graph->isForbiddenEdge(1, 2));

    const auto& forbiddenEdges = graph->getForbiddenEdges();
    EXPECT_EQ(forbiddenEdges.size(), 2);
}

TEST_F(GraphConstraintsTest, RequiredEdgeTest) {
    auto dataset = createTestDataset();
    auto graph = std::make_shared<Graph>(dataset);

    graph->addRequiredEdge(0, 1);
    graph->addRequiredEdge(1, 2);

    EXPECT_TRUE(graph->isRequiredEdge(0, 1));
    EXPECT_TRUE(graph->isRequiredEdge(1, 2));
    EXPECT_FALSE(graph->isRequiredEdge(2, 3));

    const auto& requiredEdges = graph->getRequiredEdges();
    EXPECT_EQ(requiredEdges.size(), 2);
}

TEST_F(GraphConstraintsTest, DirectionConstraintTest) {
    auto dataset = createTestDataset();
    auto graph = std::make_shared<Graph>(dataset);

    graph->addDirectionConstraint(0, 1);
    graph->addDirectionConstraint(2, 3);

    EXPECT_TRUE(graph->hasDirectionConstraint(0, 1));
    EXPECT_TRUE(graph->hasDirectionConstraint(2, 3));
    EXPECT_FALSE(graph->hasDirectionConstraint(1, 0));

    const auto& constraints = graph->getDirectionConstraints();
    EXPECT_EQ(constraints.size(), 2);
}

TEST_F(GraphConstraintsTest, MultipleConstraintsTest) {
    auto dataset = createTestDataset();
    auto graph = std::make_shared<Graph>(dataset);

    // Add forbidden edge
    graph->addForbiddenEdge(0, 3);
    
    // Add required edge
    graph->addRequiredEdge(0, 1);
    
    // Add direction constraint
    graph->addDirectionConstraint(1, 2);

    EXPECT_TRUE(graph->isForbiddenEdge(0, 3));
    EXPECT_TRUE(graph->isRequiredEdge(0, 1));
    EXPECT_TRUE(graph->hasDirectionConstraint(1, 2));

    EXPECT_FALSE(graph->isForbiddenEdge(0, 1));
    EXPECT_FALSE(graph->isRequiredEdge(0, 3));
    EXPECT_FALSE(graph->hasDirectionConstraint(0, 1));
}
