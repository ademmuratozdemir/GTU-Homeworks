#ifndef NOT_H
#define NOT_H
#include "Gate.h"
class Not : public Gate
{
private:
    Gate *input;

public:
    Not(string id, Gate *input);
    virtual bool calculate();
};
Not::Not(string id, Gate *input)
    : Gate(id)
{
    this->input = input;
}

// implementation of "not"
bool Not::calculate()
{
    return (!input->calculate());
}
#endif