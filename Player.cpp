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
    //CHANGE
    for(int i = 0; i < 5; i++) {
        Card *card = new Card();
        //name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
        card->init("Fireball|Cast a fireball at an enemy, dealing 5 damage|Basic|1|;none|0|5|choose|;");
        deck.push_back(card);
    }
    for(int i = 0; i < 5; i++) {
        Card *card = new Card();
        card->init("Defend|Don 5 armor|Basic|1|;armor|5|0|self|;");
        deck.push_back(card);
    }
    Card *card3 = new Card();
    card3->init("Embolden|Give yourself 1 energy and 1 rage|Common|0|;rage|1|0|self|;stamina|1|0|self|;");
    deck.push_back(card3);
    init_all_common_cards();
    init_all_rare_cards();
}

void Player::init_deck_before_combat() {
    hand.clear();
    discard_pile.clear();
    draw_pile.clear();
    for(int i = 0; i < deck.size(); i++) {
        draw_pile.push_back(deck[i]);
    }
}

void Player::clear_effects() {
    effects.clear();
}

void Player::return_hand() { //put all hand in discard pile
    while(!hand.empty()) {
        //cout << "returning hand" << endl;
        discard_pile.push_back(hand[0]);
        hand.erase(hand.begin());
    }
}

bool Player::isDead() {
    return hp==0;
}

void Player::get_card(int numGet, int numCards) {
    int szC = common_cards.size();
    int szR = rare_cards.size();
    vector<int> cards_choose;
    vector<string> classifs;
    for(int i = 0; i < numCards; i++) {
        int randNum = rand()%(3);
        if(randNum == 0) { // 1/3 chance to get rare card
            int rn = rand()%(szR);
            if(std::find(cards_choose.begin(), cards_choose.end(), rn) != cards_choose.end()) {
                i--;
            } else {
                cards_choose.push_back(rn);
                classifs.push_back("rare");
            }
        } else {
            int rn = rand()%(szC);
            if(std::find(cards_choose.begin(), cards_choose.end(), rn) != cards_choose.end()) {
                i--;
            } else {
                cards_choose.push_back(rn);
                classifs.push_back("common");
            }
        }
    }
    while(numGet > 0) { //THERE IS A PROBLEM WITH SOME CARDS HERE WTF TODO
        cout << "You can choose " << numGet << " more cards, or type skip to skip your remaining options." << endl;
        cout << "Card choices:" << endl;
        for(int j = 0; j < cards_choose.size(); j++) {
            cout << "Printing cards" << endl;
            if(classifs[j] == "common") {
                cout << common_cards[cards_choose[j]]->info << endl;
                vector<vector<string>> does = common_cards[cards_choose[j]]->split();
                cout << j << ":   (" << does[0][3] << ") [" << does[0][0] << "]: " << does[0][2] <<  " --" << does[0][1] << "--" << endl;
            } else if(classifs[j] == "rare") {
                cout << common_cards[cards_choose[j]]->info << endl;
                vector<vector<string>> does = rare_cards[cards_choose[j]]->split();
                cout << j << ":   (" << does[0][3] << ") [" << does[0][0] << "]: " << does[0][2] <<  " --" << does[0][1] << "--" << endl;
            }
        }
        cout << "Choose a card." << endl;
        string aa;
        cin >> aa;
        if(aa == "skip") {
            break;
        } else if(stoi(aa) < cards_choose.size()) {
            if(classifs[stoi(aa)] == "common") {
                cout << "Added a " << common_cards[cards_choose[stoi(aa)]]->split()[0][0] << " to your deck." << endl;
                deck.push_back(common_cards[cards_choose[stoi(aa)]]);
                classifs.erase(classifs.begin() + stoi(aa));
                cards_choose.erase(cards_choose.begin() + stoi(aa));
            } else if(classifs[stoi(aa)] == "rare") {
                cout << "Added a " << rare_cards[cards_choose[stoi(aa)]]->split()[0][0] << " to your deck." << endl;
                deck.push_back(rare_cards[cards_choose[stoi(aa)]]);
                classifs.erase(classifs.begin() + stoi(aa));
                cards_choose.erase(cards_choose.begin() + stoi(aa));
            }
            numGet--;
        } else {
            cout << "Choose one of the available cards." << endl;
        }
    }
}

void Player::display_deck() {
    cout << deck.size() << " cards in deck:" << endl;
    for(int i = 0; i < deck.size(); i++) {
        vector<vector<string>> does = deck[i]->split();
        cout << "(" << does[0][3] << ") [" << does[0][0] << "] --" << does[0][1] << "--" << endl;
    }
}

void Player::display_hand() {
    cout << hand.size() << " cards in hand:" << endl;
    for(int i = 0; i < hand.size(); i++) {
        vector<vector<string>> does = hand[i]->split();
        cout << to_string(i) << "  (" << does[0][3] << ") [" << does[0][0] << "] --" << does[0][1] << "--" << endl;
    }
}

void Player::display_discard_pile() {
    cout << discard_pile.size() << " cards in discard pile:" << endl;
    for(int i = 0; i < discard_pile.size(); i++) {
        vector<vector<string>> does = discard_pile[i]->split();
        cout << i << "  (" << does[0][3] << ") [" << does[0][0] << "] --" << does[0][1] << "--" << endl;
    }
}

void Player::display_draw_pile() {
    cout << draw_pile.size() << " cards in draw pile:" << endl;
    for(int i = 0; i < draw_pile.size(); i++) {
        vector<vector<string>> does = draw_pile[i]->split();
        cout << i << "  (" << does[0][3] << ") [" << does[0][0] << "] --" << does[0][1] << "--" << endl;
    }
}

void Player::round_start() {
    std::random_shuffle(draw_pile.begin(), draw_pile.end());
    draw_cards(num_draw);
    stamina = max_stamina;
}

void Player::draw_cards(int num_to_draw) {
    /*for(int i = 0; i < hand.size(); i++) { //when the player draws cards, put all cards in hand into the discard pile.
        discard_pile.push_back(hand[i]);
    }
    hand.clear();*/
    //
    for(int i = 0; i < num_to_draw; i++) {

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
    if(name == "stamina") {
        stamina+=magnitude;
        return;
    } else if(name == "draw") {
        draw_cards(magnitude);
        return;
    } else if(name == "cleanse") {
        for(int i = 0; i < effects.size();) {
            if(!effects[i]->is_good) {
                effects.erase(effects.begin() + i);
            } else {
                i++;
            }
        }
        return;
    }

    int agilityAdd = 0;
    for(int i = 0; i < effects.size(); i++) {
        if(effects[i]->name == "agility") {
            agilityAdd = effects[i]->magnitude;
        }
    }
    cout << "agilityAdd: " << agilityAdd << endl;

    Effect *eff = new Effect();
    eff->init(name, magnitude);
    bool alreadyContains = false;
    for(Effect *e : effects) {
        if(e->name == eff->name) {
            if(e->name == "armor") {
                e->magnitude += (eff->magnitude + agilityAdd); //add the bonus gained from agility
                alreadyContains = true;
            } else {
                e->magnitude += eff->magnitude;
                alreadyContains = true;
            }
        }
    }
    if(!alreadyContains) {
        effects.push_back(eff);
    }
}

void Player::init_all_common_cards() {
    //CHANGE
    //name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
    Card *card = new Card();
    card->init("Enrage|Give yourself 3 rage|Common|1|;rage|3|0|self|;");
    common_cards.push_back(card);

    Card *card1 = new Card();
    card1->init("Embolden|Give yourself 1 energy and 1 rage|Common|0|;rage|3|0|self|;stamina|1|0|self|;");
    common_cards.push_back(card1);

    Card *card2 = new Card();
    card2->init("Know your enemy|Give all enemies 3 weak and 3 vulnerable|Common|2|;weak|3|0|all enemies|;vulnerable|3|0|all enemies|;");
    common_cards.push_back(card2);

    Card *card3 = new Card();
    card3->init("Hack and slash|Deal 3 damage 3 times|Common|2|;none|0|3|choose|;none|0|3|choose|;none|0|3|choose|;");
    common_cards.push_back(card3);

    Card *card4 = new Card(); //MUST SET CONDITIONAL TODO
    card4->init("Flame cuffs|Gain 4 retaliate|Common|2|;retaliate|4|0|self|;");
    common_cards.push_back(card4);

    Card *card5 = new Card();
    card5->init("Ignite|Deal 9 damage|Common|2|;none|0|9|choose|;");
    common_cards.push_back(card5);

    Card *card6 = new Card();
    card6->init("Feint|Gain 4 armor and 1 agility|Common|2|;armor|4|0|self|;agility|1|0|self|;");
    common_cards.push_back(card6);

    //THERE IS A PROBLEM WITH PRINTING THIS CARD
    Card *card7 = new Card();
    card7->init("Reinforce|Gain 2 armor 3 times|Common|1|;armor|2|0|self|;armor|2|0|self|;armor|2|0|self|;");
    common_cards.push_back(card7);

    Card *card8 = new Card();
    card8->init("Cripple|Deal 5 damage, apply 2 weak, and apply 2 vulnerable|Common|2|;none|0|5|choose|;weak|2|0|choose|;vulnerable|2|0|choose|;");
    common_cards.push_back(card8);

    //SET CONDITIONAL
    Card *card9 = new Card();
    card9->init("Devour|Deal 5 damage. If this kills an enemy, gain 5 hp and 2 stamina|Common|1|;none|0|5|choose|;");
    common_cards.push_back(card9);

    Card *card10 = new Card();
    card10->init("Strategize|Draw 2 cards|Common|1|;draw|2|0|self|;");
    common_cards.push_back(card10);

    Card *card11 = new Card();
    card11->init("Counter|Gain 2 retaliate and 1 agility|Common|1|;retaliate|2|0|self|;agility|1|0|self|;");
    common_cards.push_back(card11);

    Card *card12 = new Card();
    card12->init("Big punch|Deal 30 damage|Common|4|;none|0|30|choose|;");
    common_cards.push_back(card12);

    Card *card13 = new Card();
    card13->init("Jab|Deal 3 damage|Common|0|;none|0|3|choose|;");
    common_cards.push_back(card12);

    Card *card14 = new Card();
    card14->init("War cry|Give yourself 5 rage and give all enemies 5 weak and 5 vulnerable|Common|3|;rage|5|0|self|;vulnerable|5|0|all enemies|;weak|5|0|all enemies|;");
    common_cards.push_back(card14);

    Card *card15 = new Card();
    card15->init("What doesn't kill you|Take 5 damage and gain 5 rage|Common|1|;none|0|5|self|;rage|5|0|self|;");
    common_cards.push_back(card15);

    Card *card16 = new Card();
    card16->init("Impenetrable armor|Retain your armor for 1 turn.|Common|1|;retain armor|1|0|self|;");
    common_cards.push_back(card16);

    Card *card17 = new Card();
    card17->init("Know yourself|Give yourself 3 rage and 2 agility|Common|2|;rage|3|0|self|;agility|2|0|self|;");
    common_cards.push_back(card17);

    Card *card18 = new Card();
    card18->init("Close combat|Deal 5 damage to the closest enemy X times.|Common|X|;none|0|5|closest enemy|;");
    common_cards.push_back(card18);

    Card *card19 = new Card();
    card19->init("Uppercut|Deal 20 damage to the closest enemy and gain 2 vulnerable.|Common|1|;none|0|20|closest enemy|;vulnerable|2|0|self|;");
    common_cards.push_back(card19);

    //conditional
    Card *card20 = new Card();
    card20->init("Cross|Deal 5 damage. If the enemy intends to attack, deal 10 more.|Common|1|;none|0|5|choose|;none|0|10|choose|;");
    common_cards.push_back(card20);

    Card *card21 = new Card();
    card21->init("Hook|Deal 10 damage to the closest enemy.|Common|1|;none|0|10|closest enemy|;");
    common_cards.push_back(card21);

    Card *card22 = new Card();
    card22->init("Grab|Deal 3 damage to the closest enemy and give it 2 vulnerable|Common|1|;none|0|3|closest enemy|;vulnerable|2|0|closest enemy|;");
    common_cards.push_back(card22);

}
void Player::init_all_rare_cards() {
    //name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
    Card *card = new Card();
    card->init("Lunge|Give yourself 7 rage and 7 vulnerable|Rare|1|;rage|7|0|self|;frail|7|0|self|;");
    rare_cards.push_back(card);

    //need damage = cards/turn
    Card *card1 = new Card();
    card1->init("Kill kill kill|Deal 8X damage.|Rare|X|;none|0|8|choose|;");
    rare_cards.push_back(card1);

    //add cards to hand
    Card *card2 = new Card();
    card2->init("Die die die|Gain 3 rage and add 3 jabs to your hand|Rare|3|;none|3|0|choose|;");
    rare_cards.push_back(card2);

    //convert armor into rage
    Card *card3 = new Card();
    card3->init("Tear off|Convert your armor into rage.|Rare|2|;convert|0|0|self|;");
    rare_cards.push_back(card3);

    Card *card4 = new Card();
    card3->init("Taunt|Give all enemies 3 rage and 6 vulnerable.|Rare|1|;rage|3|0|all enemies|;vulnerable|6|0|all enemies|;");
    rare_cards.push_back(card4);

    Card *card5 = new Card();
    card5->init("Iron will|Gain 15 armor. Retain your armor for 1 turn.|Rare|2|;armor|15|0|self|;retain armor|1|0|self|;");
    rare_cards.push_back(card5);

    Card *card6 = new Card();
    card6->init("Be like water|Gain 5 rage and 3 agility.|Rare|2|;rage|5|0|self|;agility|3|0|self|;");
    rare_cards.push_back(card6);

    Card *card101 = new Card(); //lmao whoops
    card6->init("Be like water|Gain 5 rage and 3 agility.|Rare|2|;rage|5|0|self|;agility|3|0|self|;");
    rare_cards.push_back(card101);

    Card *card7 = new Card();
    card7->init("Clear mind|Remove all negative effects.|Rare|1|;cleanse|1|0|self|;");
    rare_cards.push_back(card7);

    Card *card8 = new Card();
    card8->init("Flying knee|Deal 30 damage to the closest enemy.|Rare|3|;none|0|35|closest enemy|;");
    rare_cards.push_back(card8);

    Card *card9 = new Card();
    card9->init("Mixup|Your next attack deals double damage.|Rare|2|;2x|1|0|self|;");
    rare_cards.push_back(card9);

    //conditional
    Card *card10 = new Card();
    card10->init("Overhead|Deal 5 damage. Deal 10 more if the enemy does not intend to attack.|Rare|1|;none|0|5|choose|;none|0|10|choose|;");
    rare_cards.push_back(card10);

    Card *card11 = new Card();
    card11->init("Parry|For the next turn, whenever you successfully block an attack, give the attacker 2 vulnerable.|Rare|1|;vulnerable|2|0|attacker|;");
    rare_cards.push_back(card11);

    Card *card12 = new Card();
    card12->init("Hard read|If the enemy intends to attack, gain 10 armor. Otherwise, deal 10 damage.|Rare|1|;armor|10|0|self|;none|0|10|choose|;");
    rare_cards.push_back(card12);

    Card *card13 = new Card();
    card13->init("Study opponent|If the enemy intends to attack, gain 2 armor and 1 retaliate. Otherwise, gain 2 rage and 1 agility.|Rare|0|;none|0|0|choose|;");

    rare_cards.push_back(card13);


}
