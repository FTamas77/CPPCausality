#ifndef CSVREADER_H
#define CSVREADER_H

#include <vector>
#include <string>
#include "dataset.h"

class CSVReader {
public:
    static std::vector<Column> readCSVFile(const std::string& filename, int expectedColumnCount);
};

#endif // CSVREADER_H
