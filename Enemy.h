#pragma once

#include "Card.h"
#include<string>
#include<vector>
using namespace std;

class Enemy {
public:             // Access specifier

    Enemy();
    ~Enemy();

    string name;
    vector<Card*> attacks;
    int hp;
    int rarity;
    void init(string name, vector<Card*> attacks, int hp, int rarity);
    bool is_dead();
    void take_damage(int dmg);

};
