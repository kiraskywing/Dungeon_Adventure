#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include "Player.h"

using namespace std;

int randomInt(const int, const int);
bool oddFunction(const int);

class Monster: public GameCharacter {
public:
    Monster(string name="", int maxHp=100, int curHp=100, int atk=100, int mny=100, int car=10)
        :GameCharacter(name, "Monster", maxHp, curHp, atk, mny, car) {}
    virtual ~Monster() {}

    /* Virtual function that you need to complete   */
    /* In Monster, this function should deal with   */
    /* the combat system.                           */
    virtual bool triggerEvent(Object*) override;
    
    void attackFunction(GameCharacter*, GameCharacter*);
    bool retreatFunction(Player*, Monster*);
};

#endif // ENEMY_H_INCLUDED
