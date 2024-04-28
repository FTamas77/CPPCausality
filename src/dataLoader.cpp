#include "dataLoader.h"
#include "dataset.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

void DataLoader::loadCSV(const std::string &filename, Dataset &dataset)
{
    std::cout << "Loading CSV file: " << filename << "\n";

    std::ifstream file(filename);

    // Check if the file was opened successfully
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filename << "\n";
        throw std::runtime_error("Could not open file");
    }

    std::cout << "File opened successfully.\n";

    std::vector<Column> tmp_data;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);

        Column row;
        std::string value;
        while (std::getline(ss, value, ','))
        {
            row.push_back(std::stoi(value));
        }

        tmp_data.push_back(row);
    }

    // Transpose the data
    for (size_t i = 0; i < tmp_data[0].size(); ++i)
    {
        Column column;
        for (size_t j = 0; j < tmp_data.size(); ++j)
        {
            column.push_back(tmp_data[j][i]);
        }

        dataset.addColumn(column);
    }

    std::cout << "Finished loading data.\n";

    file.close();
}