#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <memory>

class ClassType;

class Property {
public:
    Property(const std::string& name, std::shared_ptr<ClassType> domain);

    virtual ~Property() = default;

    const std::string& getName() const;
    std::shared_ptr<ClassType> getDomain() const;

protected:
    std::string name;
    std::shared_ptr<ClassType> domain;
};

class ObjectProperty : public Property {
public:
    ObjectProperty(const std::string& name, std::shared_ptr<ClassType> domain, std::shared_ptr<ClassType> range);

    std::shared_ptr<ClassType> getRange() const;

private:
    std::shared_ptr<ClassType> range;
};

class DataProperty : public Property {
public:
    DataProperty(const std::string& name, std::shared_ptr<ClassType> domain, const std::string& dataType);

    const std::string& getDataType() const;

private:
    std::string dataType;
};

#endif // PROPERTY_H
