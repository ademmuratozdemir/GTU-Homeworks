#ifndef HUMANIC_GUARD
#define HUMANIC_GUARD

#include <iostream>
#include "Robot.h"

class Humanic : public Robot
{
    protected:
        Humanic();
        Humanic(int newType, int newStrength, int newHit);
        int getDamage();
        
};
#endif