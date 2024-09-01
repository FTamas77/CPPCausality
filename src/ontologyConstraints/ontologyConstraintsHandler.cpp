#include "ontologyConstraintsHandler.h"
#include "property.h"
#include "ontology.h"
#include "graph.h"
#include <memory>

OntologyConstraintsHandler::OntologyConstraintsHandler(const std::string& cannotCausePropertyName)
    : cannotCausePropertyName(cannotCausePropertyName) {}

void OntologyConstraintsHandler::applyConstraints(const std::shared_ptr<Ontology>& ontology, std::shared_ptr<Graph> graph) {
    auto properties = ontology->getProperties(cannotCausePropertyName);

    for (const auto& property : properties) {
        auto objectProperty = std::dynamic_pointer_cast<ObjectProperty>(property);
        if (objectProperty) {
            int from = std::stoi(objectProperty->getDomain()->getName());
            int to = std::stoi(objectProperty->getRange()->getName());
            graph->addForbiddenEdge(from, to);
        }
        else {
            // Handle the case where the property is not an ObjectProperty
            // You might want to log an error or handle other types of properties
        }
    }
}
