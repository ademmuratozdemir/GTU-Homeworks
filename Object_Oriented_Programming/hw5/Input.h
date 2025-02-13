#ifndef INPUT_H
#define INPUT_H
#include "Gate.h"
class Input : public Gate
{
public:
    Input();
    Input(string id);
    virtual bool calculate();
};

Input::Input()
    : Gate()
{
    // Intentionally empty
}
Input::Input(string id)
    : Gate(id)
{
    // Intentionally empty
}
bool Input::calculate()
{
    return getData();
}
#endif