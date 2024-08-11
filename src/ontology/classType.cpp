#include "ClassType.h"
#include "Property.h"

ClassType::ClassType(const std::string& name) : name(name) {}

const std::string& ClassType::getName() const {
    return name;
}

void ClassType::addProperty(const std::shared_ptr<Property>& property) {
    properties[property->getName()] = property;
}

std::shared_ptr<Property> ClassType::getPropertyByName(const std::string& propertyName) const {
    auto it = properties.find(propertyName);
    if (it != properties.end()) {
        return it->second;
    }
    return nullptr;
}
