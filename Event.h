#pragma once

#include "Enemy.h"
#include<vector>
#include<string>
#include<map>
using namespace std;

class Event {
public:
    Event();
    ~Event();

    void init(string classif, int num_enemies);

    bool is_bonfire = false;
    bool is_enemy = false;
    bool is_elite_enemy = false;
    bool is_shop = false;
    bool is_chance = false;
    bool is_relic = false;
    bool is_start = false;
    bool is_boss = false;
    bool fight = false;
    int num_enemies;
    string classif;
    vector<Enemy*> curr_enemies;
    void init_enemies();

};
