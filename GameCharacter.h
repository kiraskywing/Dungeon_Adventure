#ifndef GAMECHARACTER_H_INCLUDED
#define GAMECHARACTER_H_INCLUDED

#include <iostream>
#include <string>
#include <cctype>
#include "Object.h"

using namespace std;

class GameCharacter: public Object {
private:
    int maxHealth, currentHealth, attack, money, criticalAttackRate;
public:
    GameCharacter(string name="", string tag="", int maxHp=0, int curHp=0, int atk=0, int mny=0, int car=0)
        : Object(name, tag), maxHealth(maxHp), currentHealth(curHp), attack(atk), money(mny), criticalAttackRate(car) {}
    virtual ~GameCharacter() {}
    
    int inputFilter(const int);
    
    virtual bool triggerEvent(Object*) override { return true; }
    
    bool checkIsDead() { return currentHealth <= 0; }
    int takeDamage(int dmg) { currentHealth -= (dmg > currentHealth ? currentHealth : dmg); return dmg; }

    /* Set & Get function*/
    void setMaxHealth(int maxHp) { maxHealth = maxHp; }
    void setCurrentHealth(int hp) { currentHealth = hp; }
    void setAttack(int atk) { attack = atk; }
    void setMoney(int mny) { money = mny; }
    void setCriticalAttackRate(int car) { criticalAttackRate = car; }
    int getMaxHealth() { return maxHealth; }
    int getCurrentHealth() { return currentHealth; }
    int getAttack() { return attack; }
    int getMoney() { return money; }
    int getCriticalAttackRate() { return criticalAttackRate; }
};

#endif // GAMECHARACTER_H_INCLUDED