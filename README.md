# Ontology-Enhanced Fast Causal Inference (FCI)

This repository offers an enhanced implementation of the **Fast Causal Inference (FCI)** algorithm, integrating domain-specific ontologies to improve causal discovery accuracy, especially in the presence of latent confounders.

## Key Features

- **Ontology-Guided Causal Discovery**: Utilizes domain knowledge to refine the search space, reducing spurious causal links.
- **Enhanced Edge Orientation**: Incorporates expert knowledge to resolve ambiguous causal directions.
- **Robustness to Latent Confounders**: Maintains FCI's ability to account for hidden variables while enhancing reliability.
- **Scalability**: Optimized for large datasets where traditional methods may falter.

## Installation

To set up the project:

```sh
git clone https://github.com/FTamas77/CPPCausality.git
cd CPPCausality
mkdir build && cd build
cmake ..
make
```

## Example Usage

This example demonstrates how to implement the Ontology-Enhanced FCI algorithm in your project. The process involves three simple steps: configuring the API, loading your dataset, and executing the algorithm.

### C++ Implementation Example

```cpp
#include "CausalDiscoveryAPI.h"
#include <iostream>

int main() {
    try {
        // Initialize the causal discovery API
        CausalDiscoveryAPI api;
        
        // Set significance level for independence tests
        api.setAlpha(0.05);  // 95% confidence level
        
        // Load your dataset from a CSV file
        api.loadDatasetFromFile(std::string(PROJECT_SOURCE_DIR) + "/KV-41762_202301_test.csv");
        
        // Execute the ontology-enhanced FCI algorithm
        api.run();
        
        // Output the discovered causal graph
        api.printGraph();
        
        std::cout << "Causal discovery completed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

### Build Configuration

Add this to your CMakeLists.txt to integrate the library into your project:

```cmake
cmake_minimum_required(VERSION 3.14)
project(CausalDiscoveryExample VERSION 1.0 LANGUAGES CXX)

# Configure C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create executable
add_executable(main_example main.cpp)

# Configure library paths
target_include_directories(main_example PRIVATE ${CMAKE_SOURCE_DIR}/../src/include)
target_link_directories(main_example PRIVATE ${CMAKE_SOURCE_DIR}/../build/Debug/Debug)

# Link dependencies
target_link_libraries(main_example PRIVATE causalDiscovery csvreader)

# Define source directory for data file access
target_compile_definitions(main_example PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
```

The example will load your dataset, perform causal discovery using the enhanced FCI algorithm, and output the resulting causal graph structure.

## Known Issues

### Conan Package Path Issues

There is currently a temporary fix in our CMakeLists.txt for some Conan packages, particularly Google Test:

```cmake
# Temporary fix for some conan packages
set(GTEST_INC_DIR "c:/conan_data/b/gtest9909e8a124da0/p/include/")
set(GTEST_LIB_DIR "c:/conan_data/b/gtest9909e8a124da0/p/lib/")
set(GTEST_LIB "${GTEST_LIB_DIR}/gtest.lib")
set(GTEST_MAIN_LIB "${GTEST_LIB_DIR}/gtest_main.lib")
```

This hardcoded path is necessary because the Conan-provided path resolution isn't working correctly. If you encounter build errors related to missing GTest libraries or includes, you may need to adjust these paths to match your Conan cache location.

## Applications

The Ontology-Enhanced FCI algorithm has diverse real-world applications:

- **Emissions Reduction**: Identifying key factors influencing CO₂ emissions in transportation networks, helping prioritize intervention points.

- **Policy Development**: Informing regulatory decisions through a better understanding of causal drivers, enabling evidence-based policymaking.

- **Process Optimization**: Enhancing efficiency in industrial and logistical operations via causal insights, reducing waste and improving resource allocation.

