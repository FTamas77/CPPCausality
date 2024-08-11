#include "Ontology.h"
#include "Individual.h"
#include "ClassType.h"
#include "Property.h"
#include <gtest/gtest.h>
#include <vector>
#include <set>
#include <memory>

using namespace std;

// Helper function to create a ClassType for testing
shared_ptr<ClassType> createClassType(const string& name) {
    auto classType = make_shared<ClassType>(name);
    return classType;
}

// Test Case for adding and retrieving a ClassType
TEST(OntologyTest, AddAndRetrieveClassType) {
    Ontology ontology;

    // Add a class type
    string className = "Person";
    auto classType = createClassType(className);
    ontology.addClassType(classType);

    // Retrieve and check the class type
    auto retrievedClassType = ontology.getClassType(className);
    //ASSERT_NE(retrievedClassType, nullptr);
    EXPECT_EQ(retrievedClassType->getName(), className);
}

// Test Case for adding and retrieving an ObjectProperty
TEST(OntologyTest, AddAndRetrieveObjectProperty) {
    Ontology ontology;

    // Create class types for domain and range
    string className1 = "Person";
    string className2 = "Organization";
    auto classType1 = createClassType(className1);
    auto classType2 = createClassType(className2);
    ontology.addClassType(classType1);
    ontology.addClassType(classType2);

    // Add an object property
    string propertyName = "worksFor";
    auto objectProperty = make_shared<ObjectProperty>(propertyName, classType1, classType2);
    ontology.addProperty(objectProperty);

    // Retrieve and check the object property
    auto retrievedProperty = ontology.getProperty(propertyName);
    //ASSERT_NE(retrievedProperty, nullptr);
    auto retrievedObjectProperty = dynamic_pointer_cast<ObjectProperty>(retrievedProperty);
    //ASSERT_NE(retrievedObjectProperty, nullptr);
    EXPECT_EQ(retrievedObjectProperty->getDomain()->getName(), className1);
    EXPECT_EQ(retrievedObjectProperty->getRange()->getName(), className2);
}

// Test Case for adding and retrieving an Individual
TEST(OntologyTest, AddAndRetrieveIndividual) {
    Ontology ontology;

    // Create a class type
    string className = "Person";
    auto classType = createClassType(className);
    ontology.addClassType(classType);

    // Add an individual
    string individualName = "Alice";
    auto individual = make_shared<Individual>(individualName, classType);
    ontology.addIndividual(individual);

    // Retrieve and check the individual
    auto retrievedIndividual = ontology.getIndividual(individualName);
    //ASSERT_NE(retrievedIndividual, nullptr);
    EXPECT_EQ(retrievedIndividual->getName(), individualName);
    //EXPECT_EQ(retrievedIndividual->getClassType()->getName(), className);
}

// Test Case for setting and retrieving an ObjectProperty value for an Individual
TEST(OntologyTest, SetAndRetrieveObjectProperty) {
    Ontology ontology;

    // Create class types
    string className1 = "Person";
    string className2 = "Organization";
    auto classType1 = createClassType(className1);
    auto classType2 = createClassType(className2);
    ontology.addClassType(classType1);
    ontology.addClassType(classType2);

    // Add object property
    string propertyName = "worksFor";
    auto objectProperty = make_shared<ObjectProperty>(propertyName, classType1, classType2);
    ontology.addProperty(objectProperty);

    // Add individuals
    string individualName1 = "Alice";
    string individualName2 = "AcmeCorp";
    auto individual1 = make_shared<Individual>(individualName1, classType1);
    auto individual2 = make_shared<Individual>(individualName2, classType2);
    ontology.addIndividual(individual1);
    ontology.addIndividual(individual2);

    // Set object property
    ontology.setObjectProperty(individualName1, propertyName, individualName2);

    // Check if the property was set correctly
    auto retrievedIndividual = ontology.getIndividual(individualName1);
    //ASSERT_NE(retrievedIndividual, nullptr);
    auto retrievedObjectProperty = retrievedIndividual->getObjectProperty(propertyName);
    ASSERT_NE(retrievedObjectProperty, nullptr);
    //EXPECT_EQ(retrievedObjectProperty->getName(), individualName2);
}

// Test Case for setting and retrieving a DataProperty value for an Individual
TEST(OntologyTest, SetAndRetrieveDataProperty) {
    Ontology ontology;

    // Create class type
    string className = "Person";
    auto classType = createClassType(className);
    ontology.addClassType(classType);

    // Add data property
    string propertyName = "age";
    auto dataProperty = make_shared<DataProperty>(propertyName, classType, "int");
    ontology.addProperty(dataProperty);

    // Add individual
    string individualName = "Alice";
    auto individual = make_shared<Individual>(individualName, classType);
    ontology.addIndividual(individual);

    // Set data property
    ontology.setDataProperty(individualName, propertyName, 30);

    // Check if the property was set correctly
    auto retrievedIndividual = ontology.getIndividual(individualName);
    //ASSERT_NE(retrievedIndividual, nullptr);
    auto retrievedDataProperty = retrievedIndividual->getDataPropertyValue(propertyName);
    //ASSERT_NE(retrievedDataProperty, nullptr);
    //EXPECT_EQ(std::get<int>(retrievedDataProperty), 30);
}

// Test Case for handling errors when adding properties with invalid domains or ranges
TEST(OntologyTest, AddPropertyWithInvalidDomainOrRange) {
    Ontology ontology;

    // Try adding an object property with a non-existent domain
    auto objectProperty = make_shared<ObjectProperty>("worksFor", nullptr, createClassType("Person"));
    EXPECT_ANY_THROW(ontology.addProperty(objectProperty));

    // Try adding an object property with a non-existent range
    objectProperty = make_shared<ObjectProperty>("worksFor", createClassType("Person"), nullptr);
    EXPECT_ANY_THROW(ontology.addProperty(objectProperty));
}
