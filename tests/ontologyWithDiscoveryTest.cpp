#include "ontology.h"
#include "individual.h"
#include "classType.h"
#include "property.h"
#include "graph.h"
#include "dataset.h"
#include "causalDiscovery.h"
#include "CSVReader.h"
#include "ontologyConstraintsHandler.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <vector>

class EmissionOntologyTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    std::shared_ptr<Ontology> createFullOntology() {
        auto ontology = std::make_shared<Ontology>();

        // All possible features
        auto hengerurtartalomClass = std::make_shared<ClassType>("0");  // CylinderCapacity
        auto teljesitmenyClass = std::make_shared<ClassType>("1");      // EnginePower
        auto elhaladasiZajClass = std::make_shared<ClassType>("2");     // PassingNoise
        auto co2KibocsatasClass = std::make_shared<ClassType>("3");     // CO2Emissions

        ontology->addClassType(hengerurtartalomClass);
        ontology->addClassType(teljesitmenyClass);
        ontology->addClassType(elhaladasiZajClass);
        ontology->addClassType(co2KibocsatasClass);

        // Forbidden: PassingNoise -> CO2
        auto cannotCauseProperty = std::make_shared<ObjectProperty>(
            "cannotCause", elhaladasiZajClass, co2KibocsatasClass);
        ontology->addProperty(cannotCauseProperty);

        // Required: CylinderCapacity -> EnginePower
        auto mustCauseProperty = std::make_shared<ObjectProperty>(
            "mustCause", hengerurtartalomClass, teljesitmenyClass);
        ontology->addProperty(mustCauseProperty);

        // Direction: CylinderCapacity -> EnginePower
        auto directionProperty = std::make_shared<ObjectProperty>(
            "directionOnly", hengerurtartalomClass, teljesitmenyClass);
        ontology->addProperty(directionProperty);

        return ontology;
    }
};

TEST_F(EmissionOntologyTest, ForbiddenCauseTest) {
    auto columns = CSVReader::readCSVFile("KV-41762_202301_test.csv", 4);
    auto data = std::make_shared<Dataset>(std::move(columns));
    auto graph = std::make_shared<Graph>(data);

    auto ontology = createFullOntology();
    OntologyConstraintsHandler constraintsHandler("cannotCause");
    constraintsHandler.applyConstraints(ontology, graph);

    double alpha = 0.05;
    CausalDiscovery fci;
    fci.runFCI(graph, alpha);

    // Verify PassingNoise (2) does not cause CO2 (3)
    EXPECT_FALSE(graph->hasDirectedEdge(2, 3)) 
        << "Forbidden edge PassingNoise->CO2 should not exist";
}

TEST_F(EmissionOntologyTest, RequiredCauseTest) {
    auto columns = CSVReader::readCSVFile("KV-41762_202301_test.csv", 4);
    auto data = std::make_shared<Dataset>(std::move(columns));
    auto graph = std::make_shared<Graph>(data);

    auto ontology = createFullOntology();
    OntologyConstraintsHandler constraintsHandler("", "mustCause");
    constraintsHandler.applyConstraints(ontology, graph);

    double alpha = 0.05;
    CausalDiscovery fci;
    fci.runFCI(graph, alpha);

    // Verify CylinderCapacity (0) -> EnginePower (1) exists
    bool hasEdge = graph->hasDirectedEdge(0, 1) || graph->hasDirectedEdge(1, 0);
    EXPECT_TRUE(hasEdge) 
        << "Required edge CylinderCapacity<->EnginePower should exist";
}

TEST_F(EmissionOntologyTest, DirectionConstraintTest) {
    auto columns = CSVReader::readCSVFile("KV-41762_202301_test.csv", 4);
    auto data = std::make_shared<Dataset>(std::move(columns));
    auto graph = std::make_shared<Graph>(data);

    auto ontology = createFullOntology();
    OntologyConstraintsHandler constraintsHandler("", "", "directionOnly");
    constraintsHandler.applyConstraints(ontology, graph);

    double alpha = 0.05;
    CausalDiscovery fci;
    fci.runFCI(graph, alpha);

    // If edge exists, verify direction: 0 -> 1
    if (graph->hasDirectedEdge(0, 1) || graph->hasDirectedEdge(1, 0)) {
        EXPECT_TRUE(graph->hasDirectedEdge(0, 1))
            << "Direction constraint: CylinderCapacity->EnginePower";
        EXPECT_FALSE(graph->hasDirectedEdge(1, 0))
            << "Reverse direction should not exist";
    }
}

TEST_F(EmissionOntologyTest, AllConstraintsIntegrationTest) {
    auto columns = CSVReader::readCSVFile("KV-41762_202301_test.csv", 4);
    auto data = std::make_shared<Dataset>(std::move(columns));
    auto graph = std::make_shared<Graph>(data);

    auto ontology = createFullOntology();
    OntologyConstraintsHandler constraintsHandler("cannotCause", "mustCause", "directionOnly");
    constraintsHandler.applyConstraints(ontology, graph);

    double alpha = 0.05;
    CausalDiscovery fci;
    fci.runFCI(graph, alpha);
    
    graph->printGraph();

    // Verify all constraints together
    EXPECT_FALSE(graph->hasDirectedEdge(2, 3)) 
        << "Forbidden: PassingNoise should not cause CO2";
    
    bool hasRequiredEdge = graph->hasDirectedEdge(0, 1) || graph->hasDirectedEdge(1, 0);
    EXPECT_TRUE(hasRequiredEdge) 
        << "Required: CylinderCapacity<->EnginePower should exist";
    
    if (hasRequiredEdge) {
        EXPECT_TRUE(graph->hasDirectedEdge(0, 1))
            << "Direction: CylinderCapacity->EnginePower";
    }
}

