#include "datagenerator.h"
#include "datarow.h"
#include <random>

Dataset DataGenerator::generateData(int numRecords, int numColumns)
{
    Dataset data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int n = 0; n < numRecords; ++n)
    {
        Datarow row;
        for (int i = 0; i < numColumns; ++i)
        {
            row.columns.push_back(dis(gen));
        }
        data.addRow(row);
    }

    return std::move(data);
}
