#include "Dataset.h"
#include <string>
#include <vector>

class DataLoader
{
public:
    void loadCSV(const std::string &filename, Dataset &dataset);
};