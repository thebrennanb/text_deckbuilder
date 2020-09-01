#include "Player.h"
#include "Enemy.h"
#include "Effect.h"

Effect::Effect()
{}

Effect::~Effect()
{}

void Effect::init(string name, int magnitude) {
    this->name = name;
    this->magnitude = magnitude;
    init_effect(name);
}

void Effect::init_effect(string n) {
    if(n == "armor") {
        single_turn = true;
        lasting = false;
        decrements = false;
        is_good = true;
    } else if(n == "rage") {
        single_turn = false;
        lasting = false;
        decrements = true;
        is_good = true;
    } else if(n == "agility") {
        single_turn = false;
        lasting = false;
        decrements = true;
        is_good = true;
    } else if(n == "vulnerable") {
        single_turn = false;
        lasting = false;
        decrements = true;
        is_good = false;
    } else if(n == "weak") {
        single_turn = false;
        lasting = false;
        decrements = true;
        is_good = false;
    } else if(n == "retaliate") {
        single_turn = false;
        lasting = true;
        decrements = false;
        is_good = true;
    } else if(n == "retain_armor") {
        single_turn = false;
        lasting = false;
        decrements = true;
        is_good = true;
    }
}
