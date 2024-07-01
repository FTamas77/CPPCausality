#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <cstddef>
#include <memory>

typedef std::vector<int> Column;

class Dataset
{
    std::vector<std::shared_ptr<Column>> m_columns;

public:

    Dataset() = default;

    Dataset(std::initializer_list<Column> init_list)
    {
        m_columns.reserve(init_list.size());

        for (const auto& column : init_list)
        {
            addColumn(column);
        }
    }

    void addColumn(const Column &column)
    {
        m_columns.push_back(std::make_shared<Column>(column));
    }

    size_t getNumOfColumns() const
    {
        return m_columns.size();
    }

    std::shared_ptr<Column> getColumn(int i) const
    {
        if (i >= 0 && i < m_columns.size())
        {
            return m_columns[i];
        }
        else
        {
            return nullptr;
            // TODO: throw std::out_of_range("Index out of range in Dataset::getColumn");
        }
    }
};

#endif // DATASET_H