#pragma once

#include "Card.h"
#include "Relic.h"

#include<vector>
#include<string>
#include<map>
using namespace std;

class Player {
public:             // Access specifier

    Player();
    ~Player();
    void init_deck();

    vector<Card*> deck;
    vector<Card*> draw_pile;
    vector<Card*> discard_pile;
    vector<Card*> hand;
    int hp = 50;
    int max_stamina = 5;
    int stamina = 0;
    int num_draw = 5;
    vector<Relic> relics;
    map<string, int> effectsMagnitudes;
    vector<string> effects;
    int target_enemy;

    bool isDead();
    void take_damage(int dmg);
    void init_deck_before_combat();
    void draw_cards();
    void display_hand();
    void display_draw_pile();
    void display_discard_pile();
    void display_deck();
    void round_start();
};
