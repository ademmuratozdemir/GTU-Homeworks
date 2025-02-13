#ifndef ROBOCOP_GUARD
#define ROBOCOP_GUARD

#include <iostream>
#include "Humanic.h"

class Robocop : public Humanic
{
    public:
        Robocop();
        Robocop(int newType, int newStrength, int newHit);
        int getDamage();
        
};
#endif