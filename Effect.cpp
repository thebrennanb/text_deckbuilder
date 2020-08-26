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
    if(name == "armor") {
        init_armor();
    }
}

void Effect::init_armor() {
    single_turn = true;
    lasting = false;
    decrements = false;
}
