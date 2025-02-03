#include "Ontology.h"
#include "OntologyXMLReader.h"
#include <gtest/gtest.h>
#include <fstream>

/*
---------------------------------
|          Ontology             |
---------------------------------

Classes:
    - Vehicle
    - Emission

Object Properties:
    - hasEmission (Vehicle → Emission)

Data Properties:
    - co_v_1 (Emission → float)

Individuals:
    - Car1 (Type: Vehicle)
        ├── hasEmission → Em1 (Type: Emission)

    - Em1 (Type: Emission)
        ├── co_v_1 → 0.3 (float)

Graph Representation:

         +-------------+                      +-------------+
         |   Vehicle   |                      |  Emission   |
         +-------------+                      +-------------+
                 |                                    |
                 | hasEmission                       | co_v_1 (0.3)
                 ↓                                    ↓
         +-------------+                      +-------------+
         |   Car1      | -----> (Em1) -------> |    Em1     |
         +-------------+                      +-------------+

Legend:
    - Classes: Rectangles [Vehicle], [Emission]
    - Object Properties: "hasEmission" (links Car1 → Em1)
    - Data Properties: "co_v_1" (stores 0.3 in Em1)

*/

class OntologyXMLReaderTest : public ::testing::Test {
protected:
    std::string testFilePath = "test_ontology.rdf";

    void SetUp() override {
        std::ofstream file(testFilePath);
        ASSERT_TRUE(file.is_open());

        file << R"(<?xml version="1.0"?>
        <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
                 xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
                 xmlns:owl="http://www.w3.org/2002/07/owl#"
                 xmlns="http://www.semanticweb.org/ontology-hun#">

            <!-- Classes -->
            <owl:Class rdf:about="#Vehicle"/>
            <owl:Class rdf:about="#Emission"/>

            <!-- Object Properties -->
            <owl:ObjectProperty rdf:about="#hasEmission">
                <rdfs:domain rdf:resource="#Vehicle"/>
                <rdfs:range rdf:resource="#Emission"/>
            </owl:ObjectProperty>

            <!-- Data Properties -->
            <owl:DatatypeProperty rdf:about="#co_v_1">
                <rdfs:domain rdf:resource="#Emission"/>
                <rdfs:range rdf:resource="http://www.w3.org/2001/XMLSchema#float"/>
            </owl:DatatypeProperty>

            <!-- Individuals -->
            <owl:NamedIndividual rdf:about="#Car1">
                <rdf:type rdf:resource="#Vehicle"/>
                <hasEmission rdf:resource="#Em1"/>
            </owl:NamedIndividual>

            <owl:NamedIndividual rdf:about="#Em1">
                <rdf:type rdf:resource="#Emission"/>
                <co_v_1>0.3</co_v_1>
            </owl:NamedIndividual>

        </rdf:RDF>)";

        file.close();
    }

    void TearDown() override {
        remove(testFilePath.c_str());
    }
};

TEST_F(OntologyXMLReaderTest, LoadOntologyFromRDF) {
    Ontology ontology;
    bool success = OntologyXMLReader::loadFromRDF(ontology, "test_ontology.rdf");
    ASSERT_TRUE(success);

    // Check Classes
    EXPECT_NE(ontology.getClassType("Vehicle"), nullptr);
    EXPECT_NE(ontology.getClassType("Emission"), nullptr);

    // Check Properties
    EXPECT_FALSE(ontology.getProperties("hasEmission").empty());
    EXPECT_FALSE(ontology.getProperties("co_v_1").empty());

    // Check Individuals
    EXPECT_NE(ontology.getIndividual("Car1"), nullptr);
    EXPECT_NE(ontology.getIndividual("Em1"), nullptr);

    // Check Object Properties
    // Assuming `getObjectPropertyValue` is implemented
    // EXPECT_EQ(ontology.getIndividual("Car1")->getObjectPropertyValue("hasEmission"), "Em1");

    // Check Data Properties
    // Assuming `getDataPropertyValue` is implemented
    // EXPECT_EQ(ontology.getIndividual("Em1")->getDataPropertyValue("co_v_1"), "0.3");
}
