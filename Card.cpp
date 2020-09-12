#include "Card.h"
#include<string>
#include<vector>
#include<iostream>
using namespace std;

Card::Card()
{}

Card::~Card()
{}

/*void Card::init(string name, string description, string classification, string effect_applied, int effect_magnitude, int damage, string target, int stamina_cost) {
    this->name = name;
    this->classification = classification;
    this->effect_applied = effect_applied;
    this->effect_magnitude = effect_magnitude;
    this->damage = damage;
    this->target = target;
    this->description = description;
    this->stamina_cost = stamina_cost;
}*/

void Card::init(string name, string description, string rarity, string stamina_cost, string card_type, string info) {
    playable = true;
    this->name = name;
    this->stamina_cost = stamina_cost;
    this->card_type = card_type;
    this->description = description;
    this->rarity = rarity;
    this->info = info;
}

void Card::set_unplayable() {
    playable = false;
}

void Card::set_consume() {
    consume = true;
}

/*
void Card::set_conditional(string cond) {
    //possible conditionals: :enemy;attack;0; (enemy attack for >= 0)    :self;cards played this turn;3 (cards played this turn >= 3)
    //example:
    //target;what does it do;what do you do; = repeat
    //:enemy;attack;0;armor|4|0|self|;retaliate|2|0|self|;:enemy;defend;rage|1|0|self|;agility|1|0|self|;:



}*/

void Card::upgrade(string upg) {
    //name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
    if(upg == "holdover") {
        holdover = true;
    } else if(upg == "retain") {
        retain = true;
    } else if(upg == "remove consume|cost+1") {
        consume = false;
        stamina_cost = to_string(stoi(stamina_cost)+1)
    } else if(upg == "damage+10") { //This will deal an extra 20X damage, maybe don't want this
        if(card_type == "attack") {
            info+="none|0|10|choose|;";
        } else if(card_type == "attack-closest") {
            info+="none|0|10|closest enemy|;";
        }
        //change description
        //also maybe put an extra upgrade string variable in card so that X cards don't do this X times
    } else if(upg == "damage+20|consume") { //This will deal an extra 20X damage, maybe don't want this
        if(card_type == "attack") {
            info+="none|0|20|choose|;";
        } else if(card_type == "attack-closest") {
            info+="none|0|20|closest enemy|;";
        }
        consume = true;
        //change description
        //also maybe put an extra upgrade string variable in card so that X cards don't do this X times
    } else if(upg == "cost-1") {
        stamina_cost = to_string(stoi(stamina_cost)-1); //decrease cost by 1, ensured it won't be X.
    }
}

vector<vector<string>> Card::split() {
    size_t pos1 = 0;
    size_t pos2 = 0;
    string token1;
    string token2;
    vector<vector<string>> rtn;
    string s = info;

    while ((pos1 = s.find(d1)) != std::string::npos) { //go thru and separate by ;
        vector<string> t;
        token1 = s.substr(0, pos1);
        //cout << "token1: " << token1 << endl;

        while ((pos2 = token1.find(d2)) != std::string::npos) { //go thru separated string and separate by ,
            token2 = token1.substr(0, pos2);
            t.push_back(token2);
            token1.erase(0, pos2 + 1);
            s.erase(0, pos2 + 1);
            //cout << s << endl;
        }
        /*for(int i = 0; i < t.size(); i++) {
            cout << t[i] << endl;
        }*/
        s.erase(0, 1);
        rtn.push_back(t);
    }
    return rtn;
}

