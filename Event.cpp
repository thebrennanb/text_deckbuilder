#include "Player.h"
#include "Enemy.h"
#include "Relic.h"
#include "Card.h"
#include "Event.h"
#include <iostream>
#include<sstream>
#include<algorithm>
Event::Event()
{}

Event::~Event()
{}

void Event::init(string classif, int num_enemies) {
    this->classif = classif;
    if(classif == "enemy") {
        is_enemy = true;
        fight = true;
    } else if(classif == "elite enemy") {
        is_elite_enemy = true;
        fight = true;
    } else if(classif == "bonfire") {
        is_bonfire = true;
    } else if(classif == "chance") {
        is_chance = true;
    } else if(classif == "shop") {
        is_shop = true;
        init_upgrades();
    } else if(classif == "treasure") { //else
        is_treasure = true;
    } else if(classif == "boss") { //else
        is_boss = true;
        fight = true;
    } else if(classif == "start") { //else
        is_start = true;
    }
    this->num_enemies = num_enemies;
}

void Event::set_lower_events(vector<Event*> lower_events) {
    this->lower_events = lower_events;
}

void Event::init_elite_enemies() {
    curr_enemies.clear();
    int randN = rand()%(3); //0 to 2

    if(randN == 0) {
        //gladiator + henchman
        Enemy *gladiator = new Enemy();
        vector<Card*> v;
        Card *card = new Card();
        //name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
        card->init("Execute", "Deal 20 damage", "a", "0", "attack", "none|0|20|player|;");
        Card *card1 = new Card();
        card1->init("Strike", "Deal 6 damage", "a", "0", "attack", "none|0|3|player|;");
        Card *card2 = new Card();
        card2->init("Mace swing", "Deal 7 damage and add 1 wound to the player's discard pile", "a", "0", "attack", "add_status_wound|1|0|player|;none|0|7|player|;");
        v.push_back(card1); //swapped order
        v.push_back(card);
        v.push_back(card2);
        gladiator->init("Gladiator", v, 160, 0);

        int randNn = rand()%(5)-2; //-2 to 2
        vector<Card*> v2;
        Card *card5 = new Card();
        card5->init("Strike", "Deal 3 damage", "a", "0", "attack", "none|0|3|player|;");
        v2.push_back(card5);
        Enemy *henchman = new Enemy();
        henchman->init("Henchman", v2, 20+randNn, 0);

        curr_enemies.push_back(gladiator);
        curr_enemies.push_back(henchman);
    } else if(randN == 1) {
        //illusionist
        vector<Card*> v;
        Card *card = new Card();
        card->init("Blinding flash", "Add 2 daze to the player's discard pile", "a", "0", "add_status", "add_status_daze|2|0|player|;");
        Card *card2 = new Card();
        card2->init("Trick", "Deal 5 damage and add 1 daze to the player's discard pile", "a", "0", "attack", "none|0|5|player|;add_status_daze|1|0|player|;");
        v.push_back(card);
        v.push_back(card2);

        int randNn = rand()%(5)-2; //-2 to 2
        Enemy *enemy1 = new Enemy();
        enemy1->init("Illusionist", v, 35+randNn, 0);
        curr_enemies.push_back(enemy1);

        vector<Card*> v2;
        v2.push_back(card2);
        v2.push_back(card);
        randNn = rand()%(5)-2; //-2 to 2
        Enemy *enemy2 = new Enemy();
        enemy2->init("Illusionist", v2, 35+randNn, 0);

        Enemy *enemy3 = new Enemy();
        enemy3->init("Illusionist", v, 35+randNn, 0);
        curr_enemies.push_back(enemy3);

        curr_enemies.push_back(enemy2);


    } else if(randN == 2) {

        //illusionist
        vector<Card*> v;
        Card *card = new Card();
        card->init("Serrated edge", "Deal 3 damage and add 3 bleed to the player's discard pile", "a", "0", "attack", "add_status_bleed|3|0|player|;none|0|3|player|;");
        Card *card2 = new Card();
        card2->init("7 swords", "Deal 1 damage, 7 times", "a", "0", "attack", "none|0|1|player|;none|0|1|player|;none|0|1|player|;none|0|1|player|;none|0|1|player|;none|0|1|player|;none|0|1|player|;");
        Card *card3 = new Card();
        card3->init("Pin", "Deal 5 damage and add 1 bleed to the player's discard pile", "a", "0", "attack", "add_status_bleed|1|0|player|;none|0|5|player|;");
        Card *card4 = new Card();
        card4->init("Hone swords", "Gain 5 rage", "a", "0", "effect", "rage|4|0|self|;");
        v.push_back(card);
        v.push_back(card2);
        v.push_back(card3);
        v.push_back(card4);

        int randNn = rand()%(5)-2; //-2 to 2
        Enemy *enemy1 = new Enemy();
        enemy1->init("Sword slinger", v, 80+randNn, 0);
        curr_enemies.push_back(enemy1);

        //think of one
    }

}

void Event::init_enemies() {
    curr_enemies.clear();
    int randN = rand()%(4); //0 to 3

    if(randN == 0) {
        //bandit
        vector<Card*> v;
        Card *card = new Card();
        card->init("Stab", "Deal 5 damage", "a", "0", "attack", "none|0|5|player|;");
        v.push_back(card);
        for(int i = 0; i < 2; i++) {
            int randNn = rand()%(5)-2; //-2 to 2
            Enemy *enemy1 = new Enemy();
            enemy1->init("Bandit", v, 30+randNn, 0);
            curr_enemies.push_back(enemy1);
        }

    } else if(randN == 1) {
        //slime
        vector<Card*> v;
        Card *card = new Card();
        card->init("Lick", "Give 1 vulnerable", "a", "0", "effect", "vulnerable|1|0|player|;");
        Card *card2 = new Card();
        card2->init("Phagocytosis", "Deal 3 damage", "a", "0", "attack", "none|0|3|player|;");
        v.push_back(card);
        v.push_back(card2);
        for(int i = 0; i < 5; i++) { //5 slimes
            int randNn = rand()%(5)-2; //-2 to 2
            Enemy *enemy1 = new Enemy();
            enemy1->init("Slime", v, 10+randNn, 0);
            curr_enemies.push_back(enemy1);
            randNn = rand()%(2);
            if(randNn == 0) { //swap cards
                v.push_back(v[0]);
                v.erase(v.begin());
            }
        }

    } else if(randN == 2) {
        //demon acolyte
        vector<Card*> v;
        Card *card = new Card();
        card->init("Hellpower", "Gain 4 rage", "a", "0", "effect", "rage|3|0|self|;");
        Card *card5 = new Card();
        card5->init("Swipe", "Deal 8 damage", "a", "0", "attack", "none|0|8|player|;");
        Card *card2 = new Card();
        card2->init("Screech", "Give 3 weak", "a", "0", "effect", "weak|3|0|player|;");
        Card *card4 = new Card();
        card4->init("Brimstone", "Deal 30 damage", "a", "0", "attack", "none|0|30|player|;");
        v.push_back(card);
        v.push_back(card5);
        v.push_back(card2);
        v.push_back(card4);
        int randNn = rand()%(11)-5; //-5 to 5
        Enemy *enemy1 = new Enemy();
        enemy1->init("Demon acolyte", v, 60+randNn, 0);
        curr_enemies.push_back(enemy1);


    } else if(randN == 3) {
        //stone guard
        vector<Card*> v;
        Card *card = new Card();
        card->init("Defensive stance", "Gain 15 armor", "a", "0", "effect", "armor|15|0|self|;");
        Card *card2 = new Card();
        card2->init("Thorn armor", "Gain 4 retaliate", "a", "0", "effect", "retaliate|4|0|self|;");
        Card *card3 = new Card();
        card3->init("Defensive stance", "Gain 15 armor", "a", "0", "effect", "armor|15|0|self|;");
        Card *card4 = new Card();
        card4->init("Armored strike", "Deal 10 damage and gain 5 armor", "a", "0", "attack", "armor|5|0|self|;none|0|10|player|;");
        v.push_back(card);
        v.push_back(card2);
        v.push_back(card3);
        v.push_back(card4);
        int randNn = rand()%(11)-5; //-5 to 5
        Enemy *enemy1 = new Enemy();
        enemy1->init("Stone guard", v, 100+randNn, 0);
        curr_enemies.push_back(enemy1);
    }



}

void Event::init_boss() {
    curr_enemies.clear();
    Enemy *boss = new Enemy();
    vector<Card*> v;

    Card *card1 = new Card();
    card1->init("Empower", "Gain 5 rage", "a", "0", "add_status", "rage|5|0|self|;");
    v.push_back(card1);

    Card *card4 = new Card();
    card4->init("Gnash", "Deal 1 damage, 4 times", "a", "0", "attack", "none|0|1|player|;none|0|1|player|;none|0|1|player|;none|0|1|player|;");
    v.push_back(card4);

    Card *card = new Card();
    card->init("Heavy strike", "Deal 25 damage", "a", "0", "attack", "none|0|25|player|;");
    v.push_back(card);

    Card *card2 = new Card();
    card2->init("Double slash", "Deal 3 damage, twice", "a", "0", "attack", "none|0|3|player|;none|0|3|player|;");
    v.push_back(card2);

    boss->init("Blood demon", v, 300, 0);
    curr_enemies.push_back(boss);
}

bool Event::all_dead() {
    return (curr_enemies.size() == 0);
}

void Event::check_dead() { //kind of like an update enemy size method.
    for(int i = 0; i < curr_enemies.size(); i++) {
        if(curr_enemies[i]->is_dead()) {
            curr_enemies.erase(curr_enemies.begin()+i);
            i--;
        }
    }
}

void Event::skip() {
    for(int i = 0; i < curr_enemies.size(); i++) {
        curr_enemies[i]->hp = 0;
    }
}

void Event::display_enemies() {
    for(int i = 0; i < curr_enemies.size(); i++) {
        cout << i << "  " << curr_enemies[i]->name << ", hp=" << curr_enemies[i]->hp << endl;

        /*cout << "Attacks:" << endl;
        string nameCurrAtk;
        for(int j = 0; j < curr_enemies[i]->attacks.size(); j++) {
            vector<vector<string>> does = curr_enemies[i]->attacks[j]->split();
            if(j == curr_enemies[i]->card_pos) {
                nameCurrAtk = does[0][0];
            }
            if(j != curr_enemies[i]->attacks.size()-1) {
                cout << does[0][0] << ": " << does[0][1] << ", " << endl;
            } else {
                cout << does[0][0] << ": " << does[0][1] << "." << endl;
            }
        }
        cout << curr_enemies[i]->name << " intends to use " << nameCurrAtk << "." << endl;*/
        Card* c = curr_enemies[i]->attacks[curr_enemies[i]->card_pos];
        cout << curr_enemies[i]->name << " intends to use " << c->name << ": " << c->description << "." << endl;
        if(!curr_enemies[i]->effects.empty()) {
            cout << "Effects:" << endl;
            for(int q=0; q<curr_enemies[i]->effects.size(); q++) {
                cout << curr_enemies[i]->effects[q]->name << " " << curr_enemies[i]->effects[q]->magnitude << endl;
            }
        }
        cout << endl;
    }

}

bool Event::enemy_reward(Player *p) {
    p->add_to_coins(50);
    p->get_card(1, 3);
}
bool Event::elite_enemy_reward(Player *p) {
    p->add_to_coins(100);
    p->get_card(2, 5);
}
bool Event::shop_offer(Player *p) {
    cout << "Reached a shop." << endl;
    //3 relic options, 4 card options, 2 choice options, 1 deck manipulation option.
    //3 random relics

    //3 random common cards
    vector<Card*> commonCardList = get_cards_shop(p, "common", 3);

    //2 random rare cards
    vector<Card*> rareCardList = get_cards_shop(p, "rare", 2);

    //2 from: make 1 cheaper (20), +10 damage (only on contains none cards) (20), +20 and consume (only on contains none cards) (20), holdover (100), add +3 to effect(s) (80), remove consume (80),
    vector<string> upgradeList = get_upgrade_list();

    //either remove card or duplicate card
    int rem_dup = rand()%(2);

    bool leave = false;
    bool rerolled = false;
    while(!leave) {
        cout << "You have " << p->coins << " coins left. What will you buy?" << endl;
        //print common cards
        for(int i = 0; i < commonCardList.size(); i++) {
            cout << i << ":   (" << commonCardList[i]->stamina_cost << ") [" << commonCardList[i]->rarity << "]: " << commonCardList[i]->name <<  " --" << commonCardList[i]->description << "--  ($" << commonCost << ")" << endl;
        }
        //print rare cards
        for(int i = 0; i < rareCardList.size(); i++) {
            cout << i+commonCardList.size() << ":   (" << rareCardList[i]->stamina_cost << ") [" << rareCardList[i]->rarity << "]: " << rareCardList[i]->name <<  " --" << rareCardList[i]->description << "--  ($" << rareCost << ")" << endl;
        }

        if(rem_dup == 0) { //0: remove card
            cout << "Remove card ($" << removeCardCost << ")" << endl;
        } else { //1: duplicate card
            cout << "Duplicate card ($" << duplicateCardCost << ")" << endl;
        }

        cout << "Upgrade a card:" << endl;
        for(int i = 0; i < upgradeList.size(); i++) {
            if(upgradeList[i] == "cost-1") {
                cout << i << ":   Decrease the cost of a card by 1 ($" << common_upgrade_cost << ")" << endl;
            } else if(upgradeList[i] == "damage+20|consume") {
                cout << i << ":   Increase the damage of a card by 20 and give it consume ($" << common_upgrade_cost << ")" << endl;
            } else if(upgradeList[i] == "damage+10") {
                cout << i << ":   Increase the damage of a card by 10 ($" << common_upgrade_cost << ")" << endl;
            } else if(upgradeList[i] == "remove consume|cost+1") {
                cout << i << ":   Remove consume from a card and increase the cost by 1 ($" << rare_upgrade_cost << ")" << endl;
            } else if(upgradeList[i] == "holdover") {
                cout << i << ":   Give a card holdover ($" << rare_upgrade_cost << ")" << endl;
            }  else if(upgradeList[i] == "retain") {
                cout << i << ":   Give a card retain ($" << rare_upgrade_cost << ")" << endl;
            }
        }

        if(rerolled) {
            cout << "Reroll offers (sold out)" << endl;
        } else {
            cout << "Reroll offers ($" << reroll_cost << ")" << endl;
        }

        bool madeAction = false;
        while(!madeAction) {
            string temp;
            vector<string> input;
            string line;
            getline(cin, line);
            istringstream iss(line);
            while(iss >> temp) {
                input.push_back(temp);
            }
            if(input.empty()) {
                cout << "Input something." << endl;
            } else {
                if(input.size() == 1) { //all inputs with 1 word
                    if(input[0] == "leave") {
                        madeAction = true;
                        leave = true;
                    } else if(input[0] == "help") {
                        cout << "Make an action:\n\"buy <type> <position>\" to buy something (either a relic, an upgrade, or a card-eg. buy c 0), \"buy relic <relic>\" to buy a relic, \"display deck\" to display your deck, \"reroll\" to re-roll offers, \"remove\" or \"duplicate\" a card (eg. remove card), \"leave\" to leave the shop, \"me\" to see everything about you." << endl;
                    } else if(input[0] == "me") {
                        p->display_player();
                    } else if(input[0] == "reroll" && !rerolled) {
                        if(p->coins >= reroll_cost) {
                            commonCardList.clear();
                            //3 random common cards
                            commonCardList = get_cards_shop(p, "common", 3);

                            rareCardList.clear();
                            //2 random rare cards
                            rareCardList = get_cards_shop(p, "rare", 2);
                            if(rem_dup == 1) { //swap rem_dups
                                rem_dup = 0;
                            } else {
                                rem_dup = 1;
                            }

                            upgradeList.clear();
                            upgradeList = get_upgrade_list();

                            p->coins-=reroll_cost;
                            rerolled = true;
                            madeAction = true;
                        } else {
                            cout << "You don't have enough coins." << endl;
                        }
                    } else {
                        cout << "Could not understand." << endl;
                    }
                } else if(input.size() >= 2) { //all the rest require > 1 input

                    if(input[0] == "display" && input[1] == "deck") {
                        p->display_deck();

                    } else if(rem_dup == 0 && input[0] == "remove" && input[1] == "card") {
                        if(p->coins >= removeCardCost) {
                            cout << "Which card will you remove? (Or type back to go back.)" << endl;
                            for(int aa = 0; aa < p->deck.size(); aa++) {
                                cout << aa << "   (" << p->deck[aa]->stamina_cost << ") [" << p->deck[aa]->name << "] --" << p->deck[aa]->description << "--" << endl;
                            }
                            string pos;
                            while(1) {
                                cin >> pos;
                                if(pos == "back") {
                                    madeAction = true;
                                    break;
                                } else if(!is_number(pos)) {
                                    cout << "Choose a card based on its index." << endl;
                                } else {
                                    if(stoi(pos) < p->deck.size()) {
                                        cout << "Removed " << p->deck[stoi(pos)]->name << endl;
                                        p->deck.erase(p->deck.begin() + stoi(pos));
                                        p->coins-=removeCardCost;
                                        removeCardCost+=removeCardCostIncrease;
                                        madeAction = true;
                                        rem_dup = 1; //if removed a card, next option to duplicate a card (swap the options)
                                        break;
                                    } else {
                                        cout << "Choose a card in your deck." << endl;
                                    }
                                }
                            }
                        } else {
                            cout << "You don't have enough coins." << endl;
                        }
                    } else if(rem_dup == 1 && input[0] == "duplicate" && input[1] == "card") {
                        if(p->coins >= duplicateCardCost) {
                            cout << "Which card will you duplicate? (Or type back to go back)" << endl;
                            for(int aa = 0; aa < p->deck.size(); aa++) {
                                cout << aa << "   (" << p->deck[aa]->stamina_cost << ") [" << p->deck[aa]->name << "] --" << p->deck[aa]->description << "--" << endl;
                            }
                            string pos;
                            while(1) {
                                cin >> pos;
                                if(pos == "back") {
                                    madeAction = true;
                                    break;
                                } else if(!is_number(pos)) {
                                    cout << "Choose a card based on its index." << endl;
                                } else {
                                    if(stoi(pos) < p->deck.size()) {
                                        string name = p->deck[stoi(pos)]->name;
                                        p->deck.push_back(p->card_name_map[name]); //doing it this way to not deal with
                                        cout << "Duplicated " << name << endl;
                                        p->coins-=duplicateCardCost;
                                        duplicateCardCost+=duplicateCardCostIncrease;
                                        madeAction = true;
                                        rem_dup = 0; //if duplicated a card, next option to remove a card (swap the options)
                                        break;
                                    } else {
                                        cout << "Choose a card in your deck." << endl;
                                    }
                                }
                            }
                        } else {
                            cout << "You don't have enough coins." << endl;
                        }
                    } else if(input[0] == "buy") { //buy something
                        if(input[1] == "c") {
                            if(!is_number(input[2])) {
                                cout << "Choose a card based on its index." << endl;
                            } else {
                                int pos = stoi(input[2]);
                                if(pos < commonCardList.size()+rareCardList.size()) {
                                    if(pos < commonCardList.size()) {
                                        if(p->coins >= commonCost) {
                                            cout << "Added a " << commonCardList[pos]->name << " to your deck." << endl;
                                            p->deck.push_back(commonCardList[pos]);
                                            commonCardList.erase(commonCardList.begin() + pos);
                                            p->coins-=commonCost;
                                            madeAction = true;
                                        } else  {
                                            cout << "You don't have enough coins." << endl;
                                        }
                                    } else {
                                        if(p->coins >= rareCost) {
                                            cout << "Added a " << rareCardList[pos-commonCardList.size()]->name << " to your deck." << endl;
                                            p->deck.push_back(rareCardList[pos-commonCardList.size()]);
                                            rareCardList.erase(rareCardList.begin() + pos-commonCardList.size());
                                            p->coins-=rareCost;
                                            madeAction = true;
                                        } else {
                                            cout << "You don't have enough coins." << endl;
                                        }
                                    }
                                }
                            }
                        } else if(input[1] == "relic") {

                        } else if(input[1] == "upgrade") {
                            if(!is_number(input[2])) {
                                cout << "Choose an upgrade based on its index (eg: buy upgrade 0)." << endl;
                            } else {
                                int upgradeNum = stoi(input[2]);
                                if(upgradeNum < upgradeList.size()) {
                                    if(p->coins >= get_upgrade_cost(upgradeList[upgradeNum])) {
                                        cout << "Which card will you upgrade? (Or type back to go back.)" << endl;
                                        map<int, int> card_convs = print_cards_upgrade(upgradeList[upgradeNum], p);
                                        string pos;
                                        while(1) {
                                            cin >> pos;
                                            if(pos == "back") {
                                                madeAction = true;
                                                break;
                                            } else if(!is_number(pos)) {
                                                cout << "Choose a card based on its index." << endl;
                                            } else {
                                                map<int,int>::iterator it = card_convs.find(stoi(pos));
                                                if(it == card_convs.end()) {
                                                    cout << "Choose an existing card." << endl;
                                                } else {
                                                    p->coins-=get_upgrade_cost(upgradeList[upgradeNum]);
                                                    cout << "Upgraded " << p->deck[it->second]->name << endl;
                                                    p->deck[it->second]->upgrade(upgradeList[upgradeNum]);
                                                    madeAction = true;
                                                    upgradeList.erase(upgradeList.begin() + upgradeNum); //remove this upgrade from the list
                                                    break;
                                                }
                                            }
                                        }
                                    } else {
                                        cout << "You don't have enough coins." << endl;
                                    }
                                } else {
                                    cout << "Choose an existing upgrade." << endl;
                                }
                            }
                        }

                    } else {
                        cout << "Could not understand." << endl;
                    }
                } else {
                    cout << "Could not understand" << endl;
                }
                cout << endl;
            }
        }

    }
}
vector<Card*> Event::get_cards_shop(Player *p, string rarity, int numToSee) {
    vector<Card*> cards;
    vector<int> cardsChosen;

    if(rarity == "common") {
        int szC = p->common_cards.size();
        for(int i = 0; i < numToSee; i++) {
            int rn = rand()%(szC);
            if(std::find(cardsChosen.begin(), cardsChosen.end(), rn) != cardsChosen.end()) {
                i--;
            } else {
                cardsChosen.push_back(rn);
                cards.push_back(p->common_cards[rn]);
            }
        }
    } else if(rarity == "rare") {
        int szR = p->rare_cards.size();
        for(int i = 0; i < numToSee; i++) {
            int rn = rand()%(szR);
            if(std::find(cardsChosen.begin(), cardsChosen.end(), rn) != cardsChosen.end()) {
                i--;
            } else {
                cardsChosen.push_back(rn);
                cards.push_back(p->rare_cards[rn]);
            }
        }
    }
    return cards;
}
int Event::get_upgrade_cost(string upg) {
    if(upg == "cost-1" || upg == "damage+20|consume" || upg == "damage+10") {
        return common_upgrade_cost;
    } else {
        return rare_upgrade_cost;
    }
}
map<int, int> Event::print_cards_upgrade(string upg, Player *p) {
    int runningNum = 0;
    map<int, int> card_convs;
    //v these 2 can only apply to certain cards
    if(upg == "cost-1") { //bad way of doing this but oh well
        for(int aa = 0; aa < p->deck.size(); aa++) {
            if(p->deck[aa]->stamina_cost != "X" && stoi(p->deck[aa]->stamina_cost) > 0) { //only print cards with cost > 1
                cout << runningNum << "   (" << p->deck[aa]->stamina_cost << ") [" << p->deck[aa]->name << "] --" << p->deck[aa]->description << "--" << endl;
                card_convs.insert(std::make_pair(runningNum, aa)); // the card index printed :: the actual card index in deck
                runningNum++;
            }
        }
    } else if(upg == "remove consume|cost+1") { //bad way of doing this but oh well
        for(int aa = 0; aa < p->deck.size(); aa++) {
            if(p->deck[aa]->consume) { //only print cards with consume
                cout << runningNum << "   (" << p->deck[aa]->stamina_cost << ") [" << p->deck[aa]->name << "] --" << p->deck[aa]->description << "--" << endl;
                card_convs.insert(std::make_pair(runningNum, aa)); // the card index printed :: the actual card index in deck
                runningNum++;
            }
        }
    } else if(upg == "damage+20|consume" || upg == "damage+10") {
        for(int aa = 0; aa < p->deck.size(); aa++) {
            if(p->deck[aa]->card_type == "attack" || p->deck[aa]->card_type == "attack-closest") { //only print cards that are attacks
                cout << runningNum << "   (" << p->deck[aa]->stamina_cost << ") [" << p->deck[aa]->name << "] --" << p->deck[aa]->description << "--" << endl;
                card_convs.insert(std::make_pair(runningNum, aa)); // the card index printed :: the actual card index in deck
                runningNum++;
            }
        }
    } else {
        for(int aa = 0; aa < p->deck.size(); aa++) {
            cout << aa << "   (" << p->deck[aa]->stamina_cost << ") [" << p->deck[aa]->name << "] --" << p->deck[aa]->description << "--" << endl;
            card_convs.insert(std::make_pair(aa, aa)); // the card index printed :: the actual card index in deck
        }
    }
    return card_convs;
}
vector<string> Event::get_upgrade_list() {
    vector<int> upgradesGot;
    vector<string> rtn;
    for(int i = 0; i < common_upgrade_number; i++) {
        int randN = rand()%(common_upgrades.size());
        if(std::find(upgradesGot.begin(), upgradesGot.end(), randN) == upgradesGot.end()) {
            rtn.push_back(common_upgrades[randN]);
            upgradesGot.push_back(randN);
        } else {
            i--;
        }
    }
    upgradesGot.clear();
    for(int i = 0; i < rare_upgrade_number; i++) {
        int randN = rand()%(rare_upgrades.size());
        if(std::find(upgradesGot.begin(), upgradesGot.end(), randN) == upgradesGot.end()) {
            rtn.push_back(rare_upgrades[randN]);
            upgradesGot.push_back(randN);
        } else {
            i--;
        }
    }
    return rtn;
}


bool Event::chance_offer(Player *p) {

}
bool Event::get_treasure(Player *p) {
    p->add_to_coins(200);
    p->get_card(1, 4);
    p->get_card(1, 4);
}
bool Event::boss_reward(Player *p) {
    //p->add_to_coins(200);
}

void Event::init_upgrades() {
    //common = decrease cost by 1, +20 dmg and consume, +10 dmg, efficacy+1 (maybe later)
    //rare = remove consume and increase cost by 1, holdover, retain, efficacy*2 (maybe later)
    common_upgrades.push_back("cost-1");
    common_upgrades.push_back("damage+20|consume");
    common_upgrades.push_back("damage+10");

    rare_upgrades.push_back("remove consume|cost+1");
    rare_upgrades.push_back("holdover");
    rare_upgrades.push_back("retain");
}

bool Event::is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
