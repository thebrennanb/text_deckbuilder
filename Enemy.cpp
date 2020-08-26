#include<string>
#include<vector>
#include "Enemy.h"
#include<iostream>
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
    hp -= dmg;
    if(hp < 0) {
        hp = 0;
    }
}
