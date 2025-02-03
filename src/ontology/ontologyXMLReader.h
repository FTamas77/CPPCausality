#ifndef ONTOLOGY_XML_READER_H
#define ONTOLOGY_XML_READER_H

#include "Ontology.h"
#include <string>

class OntologyXMLReader {
public:
    static bool loadFromRDF(Ontology& ontology, const std::string& filePath);
};

#endif // ONTOLOGY_XML_READER_H
