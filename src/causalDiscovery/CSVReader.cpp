#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<Column> CSVReader::readCSVFile(const std::string& filename, int expectedColumnCount) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file: " + filename);
    }

    std::string line;
    std::vector<Column> columns(expectedColumnCount);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<double> row_values;
        bool valid_row = true;

        for (int i = 0; i < expectedColumnCount; ++i) {
            if (std::getline(ss, token, ',')) {
                try {
                    double value = std::stod(token);
                    row_values.push_back(value);
                }
                catch (const std::invalid_argument&) {
                    valid_row = false;
                    break;
                }
                catch (const std::out_of_range&) {
                    valid_row = false;
                    break;
                }
            }
            else {
                valid_row = false;
                break;
            }
        }

        if (valid_row && row_values.size() == expectedColumnCount) {
            for (int i = 0; i < expectedColumnCount; ++i) {
                columns[i].push_back(row_values[i]);
            }
        }
    }

    file.close();
    return columns;
}
