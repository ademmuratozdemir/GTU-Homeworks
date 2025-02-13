#ifndef BULLDOZER_GUARD
#define BULLDOZER_GUARD

#include <iostream>
#include "Robot.h"

class Bulldozer : public Robot
{
    protected:
        Bulldozer();
        Bulldozer(int newType, int newStrength, int newHit);
        int getDamage();
        
};
#endif