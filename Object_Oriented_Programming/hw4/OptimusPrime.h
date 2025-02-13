#ifndef OPTIMUSPRIME_GUARD
#define OPTIMUSPRIME_GUARD

#include <iostream>
#include "Humanic.h"

class OptimusPrime : public Humanic
{
    public:
        OptimusPrime();
        OptimusPrime(int newType, int newStrength, int newHit);
        int getDamage();
        
};
#endif