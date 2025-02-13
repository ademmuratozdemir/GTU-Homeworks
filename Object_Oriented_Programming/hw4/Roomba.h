#ifndef ROOMBA_GUARD
#define ROOMBA_GUARD

#include <iostream>
#include "Robot.h"

class Roomba : public Robot
{
    protected:
        Roomba();
        Roomba(int newType, int newStrength, int newHit);
        int getDamage();
        
};
#endif