#ifndef CAUSALDISCOVERYAPI_H
#define CAUSALDISCOVERYAPI_H

#include <memory>
#include <string>

// Forward declarations of internal classes
class CausalDiscovery;
class Dataset;
class Graph;

class CausalDiscoveryAPI {
public:
    // Constructor
    CausalDiscoveryAPI();

    // Destructor
    ~CausalDiscoveryAPI();

    // Set significance level
    void setAlpha(double alpha);

    // Load data from a Dataset or a file
    void loadDataset(const Dataset& dataset);
    void loadDatasetFromFile(const std::string& filename);

    // Run the FCI algorithm
    void run();

    // Get the resulting graph
    std::shared_ptr<Graph> getResultingGraph() const;

    // Save the resulting graph to a file
    void saveResult(const std::string& filename) const;

private:
    std::shared_ptr<CausalDiscovery> causalDiscovery_; // Internal class, hidden from the user
    double alpha_;                                     // Significance level
    std::shared_ptr<Graph> graph_;                    // Current graph
};

#endif // CAUSALDISCOVERYAPI_H
