#ifndef DUNGEON_H_INCLUDED
#define DUNGEON_H_INCLUDED

// #define SEE_WHOLE_MAP

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <random>
#include <fstream>
#include <sstream>
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Room.h"

using namespace std;

class Dungeon {
private:
    Player player;
    vector<Room> rooms;
    
    int map_row, map_col;
    int maxMonsterNumber, maxChestNumber;
    int currentMonsterNumber, currentChestNumber;
    Room *boss_room;
    
    random_device rd;
public:
    Dungeon() { 
        map_row = 4; map_col = 5; 
        currentMonsterNumber = maxMonsterNumber = 3;
        currentChestNumber = maxChestNumber = 3;
    }
    ~Dungeon() {}

    int randomInt(const int, const int);
    int getRandomRoomNumber();
    int inputFilter(const int);
    
    /* Create a new player, and give him/her basic status */
    void createPlayer();

    /* Create a map, which include several different rooms */
    void createMap();

    void createNPC();
    void createMonster();
    void createChest(int);
    
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

class Record {
private:
    void saveDungoen(Dungeon*, ofstream&);
    void savePlayer(Player&, ofstream&);
    void saveRooms(vector<Room>&, ofstream&);
    void saveMonster(Monster*, ofstream&);
    void saveItem(Item*, ofstream&);
    void saveNPC(NPC*, ofstream&);
    void loadDungoen(Dungeon*, vector<Room>&, ifstream&);
    void loadPlayer(Player&, vector<Room>&, ifstream&);
    void loadRooms(vector<Room>&, ifstream&);
    Monster* loadMonster(stringstream&, ifstream&);
    Item* loadItem(stringstream&, ifstream&);
    NPC* loadNPC(stringstream&, ifstream&);
public:
    Record() {}
    void saveToFile(Dungeon*, Player&, vector<Room>&);
    bool loadFromFile(Dungeon*, Player&, vector<Room>&);
};

#endif // DUNGEON_H_INCLUDED