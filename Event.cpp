#include "Player.h"
#include "Enemy.h"
#include "Relic.h"
#include "Card.h"

#include "Event.h"

Event::Event()
{}

Event::~Event()
{}

void Event::init(string classif, int num_enemies) {
    this->classif = classif;
    if(classif == "enemies") {
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
    } else if(classif == "relic") { //else
        is_relic = true;
    } else if(classif == "boss") { //else
        is_boss = true;
        fight = true;
    } else if(classif == "start") { //else
        is_start = true;
    }
    this->num_enemies = num_enemies;
}

void Event::init_enemies() {
    Enemy *enemy1 = new Enemy();
    vector<Card*> v;
    Card *card = new Card();
    card->init("sample enemy attack", "", "", "", 0, 3, "single player", 0);
    v.push_back(card);
    enemy1->init("sample enemy 1", v, 50, 0);
    Enemy *enemy2 = new Enemy();
    enemy2->init("sample enemy 2", v, 20, 0);
    curr_enemies.push_back(enemy1);
    curr_enemies.push_back(enemy2);
}
