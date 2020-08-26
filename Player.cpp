#include "Card.h"
#include "Relic.h"
#include "Effect.h"

#include "Player.h"

#include<vector>
#include<string>
#include<algorithm>
#include<random>
#include<iostream>
using namespace std;

Player::Player()
{}

Player::~Player()
{}

void Player::init_deck() { //set initial cards
    for(int i = 0; i < 5; i++) {
        Card *card = new Card();
        card->init("Fireball", "Cast a fireball at an enemy.", "Basic", "0", 0, 5, "single enemy", 2);
        deck.push_back(card);
    }
    for(int i = 0; i < 5; i++) {
        Card *card = new Card();
        card->init("Defend", "Don armor.", "Basic", "armor", 5, 0, "self", 2);
        deck.push_back(card);
    }
}

void Player::init_deck_before_combat() {
    for(int i = 0; i < deck.size(); i++) {
        draw_pile.push_back(deck[i]);
    }
    discard_pile.clear();
}

bool Player::isDead() {
    return hp==0;
}

void Player::display_deck() {
    cout << deck.size() << " cards in deck:" << endl;
    for(int i = 0; i < deck.size(); i++) {
        cout << "(" << deck[i]->stamina_cost << ") " << deck[i]->name << " --" << deck[i]->description << "--" << endl;
    }
}

void Player::display_hand() {
    cout << hand.size() << " cards in hand:" << endl;
    for(int i = 0; i < hand.size(); i++) {
        cout << i << "  (" << hand[i]->stamina_cost << ") " << hand[i]->name << " --" << hand[i]->description << "--" << endl;
    }
}

void Player::display_discard_pile() {
    cout << discard_pile.size() << " cards in discard pile:" << endl;
    for(int i = 0; i < discard_pile.size(); i++) {
        cout << "(" << discard_pile[i]->stamina_cost << ") " << discard_pile[i]->name << " --" << discard_pile[i]->description << "--" << endl;
    }
}

void Player::display_draw_pile() {
    cout << draw_pile.size() << " cards in draw pile:" << endl;
    for(int i = 0; i < draw_pile.size(); i++) {
        cout << "(" << draw_pile[i]->stamina_cost << ") " << draw_pile[i]->name << " --" << draw_pile[i]->description << "--" << endl;
    }
}

void Player::round_start() {
    std::random_shuffle(draw_pile.begin(), draw_pile.end());
    draw_cards();
    stamina = max_stamina;
}

void Player::draw_cards() {
    for(int i = 0; i < hand.size(); i++) { //when the player draws cards, put all cards in hand into the discard pile.
        discard_pile.push_back(hand[i]);
    }
    hand.clear();
    //
    for(int i = 0; i < num_draw; i++) {

        hand.push_back(draw_pile[0]);
        draw_pile.erase(draw_pile.begin()); //draw a card from draw pile and remove it from draw pile.

        if(draw_pile.empty()) { //shuffle discard pile back into draw pile
            for(int j = 0; j < discard_pile.size(); j++) {
                draw_pile.push_back(discard_pile[j]);
            }
            discard_pile.clear();
            auto rng = std::default_random_engine {};
            std::shuffle(std::begin(draw_pile), std::end(draw_pile), rng);
        }
    }
}

void Player::take_damage(int dmg) {
    int dmgMitigated = 0;
    vector<int> remove_eff;
    for(int i = 0; i < effects.size(); i++) { //account for any effects that might change dmg dealt
        if(effects[i]->name == "armor") {
            effects[i]->magnitude-=dmg;
            if(effects[i]->magnitude > 0) {
                dmgMitigated = dmg;
            } else if(effects[i]->magnitude == 0) {
                dmgMitigated = dmg;
                remove_eff.push_back(i);
            } else if(effects[i]->magnitude < 0) {
                dmgMitigated = effects[i]->magnitude+dmg; //add back to get pre-dmg magnitude
                remove_eff.push_back(i);
            }
        }
    }
    for(int x : remove_eff) { //remove all effects that have been "used up"
        effects.erase(effects.begin() + x);
    }

    dmg-=dmgMitigated;

    hp-=dmg;
    cout << "You took " << dmg << " damage." << endl;

    if(hp < 0) {
        hp = 0;
    }
}

void Player::add_to_coins(int n) {
    coins+=n;
    cout << "You gained " << n << " coins." << endl;
}

void Player::add_effect(string name, int magnitude) {
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
