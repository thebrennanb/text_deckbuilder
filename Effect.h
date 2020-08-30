#pragma once
#include "Card.h"
#include "Effect.h"

#include<string>
#include<vector>
using namespace std;

class Effect {
public:

    Effect();
    ~Effect();

    void init(string name, int magnitude);
    void init_effect(string n);
    string name;
    int magnitude;
    bool single_turn;
    bool lasting;
    bool decrements;
    bool is_good;

};
