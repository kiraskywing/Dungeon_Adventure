#include "Record.h"

void Record::saveToFile(Dungeon* d, Player& p, vector<Room*>& rms) {
    ofstream outFile("savings.txt");
    if (!outFile.is_open()) {
        cout << "Error opening save file" << endl;
        return;
    }
    saveDungoen(d, outFile);
    savePlayer(p, outFile);
    saveRooms(rms, outFile);
    outFile.close();
    cout << endl << "Save success!";
    inputOptimizer(0, "pause");
}

void Record::saveDungoen(Dungeon* d, ofstream& ofs) {
    ofs << "Dungeon" << ' ' 
        << d->getMapRow() << ' ' << d->getMapCol() << ' ' 
        << d->getCurMonNumber() << ' ' << d->getCurChestNumber() << ' ' 
        << d->getBossRoom()->getIndex() << endl;
}

void Record::savePlayer(Player& p, ofstream& ofs) {
    const vector<Item*>& pInv = p.getBackpack();
    ofs << p.getTag() << ' ' << p.getName() << ' '
        << p.getMaxHealth() << ' ' << p.getCurrentHealth() << ' '
        << p.getAttack() << ' ' << p.getMoney() << ' ' << p.getCriticalAttackRate() << ' '
        << p.getCurrentRoom()->getIndex() << ' ' << p.getPreviousRoom()->getIndex() << ' '
        << p.getWeaponName() << ' ' << p.getWeaponAttack() << ' ' << p.getAddedAttack() << ' ' 
        << p.getLevel() << ' ' << p.getBackpackSize() << ' ' << pInv.size() << endl;
    
    for (Item* itm : pInv) saveItem(itm, ofs);
}

void Record::saveRooms(vector<Room*>& rms, ofstream& ofs) {
    Object* obj = nullptr;
    Monster* mon = nullptr;
    Item* itm = nullptr;
    NPC* npc = nullptr;
    for (Room* rm : rms) {
        ofs << "Room" << ' ' << rm->getIndex() << ' ' 
            << rm->getIsVisited() << ' ' << rm->getIsExit() << ' '
            << (rm->getUpRoom() ? rm->getUpRoom()->getIndex() : -1) << ' ' 
            << (rm->getDownRoom() ? rm->getDownRoom()->getIndex() : -1) << ' ' 
            << (rm->getLeftRoom() ? rm->getLeftRoom()->getIndex() : -1) << ' ' 
            << (rm->getRightRoom() ? rm->getRightRoom()->getIndex() : -1) << ' ';
        
        obj = rm->getObject();
        mon = dynamic_cast<Monster*>(obj);
        itm = dynamic_cast<Item*>(obj);
        npc = dynamic_cast<NPC*>(obj);
        ofs << (mon ? 1 : 0) << ' ' << (itm ? 1 : 0) << ' ' << (npc ? 1 : 0) << ' ' << endl;
        if (mon) saveMonster(mon, ofs);
        if (itm) saveItem(itm, ofs);
        if (npc) saveNPC(npc, ofs);
    }
}

void Record::saveMonster(Monster* mon, ofstream& ofs) {
    ofs << mon->getTag() << ' ' << mon->getName() << ' '
        << mon->getMaxHealth() << ' ' << mon->getCurrentHealth() << ' '
        << mon->getAttack() << ' ' << mon->getMoney() << ' ' 
        << mon->getCriticalAttackRate() << endl;
}

void Record::saveItem(Item* itm, ofstream& ofs) {
    ofs << itm->getTag() << ' ' << itm->getName() << ' ' 
        << itm->getHealth() << ' ' << itm->getAttack() << ' ' 
        << itm->getMoney() << ' ' << itm->getCriticalAttackRate() << endl;
}

void Record::saveNPC(NPC* npc, ofstream& ofs) {
    const vector<Item*>& nCom = npc->getCommodity();
    ofs << npc->getTag() << ' ' << npc->getName() << ' ' << nCom.size() << endl;
    
    for (Item* itm : nCom) saveItem(itm, ofs);
}

bool Record::loadFromFile(Dungeon* d, Player& p, vector<Room*>& rms) {
    ifstream inFile("savings.txt");
    if (!inFile.is_open()) {
        cout << "No save file or error opening save file" << endl
             << "Please enter again: ";
        return false;
    }

    loadDungoen(d, rms, inFile);
    loadPlayer(p, rms, inFile);
    loadRooms(rms, inFile);
    inFile.close();
    
    cout << endl << "Load success!";
    inputOptimizer(0, "pause");
    return true;
}

void Record::loadDungoen(Dungeon* d, vector<Room*>& rms, ifstream& ifs) {
    string line, name;
    getline(ifs, line);
    stringstream iss(line);
    int r, c, curMon, curCh, n_rooms, bRM;
    iss >> name >> r >> c >> curMon >> curCh >> bRM;
    
    // cout << name << ' ' << r << ' ' << c << ' ' << curMon << ' ' 
    //      << curCh << ' ' << bRM << endl;
    
    n_rooms = r * c;
    d->setCurMonNumber(curMon); 
    d->setCurChestNumber(curCh);
    Room* rm = nullptr;
    for (Room* rm : rms) { delete rm; }
    rms.clear();
    for (int i = 0; i < n_rooms; i++) {
        rm = new Room(i, false);
        rms.push_back(rm);
    }
    d->setBossRoom(rms[bRM]);
}

void Record::loadPlayer(Player& p, vector<Room*>& rms, ifstream& ifs) {
    string line, pName, wName;
    getline(ifs, line);
    stringstream ss(line);
    int maxHp, curHp, atk, money, car, cRM, pRM, wAtk, addedAtk, lv, maxN, curN;
    ss >> pName >> pName >> maxHp >> curHp >> atk >> money 
       >> car >> cRM >> pRM >> wName >> wAtk >> addedAtk >> lv >> maxN >> curN;
    p.setName(pName); p.setMaxHealth(maxHp); p.setCurrentHealth(curHp);
    p.setAttack(atk); p.setMoney(money); p.setCriticalAttackRate(car);
    p.setCurrentRoom(rms[cRM]); p.setPreviousRoom(rms[pRM]);
    p.setWeaponName(wName); p.setWeaponAttack(wAtk); p.setAddedAttack(addedAtk);
    p.setLevel(lv); p.setBackpackSize(maxN);
    
    // cout << pName << ' ' << maxHp << ' ' << curHp << ' ' << atk << ' ' << money << ' ' 
    //      << car << ' ' << cRM << ' ' << pRM << ' ' << wName << ' ' << wAtk << ' ' 
    //      << addedAtk << ' ' << lv << ' ' << maxN << ' ' << curN << endl;
    
    vector<Item*> itms;
    vector<Item*>& backpack = p.getBackpack();
    for (Item* itm : backpack) { delete itm; }
    backpack.clear();
    for (int i = 0; i < curN; i++) 
        itms.push_back(loadItem(ss, ifs));
    p.setBackpack(itms);
}

void Record::loadRooms(vector<Room*>& rms, ifstream& ifs) {
    string line, tag;
    stringstream ss;
    int idx, up, down, left, right;
    bool iVt, iEt, hasMon, hasItm, hasNpc; 
    for (Room* rm : rms) {
        getline(ifs, line);
        ss.str(""); ss.clear(); ss << line;
        ss >> tag >> idx >> iVt >> iEt >> up >> down >> left >> right 
           >> hasMon >> hasItm >> hasNpc;
        
        // rm->setIndex(idx);
        rm->setIsVisited(iVt); rm->setIsExit(iEt);
        if (up != -1) rm->setUpRoom(rms[up]); 
        if (down != -1) rm->setDownRoom(rms[down]); 
        if (left != - 1) rm->setLeftRoom(rms[left]); 
        if (right != -1) rm->setRightRoom(rms[right]);

        // cout << tag << ' ' << idx << ' ' << iVt << ' ' << iEt << ' ' 
        //      << up << ' ' << down << ' ' << left << ' ' << right << ' ' 
        //      << hasMon << ' ' << hasItm << ' ' << hasNpc << ' ' << endl;
        
        // if (hasMon) loadMonster(ss, ifs);
        // if (hasItm) loadItem(ss, ifs);
        // if (hasNpc) loadNPC(ss, ifs);
        if (hasMon) rm->setObject(loadMonster(ss, ifs));
        if (hasItm) rm->setObject(loadItem(ss, ifs));
        if (hasNpc) rm->setObject(loadNPC(ss, ifs));
    }
}

Monster* Record::loadMonster(stringstream& ss, ifstream& ifs) {
    Monster* mon = nullptr;
    string line, tag, name;
    int maxHp, curHp, atk, money, car;
    getline(ifs, line);
    ss.str(""); ss.clear(); ss << line;
    ss >> tag >> name >> maxHp >> curHp >> atk >> money >> car;
    mon = new Monster(name, maxHp, curHp, atk, money, car);

    // cout << tag << ' ' << name << ' ' << maxHp << ' ' << curHp << ' ' 
    //      << atk << ' ' << money << ' ' << car << endl;
    
    return mon;
}

Item* Record::loadItem(stringstream& ss, ifstream& ifs) {
    Item* itm = nullptr;
    string line, tag, name;
    int hp, atk, money, car;
    getline(ifs, line);
    ss.str(""); ss.clear(); ss << line;
    ss >> tag >> name >> hp >> atk >> money >> car;
    itm = new Item(name, hp, atk, money, car);

    // cout << tag << ' ' << name << ' ' << hp << ' ' << atk << ' ' << money << ' ' << car << endl;
    
    return itm;
}

NPC* Record::loadNPC(stringstream& ss, ifstream& ifs) {
    NPC* npc = nullptr;
    string line, tag, name;
    int n_itms;
    getline(ifs, line);
    ss.str(""); ss.clear(); ss << line;
    ss >> tag >> name >> n_itms;
    npc = new NPC(name);

    // cout << tag << ' ' << name << ' ' << n_itms << endl;

    vector<Item*> itms;
    for (int i = 0; i < n_itms; i++) 
        itms.push_back(loadItem(ss, ifs));
    npc->setCommodity(itms);

    return npc;
}