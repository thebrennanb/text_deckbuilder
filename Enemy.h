#pragma once

#include "Card.h"
#include "Effect.h"

#include<string>
#include<vector>
using namespace std;

class Enemy {
public:             // Access specifier

    Enemy();
    ~Enemy();

    string name;
    vector<Card*> attacks;
    vector<Effect*> effects;
    int hp;
    int max_hp;
    int rarity;
    int card_pos;
    void init(string name, vector<Card*> attacks, int max_hp, int rarity);
    bool is_dead();
    void increment_card_pos();
    void take_damage(int dmg);

};
