#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <string>
#include <memory>
#include <unordered_map>
#include "Property.h"

class ClassType;

class Individual {
public:
    Individual(const std::string& name, std::shared_ptr<ClassType> classType);

    const std::string& getName() const;
    std::shared_ptr<ClassType> getClassType() const;

    std::shared_ptr<Individual> getObjectProperty(const std::string& propertyName) const;
    std::string getDataPropertyValue(const std::string& propertyName) const;

    void setObjectProperty(const std::string& propertyName, std::shared_ptr<Individual> value);
    void setDataPropertyValue(const std::string& propertyName, const std::string& value);

private:
    std::string name;
    std::shared_ptr<ClassType> classType;

    // The first strings are the names of the properties (instances of ObjectProperty or DataProperty)
    std::unordered_map<std::string, std::shared_ptr<Individual>> objectProperties;
    // The second string is the value of the property (for instances of DataProperty)
    std::unordered_map<std::string, std::string> dataProperties;
};

#endif // INDIVIDUAL_H
