#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "Object.h"

using namespace std;

class Room {
private:
    Room *upRoom, *downRoom, *leftRoom, *rightRoom;
    int index;
    bool isVisited, isExit;
    Object* object; /*contain only 1 object, including monster, npc, etc*/
public:
    Room(int idx=0, bool iVt=false, bool iEt=false, Object* obj=nullptr) {
        leftRoom = rightRoom = upRoom = downRoom = nullptr;
        isVisited = iVt; isExit = iEt; index = idx; 
        object = obj;
    }
    ~Room() { popObject(); }
    
    /*pop out the specific object, used when the interaction is done*/
    void popObject() { delete object; object = nullptr; } 

    /* Set & Get function*/
    void setIndex(int idx) { index = idx; }
    void setIsVisited(bool iVt) { isVisited = iVt; }
    void setIsExit(bool iEt) { isExit = iEt; }
    void setUpRoom(Room* up) { upRoom = up; }
    void setDownRoom(Room* down) { downRoom = down; }
    void setLeftRoom(Room* left) { leftRoom = left; }
    void setRightRoom(Room* right) { rightRoom = right; }
    void setObject(Object* obj) { object = obj; }
    int getIndex() { return index; }
    bool getIsVisited() { return isVisited; }
    bool getIsExit() { return isExit; }
    Room* getUpRoom() { return upRoom; }
    Room* getDownRoom() { return downRoom; }
    Room* getLeftRoom() { return leftRoom; }
    Room* getRightRoom() { return rightRoom; }
    Object* getObject() { return object; }
};

#endif // ROOM_H_INCLUDED
