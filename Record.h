#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Dungeon.h"

using namespace std;

class Dungeon;

class Record {
private:
    void saveDungoen(Dungeon*, ofstream&);
    void savePlayer(Player&, ofstream&);
    void saveRooms(vector<Room*>&, ofstream&);
    void saveMonster(Monster*, ofstream&);
    void saveItem(Item*, ofstream&);
    void saveNPC(NPC*, ofstream&);
    void loadDungoen(Dungeon*, vector<Room*>&, ifstream&);
    void loadPlayer(Player&, vector<Room*>&, ifstream&);
    void loadRooms(vector<Room*>&, ifstream&);
    Monster* loadMonster(stringstream&, ifstream&);
    Item* loadItem(stringstream&, ifstream&);
    NPC* loadNPC(stringstream&, ifstream&);
public:
    Record() {}
    ~Record() {}
    void saveToFile(Dungeon*, Player&, vector<Room*>&);
    bool loadFromFile(Dungeon*, Player&, vector<Room*>&);
};

#endif // RECORD_H_INCLUDED