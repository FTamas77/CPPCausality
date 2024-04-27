#ifndef DATASET_H
#define DATASET_H

#include "dataRow.h"
#include <vector>
#include <cstddef>

class Dataset
{
    std::vector<Datarow> data;

public:
    void addRow(const Datarow &row);

    size_t getNumOfColumns() const
    {
        if (data.size() > 0)
        {
            return data[0].columns.size();
        }

		return 0;
    }
};

#endif // DATASET_H