# CPPCausality

**High-Performance Ontology-Guided Causal Discovery for Vehicle Emissions**

[![DOI](https://img.shields.io/badge/DOI-10.1080%2F15568318.2025.2588608-blue)](https://doi.org/10.1080/15568318.2025.2588608)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Tests](https://img.shields.io/badge/tests-6%2F6%20passing-success)](https://github.com/FTamas77/CPPCausality)

> **📄 Publication:** Fekete, T., & Wicaksono, H. (2025). Ontology-guided causal discovery and inference for reducing CO2 emissions in transportation. *International Journal of Sustainable Transportation*. [DOI: 10.1080/15568318.2025.2588608](https://doi.org/10.1080/15568318.2025.2588608)

> **🐍 Python Version:** For GUI and DoWhy inference, see [Causality Repository](https://github.com/FTamas77/Causality)

This is the **C++ implementation** of ontology-guided FCI (Fast Causal Inference) used in our publication. It provides **10-50× faster** execution than Python with native ontology constraint handling.

## Why C++ Version?

| Feature | Python (causal-learn) | C++ (This Repo) |
|---------|----------------------|-----------------|
| Speed | Baseline | **10-50× faster** |
| Ontology Constraints | Post-processing | **Native integration** |
| Memory | High | **Optimized** |
| Large Datasets | Slow (>100K records) | **Fast (463K records)** |
| Use Case | Prototyping, GUI | **Production, Batch** |

## Key Features

- **Ontology-Guided Causal Discovery**: Utilizes domain knowledge to refine the search space, reducing spurious causal links.
- **Enhanced Edge Orientation**: Incorporates expert knowledge to resolve ambiguous causal directions.
- **Robustness to Latent Confounders**: Maintains FCI's ability to account for hidden variables while enhancing reliability.
- **Scalability**: Optimized for large datasets where traditional methods may falter.
- **Performance**: 10-50× speedup over Python implementations (causal-learn) for large datasets.

## Paper Reproduction Benchmark

Reproduce the exact results from our IJST publication:

```sh
cd build
./examples/benchmark_paper
```

**Expected Output:**
```
=== PAPER REPRODUCTION BENCHMARK ===
Dataset: 463,568 vehicle records (Jan-Mar 2023)

Without Ontology Constraints:
  Execution time: ~8.3s
  Spurious edges: 4
  F1-Score: 0.67

With Ontology Constraints:
  Execution time: ~8.7s
  Spurious edges: 0 (-100%)
  F1-Score: 0.78 (+16%)

Speedup vs Python: 15× faster (Python: 127s)
```

See `data/reference_results.json` for complete expected metrics and ground truth.

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

### Run Tests

```sh
ctest --preset debug
```

All 6 tests should pass (unit + integration tests for ontology constraints).

## Ontology Constraints (Paper Methodology)

As described in Section 4.3 of our publication, three types of ontology constraints are fully implemented:

### 1. Forbidden Adjacencies ✅
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
OntologyConstraintsHandler handler("cannotCause");
handler.applyConstraints(ontology, graph);
```

### 2. Required Adjacencies ✅
Enforce domain-known relationships (e.g., capacity → power).

**XML Definition:**
```xml
<owl:ObjectProperty rdf:about="#mustCause">
    <rdfs:domain rdf:resource="#CylinderCapacity"/>
    <rdfs:range rdf:resource="#EnginePower"/>
</owl:ObjectProperty>
```

**C++ Usage:**
```cpp
OntologyConstraintsHandler handler("", "mustCause");
handler.applyConstraints(ontology, graph);
```

### 3. Direction-Only Constraints ✅
Orient edges based on causal flow without forcing their presence.

**C++ Usage:**
```cpp
OntologyConstraintsHandler handler("", "", "directionOnly");
handler.applyConstraints(ontology, graph);
```

**Complete Example:** See `examples/benchmark_paper.cpp` and `tests/ontologyWithDiscoveryTest.cpp`.

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

### Advanced Usage (With All 3 Constraint Types)

```cpp
#include "CausalDiscoveryAPI.h"
#include "OntologyConstraintsHandler.h"
#include <iostream>

int main() {
    try {
        CausalDiscoveryAPI api;
        api.setAlpha(0.05);
        api.loadDatasetFromFile("KV-41762_202301_test.csv", 4);
        
        // Apply ALL three constraint types (as in paper)
        auto ontology = createVehicleOntology();
        OntologyConstraintsHandler handler("cannotCause", "mustCause", "directionOnly");
        handler.applyConstraints(ontology, api.getResultingGraph());
        
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

## Performance Comparison

**Benchmarked on 463K vehicle records:**

| Implementation | Time | Speedup | F1-Score | Spurious Edges |
|----------------|------|---------|----------|----------------|
| Python (causal-learn) | 127s | 1× | 0.67 | 4 |
| C++ (unconstrained) | 8.3s | **15.3×** | 0.67 | 4 |
| C++ (constrained) | 8.7s | **14.6×** | **0.78** | **0** |

- **15× faster** than Python
- **Minimal overhead** (4.8%) from constraint checking
- **16% F1-score improvement** with ontology
- **100% spurious edge elimination**

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

## Repository Structure

```
CPPCausality/
├── src/
│   ├── causalDiscovery/      # FCI, PC, GES implementations
│   ├── ontology/              # OWL ontology parsing
│   └── ontologyConstraints/   # Constraint application
├── tests/                     # 6 unit + integration tests
├── examples/
│   ├── main.cpp              # Basic usage
│   └── benchmark_paper.cpp   # Paper reproduction
├── data/
│   └── reference_results.json # Ground truth & metrics
└── ontology/
    └── vehicle_constraints.owl # Example constraints

