#include "Card.h"
#include<string>
#include<vector>
using namespace std;

Card::Card()
{}

Card::~Card()
{}

void Card::init(string name, string description, string classification, string effect_applied, int effect_magnitude, int damage, string target, int stamina_cost) {
    this->name = name;
    this->classification = classification;
    this->effect_applied = effect_applied;
    this->effect_magnitude = effect_magnitude;
    this->damage = damage;
    this->target = target;
    this->description = description;
    this->stamina_cost = stamina_cost;
}
