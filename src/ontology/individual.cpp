#include "Individual.h"
#include "ClassType.h"

Individual::Individual(const std::string& name, std::shared_ptr<ClassType> classType)
    : name(name), classType(std::move(classType)) {}

const std::string& Individual::getName() const {
    return name;
}

std::shared_ptr<ClassType> Individual::getClassType() const {
    return classType;
}

std::shared_ptr<Individual> Individual::getObjectProperty(const std::string& propertyName) const {
    auto it = objectProperties.find(propertyName);
    if (it != objectProperties.end()) {
        return it->second;
    }

    return nullptr;
}

std::string Individual::getDataPropertyValue(const std::string& propertyName) const {
    auto it = dataProperties.find(propertyName);
    if (it != dataProperties.end()) {
        return it->second;
    }

    return "";
}

void Individual::setObjectProperty(const std::string& propertyName, std::shared_ptr<Individual> value) {
    objectProperties[propertyName] = value;
}

void Individual::setDataPropertyValue(const std::string& propertyName, const std::string& value) {
    dataProperties[propertyName] = value;
}
