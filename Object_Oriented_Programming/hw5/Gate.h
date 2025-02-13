#ifndef GATE_H
#define GATE_H
#include <iostream>
#include <string>
using namespace std;
class Gate
{
private:
    string id;
    bool data;

public:
    Gate();
    Gate(string id);
    void setId(string id);
    string getId() const;
    void setData(bool data);
    bool getData() const;
    virtual bool calculate() = 0;
};

Gate::Gate()
{
    id = "null";
}

Gate::Gate(string id)
{
    this->id = id;
}

void Gate::setId(string id)
{
    this->id = id;
}

string Gate::getId() const
{
    return id;
}

void Gate::setData(bool data)
{
    this->data = data;
}
bool Gate::getData() const
{
    return data;
}
#endif
