#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "Dataset.h"

class DataGenerator
{
public:
    Dataset generateData(int numRecords, int numColumns);
};

#endif // DATAGENERATOR_H