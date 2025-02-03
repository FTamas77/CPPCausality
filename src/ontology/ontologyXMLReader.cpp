#include "OntologyXMLReader.h"
#include "pugixml.hpp"
#include <iostream>

void extractNamespacePrefix(std::string& value) {
    size_t pos = value.find_last_of('#');
    if (pos != std::string::npos) {
        value = value.substr(pos + 1);
    }
}

bool OntologyXMLReader::loadFromRDF(Ontology& ontology, const std::string& filePath) {
    pugi::xml_document doc;
    if (!doc.load_file(filePath.c_str())) {
        std::cerr << "Error loading RDF/XML file: " << filePath << std::endl;
        return false;
    }

    // Load Classes
    for (auto& node : doc.child("rdf:RDF").children("owl:Class")) {
        std::string name = node.attribute("rdf:about").as_string();
        extractNamespacePrefix(name);
        if (!name.empty() && ontology.getClassType(name) == nullptr) {
            ontology.addClassType(std::make_shared<ClassType>(name));
        }
    }

    // Load Object Properties
    for (auto& node : doc.child("rdf:RDF").children("owl:ObjectProperty")) {
        std::string name = node.attribute("rdf:about").as_string();
        extractNamespacePrefix(name);
        ontology.addProperty(std::make_shared<Property>(name, nullptr));
    }

    // Load Data Properties
    for (auto& node : doc.child("rdf:RDF").children("owl:DatatypeProperty")) {
        std::string name = node.attribute("rdf:about").as_string();
        extractNamespacePrefix(name);
        ontology.addProperty(std::make_shared<Property>(name, nullptr));
    }

    // Load Individuals
    for (auto& node : doc.child("rdf:RDF").children("owl:NamedIndividual")) {
        std::string name = node.attribute("rdf:about").as_string();
        extractNamespacePrefix(name);

        std::string type = node.child("rdf:type").attribute("rdf:resource").as_string();
        extractNamespacePrefix(type);

        auto classType = ontology.getClassType(type);
        if (!classType) {
            std::cerr << "Warning: ClassType '" << type << "' not found for Individual '" << name << "'. Skipping.\n";
            continue;
        }

        auto individual = std::make_shared<Individual>(name, classType);
        ontology.addIndividual(individual);
    }

    // Assign Object & Data Properties
    for (auto& node : doc.child("rdf:RDF").children("owl:NamedIndividual")) {
        std::string name = node.attribute("rdf:about").as_string();
        extractNamespacePrefix(name);

        // Load Object Properties
        for (auto& propNode : node.children()) {
            std::string propName = propNode.name();
            extractNamespacePrefix(propName);

            if (ontology.getProperties(propName).empty()) {
                continue; // Skip if not a known property
            }

            std::string value = propNode.attribute("rdf:resource").as_string();
            if (!value.empty()) {
                extractNamespacePrefix(value);

                if (ontology.getIndividual(value) == nullptr) {
                    std::cerr << "Warning: Individual '" << value << "' referenced in property '" << propName << "' does not exist.\n";
                    continue;
                }

                ontology.setObjectProperty(name, propName, value);
            }
            else {
                std::string dataValue = propNode.child_value();
                ontology.setDataProperty(name, propName, dataValue);
            }
        }
    }

    return true;
}
