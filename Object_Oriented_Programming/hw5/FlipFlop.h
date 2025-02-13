#ifndef FLIPFLOP_H
#define FLIPFLOP_H
#include "Gate.h"
class FlipFlop : public Gate
{
private:
    Gate *input;
    bool former_out = false;
    bool calculated = false;

public:
    FlipFlop(string id, Gate *input);
    void reset();
    virtual bool calculate();
};
FlipFlop::FlipFlop(string id, Gate *input)
    : Gate(id)
{
    this->input = input;
}
void FlipFlop::reset()
{
    calculated = false;
}
//  implementation of "flipflop"
bool FlipFlop::calculate()
{

    if (calculated)
    {
        return former_out;
    }
    else
    {
        former_out = input->calculate() ^ former_out;
        calculated = true;
        return former_out;
    }
}
#endif