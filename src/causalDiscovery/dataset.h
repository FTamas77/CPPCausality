#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <cstddef>
#include <memory>
#include <utility> // For std::move

typedef std::vector<double> Column;

class Dataset
{
    std::vector<std::shared_ptr<Column>> m_columns;

public:

    Dataset() = default;

    Dataset(std::vector<Column> init_vector)
    {
        m_columns.reserve(init_vector.size());

        for (auto& column : init_vector)
        {
            addColumn(std::move(column));
        }
    }

    virtual ~Dataset() = default;

    void addColumn(const Column& column)
    {
        m_columns.push_back(std::make_shared<Column>(column));
    }

    void addColumn(Column&& column)
    {
        m_columns.push_back(std::make_shared<Column>(std::move(column)));
    }

    size_t getNumOfColumns() const
    {
        return m_columns.size();
    }

    virtual std::shared_ptr<Column> getColumn(int i) const
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
