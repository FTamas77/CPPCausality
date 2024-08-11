#include "Ontology.h"

void Ontology::addClassType(const std::shared_ptr<ClassType>& classType) {
    classTypes[classType->getName()] = classType;
}

std::shared_ptr<ClassType> Ontology::getClassType(const std::string& name) const {
    auto it = classTypes.find(name);
    return (it != classTypes.end()) ? it->second : nullptr;
}

void Ontology::addProperty(const std::shared_ptr<Property>& property) {
    properties[property->getName()] = property;
}

std::shared_ptr<Property> Ontology::getProperty(const std::string& name) const {
    auto it = properties.find(name);
    return (it != properties.end()) ? it->second : nullptr;
}

void Ontology::addIndividual(const std::shared_ptr<Individual>& individual) {
    individuals[individual->getName()] = individual;
}

std::shared_ptr<Individual> Ontology::getIndividual(const std::string& name) const {
    auto it = individuals.find(name);
    return (it != individuals.end()) ? it->second : nullptr;
}

void Ontology::setObjectProperty(const std::string& individualName, const std::string& propertyName, const std::string& value) {
    // Implementation to set an object property on an individual
}

void Ontology::setDataProperty(const std::string& individualName, const std::string& propertyName, int value) {
    // Implementation to set a data property on an individual
}
