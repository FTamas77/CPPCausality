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

    std::shared_ptr<Ontology> createSampleOntology() {
        auto ontology = std::make_shared<Ontology>();

        // All possible features
        auto hengerurtartalomClass = std::make_shared<ClassType>("0");
        auto teljesitmenyClass = std::make_shared<ClassType>("1");
        auto elhaladasiZajClass = std::make_shared<ClassType>("2");
        auto co2KibocsatasClass = std::make_shared<ClassType>("3");

        ontology->addClassType(hengerurtartalomClass);
        ontology->addClassType(teljesitmenyClass);
        ontology->addClassType(elhaladasiZajClass);
        ontology->addClassType(co2KibocsatasClass);

        // This will be a forbidden relationship
        auto cannotCauseProperty = std::make_shared<ObjectProperty>("cannotCause", elhaladasiZajClass, co2KibocsatasClass);
        ontology->addProperty(cannotCauseProperty);

        return ontology;
    }
};

TEST_F(EmissionOntologyTest, SimpleForbiddenCauseTest) {
    auto columns = CSVReader::readCSVFile("KV-41762_202301_test.csv", 4);
    auto data = std::make_shared<Dataset>(std::move(columns));
    auto graph = std::make_shared<Graph>(data);

    // Apply ontology constraints to the graph
    auto ontology = createSampleOntology();
    OntologyConstraintsHandler constraintsHandler("cannotCause");
    constraintsHandler.applyConstraints(ontology, graph);

    double alpha = 0.05;
    CausalDiscovery fci;
    fci.runFCI(graph, alpha);
    graph->printGraph();

    //auto expectedGraph = std::make_shared<Graph>(data);
    //expectedGraph->addDirectedEdge(0, 1);
    //expectedGraph->addDirectedEdge(0, 2);
    //expectedGraph->addDirectedEdge(0, 3);
    //expectedGraph->addDirectedEdge(1, 3);
    //expectedGraph->addDirectedEdge(2, 3); // it is forbidden

    //ASSERT_EQ(graph, expectedGraph) << "FCI algorithm should produce the expected graph structure";
}

