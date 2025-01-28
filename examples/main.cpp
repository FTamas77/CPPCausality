#include "CausalDiscoveryAPI.h"
#include <iostream>

int main() {
    try {
        CausalDiscoveryAPI api;

        api.setAlpha(0.05);

        api.loadDatasetFromFile(std::string(PROJECT_SOURCE_DIR) + "/KV-41762_202301_test.csv");

        api.run();

        api.printGraph();

        std::cout << "Causal discovery completed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
