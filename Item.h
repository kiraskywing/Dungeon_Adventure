#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "Object.h"
#include "Player.h"

using namespace std;

class player;

class Item: public Object {
private:
    int health, attack, money, criticalAttackRate;
public:
    Item(string name="", int hp=0, int atk=0, int mny=0, int car=0)
        : Object(name, "Item"), health(hp), attack(atk), money(mny), criticalAttackRate(car) {}
    virtual ~Item() {}

    /* Virtual function that you need to complete    */
    /* In Item, this function should deal with the   */
    /* pick up action. You should add status to the  */
    /* player.                                       */
    virtual bool triggerEvent(Object*) override;

    /* Set & Get function*/
    void setHealth(int hp) { health = hp; }
    void setAttack(int atk) { attack = atk; }
    void setMoney(int mny) { money = mny; }
    void setCriticalAttackRate(int r) { criticalAttackRate = r; }
    int getHealth() { return health; }
    int getAttack() { return attack; }
    int getMoney() { return money; }
    int getCriticalAttackRate() { return criticalAttackRate; }
};

#endif // ITEM_H_INCLUDED
