#ifndef ONTOLOGY_CONSTRAINTS_HANDLER_H
#define ONTOLOGY_CONSTRAINTS_HANDLER_H

#include <memory>
#include <string>

class Ontology;
class Graph;
class Property;

class OntologyConstraintsHandler {
public:
    explicit OntologyConstraintsHandler(const std::string& cannotCausePropertyName);

    void applyConstraints(const std::shared_ptr<Ontology>& ontology, std::shared_ptr<Graph> graph);

private:
    std::string cannotCausePropertyName;
};

#endif
