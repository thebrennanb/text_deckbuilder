#pragma once

#include<string>
#include<vector>
using namespace std;

class Card {

public:
    Card();
    ~Card();
    void init(string name, string description, string classification, string effect_applied, int effect_magnitude, int damage, string target, int stamina_cost);

    string name;
    string classification;
    string effect_applied;
    int effect_magnitude;
    int damage;
    string target;
    string description;
    int stamina_cost;
};
