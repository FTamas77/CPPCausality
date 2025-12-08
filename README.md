# Ontology-Enhanced Fast Causal Inference (FCI)

> **Part of Publication:** Fekete, T., & Wicaksono, H. (2025). Ontology-guided causal discovery and inference for reducing CO2 emissions in transportation. *International Journal of Sustainable Transportation*. [DOI: 10.1080/15568318.2025.2588608](https://doi.org/10.1080/15568318.2025.2588608)

> **Related Repositories:**
> - 📊 [Python Implementation + Dataset](https://github.com/FTamas77/Causality) - GUI, DoWhy inference, 463K vehicle dataset
> - 📄 [Dataset Documentation](https://github.com/FTamas77/Causality/blob/develop/datasets/vehicles/README.md) ⭐ *Link in paper*

This repository provides the **high-performance C++ implementation** with native ontology constraint handling, as described in our publication.

## Key Features

- **Ontology-Guided Causal Discovery**: Utilizes domain knowledge to refine the search space, reducing spurious causal links.
- **Enhanced Edge Orientation**: Incorporates expert knowledge to resolve ambiguous causal directions.
- **Robustness to Latent Confounders**: Maintains FCI's ability to account for hidden variables while enhancing reliability.
- **Scalability**: Optimized for large datasets where traditional methods may falter.
- **Performance**: 10-100× speedup over Python implementations (causal-learn) for large datasets.

## Installation

### Prerequisites

- CMake 3.16+
- C++20 compatible compiler (MSVC 2022, GCC 10+, Clang 10+)
- Conan package manager

### Build Instructions

```sh
git clone https://github.com/FTamas77/CPPCausality.git
cd CPPCausality
mkdir build && cd build
conan install .. --build=missing --output-folder . -s build_type=Debug
conan install .. --build=missing --output-folder . -s build_type=Release
cmake --preset default
cmake --build --preset debug
```

## Ontology Constraints (Paper Methodology)

As described in Section 4.3 of our publication, three types of ontology constraints can be integrated:

### 1. Forbidden Adjacencies
Block physically implausible edges (e.g., noise cannot cause emissions).

**XML Definition:**
```xml
<owl:ObjectProperty rdf:about="#cannotCause">
    <rdfs:domain rdf:resource="#PassingNoise"/>
    <rdfs:range rdf:resource="#CO2Emissions"/>
</owl:ObjectProperty>
```

**C++ Usage:**
```cpp
auto ontology = std::make_shared<Ontology>();
OntologyXMLReader::loadFromRDF(*ontology, "constraints.owl");

OntologyConstraintsHandler handler("cannotCause");
handler.applyConstraints(ontology, graph);
```

### 2. Required Adjacencies  
Enforce domain-known relationships (e.g., capacity → power).

**XML Definition:**
```xml
<owl:ObjectProperty rdf:about="#mustCause">
    <rdfs:domain rdf:resource="#CylinderCapacity"/>
    <rdfs:range rdf:resource="#EnginePower"/>
</owl:ObjectProperty>
```

### 3. Direction-Only Constraints
Orient edges based on causal flow without forcing their presence.

**Implementation Note:** Currently in development. Direction constraints will be applied only during FCI orientation stage, never forcing edge creation.

### Complete Example

See `tests/ontologyWithDiscoveryTest.cpp` for a working example integrating ontology constraints with FCI.

## Example Usage

### Basic Usage (Without Ontology Constraints)

```cpp
#include "CausalDiscoveryAPI.h"
#include <iostream>

int main() {
    try {
        CausalDiscoveryAPI api;
        api.setAlpha(0.05);
        api.loadDatasetFromFile("KV-41762_202301_test.csv", 4);
        api.run();
        api.printGraph();
        
        std::cout << "Causal discovery completed!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

### Advanced Usage (With Ontology Constraints)

```cpp
#include "CausalDiscoveryAPI.h"
#include "OntologyConstraintsHandler.h"
#include "OntologyXMLReader.h"
#include <iostream>

int main() {
    try {
        // Initialize the causal discovery API
        CausalDiscoveryAPI api;
        api.setAlpha(0.05);
        
        // Load dataset
        api.loadDatasetFromFile("KV-41762_202301_test.csv", 4);
        
        // Load ontology constraints (as in paper)
        auto ontology = std::make_shared<Ontology>();
        OntologyXMLReader::loadFromRDF(*ontology, "vehicle_constraints.owl");
        
        OntologyConstraintsHandler handler("cannotCause");
        handler.applyConstraints(ontology, api.getResultingGraph());
        
        // Execute the ontology-enhanced FCI algorithm
        api.run();
        api.printGraph();
        
        std::cout << "Causal discovery completed!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

## Available Datasets

The complete dataset used in our publication (463,568 Hungarian vehicle records):

📊 **[Vehicle Emissions Dataset + Documentation](https://github.com/FTamas77/Causality/blob/develop/datasets/vehicles/README.md)**

This dataset contains:
- 463,568 passenger vehicle inspection records (Jan-Mar 2023)
- Engine performance, emissions, fuel consumption, noise levels
- Complete preprocessing and usage examples
- Reproduction guide for paper results

These datasets are formatted for immediate use with the API demonstrated in the examples above.

## Performance Comparison

This C++ implementation provides:
- **10-100× speedup** over Python (causal-learn) for large datasets
- Native ontology constraint handling (no Python overhead)
- Production-ready performance for 400K+ vehicle records

For interactive analysis and DoWhy integration, see the [Python GUI](https://github.com/FTamas77/Causality/tree/develop/cross-sectional-proc).

## Applications

The Ontology-Enhanced FCI algorithm has diverse real-world applications:

- **Emissions Reduction**: Identifying key factors influencing CO₂ emissions in transportation networks, helping prioritize intervention points.
- **Policy Development**: Informing regulatory decisions through a better understanding of causal drivers, enabling evidence-based policymaking.
- **Process Optimization**: Enhancing efficiency in industrial and logistical operations via causal insights, reducing waste and improving resource allocation.

## Citation

If you use this implementation in your research, please cite:

```bibtex
@article{fekete2025ontology,
  title={Ontology-guided causal discovery and inference for reducing CO2 emissions in transportation},
  author={Fekete, Tamas and Wicaksono, Hendro},
  journal={International Journal of Sustainable Transportation},
  year={2025},
  publisher={Taylor \& Francis},
  doi={10.1080/15568318.2025.2588608}
}
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

