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

    auto retrievedProperty = ontology.getProperty(propertyName);
    ASSERT_NE(retrievedProperty, nullptr);

    auto retrievedObjectProperty = dynamic_pointer_cast<ObjectProperty>(retrievedProperty);
    ASSERT_NE(retrievedObjectProperty, nullptr);
    EXPECT_EQ(retrievedObjectProperty->getDomain()->getName(), className1);
    EXPECT_EQ(retrievedObjectProperty->getRange()->getName(), className2);
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

TEST(OntologyTest, DISABLED_AddPropertyWithInvalidDomainOrRange) {
    Ontology ontology;

    // TODO: this check is not yet implemented
    auto objectProperty = make_shared<ObjectProperty>("worksFor", nullptr, createClassType("Person"));
    EXPECT_ANY_THROW(ontology.addProperty(objectProperty));

    objectProperty = make_shared<ObjectProperty>("worksFor", createClassType("Person"), nullptr);
    EXPECT_ANY_THROW(ontology.addProperty(objectProperty));
}
