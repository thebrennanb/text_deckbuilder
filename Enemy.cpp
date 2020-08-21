#include<string>
#include<vector>
#include "Enemy.h"
using namespace std;

Enemy::Enemy()
{}

Enemy::~Enemy()
{}

void Enemy::init(string name, vector<Card*> attacks, int hp, int rarity) {
    this->name = name;
    this->attacks = attacks;
    this->hp = hp;
    this->rarity = rarity;
}

bool Enemy::is_dead() {
    return hp == 0;
}

void Enemy::take_damage(int dmg) {
    hp -= dmg;
    if(hp < 0) {
        hp = 0;
    }
}
