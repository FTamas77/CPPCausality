#ifndef ONTOLOGY_H
#define ONTOLOGY_H

#include "ClassType.h"
#include "Individual.h"
#include "Property.h"
#include <memory>
#include <unordered_map>
#include <string>

class Ontology {
public:
    void addClassType(const std::shared_ptr<ClassType>& classType);
    std::shared_ptr<ClassType> getClassType(const std::string& name) const;

    void addProperty(const std::shared_ptr<Property>& property);
    std::shared_ptr<Property> getProperty(const std::string& name) const;

    void addIndividual(const std::shared_ptr<Individual>& individual);
    std::shared_ptr<Individual> getIndividual(const std::string& name) const;

    void setObjectProperty(const std::string& individualName, const std::string& propertyName, const std::string& value);
    void setDataProperty(const std::string& individualName, const std::string& propertyName, const std::string& value);

private:
    std::unordered_map<std::string, std::shared_ptr<ClassType>> classTypes;
    std::unordered_map<std::string, std::shared_ptr<Property>> properties;
    std::unordered_map<std::string, std::shared_ptr<Individual>> individuals;
};

#endif // ONTOLOGY_H
