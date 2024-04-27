#include "dataLoader.h"
#include "dataset.h"
#include "dataRow.h"
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

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);

        Datarow row;
        std::string value;
        while (std::getline(ss, value, ','))
        {
            row.columns.push_back(std::stoi(value));
        }

        dataset.addRow(row);
    }

    std::cout << "Finished loading data.\n";

    file.close();
}