#include "NPC.h"

bool NPC::triggerEvent(Object* obj) {
    Player* p = dynamic_cast<Player*>(obj);
    cout << endl << "Welcome to " << getName() << ", please choose action\n"
         << "(a) Buy item\n"
         << "(b) Leave\n"
         << "Enter: ";
    int i = inputOptimizer(2);
    
    if (i == 0) {
        while (true) {
            i = chooseCommodity(p);
            if (i == commodity.size()) break;
            buyCommodity(p, i);
        }
    }

    return false;
}

int NPC::chooseCommodity(Player* p) {
    Item* itm = nullptr;
    string iName;
    int n = commodity.size();
    int cur_money = p->getMoney();
    int cur_n = p->getBackpackSize() - p->getBackpack().size();
    cout << endl << "Your money: " << cur_money 
         << endl << "Number of empty slots in your backpack: " << cur_n << endl
                 << "Please choose one item to buy: " << endl;
    for (int j = 0; j < n; j++) {
        itm = commodity[j];
        iName = itm->getName();
        cout << "(" << (char)('a'+j) << ") " << iName << ", ";
        if (iName.find("Potion") != string::npos) 
            cout << "recover health: " << itm->getHealth() << ", ";
        else {
            cout << "weapon attack: " << itm->getAttack() << ", "
                 << "critical attack rate: " << itm->getCriticalAttackRate() << ", ";
        }
            cout << "price: " << (iName.find("Potion") != string::npos ? 200 : 400) * (1 + 3 * (j % 3)) << endl; 
    }
            cout << "(" << (char)('a'+n) << ") " << "Leave" << endl
                 << "Enter: ";
    return inputOptimizer(n+1);
}

void NPC::buyCommodity(Player* p, int i) {
    Item* itm = commodity[i];
    string iName = itm->getName();
    int price = (iName.find("Potion") != string::npos ? 200 : 400) * (1 + 3 * (i % 3));
    int cur_money = p->getMoney();
    int cur_n = p->getBackpackSize() - p->getBackpack().size();
    
    if (cur_money < price) {
        cout << endl << "You don't have enough money." << endl;
        inputOptimizer(0, "pause");
        return;
    }
    
    if (iName.find("Potion") == string::npos && itm->triggerEvent(p)) {
        p->updateStatus(0, 0, -price, 0);
    }
    else if (iName.find("Potion") != string::npos) {
        int n_maxBuy = cur_money / price;
        n_maxBuy = (cur_n < n_maxBuy ? cur_n : n_maxBuy);
        if (n_maxBuy == 0) {
            cout << endl << "Your backpack is full." << endl;
            inputOptimizer(0, "pause");
            return;
        }

        cout << endl << "How many potions do you want to buy?" << endl
                     << "Enter number: (max: " << n_maxBuy << ", 0 to cancel)" << endl
                     << "Enter: ";
        i = inputOptimizer(n_maxBuy + 1);

        if (i == 0) return;
        
        vector<Item*> objs;
        Item* obj;
        for (int k = 0; k < i; k++) {
            obj = new Item();
            *obj = *itm;
            objs.push_back(obj);
        }
        p->setBackpack(objs);
        cout << endl << "You got " << i << " " << itm->getName() << endl;
        p->updateStatus(0, 0, -price * i, 0);
    }
    inputOptimizer(0, "pause");
}