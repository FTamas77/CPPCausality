#include "CausalDiscoveryAPI.h"
#include "CausalDiscovery.h"
#include "Dataset.h"
#include "Graph.h"

#include <stdexcept>
#include <iostream>

CausalDiscoveryAPI::CausalDiscoveryAPI()
    : causalDiscovery_(std::make_shared<CausalDiscovery>()),
    alpha_(0.05),
    graph_(nullptr)
{
}

CausalDiscoveryAPI::~CausalDiscoveryAPI() = default;

void CausalDiscoveryAPI::setAlpha(double alpha) {
    if (alpha <= 0.0 || alpha >= 1.0) {
        throw std::invalid_argument("Alpha must be in the range (0, 1).");
    }

    alpha_ = alpha;
}

void CausalDiscoveryAPI::loadDataset(const Dataset& dataset) {
    auto datasetPtr = std::make_shared<Dataset>(dataset);
    graph_ = std::make_shared<Graph>(datasetPtr);
}


void CausalDiscoveryAPI::loadDatasetFromFile(const std::string& filename) {
    Dataset dataset;

    // TODO: Implement loading from file
    //dataset.loadFromFile(filename);

    loadDataset(dataset);
}


void CausalDiscoveryAPI::run() {
    if (!graph_) {
        throw std::runtime_error("No dataset loaded. Please load a dataset before running the algorithm.");
    }

    causalDiscovery_->runFCI(graph_, alpha_);
}

std::shared_ptr<Graph> CausalDiscoveryAPI::getResultingGraph() const {
    if (!graph_) {
        throw std::runtime_error("No graph has been generated yet.");
    }

    return graph_;
}

void CausalDiscoveryAPI::saveResult(const std::string& filename) const {
    if (!graph_) {
        throw std::runtime_error("No graph has been generated yet.");
    }

    // TODO: Implement saving to file
    //graph_->saveToFile(filename);
}
