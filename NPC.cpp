#include "NPC.h"

using namespace std;

bool NPC::triggerEvent(Object* obj) {
    Player* p = dynamic_cast<Player*>(obj);
    cout << endl << "Welcome to " << getName() << ", please choose action\n"
         << "(a) Buy item\n"
         << "(b) Leave\n"
         << "Enter: ";
    
    int i = inputFilter(2);
    
    if (i == 0) {
        const vector<Object*>& pInv = p->getInventory();
        Item* itm = nullptr;
        string iName;
        while (true) {
            int j, n = commodity.size();
            int cur_money = p->getMoney();
            int cur_n = p->getInventorySize() - pInv.size();
            cout << endl << "Your money: " << cur_money 
                 << endl << "Number of empty slots in your inventory: " << cur_n << endl
                 << "Please choose one item to buy: " << endl;
            for (j = 0; j < n; j++) {
                itm = dynamic_cast<Item*>(commodity[j]);
                iName = itm->getName();
                cout << "(" << (char)('a'+j) << ") " << iName << ", ";
                if (iName.find("Posion") != string::npos) 
                    cout << "recover health: " << itm->getHealth() << ", ";
                else {
                    cout << "weapon attack: " << itm->getAttack() 
                         << ", critical attack rate: " << itm->getCriticalAttackRate() << ", ";
                }
                cout << "price: " << (iName.find("Posion") != string::npos ? 200 : 400) * (1 + 3 * (j % 3)) << endl; 
            }
            cout << "(" << (char)('a'+j) << ") " << "Leave" << endl
                 << "Enter: ";
            
            i = inputFilter(n + 1);
            
            if (i == n) break;
            
            itm = dynamic_cast<Item*>(commodity[i]);
            iName = itm->getName();
            int price = (iName.find("Posion") != string::npos ? 200 : 400) * (1 + 3 * (i % 3));
            
            if (cur_money < price) {
                cout << endl << "You don't have enough money." << endl;
                continue;
            }
            
            if (iName.find("Posion") == string::npos && itm->triggerEvent(p)) {
                p->updateStatus(0, 0, -price, 0);
            }
            else if (iName.find("Posion") != string::npos) {
                int n_maxBuy = cur_money / price;
                n_maxBuy = (cur_n < n_maxBuy ? cur_n : n_maxBuy);
                if (n_maxBuy == 0) {
                    cout << "Your inventory is full." << endl;
                    continue;
                }

                cout << endl << "How many posions do you want to buy?" << endl
                     << "Enter number: (max: " << n_maxBuy << ", 0 to cancel)" << endl
                     << "Enter: ";
                i = inputFilter(n_maxBuy + 1);

                if (i == 0) continue;
                vector<Object*> objs;
                Item* obj;
                for (int k = 0; k < i; k++) {
                    obj = new Item();
                    *obj = *itm;
                    objs.push_back(obj);
                }
                p->setInventory(objs);
                cout << endl << "You got " << i << " " << itm->getName() << endl;
                p->updateStatus(0, 0, -price * i, 0);
            }
        }
    }

    return false;
}

