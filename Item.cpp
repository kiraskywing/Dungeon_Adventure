#include "Item.h"

bool Item::triggerEvent(Object *obj) {
    Player* p = dynamic_cast<Player*>(obj);
    string itmName = getName();
    
    if (itmName.find("Sword") != string::npos) {
        if (itmName == p->getWeaponName()) {
            cout << endl << "You already have " << itmName << endl;
            return false;
        }
        
        cout << endl << "You got " << itmName << endl;
        int preAtk = p->getAttack(), preAddAtk = p->getAddedAttack(), preWeaponAtk = p->getWeaponAttack();
        p->setWeaponName(itmName);
        p->setWeaponAttack(attack);
        p->setAttack(preAtk - preWeaponAtk); 
        p->setAddedAttack(preAddAtk - preWeaponAtk);
    }
    else if (itmName.find("Posion") != string::npos) 
        cout << endl << "You choose to use " << itmName << endl;
    else 
        cout << endl << "You got " << itmName << endl;

    return p->updateStatus(health, attack, money, criticalAttackRate);
}