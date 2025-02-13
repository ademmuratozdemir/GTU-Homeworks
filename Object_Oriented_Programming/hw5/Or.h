#ifndef OR_H
#define OR_H
#include "Gate.h"
class Or : public Gate
{
private:
    Gate *input1;
    Gate *input2;

public:
    Or(string id, Gate *input1, Gate *input2);
    virtual bool calculate();
};
Or::Or(string id, Gate *input1, Gate *input2)
    : Gate(id)
{
    this->input1 = input1;
    this->input2 = input2;
}

//implementation of "or"
bool Or::calculate()
{
    bool val1 = input1->calculate();
    bool val2 = input2->calculate();
    return (val1 || val2);
}
#endif