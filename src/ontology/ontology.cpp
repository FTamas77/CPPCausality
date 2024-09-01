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
    properties.insert({ property->getName(), property });
}

std::vector<std::shared_ptr<Property>> Ontology::getProperties(const std::string& name) const {
    std::vector<std::shared_ptr<Property>> propertyList;
    auto range = properties.equal_range(name);  // Get the range of properties with the same name

    for (auto it = range.first; it != range.second; ++it) {
        propertyList.push_back(it->second);
    }

    return propertyList;
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

    auto properties = getProperties(propertyName);
    if (properties.empty()) {
        throw std::runtime_error("Property not found: " + propertyName);
    }

    auto targetIndividual = getIndividual(value);
    if (!targetIndividual) {
        throw std::runtime_error("Target individual not found: " + value);
    }

    // Apply the object property to all matching properties
    for (const auto& property : properties) {
        individual->setObjectProperty(propertyName, targetIndividual);
    }
}

void Ontology::setDataProperty(const std::string& individualName, const std::string& propertyName, const std::string& value) {
    auto individual = getIndividual(individualName);
    if (!individual) {
        throw std::runtime_error("Individual not found: " + individualName);
    }

    auto properties = getProperties(propertyName);
    if (properties.empty()) {
        throw std::runtime_error("Property not found: " + propertyName);
    }

    for (const auto& property : properties) {
        std::string valueStr = value;
        individual->setDataPropertyValue(propertyName, valueStr);
    }
}

