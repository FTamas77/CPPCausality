#include "CausalDiscoveryAPI.h"
#include "CausalDiscovery.h"
#include "CSVReader.h"
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

void CausalDiscoveryAPI::loadDatasetFromFile(const std::string& filename) {

    auto columns = CSVReader::readCSVFile(filename, 4);
    auto data = std::make_shared<Dataset>(std::move(columns));
    graph_ = std::make_shared<Graph>(data);
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

void CausalDiscoveryAPI::printGraph() const {
    if (!graph_) {
        throw std::runtime_error("No graph has been generated yet.");
    }

    graph_->printGraph();
}
