#include "ontologyConstraintsHandler.h"
#include "property.h"
#include "ontology.h"
#include "graph.h"
#include <memory>

OntologyConstraintsHandler::OntologyConstraintsHandler(
    const std::string& cannotCausePropertyName,
    const std::string& mustCausePropertyName,
    const std::string& directionPropertyName)
    : cannotCausePropertyName(cannotCausePropertyName)
    , mustCausePropertyName(mustCausePropertyName)
    , directionPropertyName(directionPropertyName) {}

void OntologyConstraintsHandler::applyConstraints(const std::shared_ptr<Ontology>& ontology, std::shared_ptr<Graph> graph) {
    // Apply forbidden edges (cannotCause)
    if (!cannotCausePropertyName.empty()) {
        auto properties = ontology->getProperties(cannotCausePropertyName);
        for (const auto& property : properties) {
            auto objectProperty = std::dynamic_pointer_cast<ObjectProperty>(property);
            if (objectProperty) {
                int from = std::stoi(objectProperty->getDomain()->getName());
                int to = std::stoi(objectProperty->getRange()->getName());
                graph->addForbiddenEdge(from, to);
            }
        }
    }

    // Apply required edges (mustCause)
    if (!mustCausePropertyName.empty()) {
        auto properties = ontology->getProperties(mustCausePropertyName);
        for (const auto& property : properties) {
            auto objectProperty = std::dynamic_pointer_cast<ObjectProperty>(property);
            if (objectProperty) {
                int from = std::stoi(objectProperty->getDomain()->getName());
                int to = std::stoi(objectProperty->getRange()->getName());
                graph->addRequiredEdge(from, to);
            }
        }
    }

    // Apply direction constraints (directionOnly)
    if (!directionPropertyName.empty()) {
        auto properties = ontology->getProperties(directionPropertyName);
        for (const auto& property : properties) {
            auto objectProperty = std::dynamic_pointer_cast<ObjectProperty>(property);
            if (objectProperty) {
                int from = std::stoi(objectProperty->getDomain()->getName());
                int to = std::stoi(objectProperty->getRange()->getName());
                graph->addDirectionConstraint(from, to);
            }
        }
    }
}
