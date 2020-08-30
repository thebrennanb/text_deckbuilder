#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Relic.h"
#include "Event.h"

#include<vector>
#include<map>
#include<string>
using namespace std;

class Game {

public:             // Access specifier

    Game(); //constructor
    ~Game(); //deconstructor

    void init();

    //map constants
    int num_events = 40;
    int max_events_per_row = 6;

    Player *player = new Player();
    bool playerQuit = false;
    bool end_game = false;
    pair<int, int> player_location = make_pair(0, 0);
    vector<vector<Event*>> event_map; //layers of events
    vector<vector<string>> connections;

    void play_player_turn();
    void play_card(int idx);
    void play_enemy_turn();
    void display_player();
    void display_enemies();
    void init_event_map();
    void play_map();
    void display_map();
    void inc_player_effects();
    void inc_enemy_effects(Event* ev);

    int calc_damage_players_attack(int dmg);
    int calc_damage_enemy_attack(int dmg, int idx); //idx = index of enemy who used attack

    //events
    void do_event(Event* ev);
    void combat();

    map<int, string> event_chances;

    map<string, char> event_conv;

};

