#include "Individual.h"
#include "ClassType.h"

// Constructor
Individual::Individual(const std::string& name, std::shared_ptr<ClassType> classType)
    : name(name), classType(std::move(classType)) {}

// Get the name of the individual
const std::string& Individual::getName() const {
    return name;
}

// Get the class type of the individual
std::shared_ptr<ClassType> Individual::getClassType() const {
    return classType;
}

// Get an object property by its name
std::shared_ptr<Individual> Individual::getObjectProperty(const std::string& propertyName) const {
    auto it = objectProperties.find(propertyName);
    if (it != objectProperties.end()) {
        return it->second;
    }
    return nullptr;
}

// Get a data property value by its name
std::string Individual::getDataPropertyValue(const std::string& propertyName) const {
    auto it = dataProperties.find(propertyName);
    if (it != dataProperties.end()) {
        return it->second;
    }
    return "";
}

// Set an object property
void Individual::setObjectProperty(const std::string& propertyName, std::shared_ptr<Individual> value) {
    objectProperties[propertyName] = std::move(value);
}

// Set a data property value
void Individual::setDataPropertyValue(const std::string& propertyName, const std::string& value) {
    dataProperties[propertyName] = value;
}
