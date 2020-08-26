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
    /*for(int i = 0; i < lower_events.size(); i++) {
        lower_events[i]->upper_events.push_back(this); //add this event to each of lower event's list of upper events
    }*/
}

void Event::init_elite_enemies() {
    //gladiator + henchman
    curr_enemies.clear();
    Enemy *gladiator = new Enemy();
    vector<Card*> v;
    Card *card = new Card();
    card->init("execute", "", "", "", 0, 8, "single player", 0);
    Card *card1 = new Card();
    card1->init("strike", "", "", "", 0, 3, "single player", 0);
    v.push_back(card);
    v.push_back(card1);
    gladiator->init("Gladiator", v, 50, 0);

    vector<Card*> v2;
    Card *card2 = new Card();
    card2->init("strike", "", "", "", 0, 3, "single player", 0);
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
    card->init("stab", "", "", "", 0, 5, "single player", 0);
    v.push_back(card);
    enemy1->init("Bandit", v, 30, 0);
    curr_enemies.push_back(enemy1);
}

void Event::init_boss() {
    curr_enemies.clear();
    Enemy *boss = new Enemy();
    vector<Card*> v;

    Card *card1 = new Card();
    card1->init("Empower", "", "", "", 0, 0, "single player", 0);
    v.push_back(card1);

    Card *card = new Card();
    card->init("Heavy strike", "", "", "", 0, 15, "single player", 0);
    v.push_back(card);

    boss->init("Blood demon", v, 100, 0);
    curr_enemies.push_back(boss);
}

bool Event::all_dead() {
    int cnt_dead = 0;

    for(int i = 0; i < curr_enemies.size(); i++) {
        if(curr_enemies[i]->is_dead()) {
            cnt_dead++;
        }
    }
    return (cnt_dead == curr_enemies.size());

}

void Event::skip() {
    for(int i = 0; i < curr_enemies.size(); i++) {
        curr_enemies[i]->hp = 0;
    }
}

bool Event::enemy_reward(Player *p) {
    p->add_to_coins(50);
}
bool Event::elite_enemy_reward(Player *p) {
    p->add_to_coins(100);
}
bool Event::shop_offer(Player *p) {

}
bool Event::chance_offer(Player *p) {

}
bool Event::get_treasure(Player *p) {

}
bool Event::boss_reward(Player *p) {
    p->add_to_coins(200);
}
