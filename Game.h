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

    int num_events = 20;

    Player *player = new Player();
    bool playerQuit = false;
    bool end_game = false;
    pair<int, int> player_location = make_pair(0, 0);
    vector<vector<Event*>> event_map; //layers of events
    void play_player_turn();
    bool play_enemy_turn(); //returns true if all enemies are dead, false otherwise.
    void display_player();
    void display_enemies();
    void combat();
    void init_event_map();
    void play_map();
    void display_map();

    map<int, string> event_chances;

};

