#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
#include "Item.h"

using namespace std;

class NPC: public GameCharacter {
private:
    vector<Item*> commodity;
public:
    NPC(string name="") : GameCharacter(name, "NPC") { commodity.reserve(6); }
    virtual ~NPC() { for (Item* obj : commodity) { delete obj; obj = nullptr; } }
    
    int chooseCommodity(Player*);
    void buyCommodity(Player*, int);
    
    /* Virtual function that you need to complete   */
    /* In NPC, this function should deal with the   */
    /* transaction in easy implementation           */
    virtual bool triggerEvent(Object*) override;

    /* Set & Get function*/
    void setCommodity(const vector<Item*>& itms) { commodity.insert(commodity.end(), itms.begin(), itms.end()); }
    vector<Item*>& getCommodity() { return commodity; }
};

#endif // NPC_H_INCLUDED
