#ifndef AND_H
#define AND_H
#include "Gate.h"
class And : public Gate
{
private:
    Gate *input1;
    Gate *input2;

public:
    And(string id, Gate *input1, Gate *input2);
    virtual bool calculate();
};
And::And(string id, Gate *input1, Gate *input2)
    : Gate(id)
{
    this->input1 = input1;
    this->input2 = input2;
}
bool And::calculate()
{
    bool val1 = input1->calculate();    // implementation of "and operation"
    bool val2 = input2->calculate();
    return (val1 && val2);
}
#endif