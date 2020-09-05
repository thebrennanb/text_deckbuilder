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
    bool is_treasure = false;
    bool is_start = false;
    bool is_boss = false;
    bool fight = false;
    int num_enemies;
    string classif;
    vector<Enemy*> curr_enemies;
    vector<Event*> lower_events;
    void set_lower_events(vector<Event*> lower_events);

    void init_enemies();
    void init_elite_enemies();
    void init_boss();
    bool enemy_reward(Player *p);
    bool elite_enemy_reward(Player *p);
    bool shop_offer(Player *p);
    vector<Card*> get_cards_shop(Player *p, string rarity, int numToSee);
    int rareCost = 70;
    int commonCost = 30;

    bool chance_offer(Player *p);
    bool get_treasure(Player *p);
    bool boss_reward(Player *p);


    void display_enemies();
    void check_dead();

    bool all_dead();
    void skip();

};
