#include "Ontology.h"
#include <stdexcept>

void Ontology::addClassType(const std::shared_ptr<ClassType>& classType) {
    classTypes[classType->getName()] = classType;
}

std::shared_ptr<ClassType> Ontology::getClassType(const std::string& name) const {
    auto it = classTypes.find(name);
    return (it != classTypes.end()) ? it->second : nullptr;
}

void Ontology::addProperty(const std::shared_ptr<Property>& property) {
    // TODO: Check if property is valid or already exists
    properties[property->getName()] = property;
}

std::shared_ptr<Property> Ontology::getProperty(const std::string& name) const {
    auto it = properties.find(name);
    return (it != properties.end()) ? it->second : nullptr;
}

void Ontology::addIndividual(const std::shared_ptr<Individual>& individual) {
    // TODO: Check if individual is valid or already exists
    individuals[individual->getName()] = individual;
}

std::shared_ptr<Individual> Ontology::getIndividual(const std::string& name) const {
    auto it = individuals.find(name);
    return (it != individuals.end()) ? it->second : nullptr;
}

void Ontology::setObjectProperty(const std::string& individualName, const std::string& propertyName, const std::string& value) {

    auto individual = getIndividual(individualName);
    if (!individual) {
        throw std::runtime_error("Individual not found: " + individualName);
    }

    auto property = getProperty(propertyName);
    if (!property) {
        throw std::runtime_error("Property not found: " + propertyName);
    }

    /*if (property->getType() != PropertyType::ObjectProperty) {
        throw std::runtime_error("Property is not an ObjectProperty: " + propertyName);
    }*/

    auto targetIndividual = getIndividual(value);
    if (!targetIndividual) {
        throw std::runtime_error("Target individual not found: " + value);
    }

    individual->setObjectProperty(propertyName, targetIndividual);
}

void Ontology::setDataProperty(const std::string& individualName, const std::string& propertyName, const std::string& value) {

    auto individual = getIndividual(individualName);
    if (!individual) {
        throw std::runtime_error("Individual not found: " + individualName);
    }

    auto property = getProperty(propertyName);
    if (!property) {
        throw std::runtime_error("Property not found: " + propertyName);
    }

    /*if (property->getType() != PropertyType::DataProperty) {
        throw std::runtime_error("Property is not a DataProperty: " + propertyName);
    }*/

    std::string valueStr = value;

    individual->setDataPropertyValue(propertyName, valueStr);
}
