#ifndef ROBOT_GUARD
#define ROBOT_GUARD

#include <iostream>

using namespace std;

class Robot 
{
    
    private:
        int robotType;
        int strength;
        int hitpoints;

    //a member data which defines the type 
    //a member data which stores the strength 
    //a member data which stores the hitpoints 
    //a helper function which returns the robot type 
    protected:
        Robot( ); 
    // Initialize to bulldozer, 10 strength, 10 hit points 
        Robot(int newType, int newStrength, int newHit); 
    // Initialize robot to new type, strength, hit points 
    // Also add appropriate accessor and mutator functions 
    // for type, strength, and hit points
        int getStrength();
        int getHitpoints();
        void setStrength(int newStrength);
        void setHitpoints(int newHit);
    
        int getDamage(); 
    // Returns amount of damage this robot
        string getType();

// inflicts in one round of combat

};




#endif


