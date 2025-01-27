#include "CausalDiscoveryAPI.h"
#include <iostream>

int main() {
    try {
        CausalDiscoveryAPI api;

        api.setAlpha(0.05);

        api.loadDatasetFromFile("data.csv");

        api.run();

        api.saveResult("output_graph.txt");

        std::cout << "Causal discovery completed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
