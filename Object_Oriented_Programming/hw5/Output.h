#ifndef OUTPUT_H
#define OUTPUT_H
#include "Gate.h"
class Output : public Gate
{
public:
    Output();
    Output(string id);
    virtual bool calculate();
};

Output::Output()
    : Gate()
{
    // Intentionally empty
}
Output::Output(string id)
    : Gate(id)
{
    // Intentionally empty
}
bool Output::calculate()
{
    return getData();
}
#endif