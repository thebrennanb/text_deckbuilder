#include "Player.h"
#include "Enemy.h"
#include "Relic.h"
#include "Card.h"

#include "Event.h"
#include <iostream>

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
    //gladiator + henchman
    curr_enemies.clear();
    Enemy *gladiator = new Enemy();
    vector<Card*> v;
    Card *card = new Card();
    //name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
    card->init("Execute|Deal 8 damage|a|0|;none|0|8|player|;");
    Card *card1 = new Card();
    card1->init("Strike|Deal 3 damage|a|0|;none|0|3|player|;");
    v.push_back(card);
    v.push_back(card1);
    gladiator->init("Gladiator", v, 50, 0);

    vector<Card*> v2;
    Card *card2 = new Card();
    card2->init("Strike|Deal 3 damage|a|0|;none|0|3|player|;");
    v2.push_back(card2);
    Enemy *henchman = new Enemy();
    henchman->init("Henchman", v2, 20, 0);
    curr_enemies.push_back(gladiator);
    curr_enemies.push_back(henchman);
}

void Event::init_enemies() {
    //bandit
    curr_enemies.clear();
    Enemy *enemy1 = new Enemy();
    vector<Card*> v;
    Card *card = new Card();
    card->init("Stab|Deal 5 damage|a|0|;none|0|5|player|;");
    v.push_back(card);
    enemy1->init("Bandit", v, 30, 0);
    curr_enemies.push_back(enemy1);
}

void Event::init_boss() {
    curr_enemies.clear();
    Enemy *boss = new Enemy();
    vector<Card*> v;

    Card *card1 = new Card();
    card1->init("Empower|Gain 4 rage|a|0|;rage|10|0|self|;");
    v.push_back(card1);

    Card *card = new Card();
    card->init("Heavy strike|Deal 10 damage|a|0|;none|0|10|player|;");
    v.push_back(card);

    boss->init("Blood demon", v, 100, 0);
    curr_enemies.push_back(boss);
}

bool Event::all_dead() {
    return (curr_enemies.size() == 0);
}

void Event::check_dead() { //kind of like an update enemy size method.
    for(int i = 0; i < curr_enemies.size(); i++) {
        if(curr_enemies[i]->is_dead()) {
            curr_enemies.erase(curr_enemies.begin()+i);
            i = 0;
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
        vector<vector<string>> splt = curr_enemies[i]->attacks[curr_enemies[i]->card_pos]->split();
        cout << curr_enemies[i]->name << " intends to use " << splt[0][0] << ": " << splt[0][1] << "." << endl;
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

}
bool Event::chance_offer(Player *p) {

}
bool Event::get_treasure(Player *p) {
    p->add_to_coins(200);
}
bool Event::boss_reward(Player *p) {
    //p->add_to_coins(200);
}
