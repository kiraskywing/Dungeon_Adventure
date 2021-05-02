#include "Dungeon.h"

int Dungeon::getRandomRoomNumber() {
    int idx;
    Room* curRM = player.getCurrentRoom();
    do {
        idx = randomInt(0, map_row * map_col - 1);
    } while ((curRM && curRM->getIndex() == idx) || rooms[idx]->getObject() || rooms[idx]->getIsExit());

    return idx;
}

void Dungeon::createPlayer() {
    string s_name;
    cout << "Please enter player's name (without white space): ";
    cin >> s_name;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    
    player.setName(s_name);
    vector<Item*> itms;
    Item* obj = nullptr;
    for (int i = 0; i < player.getBackpackSize(); i++) {
        obj = new Item("Potion_lv.0", 100);
        itms.push_back(obj);
    }
    player.setBackpack(itms);

    /* set player's start position */
    int start_room = getRandomRoomNumber();
    player.setPreviousRoom(rooms[start_room]);
    player.setCurrentRoom(rooms[start_room]);
    rooms[start_room]->setIsVisited(true);
}

void Dungeon::createMap() {
    Room* rm = nullptr;
    for (int i = 0; i < map_row * map_col; i++) {
        #ifdef SEE_WHOLE_MAP
        rm = new Room(i, true);
        #else
        rm = new Room(i, false);
        #endif
        rooms.push_back(rm);
    }
    
    for (int i = 0; i < map_row; i++) {
        for (int j = 0; j < map_col; j++) {
            if (i > 0)           {rooms[j + i * map_col]->setUpRoom(rooms[j + (i - 1) * map_col]);}
            if (i + 1 < map_row) {rooms[j + i * map_col]->setDownRoom(rooms[j + (i + 1) * map_col]);}
            if (j > 0)           {rooms[j + i * map_col]->setLeftRoom(rooms[(j - 1) + i * map_col]);}
            if (j + 1 < map_col) {rooms[j + i * map_col]->setRightRoom(rooms[(j + 1) + i * map_col]);}
        }
    }
    
    /* set goal position */
    int exitRM = getRandomRoomNumber();
    rooms[exitRM]->setIsExit(true);

    /* set boss' position */
    int bossRM = getRandomRoomNumber();
    boss_room = rooms[bossRM];
    Monster* boss = new Monster("Boss", 3000, 3000, 1000, 1000, 20);
    rooms[bossRM]->setObject(boss);
    
    /* set NPC */
    createNPC();
    
    /* set Monsters */
    createMonster();
    
    /* set Treasure */
    createChest();
}

void Dungeon::createNPC() {
    NPC* npc = new NPC("ItemShop");
    vector<Item*> itms;
    Item* obj = nullptr;
    for (int i = 0; i < 3; i++) {
        obj = new Item(string("Sword_lv.") + to_string(i + 1), 0, 1000 * pow(2, i), 0, 20 * (1 + i));
        itms.push_back(obj);
    }
    for (int i = 0; i < 3; i++) {
        obj = new Item(string("Potion_lv.") + to_string(i + 1), 500 * pow(2, i));
        itms.push_back(obj);
    }
    npc->setCommodity(itms);
    int npcRM = getRandomRoomNumber();
    rooms[npcRM]->setObject(npc);
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
        rooms[monRM]->setObject(mon);
    }
    currentMonsterNumber = maxMonsterNumber;
}

void Dungeon::createChest() {
    int lv = player.getLevel();
    int times = pow(2, lv - 1);
    Item* itm = nullptr;
    int type, iRM;
    for (int i = 0; i < maxChestNumber - currentChestNumber; i++) {
        type = randomInt(1, 3);
        if (type == 1) itm = new Item(string("Coins_lv.") + to_string(lv), 0, 0, 200 * times);
        if (type == 2) itm = new Item(string("AttackUp_lv.") + to_string(lv), 0, 90 * times);
        if (type == 3) itm = new Item(string("HealthRecover_lv.") + to_string(lv), 95 * times);
        iRM = getRandomRoomNumber();
        rooms[iRM]->setObject(itm);
    }
    currentChestNumber = maxChestNumber;
}

void Dungeon::handleMovement() {
    cout << endl << "Where do you want to go?" << endl
                 << "(a) Move Up" << endl
                 << "(b) Move Down" << endl
                 << "(c) Move Left" << endl
                 << "(d) Move Right" << endl
                 << "Enter: ";
    
    Room* curRM = player.getCurrentRoom();
    int i;
    while (true) {
        i = inputOptimizer(4);
        if (i == 0 && curRM->getUpRoom()) {
            player.changeRoom(curRM->getUpRoom()); 
            break;
        }
        if (i == 1 && curRM->getDownRoom()) {
            player.changeRoom(curRM->getDownRoom());
            break;
        }
        if (i == 2 && curRM->getLeftRoom()) {
            player.changeRoom(curRM->getLeftRoom()); 
            break;
        }
        if (i == 3 && curRM->getRightRoom()) {
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
            if (rooms[cRM]->getIsVisited()) {
                obj = rooms[cRM]->getObject();
                mon = dynamic_cast<Monster*>(obj);
                itm = dynamic_cast<Item*>(obj);
                npc = dynamic_cast<NPC*>(obj);

                if (cRM == pRM) cout << 'P';
                else if (mon) cout << (mon->getName() == "Boss" ? 'B' : 'M');
                else if (itm) cout << 'C';
                else if (npc) cout << 'S';
                else if (rooms[cRM]->getIsExit()) cout << 'G';
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
        i = inputOptimizer(3);
        if (i == 0) { 
            createMap();
            createPlayer();
            break;
        }
        else if (i == 1) {
            if (rec.loadFromFile(this, player, rooms)) {
                break;
            }
        }
        else { 
            cout << endl << "Goodbye!" << endl << endl;
            exit(1); 
        }
    }
}

void Dungeon::chooseAction() {
    // check items in current room and handle event
    Record rec;
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
            if (mon && mon->checkIsDead()) { 
                if (objName != "Boss") currentMonsterNumber--; 
                inputOptimizer(0, "pause"); 
            }
            if (player.checkIsDead()) { return; }
            if (itm)                  { currentChestNumber--; inputOptimizer(0, "pause"); }
            
            curRM->popObject();
            obj = nullptr;

            if (currentMonsterNumber == 0) {
                if (currentChestNumber < maxChestNumber) 
                    createChest();
                player.levelUp();
                createMonster();
                cout << endl << "(Auto)";
                rec.saveToFile(this, player, rooms);
            }
        }
        else {
            if (obj->getTag() != "Item")
                player.changeRoom(player.getPreviousRoom());
            inputOptimizer(0, "pause");
        }
    }
    
    showMap();
    cout << endl << "Choose one action: "
         << endl << "(a) Move"
         << endl << "(b) Check status"
         << endl << "(c) Use backpack"
         << endl << "(d) Game options"
         << endl << "Enter: ";
    int i = inputOptimizer(4);

    if (i == 0) handleMovement();
    if (i == 1) { player.triggerEvent(nullptr); showMap(); inputOptimizer(0, "pause"); }
    if (i == 2) { player.useBackpack(); }
    if (i == 3) {
        cout << endl << "Choose one action: "
             << endl << "(a) Save game"
             << endl << "(b) Load game"
             << endl << "(c) Quit"
             << endl << "Enter: ";
        
        i = inputOptimizer(3);
        if (i == 0) rec.saveToFile(this, player, rooms);
        if (i == 1) rec.loadFromFile(this, player, rooms);
        if (i == 2) { cout << endl << "Goodbye!" << endl << endl; exit(1); }
    }
}

bool Dungeon::checkGameLogic() {
    Room* curRM = player.getCurrentRoom();
    
    if (player.checkIsDead()) {
        cout << "You loss!" << endl;
        inputOptimizer(0, "pause");
        return false;
    }
    if (curRM->getIsExit()) {
        Monster* mon = dynamic_cast<Monster*>(boss_room->getObject());
        if (mon && !mon->checkIsDead()) {
            cout << endl << "You reach the goal, but the Boss is still alive." << endl;
            inputOptimizer(0, "pause");
            return true;
        }
        
        cout << endl << "You win! Game Clear!" << endl;
        inputOptimizer(0, "pause");
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
