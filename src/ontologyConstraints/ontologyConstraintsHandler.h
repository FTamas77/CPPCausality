#ifndef ONTOLOGY_CONSTRAINTS_HANDLER_H
#define ONTOLOGY_CONSTRAINTS_HANDLER_H

#include <memory>
#include <string>

class Ontology;
class Graph;
class Property;

class OntologyConstraintsHandler {
public:
    explicit OntologyConstraintsHandler(
        const std::string& cannotCausePropertyName,
        const std::string& mustCausePropertyName = "",
        const std::string& directionPropertyName = ""
    );

    void applyConstraints(const std::shared_ptr<Ontology>& ontology, std::shared_ptr<Graph> graph);

private:
    std::string cannotCausePropertyName;
    std::string mustCausePropertyName;
    std::string directionPropertyName;
};

#endif
