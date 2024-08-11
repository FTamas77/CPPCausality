#ifndef CLASSTYPE_H
#define CLASSTYPE_H

#include <string>
#include <unordered_map>
#include <memory>

class Property;

class ClassType {
public:
    explicit ClassType(const std::string& name);

    const std::string& getName() const;

    void addProperty(const std::shared_ptr<Property>& property);

    std::shared_ptr<Property> getPropertyByName(const std::string& propertyName) const;

private:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<Property>> properties;
};

#endif // CLASSTYPE_H
