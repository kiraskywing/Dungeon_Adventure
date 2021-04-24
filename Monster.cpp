#include "Monster.h"
using namespace std;

bool Monster::triggerEvent(Object* obj) {
    Player* p = dynamic_cast<Player*>(obj);
    string monName = getName();
    int i;
    cout << endl << "You encouter a \"" << monName << "\"";
    while (!this->checkIsDead() && !p->checkIsDead()) {
        cout << endl << "Choose one action:" << endl
                << "(a) Attack enemy" << endl
                << "(b) Check status" << endl
                << "(c) Use inventory" << endl
                << "(d) Retreat" << endl
                << "Enter: ";
        i = inputFilter(4);
        
        if (i == 0) {
            cout << endl;
            attackFunction(p, this);
            if (!this->checkIsDead()) 
                attackFunction(this, p);
            else {
                cout << "You beat " << monName << "!" << endl;
                p->updateStatus(0, 0, this->getMoney(), 0);
            }
        }
        else if (i == 1) obj->triggerEvent(nullptr);
        else if (i == 2) p->useInventory();
        else { if (retreatFunction(p, this)) return false; }
    }

    return true;
}

bool Monster::oddFunction(int rate) {
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator) <= rate;
}

void Monster::attackFunction(GameCharacter* a, GameCharacter* b) {
    string aName = a->getName(), bName = b->getName(), bTag = b->getTag();
    bool isCritical = oddFunction(a->getCriticalAttackRate());
    int atk = a->getAttack() * (isCritical ? 2 : 1);
    cout << aName <<"'s attack does " << b->takeDamage(atk)
         << (isCritical ? " \"Critical\" " : " ") << "damage" << endl;
    if (bTag == "Player") { cout << "Your health: " << b->getCurrentHealth() << endl; }
    else { 
        cout << bName << "'s health: " << fixed << setprecision(1) 
             << static_cast<double>(b->getCurrentHealth()) / b->getMaxHealth() * 100 << "%" << endl;
    }
}

bool Monster::retreatFunction(Player* p, Monster* m) {
    int rate = (m->getName() == "Boss" ? 25 : 50);
    bool retreatSuccess = oddFunction(rate);
    if (retreatSuccess) {
        cout << endl << "Retreat success!" << endl;
        return true;
    }
    else {
        cout << endl << "Retreat fail!" << endl << endl;
        attackFunction(m, p);
        return false;
    }
}