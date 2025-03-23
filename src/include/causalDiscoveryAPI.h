#ifndef CAUSALDISCOVERYAPI_H
#define CAUSALDISCOVERYAPI_H

#include <memory>
#include <string>

class CausalDiscovery;
class Dataset;
class Graph;

class CausalDiscoveryAPI {
public:
    CausalDiscoveryAPI();

    ~CausalDiscoveryAPI();

    void setAlpha(double alpha);

    void loadDatasetFromFile(const std::string& filename, int numColumns = 4);

    void run();

    std::shared_ptr<Graph> getResultingGraph() const;

    void printGraph() const;

private:
    std::shared_ptr<CausalDiscovery> causalDiscovery_;
    double alpha_;
    std::shared_ptr<Graph> graph_;
};

#endif // CAUSALDISCOVERYAPI_H
