#ifndef DUNGEON_H_INCLUDED
#define DUNGEON_H_INCLUDED

// #define SEE_WHOLE_MAP

#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Room.h"
#include "Record.h"

using namespace std;

class Record;

class Dungeon {
private:
    Player player;
    vector<Room*> rooms;
    int map_row, map_col;
    int maxMonsterNumber, maxChestNumber;
    int currentMonsterNumber, currentChestNumber;
    Room *boss_room;
public:
    Dungeon() { 
        map_row = 4; map_col = 5;
        maxMonsterNumber = 3;
        maxChestNumber = 3;
        rooms.reserve(map_row * map_col);
    }
    ~Dungeon() { for (Room* rm : rooms) { delete rm; rm = nullptr; } }

    int getRandomRoomNumber();
    
    /* Create a new player, and give him/her basic status */
    void createPlayer();

    /* Create a map, which include several different rooms */
    void createMap();

    void createNPC();
    void createMonster();
    void createChest();
    
    /* Deal with player's moveing action */
    void handleMovement();

    void showMap();

    /* Deal with all game initial setting       */
    /* Including create player, create map etc  */
    void startGame();

    /* Deal with the player's action     */
    /* including showing the action list */
    /* that player can do at that room   */
    /* and dealing with player's input   */
    void chooseAction();

    /* Check whether the game should end or not */
    /* Including player victory, or he/she dead */
    bool checkGameLogic();

    /* Deal with the whole game process */
    void runDungeon();

    /* getters and setters */
    void setCurMonNumber(int n) { currentMonsterNumber = n; }
    void setCurChestNumber(int n) { currentChestNumber = n; }
    void setBossRoom(Room* rm) { boss_room = rm; }
    int getMapRow() { return map_row; }
    int getMapCol() { return map_col; }
    int getCurMonNumber() { return currentMonsterNumber; }
    int getCurChestNumber() { return currentChestNumber; }
    Room* getBossRoom() { return boss_room; }
};

#endif // DUNGEON_H_INCLUDED