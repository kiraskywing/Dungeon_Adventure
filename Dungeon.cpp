#include "Dungeon.h"

int Dungeon::getRandomRoomNumber() {
    int idx;
    Room* curRM = player.getCurrentRoom();
    do {
        idx = randomInt(0, map_row * map_col - 1);
    } while ((curRM && curRM->getIndex() == idx) || rooms[idx].getObject() || rooms[idx].getIsExit());

    return idx;
}

void Dungeon::createPlayer() {
    string s_name;
    cout << "Please enter player's name (without white space): ";
    cin >> s_name;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    
    player.setName(s_name);
    vector<Object*> itms;
    Item* obj = nullptr;
    for (int i = 0; i < player.getInventorySize(); i++) {
        obj = new Item("Posion_lv.0", 100);
        itms.push_back(obj);
    }
    player.setInventory(itms);
}

void Dungeon::createMap() {
    for (int i = 0; i < map_row * map_col; i++) {
        #ifdef SEE_WHOLE_MAP
        rooms.push_back(Room(i, true));
        #else
        rooms.push_back(Room(i, false));
        #endif
    }
    
    for (int i = 0; i < map_row; i++) {
        for (int j = 0; j < map_col; j++) {
            if (i > 0)           {rooms[j + i * map_col].setUpRoom(&rooms[j + (i - 1) * map_col]);}
            if (i + 1 < map_row) {rooms[j + i * map_col].setDownRoom(&rooms[j + (i + 1) * map_col]);}
            if (j > 0)           {rooms[j + i * map_col].setLeftRoom(&rooms[(j - 1) + i * map_col]);}
            if (j + 1 < map_col) {rooms[j + i * map_col].setRightRoom(&rooms[(j + 1) + i * map_col]);}
        }
    }

    /* set player's start position */
    int start_room = getRandomRoomNumber();
    player.setPreviousRoom(&rooms[start_room]);
    player.setCurrentRoom(&rooms[start_room]);
    rooms[start_room].setIsVisited(true);

    /* set boss' position */
    int bossRM = getRandomRoomNumber();
    boss_room = &rooms[bossRM];
    Monster* boss = new Monster("Boss", 5000, 5000, 1000, 1000, 20);
    rooms[bossRM].setObject(boss);

    /* set goal position */
    int exitRM = getRandomRoomNumber();
    rooms[exitRM].setIsExit(true);
    
    /* set NPC */
    createNPC();
    
    /* set Monsters */
    createMonster();
    
    /* set Treasure */
    createChest(maxChestNumber);
}

void Dungeon::createNPC() {
    NPC* npc = new NPC("ItemShop");
    vector<Object*> itms;
    Item* obj = nullptr;
    for (int i = 0; i < 3; i++) {
        obj = new Item(string("Sword_lv.") + to_string(i + 1), 0, 1000 * pow(2, i), 0, 20 * (1 + i));
        itms.push_back(obj);
    }
    for (int i = 0; i < 3; i++) {
        obj = new Item(string("Posion_lv.") + to_string(i + 1), 500 * pow(2, i));
        itms.push_back(obj);
    }
    npc->setCommodity(itms);
    int npcRM = getRandomRoomNumber();
    rooms[npcRM].setObject(npc);
}

void Dungeon::createMonster() {
    int lv = player.getLevel();
    int times = pow(2.5, lv - 1);
    int maxHp = 150 * times;
    int curHp = maxHp, atk = 100 * times, money = 100 * times, car = 10;
    int monRM;
    for (int i = 0; i < maxMonsterNumber; i++) {
        Monster* mon = new Monster(string("Slime_lv.") + to_string(lv), maxHp, curHp, atk, money, car);
        monRM = getRandomRoomNumber();
        rooms[monRM].setObject(mon);
    }
    currentMonsterNumber = maxMonsterNumber;
}

void Dungeon::createChest(const int n) {
    int lv = player.getLevel();
    int times = pow(2, lv - 1);
    Item* itm = nullptr;
    int type, iRM;
    for (int i = 0; i < n; i++) {
        type = randomInt(1, 3);
        if (type == 1) itm = new Item(string("Coins_lv.") + to_string(lv), 0, 0, 200 * times);
        if (type == 2) itm = new Item(string("AttackUp_lv.") + to_string(lv), 0, 90 * times);
        if (type == 3) itm = new Item(string("HealthRecover_lv.") + to_string(lv), 95 * times);
        iRM = getRandomRoomNumber();
        rooms[iRM].setObject(itm);
    }
    currentChestNumber = maxChestNumber;
}

void Dungeon::handleMovement() {
    cout << endl << "Where do you want to go?" << endl;
    vector<string> moves = {"Move Up", "Move Down", "Move Left", "Move Right"};
    
    Room* curRM = player.getCurrentRoom();
    
    int i, n = moves.size();
    for (i = 0; i < n; i++)
        cout << "(" << (char)('a'+i) << ") " << moves[i] << endl;
    cout << "Please choose one direction: ";

    while (true) {
        i = inputFilter(n);
        if (moves[i][5] == 'U' && curRM->getUpRoom()) {
            player.changeRoom(curRM->getUpRoom()); 
            break;
        }
        if (moves[i][5] == 'D' && curRM->getDownRoom()) {
            player.changeRoom(curRM->getDownRoom());
            break;
        }
        if (moves[i][5] == 'L' && curRM->getLeftRoom()) {
            player.changeRoom(curRM->getLeftRoom()); 
            break;
        }
        if (moves[i][5] == 'R' && curRM->getRightRoom()) {
            player.changeRoom(curRM->getRightRoom());
            break;
        }
        cout << "You can't go there. Please choose again: ";
    }
    
    curRM = player.getCurrentRoom();
    curRM->setIsVisited(true);
}

void Dungeon::showMap() {
    int pRM = player.getCurrentRoom()->getIndex();
    Object* obj = nullptr;
    Monster* mon = nullptr;
    Item* itm = nullptr;
    NPC* npc = nullptr;
    string line(map_col * 4 + 1, '-');
    
    cout << endl << "[MAP]";
    cout << endl << ' ' << line << endl;
    for (int i = 0; i < map_row; i++) {
        for (int j = 0; j < map_col; j++) {
            int cRM = j + i * map_col;
            cout << " | ";
            if (rooms[cRM].getIsVisited()) {
                obj = rooms[cRM].getObject();
                mon = dynamic_cast<Monster*>(obj);
                itm = dynamic_cast<Item*>(obj);
                npc = dynamic_cast<NPC*>(obj);

                if (cRM == pRM) cout << 'P';
                else if (mon) cout << (mon->getName() == "Boss" ? 'B' : 'M');
                else if (itm) cout << 'C';
                else if (npc) cout << 'S';
                else if (rooms[cRM].getIsExit()) cout << 'G';
                else cout << ' ';
            }
            else cout << '?';
        }
        cout << " | " << endl << ' ' << line << endl;
    }
    cout << "?: Not explored" << endl;
    cout << "P: Player, M: Monster, B: Boss" << endl;
    cout << "S: Shop, C: Chest, G: Goal" << endl;
}

void Dungeon::startGame() {
    Record rec;
    cout << "===== Simple Dungeon Game =====" << endl;
    cout << "(a) Start new game" << endl;
    cout << "(b) Load game" << endl;
    cout << "(c) Quit" << endl;
    cout << "Please choose one option: ";
    int i;
    while (true) {
        i = inputFilter(3);
        if (i == 0) { 
            createMap();
            createPlayer();
            break;
        }
        else if (i == 1) {
            if (rec.loadFromFile(this, player, rooms))
                break;
        }
        else { 
            cout << endl << "Goodbye!" << endl << endl;
            exit(1); 
        }
    }
}

void Dungeon::chooseAction() {
    // check items in current room and use handleEvent
    Record rec;
    vector<string> actions = {"Move", "Check status", "Use inventory"};
    Monster* mon = nullptr;
    Item* itm = nullptr;
    NPC* npc = nullptr;
    Room* curRM = player.getCurrentRoom();
    
    Object* obj = nullptr;
    obj = curRM->getObject();
    mon = dynamic_cast<Monster*>(obj);
    itm = dynamic_cast<Item*>(obj);
    npc = dynamic_cast<NPC*>(obj);
    
    if (obj) {
        string objName = obj->getName();
        if (obj->triggerEvent(&player)) {
            if (mon && objName != "Boss") 
                currentMonsterNumber--;
            if (player.checkIsDead()) 
                return;
            if (itm)                      
                currentChestNumber--;
            
            curRM->popObject();
            obj = nullptr;

            if (currentMonsterNumber == 0) {
                if (currentChestNumber < maxChestNumber) 
                    createChest(maxChestNumber - currentChestNumber);
                player.levelUp();
                createMonster();
                rec.saveToFile(this, player, rooms);
            }
        }
        else {
            if (objName.find("Health") == string::npos)
                player.changeRoom(player.getPreviousRoom());
        }
    }
    
    actions.push_back("Game options");
    
    showMap();

    int i;
    cout << endl << "Choose one action: ";
    for (i = 0; i < actions.size(); i++)
        cout << endl << "(" << (char)('a'+i) << ") " << actions[i];
    cout << endl << "Enter: ";

    i = inputFilter(actions.size());

    if (actions[i][0] == 'M') handleMovement();
    if (actions[i][0] == 'C') { player.triggerEvent(nullptr); showMap(); }
    if (actions[i][0] == 'U') { player.useInventory(); }
    if (actions[i][0] == 'G') {
        cout << endl << "Choose one action: "
             << endl << "(a) Save game"
             << endl << "(b) Load game"
             << endl << "(c) Quit"
             << endl << "Enter: ";
        
        i = inputFilter(3);

        if (i == 2) { cout << endl << "Goodbye!" << endl << endl; exit(1); }
        
        if (i == 0) rec.saveToFile(this, player, rooms);
        if (i == 1) rec.loadFromFile(this, player, rooms);
    }
}

bool Dungeon::checkGameLogic() {
    Room* curRM = player.getCurrentRoom();
    
    if (player.checkIsDead()) {
        cout << "You die!" << endl;
        return false;
    }
    if (curRM->getIsExit()) {
        Monster* mon = nullptr;
        Object* obj = boss_room->getObject();
        mon = dynamic_cast<Monster*>(obj);   
        if (mon && !mon->checkIsDead()) {
            cout << endl << "You reach the goal, but the Boss is still alive." << endl;
            return true;
        }
        
        cout << endl << "Congratulation! Game Clear!" << endl;
        return false;
    }

    return true;
}

void Dungeon::runDungeon() {
    startGame();
    do 
        chooseAction();
    while (checkGameLogic());
    cout << endl << "======= Game end =======" << endl << endl;
}


void Record::saveToFile(Dungeon* d, Player& p, vector<Room>& rms) {
    ofstream outFile("savings.txt");
    if (!outFile.is_open()) {
        cout << "Error opening save file" << endl;
        return;
    }
    saveDungoen(d, outFile);
    savePlayer(p, outFile);
    saveRooms(rms, outFile);
    outFile.close();
    cout << endl << "Save success!" << endl;
}

void Record::saveDungoen(Dungeon* d, ofstream& ofs) {
    ofs << "Dungeon" << ' ' 
        << d->getMapRow() << ' ' << d->getMapCol() << ' ' 
        << d->getCurMonNumber() << ' ' << d->getCurChestNumber() << ' ' 
        << d->getBossRoom()->getIndex() << endl;
}

void Record::savePlayer(Player& p, ofstream& ofs) {
    const vector<Object*>& pInv = p.getInventory();
    ofs << p.getTag() << ' ' << p.getName() << ' '
        << p.getMaxHealth() << ' ' << p.getCurrentHealth() << ' '
        << p.getAttack() << ' ' << p.getMoney() << ' ' << p.getCriticalAttackRate() << ' '
        << p.getCurrentRoom()->getIndex() << ' ' << p.getPreviousRoom()->getIndex() << ' '
        << p.getWeaponName() << ' ' << p.getWeaponAttack() << ' ' << p.getAddedAttack() << ' ' 
        << p.getLevel() << ' ' << p.getInventorySize() << ' ' << pInv.size() << endl;
    
    Item* itm = nullptr;
    for (Object* obj : pInv) {
        itm = dynamic_cast<Item*>(obj);
        if (itm) saveItem(itm, ofs);
    }
}

void Record::saveRooms(vector<Room>& rms, ofstream& ofs) {
    Object* obj = nullptr;
    Monster* mon = nullptr;
    Item* itm = nullptr;
    NPC* npc = nullptr;
    for (Room &rm : rms) {
        ofs << "Room" << ' ' << rm.getIndex() << ' ' 
            << rm.getIsVisited() << ' ' << rm.getIsExit() << ' '
            << (rm.getUpRoom() ? rm.getUpRoom()->getIndex() : -1) << ' ' 
            << (rm.getDownRoom() ? rm.getDownRoom()->getIndex() : -1) << ' ' 
            << (rm.getLeftRoom() ? rm.getLeftRoom()->getIndex() : -1) << ' ' 
            << (rm.getRightRoom() ? rm.getRightRoom()->getIndex() : -1) << ' ';
        
        obj = rm.getObject();
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
    const vector<Object*>& nCom = npc->getCommodity();
    ofs << npc->getTag() << ' ' << npc->getName() << ' ' << nCom.size() << endl;
    Item* itm = nullptr;
    for (Object* obj : nCom) {
        itm = dynamic_cast<Item*>(obj);
        if (itm) saveItem(itm, ofs);
    }
}

bool Record::loadFromFile(Dungeon* d, Player& p, vector<Room>& rms) {
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
    
    cout << endl << "Load success!" << endl;
    return true;
}

void Record::loadDungoen(Dungeon* d, vector<Room>& rms, ifstream& ifs) {
    string line, name;
    getline(ifs, line);
    istringstream iss(line);
    int r, c, curMon, curCh, n_rooms, bRM;
    iss >> name >> r >> c >> curMon >> curCh >> bRM;
    
    // cout << name << ' ' << r << ' ' << c << ' ' << curMon << ' ' 
    //      << curCh << ' ' << bRM << endl;
    
    n_rooms = r * c;
    d->setCurMonNumber(curMon); 
    d->setCurChestNumber(curCh);
    for (int i = 0; i < n_rooms; i++)
        rms.push_back(Room(i, false));
    d->setBossRoom(&rms[bRM]);
}

void Record::loadPlayer(Player& p, vector<Room>& rms, ifstream& ifs) {
    string line, pName, wName;
    getline(ifs, line);
    stringstream ss(line);
    int maxHp, curHp, atk, money, car, cRM, pRM, wAtk, addedAtk, lv, maxN, curN;
    ss >> pName >> pName >> maxHp >> curHp >> atk >> money 
       >> car >> cRM >> pRM >> wName >> wAtk >> addedAtk >> lv >> maxN >> curN;
    p.setName(pName); p.setMaxHealth(maxHp); p.setCurrentHealth(curHp);
    p.setAttack(atk); p.setMoney(money); p.setCriticalAttackRate(car);
    p.setCurrentRoom(&rms[cRM]); p.setPreviousRoom(&rms[pRM]);
    p.setWeaponName(wName); p.setWeaponAttack(wAtk); p.setAddedAttack(addedAtk);
    p.setLevel(lv); p.setInventorySize(maxN);
    
    // cout << pName << ' ' << maxHp << ' ' << curHp << ' ' << atk << ' ' << money << ' ' 
    //      << car << ' ' << cRM << ' ' << pRM << ' ' << wName << ' ' << wAtk << ' ' 
    //      << addedAtk << ' ' << lv << ' ' << maxN << ' ' << curN << endl;
    
    vector<Object*> itms;
    for (int i = 0; i < curN; i++) 
        itms.push_back(loadItem(ss, ifs));
    p.setInventory(itms);
}

void Record::loadRooms(vector<Room>& rms, ifstream& ifs) {
    string line, tag;
    stringstream ss;
    int idx, up, down, left, right;
    bool iVt, iEt, hasMon, hasItm, hasNpc; 
    for (Room &rm : rms) {
        getline(ifs, line);
        ss.str(""); ss.clear(); ss << line;
        ss >> tag >> idx >> iVt >> iEt >> up >> down >> left >> right 
           >> hasMon >> hasItm >> hasNpc;
        
        rm.setIndex(idx); rm.setIsVisited(iVt); rm.setIsExit(iEt);
        if (up != -1) rm.setUpRoom(&rms[up]); 
        if (down != -1) rm.setDownRoom(&rms[down]); 
        if (left != - 1) rm.setLeftRoom(&rms[left]); 
        if (right != -1) rm.setRightRoom(&rms[right]);

        // cout << tag << ' ' << idx << ' ' << iVt << ' ' << iEt << ' ' 
        //      << up << ' ' << down << ' ' << left << ' ' << right << ' ' 
        //      << hasMon << ' ' << hasItm << ' ' << hasNpc << ' ' << endl;
        
        // if (hasMon) loadMonster(ss, ifs);
        // if (hasItm) loadItem(ss, ifs);
        // if (hasNpc) loadNPC(ss, ifs);
        if (hasMon) rm.setObject(loadMonster(ss, ifs));
        if (hasItm) rm.setObject(loadItem(ss, ifs));
        if (hasNpc) rm.setObject(loadNPC(ss, ifs));
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

    vector<Object*> itms;
    for (int i = 0; i < n_itms; i++) 
        itms.push_back(loadItem(ss, ifs));
    npc->setCommodity(itms);

    return npc;
}