#include "Ontology.h"
#include "Individual.h"
#include "ClassType.h"
#include "Property.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <vector>

class DataFromOntologyTest : public ::testing::Test {
protected:
    void SetUp() override {
        ontology = std::make_shared<Ontology>();

        personClass = std::make_shared<ClassType>("Person");
        organizationClass = std::make_shared<ClassType>("Organization");

        ontology->addClassType(personClass);
        ontology->addClassType(organizationClass);

        worksForProperty = std::make_shared<ObjectProperty>("worksFor", personClass, organizationClass);
        friendOfProperty = std::make_shared<ObjectProperty>("friendOf", personClass, personClass);

        ontology->addProperty(worksForProperty);
        ontology->addProperty(friendOfProperty);

        alice = std::make_shared<Individual>("Alice", personClass);
        bob = std::make_shared<Individual>("Bob", personClass);
        acmeCorp = std::make_shared<Individual>("AcmeCorp", organizationClass);

        ontology->addIndividual(alice);
        ontology->addIndividual(bob);
        ontology->addIndividual(acmeCorp);
    }

    void TearDown() override {
    }

    std::shared_ptr<Ontology> ontology;
    std::shared_ptr<ClassType> personClass;
    std::shared_ptr<ClassType> organizationClass;
    std::shared_ptr<ObjectProperty> worksForProperty;
    std::shared_ptr<ObjectProperty> friendOfProperty;
    std::shared_ptr<Individual> alice;
    std::shared_ptr<Individual> bob;
    std::shared_ptr<Individual> acmeCorp;
};

TEST_F(DataFromOntologyTest, ComplexOntologyTest) {

    // Set initial relationships
    ontology->setObjectProperty("Alice", "worksFor", "AcmeCorp");
    ontology->setObjectProperty("Alice", "friendOf", "Bob");

    // Retrieve and verify initial relationships
    auto retrievedAlice = ontology->getIndividual("Alice");
    ASSERT_NE(retrievedAlice, nullptr);

    auto aliceWorksFor = retrievedAlice->getObjectProperty("worksFor");
    ASSERT_NE(aliceWorksFor, nullptr);
    EXPECT_EQ(aliceWorksFor, acmeCorp);
    EXPECT_EQ(aliceWorksFor->getName(), "AcmeCorp");

    auto aliceFriendOf = retrievedAlice->getObjectProperty("friendOf");
    ASSERT_NE(aliceFriendOf, nullptr);
    EXPECT_EQ(aliceFriendOf->getName(), "Bob");

    // Now, modify the ontology: Add a new ClassType and Property
    auto projectClass = std::make_shared<ClassType>("Project");
    ontology->addClassType(projectClass);

    auto worksOnProperty = std::make_shared<ObjectProperty>("worksOn", personClass, projectClass);
    ontology->addProperty(worksOnProperty);

    // Add a new Individual and set a new relationship
    auto secretProject = std::make_shared<Individual>("SecretProject", projectClass);
    ontology->addIndividual(secretProject);

    ontology->setObjectProperty("Alice", "worksOn", "SecretProject");

    // Retrieve and verify the new relationship
    auto aliceWorksOn = retrievedAlice->getObjectProperty("worksOn");
    ASSERT_NE(aliceWorksOn, nullptr);
    EXPECT_EQ(aliceWorksOn->getName(), "SecretProject");

    // Add NewCorp as an individual
    auto newCorp = std::make_shared<Individual>("NewCorp", organizationClass);
    ontology->addIndividual(newCorp);

    // Now, change an existing relationship
    ontology->setObjectProperty("Alice", "worksFor", "NewCorp");

    aliceWorksFor = retrievedAlice->getObjectProperty("worksFor");
    ASSERT_NE(aliceWorksFor, nullptr);
    EXPECT_EQ(aliceWorksFor->getName(), "NewCorp");

    // Final verification of all relationships
    auto aliceFriendOfFinal = retrievedAlice->getObjectProperty("friendOf");
    ASSERT_NE(aliceFriendOfFinal, nullptr);
    EXPECT_EQ(aliceFriendOfFinal->getName(), "Bob");

    auto aliceWorksOnFinal = retrievedAlice->getObjectProperty("worksOn");
    ASSERT_NE(aliceWorksOnFinal, nullptr);
    EXPECT_EQ(aliceWorksOnFinal->getName(), "SecretProject");
}

