#include "Ontology.h"
#include "Individual.h"
#include "ClassType.h"
#include "Property.h"
#include <gtest/gtest.h>
#include <vector>
#include <set>
#include <memory>

using namespace std;

shared_ptr<ClassType> createClassType(const string& name) {
    auto classType = make_shared<ClassType>(name);
    return classType;
}

TEST(OntologyTest, AddAndRetrieveClassType) {
    Ontology ontology;

    string className = "Person";
    auto classType = createClassType(className);
    ontology.addClassType(classType);

    auto retrievedClassType = ontology.getClassType(className);
    ASSERT_NE(retrievedClassType, nullptr);
    EXPECT_EQ(retrievedClassType->getName(), className);
}

TEST(OntologyTest, AddAndRetrieveObjectProperty) {
    Ontology ontology;

    string className1 = "Person";
    string className2 = "Organization";
    auto classType1 = createClassType(className1);
    auto classType2 = createClassType(className2);
    ontology.addClassType(classType1);
    ontology.addClassType(classType2);

    string propertyName = "worksFor";
    auto objectProperty = make_shared<ObjectProperty>(propertyName, classType1, classType2);
    ontology.addProperty(objectProperty);

    auto retrievedProperties = ontology.getProperties(propertyName);
    ASSERT_FALSE(retrievedProperties.empty());  // Ensure that the property was added

    bool found = false;
    for (const auto& prop : retrievedProperties) {
        auto retrievedObjectProperty = dynamic_pointer_cast<ObjectProperty>(prop);
        if (retrievedObjectProperty &&
            retrievedObjectProperty->getDomain()->getName() == className1 &&
            retrievedObjectProperty->getRange()->getName() == className2) {
            found = true;
            break;
        }
    }
    ASSERT_TRUE(found) << "Failed to find the expected ObjectProperty with domain and range";
}

TEST(OntologyTest, AddAndRetrieveIndividual) {
    Ontology ontology;

    string className = "Person";
    auto classType = createClassType(className);
    ontology.addClassType(classType);

    string individualName = "Alice";
    auto individual = make_shared<Individual>(individualName, classType);
    ontology.addIndividual(individual);

    auto retrievedIndividual = ontology.getIndividual(individualName);
    ASSERT_NE(retrievedIndividual, nullptr);
    EXPECT_EQ(retrievedIndividual->getName(), individualName);
    EXPECT_EQ(retrievedIndividual->getClassType()->getName(), className);
}

TEST(OntologyTest, SetAndRetrieveObjectProperty) {
    Ontology ontology;

    string className1 = "Person";
    string className2 = "Organization";
    auto classType1 = createClassType(className1);
    auto classType2 = createClassType(className2);
    ontology.addClassType(classType1);
    ontology.addClassType(classType2);

    string propertyName = "worksFor";
    auto objectProperty = make_shared<ObjectProperty>(propertyName, classType1, classType2);
    ontology.addProperty(objectProperty);

    string individualName1 = "Alice";
    string individualName2 = "AcmeCorp";
    auto individual1 = make_shared<Individual>(individualName1, classType1);
    auto individual2 = make_shared<Individual>(individualName2, classType2);
    ontology.addIndividual(individual1);
    ontology.addIndividual(individual2);

    ontology.setObjectProperty(individualName1, propertyName, individualName2);

    auto retrievedIndividual = ontology.getIndividual(individualName1);
    ASSERT_NE(retrievedIndividual, nullptr);

    auto retrievedObjectProperty = retrievedIndividual->getObjectProperty(propertyName);
    ASSERT_NE(retrievedObjectProperty, nullptr);
    EXPECT_EQ(retrievedObjectProperty->getName(), individualName2);
}

TEST(OntologyTest, SetAndRetrieveDataProperty) {
    Ontology ontology;

    string className = "Person";
    auto classType = createClassType(className);
    ontology.addClassType(classType);

    string propertyName = "age";
    auto dataProperty = make_shared<DataProperty>(propertyName, classType, "int");
    ontology.addProperty(dataProperty);

    string individualName = "Alice";
    auto individual = make_shared<Individual>(individualName, classType);
    ontology.addIndividual(individual);

    ontology.setDataProperty(individualName, propertyName, "30");

    auto retrievedIndividual = ontology.getIndividual(individualName);
    ASSERT_NE(retrievedIndividual, nullptr);

    auto retrievedDataProperty = retrievedIndividual->getDataPropertyValue(propertyName);
    ASSERT_FALSE(retrievedDataProperty.empty());
    EXPECT_EQ(retrievedDataProperty, "30");
}

TEST(OntologyTest, AddAndRetrieveMultiplePropertiesWithSameName) {
    Ontology ontology;

    string className1 = "Person";
    string className2 = "Organization";
    string className3 = "Project";
    auto classType1 = createClassType(className1);
    auto classType2 = createClassType(className2);
    auto classType3 = createClassType(className3);
    ontology.addClassType(classType1);
    ontology.addClassType(classType2);
    ontology.addClassType(classType3);

    string propertyName = "worksFor";
    auto objectProperty1 = make_shared<ObjectProperty>(propertyName, classType1, classType2);
    auto objectProperty2 = make_shared<ObjectProperty>(propertyName, classType1, classType3);
    ontology.addProperty(objectProperty1);
    ontology.addProperty(objectProperty2);

    auto retrievedProperties = ontology.getProperties(propertyName);
    ASSERT_EQ(retrievedProperties.size(), 2);  // Expecting two properties with the same name

    bool found1 = false, found2 = false;
    for (const auto& prop : retrievedProperties) {
        auto retrievedObjectProperty = dynamic_pointer_cast<ObjectProperty>(prop);
        if (retrievedObjectProperty->getDomain()->getName() == className1 &&
            retrievedObjectProperty->getRange()->getName() == className2) {
            found1 = true;
        }
        if (retrievedObjectProperty->getDomain()->getName() == className1 &&
            retrievedObjectProperty->getRange()->getName() == className3) {
            found2 = true;
        }
    }

    ASSERT_TRUE(found1) << "First ObjectProperty with domain Person and range Organization not found";
    ASSERT_TRUE(found2) << "Second ObjectProperty with domain Person and range Project not found";
}

TEST(OntologyTest, DISABLED_AddPropertyWithInvalidDomainOrRange) {
    Ontology ontology;

    // TODO: this check is not yet implemented
    auto objectProperty = make_shared<ObjectProperty>("worksFor", nullptr, createClassType("Person"));
    EXPECT_ANY_THROW(ontology.addProperty(objectProperty));

    objectProperty = make_shared<ObjectProperty>("worksFor", createClassType("Person"), nullptr);
    EXPECT_ANY_THROW(ontology.addProperty(objectProperty));
}
