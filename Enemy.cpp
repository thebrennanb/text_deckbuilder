#include<string>
#include<vector>
#include "Enemy.h"
#include<iostream>
#include "Effect.h"
using namespace std;

Enemy::Enemy()
{}

Enemy::~Enemy()
{}

void Enemy::init(string name, vector<Card*> attacks, int max_hp, int rarity) {
    this->name = name;
    this->attacks = attacks;
    this->max_hp = max_hp;
    hp = max_hp;
    this->rarity = rarity;
    card_pos = 0;
}

bool Enemy::is_dead() {
    return hp == 0;
}

void Enemy::increment_card_pos() {
    //cout << "INCREMENTING from " << card_pos;
    card_pos++;
    if(card_pos >= attacks.size()) { //loop around
        card_pos = 0;
    }
    //cout << " to " << card_pos << endl;
}

void Enemy::take_damage(int dmg) {
    if(hp == 0) { //X times: don't do more times after enemy has died.
        return;
    }
    hp -= dmg;
    cout << name << " took " << dmg << " damage." << endl;
    if(hp < 0) {
        hp = 0;
        cout << name << " was slain." << endl;
    }
}

//Yeah I'll keep these 2 around, maybe case of relic poison?
void Enemy::dec_magnitude(int idx) { //might change if relic: "effect x no longer decays
    effects[idx]->magnitude--;
}
bool Enemy::erase_effect_single_turn(int idx) {
    bool retain_armor = false;
    for(int i = 0; i < effects.size(); i++) {
        if(effects[i]->name == "retain armor") {
            retain_armor = true;
        }
    }
    if(effects[idx]->name == "armor" && retain_armor) { //if armor and retain armor don't erase it
        return false;
    }

    //end: erase the effect.
    effects.erase(effects.begin() + idx);
    return true;
}

void Enemy::add_effect(string name, int magnitude) {
    Effect *eff = new Effect();
    eff->init(name, magnitude);
    bool alreadyContains = false;
    for(Effect *e : effects) {
        if(e->name == eff->name) {
            e->magnitude += eff->magnitude;
            alreadyContains = true;
        }
    }
    if(!alreadyContains) {
        effects.push_back(eff);
    }
}
