#include "Player.h"

Player::Player(string name, int hp, int atk, int addAtk, int wpAtk, int mny, int car,  int lv)
    : GameCharacter(name, "Player", hp, hp, atk + addAtk + wpAtk, mny, car) {
           currentRoom = previousRoom = nullptr;
           weaponName = "Sword_lv.0";
           backpackMaxSize = 6; addedAttack = addAtk; weaponAttack = wpAtk; level = lv;
           backpack.reserve(backpackMaxSize);
        }  
Player:: ~Player() { for (Item* itm : backpack) { delete itm; itm = nullptr; } }

bool Player::updateStatus(int hp, int addAtk, int mny, int car) {
    if (hp) {
        int pre_hp = getCurrentHealth(), max_hp = getMaxHealth();
        if (pre_hp == max_hp) {
            cout << "Sorry, your health is already full!" << endl;
            return false;
        }
        hp = (hp + pre_hp >= max_hp ? max_hp :hp + pre_hp);
        setCurrentHealth(hp);
        cout << "Your current health: " << hp << endl;
    }
    if (addAtk) {
        addedAttack += addAtk;
        addAtk += getAttack();
        setAttack(addAtk);
        cout << "Your current attack: " << addAtk << endl;
    }
    if (mny) {
        cout << "You " << (mny > 0 ? "received" : "spent") << " $" << (mny > 0 ? mny : -mny) << endl;
        mny += getMoney();
        setMoney(mny);
    }
    if (car) {
        setCriticalAttackRate(car);
        cout << "Your crtical attack rate is set to " << car << '%' << endl;
    }
    return true;
}

void Player::changeRoom(Room* nxt) {
    previousRoom = currentRoom;
    currentRoom = nxt;
}

bool Player::triggerEvent(Object* obj) {
    cout << endl << "[Player " << getName() << "'s status]" << endl;
    cout << "=> Level: " << level << endl;
    cout << "=> Health: " << getCurrentHealth() << "/" << getMaxHealth() << endl;
    cout << "=> Attack: " << getAttack() << endl;
    cout << "=> Critical attack rate: " << getCriticalAttackRate() << '%' << endl;
    cout << "=> Money: $" << getMoney() << endl;
    cout << "=> Weapon: " << weaponName << endl;
    cout << "=> Number of potions: " << backpack.size() << endl;
    return true;
}

void Player::levelUp() {
    cout << endl << "******* Level up! *******";
    level++;
    int maxHP = getMaxHealth(), curHP = getCurrentHealth();
    maxHP *= 2; curHP = maxHP;
    setMaxHealth(maxHP); setCurrentHealth(curHP);

    int atk = getAttack();
    atk = (atk - addedAttack) * 2 + addedAttack;
    setAttack(atk);

    triggerEvent(nullptr);
}

void Player::useBackpack() {
    while (true) {
        if (backpack.empty()) {
            cout << endl << "No item." << endl;
            return;
        }
        
        cout << endl << "Choose one item: ";
        Item* itm;
        int j, n = backpack.size();
        for (j = 0; j < n; j++) {
            itm = backpack[j];
            cout << endl << "(" << (char)('a'+j) << ") " << itm->getName() 
                 << ": Recover " << itm->getHealth() << " Health";
        }
        cout << endl << "(" << (char)('a'+j) << ") " << "Back"
             << endl << "Enter: ";
        int i = inputOptimizer(n + 1);
        if (i == n) return;

        cout << endl << "Choose use, discard or go back: "
             << endl << "(a) Use" 
             << endl << "(b) Discard"
             << endl << "(c) Back"
             << endl << "Enter: ";
        j = inputOptimizer(3);
        if (j == 2) return;

        itm = backpack[i];
        if ((j == 0 && itm->triggerEvent(this)) || j == 1) {
            delete backpack[i];
            if (i != n - 1) {
                for (int k = i; k < n - 1; k++)
                    backpack[k] = backpack[k + 1];
            }
            backpack.pop_back();
        }
        inputOptimizer(0, "pause");
    }
}