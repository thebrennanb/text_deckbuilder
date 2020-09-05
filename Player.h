#pragma once

#include "Card.h"
#include "Relic.h"
#include "Effect.h"

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
    vector<Card*> common_cards;
    vector<Card*> rare_cards;
    map<string, Card*> card_name_map;

    int hp = 50;
    int max_hp = 50;
    int max_stamina = 3;
    int stamina = 0;
    int num_draw = 5;
    int coins = 0;
    int max_hand_size = 10;
    vector<Relic*> relics;
    vector<Effect*> effects;
    int target;

    bool isDead();
    void take_damage(int dmg);
    void get_card(int numGet, int numCards);
    void get_cards(int numGet, bool set_from_each, int num_common, int num_rare);
    void return_hand();
    void clear_effects();

    void init_deck_before_combat();
    void draw_cards(int num_to_draw);
    void display_hand();
    void display_draw_pile();
    void display_discard_pile();
    void display_deck();
    void round_start();
    void add_to_coins(int n);
    void add_effect(string name, int magnitude);
    void dec_magnitude(int idx);
    bool erase_effect_single_turn(int idx);

    void init_all_common_cards();
    void init_all_rare_cards();
};
