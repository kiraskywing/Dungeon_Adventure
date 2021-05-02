#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "GameCharacter.h"
#include "Item.h"
#include "Room.h"

using namespace std;

class Item;

class Player: public GameCharacter {
private:
    Room *currentRoom, *previousRoom;
    vector<Item*> backpack;
    string weaponName;
    int backpackMaxSize, addedAttack, weaponAttack, level;
public:
    Player(string name="", int hp=250, int atk=50, int addAtk=0, int wpAtk=50, int mny=1000, int car=10, int lv=1);
    virtual ~Player();

    bool updateStatus(int hp,int atk, int mny, int car);
    void changeRoom(Room*);
    void levelUp();
    void useBackpack();

    /* Virtual function that you need to complete   */
    /* In Player, this function should show the     */
    /* status of player.                            */
    virtual bool triggerEvent(Object*) override;

    /* Set & Get function*/
    void setCurrentRoom(Room* rm) { currentRoom = rm; }
    void setPreviousRoom(Room* rm) { previousRoom = rm; }
    void setWeaponName(string s) {weaponName = s; }
    void setWeaponAttack(int atk) { weaponAttack = atk; }
    void setAddedAttack(int atk) { addedAttack = atk; }
    void setLevel(int lv) { level = lv; }
    void setBackpackSize(int n) { backpackMaxSize = n; }
    void setBackpack(const vector<Item*>& itms) { backpack.insert(backpack.end(), itms.begin(), itms.end()); }
    Room* getCurrentRoom() { return currentRoom; }
    Room* getPreviousRoom() { return previousRoom; }
    string getWeaponName() { return weaponName; }
    int getWeaponAttack() { return weaponAttack; }
    int getAddedAttack() { return addedAttack; }
    int getLevel() { return level; }
    int getBackpackSize() { return backpackMaxSize; }
    vector<Item*>& getBackpack() { return backpack; }
};

#endif // PLAYER_H_INCLUDED
