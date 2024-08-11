#include "Property.h"

Property::Property(const std::string& name, std::shared_ptr<ClassType> domain)
    : name(name), domain(domain) {}

const std::string& Property::getName() const {
    return name;
}

std::shared_ptr<ClassType> Property::getDomain() const {
    return domain;
}

ObjectProperty::ObjectProperty(const std::string& name, std::shared_ptr<ClassType> domain, std::shared_ptr<ClassType> range)
    : Property(name, domain), range(range) {}

std::shared_ptr<ClassType> ObjectProperty::getRange() const {
    return range;
}

DataProperty::DataProperty(const std::string& name, std::shared_ptr<ClassType> domain, const std::string& dataType)
    : Property(name, domain), dataType(dataType) {}

const std::string& DataProperty::getDataType() const {
    return dataType;
}
