#include <string>
#include <vector>

#include "Dataset.h"

class DataLoader
{
public:
    void loadCSV(const std::string &filename, Dataset &dataset);
};