#include "graph.h"
#include "dataloader.h"
#include "causalDiscovery.h"
#include <iostream>
#include <stdexcept>

int main()
{
    Dataset data;
    DataLoader loader;

    // Load the data from the CSV file
    try
    {
        loader.loadCSV("./data.csv", data);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // Set your significance level
    double alpha = 0.05;

    // Run the FCI algorithm
    CausalDiscovery fci;
    auto result = fci.FCI(data, alpha);

    return 0;
}