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
    //string name, string stamina_cost, string card_type, string description, string rarity, string info
    for(int i = 0; i < 5; i++) {
        Card *card = new Card();
        card->init("Fire fist", "Deal 5 damage", "Basic", "1", "attack", "none|0|5|choose|;");
        deck.push_back(card);
    }
    for(int i = 0; i < 5; i++) {
        Card *card = new Card();
        card->init("Defend", "Gain 5 block", "Basic", "1", "effect", "armor|5|0|self|;");
        deck.push_back(card);
    }
    init_all_common_cards();
    init_all_rare_cards();
}

void Player::init_deck_before_combat() {
    hand.clear();
    discard_pile.clear();
    draw_pile.clear();
    consume_pile.clear();
    vector<Card*> deckCopy = deck;
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(deckCopy), std::end(deckCopy), rng); //at the start of battle shuffle the deck and then add all to draw_pile.
    for(int i = 0; i < deckCopy.size(); i++) {
        draw_pile.push_back(deckCopy[i]);
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

void Player::get_cards(int numGet, bool set_from_each, int num_common, int num_rare) {
    int szC = common_cards.size();
    int szR = rare_cards.size();
    vector<int> cards_choose;
    if(set_from_each) {
        for(int i = 0; i < num_common; i++) {
            int rn = rand()%(szC);
            if(std::find(cards_choose.begin(), cards_choose.end(), rn) != cards_choose.end()) { //already has this common card
                i--;
            } else {
                cards_choose.push_back(rn);
                deck.push_back(common_cards[rn]);
                cout << i << "  (" << common_cards[rn]->stamina_cost << ") [" << common_cards[rn]->rarity << "]: " << common_cards[rn]->name <<  " --" << common_cards[rn]->description << "--" << endl;
            }
        }
        cards_choose.clear();
        for(int i = 0; i < num_rare; i++) {
            int rn = rand()%(szR);
            if(std::find(cards_choose.begin(), cards_choose.end(), rn) != cards_choose.end()) { //already has this rare card
                i--;
            } else {
                cards_choose.push_back(rn);
                deck.push_back(rare_cards[rn]);
                cout << i << "  (" << rare_cards[rn]->stamina_cost << ") [" << rare_cards[rn]->rarity << "]: " << rare_cards[rn]->name <<  " --" << rare_cards[rn]->description << "--" << endl;
            }
        }

    }
    else {
        vector<string> classifs;
        for(int i = 0; i < numGet; i++) {
            int randNum = rand()%(3);
            if(randNum == 0) { // 1/3 chance to get rare card
                int rn = rand()%(szR);
                if(std::find(cards_choose.begin(), cards_choose.end(), rn) != cards_choose.end()) { //you cannot get the same index rare and common card tho
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
        for(int j = 0; j < cards_choose.size(); j++) {
            if(classifs[j] == "common") {
                deck.push_back(common_cards[cards_choose[j]]);
                cout << "Got 1 " << " (" << common_cards[cards_choose[j]]->stamina_cost << ") [" << common_cards[cards_choose[j]]->rarity << "]: " << common_cards[cards_choose[j]]->name <<  " --" << common_cards[cards_choose[j]]->description << "--" << endl;
            } else if(classifs[j] == "rare") {
                deck.push_back(rare_cards[cards_choose[j]]);
                cout << "Got 1 " << " (" << rare_cards[cards_choose[j]]->stamina_cost << ") [" << rare_cards[cards_choose[j]]->rarity << "]: " << rare_cards[cards_choose[j]]->name <<  " --" << rare_cards[cards_choose[j]]->description << "--" << endl;
            }
        }
    }
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
            if(std::find(cards_choose.begin(), cards_choose.end(), rn) != cards_choose.end()) { //you cannot get the same index rare and common card tho
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
    while(numGet > 0) {
        cout << "You can choose " << numGet << " more cards, or type skip to skip your remaining options." << endl;
        cout << "Card choices:" << endl;
        for(int j = 0; j < cards_choose.size(); j++) {
            //cout << "Printing cards" << endl;
            if(classifs[j] == "common") {
                cout << j << "  (" << common_cards[cards_choose[j]]->stamina_cost << ") [" << common_cards[cards_choose[j]]->rarity << "]: " << common_cards[cards_choose[j]]->name <<  " --" << common_cards[cards_choose[j]]->description << "--" << endl;
            } else if(classifs[j] == "rare") {
                cout << j << "  (" << rare_cards[cards_choose[j]]->stamina_cost << ") [" << rare_cards[cards_choose[j]]->rarity << "]: " << rare_cards[cards_choose[j]]->name <<  " --" << rare_cards[cards_choose[j]]->description << "--" << endl;
            }
        }
        cout << "Choose a card." << endl;
        string aa;
        cin >> aa;
        if(aa == "skip") {
            break;
        } else if(stoi(aa) < cards_choose.size()) { //TODO: CHECK FOR NON-INTEGER INPUTS
            if(classifs[stoi(aa)] == "common") {
                cout << "Added a " << common_cards[cards_choose[stoi(aa)]]->name << " to your deck." << endl;
                deck.push_back(common_cards[cards_choose[stoi(aa)]]);
                classifs.erase(classifs.begin() + stoi(aa));
                cards_choose.erase(cards_choose.begin() + stoi(aa));
            } else if(classifs[stoi(aa)] == "rare") {
                cout << "Added a " << rare_cards[cards_choose[stoi(aa)]]->name << " to your deck." << endl;
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
        cout << "(" << deck[i]->stamina_cost << ") [" << deck[i]->name << "] --" << deck[i]->description << "--" << endl;
    }
}

void Player::display_hand() {
    cout << hand.size() << " cards in hand:" << endl;
    for(int i = 0; i < hand.size(); i++) {
        cout << i << "  (" << hand[i]->stamina_cost << ") [" << hand[i]->name << "] --" << hand[i]->description << "--" << endl;
    }
}

void Player::display_discard_pile() {
    cout << discard_pile.size() << " cards in discard pile:" << endl;
    for(int i = 0; i < discard_pile.size(); i++) {
        cout << i << "  (" << discard_pile[i]->stamina_cost << ") [" << discard_pile[i]->name << "] --" << discard_pile[i]->description << "--" << endl;
    }
}

void Player::display_draw_pile() {
    cout << draw_pile.size() << " cards in draw pile:" << endl;
    for(int i = 0; i < draw_pile.size(); i++) {
        cout << i << "  (" << draw_pile[i]->stamina_cost << ") [" << draw_pile[i]->name << "] --" << draw_pile[i]->description << "--" << endl;
    }
}

void Player::display_consume_pile() {
    cout << consume_pile.size() << " cards in consume pile:" << endl;
    for(int i = 0; i < consume_pile.size(); i++) {
        cout << i << "  (" << consume_pile[i]->stamina_cost << ") [" << consume_pile[i]->name << "] --" << consume_pile[i]->description << "--" << endl;
    }
}

void Player::display_player() {
    cout << "hp: " << hp << endl;
    cout << "Max hp: " << max_hp << endl;
    cout << "Max stamina: " << max_stamina << endl;
    cout << "Number of cards drawn each turn: " << num_draw << endl;
    cout << "Number of coins: " << coins << endl;
}

void Player::round_start() {
    draw_cards(num_draw);
    stamina = max_stamina;
}

void Player::draw_cards(int num_to_draw) {
    for(int i = 0; i < num_to_draw; i++) {
        if(hand.size() > max_hand_size) {
            cout << "Reached max hand size of " << max_hand_size << "." << endl;
            break;
        }
        if(draw_pile.empty()) { //shuffle discard pile back into draw pile
            for(int j = 0; j < discard_pile.size(); j++) {
                draw_pile.push_back(discard_pile[j]);
            }
            discard_pile.clear();
            auto rng = std::default_random_engine {};
            std::shuffle(std::begin(draw_pile), std::end(draw_pile), rng); //randomly shuffle the draw pile
        }
        hand.push_back(draw_pile[0]);
        draw_pile.erase(draw_pile.begin()); //draw a card from draw pile and remove it from draw pile.
    }
}

void Player::discard_cards() { //discards cards if discard in effects
    for(int aa = 0; aa < effects.size(); aa++) {
        if(effects[aa]->name == "discard") {
            for(int i = 0; i < effects[aa]->magnitude; i++) {
                if(hand.size() == 0) { //nothing to discard
                    break;
                } else if(hand.size() == 1) {
                    cout << "Discarded " << hand[0]->name << "." << endl;
                    discard_pile.push_back(hand[0]); //move card to discard pile
                    hand.erase(hand.begin());
                } else {
                    bool act = false;
                    cout << "Choose a card to discard." << endl;
                    for(int j = 0; j < hand.size(); j++) {
                        cout << j << "  (" << hand[j]->stamina_cost << ") [" << hand[j]->name << "] --" << hand[j]->description << "--" << endl;
                    }
                    while(!act) { //TODO what if user doesn't input an int
                        int idx;
                        cin >> idx;
                        if(idx < hand.size()) {
                            cout << "Discarded " << hand[idx]->name << "." << endl;
                            discard_pile.push_back(hand[idx]); //move card to discard pile
                            hand.erase(hand.begin()+idx);
                            act = true;
                        } else {
                            cout << "Choose an existing card." << endl;
                        }
                    }
                }
            }
            effects.erase(effects.begin() + aa); //remove this effect
            return;
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
    } else if(name == "add_status_wound") {
        for(int i = 0; i < magnitude; i++) { //TODO CHANGE THIS
            Card *card = new Card();
            card->init("Wound", "Unplayable", "Status", "0", "status", "none|0|0|self|;");
            card->set_unplayable();
            discard_pile.push_back(card);
        }
        return;
    } else if(name == "add_status_daze") { //TODO CHANGE THIS
        for(int i = 0; i < magnitude; i++) {
            Card *card = new Card();
            card->init("Daze", "Consume", "Status", "0", "status", "none|0|0|self|;");
            card->set_consume();
            discard_pile.push_back(card);
        }
        return;
    } else if(name == "add_status_bleed") { //TODO CHANGE THIS
        for(int i = 0; i < magnitude; i++) {
            Card *card = new Card();
            card->set_consume();
            card->init("Bleed", "Take 3 damage at the end of your turn. Consume.", "Status", "1", "status", "end_turn_damage|0|3|self|;"); //playing it deals 3 damage to self
            discard_pile.push_back(card);
        }
        return;
    } else if(name == "convert_armor_to_rage") { //change armor into rage
        name = "rage";
        int m = -1;
        for(int i = 0; i < effects.size(); i++) {
            if(effects[i]->name == "armor") {
                m = effects[i]->magnitude;
                effects.erase(effects.begin() + i);
            }
        }
        if(m == -1) {
            return;
        } else {
            magnitude = m;
        }
        cout << "name: " << name << " , magnitude: " << magnitude << endl;
    } else if(sizeof(name) > 7 && name.substr(0,8) == "add_card") { //add this card to hand
        for(int i = 0; i < magnitude; i++) {
            hand.push_back(card_name_map[name.substr(9,sizeof(name)-1)]);
        }
        return;
    }

    int agilityAdd = 0;
    for(int i = 0; i < effects.size(); i++) {
        if(effects[i]->name == "agility") {
            agilityAdd = effects[i]->magnitude;
        }
    }
    if(name == "armor") {
        magnitude+=agilityAdd; //add the bonus gained from agility
    }

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

void Player::dec_magnitude(int idx) { //might change if relic: "effect x no longer decays
    effects[idx]->magnitude--;
}
bool Player::erase_effect_single_turn(int idx) {
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

void Player::init_all_common_cards() {
    //string name, string description, string rarity, string stamina_cost, string card_type, string info
    Card *card = new Card();
    card->init("Enrage", "Gain 3 rage", "Common", "1", "effect", "rage|3|0|self|;");
    common_cards.push_back(card);
    card_name_map.insert(std::make_pair("Enrage", card));

    Card *card1 = new Card();
    card1->init("Embolden", "Gain 1 stamina and 1 rage", "Common", "0", "effect", "rage|3|0|self|;stamina|1|0|self|;");
    common_cards.push_back(card1);
    card_name_map.insert(std::make_pair("Embolden", card1));

    Card *card2 = new Card();
    card2->init("Know your enemy", "Give all enemies 3 weak and 3 vulnerable", "Common", "2", "effect", "weak|3|0|all enemies|;vulnerable|3|0|all enemies|;");
    common_cards.push_back(card2);
    card_name_map.insert(std::make_pair("Know your enemy", card2));

    Card *card3 = new Card();
    card3->init("Hack and slash", "Deal 3 damage, 4 times", "Common", "2", "attack", "none|0|3|choose|;none|0|3|choose|;none|0|3|choose|;none|0|3|choose|;");
    common_cards.push_back(card3);
    card_name_map.insert(std::make_pair("Hack and slash", card3));

    Card *card4 = new Card();
    card4->init("Flame cuffs", "Gain 4 retaliate", "Common", "2", "effect", "retaliate|4|0|self|;");
    common_cards.push_back(card4);
    card_name_map.insert(std::make_pair("Flame cuffs", card4));

    Card *card5 = new Card();
    card5->init("Ignite", "Deal 10 damage", "Common", "1", "attack", "none|0|10|choose|;");
    common_cards.push_back(card5);
    card_name_map.insert(std::make_pair("Ignite", card5));

    Card *card6 = new Card();
    card6->init("Feint", "Gain 4 armor and 1 agility", "Common", "1", "effect", "armor|4|0|self|;agility|1|0|self|;");
    common_cards.push_back(card6);
    card_name_map.insert(std::make_pair("Feint", card6));

    Card *card7 = new Card();
    card7->init("Reinforce", "Gain 2 armor, 3 times", "Common", "1", "effect", "armor|2|0|self|;armor|2|0|self|;armor|2|0|self|;");
    common_cards.push_back(card7);
    card_name_map.insert(std::make_pair("Reinforce", card7));

    Card *card8 = new Card();
    card8->init("Cripple", "Deal 5 damage, apply 2 weak, and apply 2 vulnerable", "Common", "2", "attack", "none|0|5|choose|;weak|2|0|choose|;vulnerable|2|0|choose|;");
    common_cards.push_back(card8);
    card_name_map.insert(std::make_pair("Cripple", card8));

    /*//SET CONDITIONAL
    Card *card9 = new Card();
    card9->init("Devour|Deal 5 damage. If this kills an enemy, gain 5 hp and 2 stamina|Common|1|;none|0|5|choose|;");
    common_cards.push_back(card9);
    card_name_map.insert(std::make_pair("Devour", card9));*/

    Card *card10 = new Card();
    card10->init("Strategize", "Draw 2 cards and then discard 2 cards", "Common", "0", "effect", "draw|2|0|self|;discard|2|0|self|;");
    common_cards.push_back(card10);
    card_name_map.insert(std::make_pair("Strategize", card10));

    Card *card11 = new Card();
    card11->init("Counter", "Gain 2 retaliate and 1 agility", "Common", "1", "effect", "retaliate|2|0|self|;agility|1|0|self|;");
    common_cards.push_back(card11);
    card_name_map.insert(std::make_pair("Counter", card11));

    Card *card12 = new Card();
    card12->init("Big punch", "Deal 35 damage", "Common", "4", "attack", "none|0|35|choose|;");
    common_cards.push_back(card12);
    card_name_map.insert(std::make_pair("Big punch", card12));

    Card *card13 = new Card();
    card13->init("Jab", "Deal 4 damage", "Common", "0", "attack", "none|0|4|choose|;");
    common_cards.push_back(card13);
    card_name_map.insert(std::make_pair("Jab", card13));

    Card *card14 = new Card();
    card14->init("War cry", "Gain 5 rage and give all enemies 5 weak and 5 vulnerable", "Common", "3", "effect", "rage|5|0|self|;vulnerable|5|0|all enemies|;weak|5|0|all enemies|;");
    common_cards.push_back(card14);
    card_name_map.insert(std::make_pair("War cry", card14));

    Card *card15 = new Card();
    card15->init("What doesn't kill you", "Lose 3 hp and gain 7 rage", "Common", "1", "effect", "none|0|3|self|;rage|7|0|self|;");
    common_cards.push_back(card15);
    card_name_map.insert(std::make_pair("What doesn't kill you", card15));

    Card *card16 = new Card();
    card16->init("Impenetrable armor", "Retain your armor for 1 turn.", "Common", "1", "effect", "retain armor|1|0|self|;");
    common_cards.push_back(card16);
    card_name_map.insert(std::make_pair("Impenetrable armor", card16));

    Card *card17 = new Card();
    card17->init("Know yourself", "Gain 4 rage and 2 agility", "Common", "2", "effect", "rage|4|0|self|;agility|2|0|self|;");
    common_cards.push_back(card17);
    card_name_map.insert(std::make_pair("Know yourself", card17));

    Card *card18 = new Card();
    card18->init("Close combat", "Deal 10 damage to the closest enemy X times.", "Common", "X", "attack-closest", "none|0|10|closest enemy|;");
    common_cards.push_back(card18);
    card_name_map.insert(std::make_pair("Close combat", card18));

    Card *card19 = new Card();
    card19->init("Uppercut", "Deal 20 damage to the closest enemy and gain 2 vulnerable.", "Common", "1", "attack-closest", "none|0|20|closest enemy|;vulnerable|2|0|self|;");
    common_cards.push_back(card19);
    card_name_map.insert(std::make_pair("Uppercut", card19));

    /*//conditional
    Card *card20 = new Card();
    card20->init("Cross|Deal 5 damage. If the enemy intends to attack, deal 10 more.|Common|1|;none|0|5|choose|;none|0|10|choose|;");
    common_cards.push_back(card20);
    card_name_map.insert(std::make_pair("Cross", card20));*/

    Card *card21 = new Card();
    card21->init("Hook", "Deal 15 damage to the closest enemy.", "Common", "1", "attack-closest", "none|0|15|closest enemy|;");
    common_cards.push_back(card21);
    card_name_map.insert(std::make_pair("Hook", card21));

    Card *card22 = new Card();
    card22->init("Grab", "Deal 3 damage to the closest enemy and give it 2 vulnerable", "Common", "1", "attack-closest", "none|0|3|closest enemy|;vulnerable|2|0|closest enemy|;");
    common_cards.push_back(card22);
    card_name_map.insert(std::make_pair("Grab", card22));

    /*Card *card23 = new Card(); //conditional
    card23->init("Spoils of war|If an enemy has died this turn, draw 2 cards.|Common|0|;draw|2|0|self|;");
    common_cards.push_back(card23);
    card_name_map.insert(std::make_pair("Spoils of war", card23));*/

    Card *card24 = new Card(); //conditional
    card24->init("Siege", "Give all enemies 2X weak and 2X vulnerable.", "Common", "X", "effect", "weak|2|0|all enemies|;vulnerable|2|0|all enemies|;");
    common_cards.push_back(card24);
    card_name_map.insert(std::make_pair("Siege", card24));

    Card *card25 = new Card();
    card25->init("Layered armor", "Gain 4X armor.", "Common", "X", "effect", "armor|4|0|self|;");
    common_cards.push_back(card25);
    card_name_map.insert(std::make_pair("Layered armor", card25));

    Card *card26 = new Card();
    card26->init("Reflexive dodge", "Gain 5 armor and 1 stamina.", "Common", "0", "effect", "armor|5|0|self|;stamina|1|0|self|;");
    common_cards.push_back(card26);
    card_name_map.insert(std::make_pair("Reflexive dodge", card26));

    Card *card27 = new Card(); //this card fucking sucks
    card27->init("Rouse", "Take 2 damage and gain 1 stamina.", "Common", "0", "effect", "none|0|2|self|;stamina|1|0|self|;");
    common_cards.push_back(card27);
    card_name_map.insert(std::make_pair("Rouse", card27));

    Card *card28 = new Card();
    card28->init("Prioritize", "Gain 2 stamina and add 2 wounds to your discard pile.", "Common", "0", "effect", "add_status_wound|2|0|self|;stamina|2|0|self|;");
    common_cards.push_back(card28);
    card_name_map.insert(std::make_pair("Prioritize", card28));

    Card *card29 = new Card();
    card29->init("Bottled anger", "Gain 7 rage. Consume.", "Common", "0", "effect", "rage|7|0|self|;");
    card29->set_consume();
    common_cards.push_back(card29);
    card_name_map.insert(std::make_pair("Bottled anger", card29));

    Card *card30 = new Card();
    card30->init("Fortify", "Gain 30 armor. Consume.", "Common", "1", "effect", "armor|30|0|self|;");
    card30->set_consume();
    common_cards.push_back(card30);
    card_name_map.insert(std::make_pair("Fortify", card30));

    Card *card31 = new Card();
    card31->init("Consecutive strikes", "Add 2 pummels to your hand.", "Common", "1", "attack", "add_card=Pummel|2|0|self|;");
    common_cards.push_back(card31);
    card_name_map.insert(std::make_pair("Consecutive strikes", card31));

    Card *card32 = new Card();
    card32->init("Focus chi", "Your attacks for the next 2 turns deal double damage.", "Common", "2", "effect", "2x|2|0|self|;");
    common_cards.push_back(card32);
    card_name_map.insert(std::make_pair("Focus chi", card32));

    Card *card33 = new Card();
    card33->init("Force palm", "Deal 5 damage and give the enemy 2 vulnerable.", "Common", "1", "attack", "none|0|5|choose|;vulnerable|2|0|choose|;");
    common_cards.push_back(card33);
    card_name_map.insert(std::make_pair("Force palm", card33));

    Card *card34 = new Card();
    card34->init("Like a feather", "Gain 3 agility.", "Common", "1", "effect", "agility|3|0|self|;");
    common_cards.push_back(card34);
    card_name_map.insert(std::make_pair("Like a feather", card34));

    Card *card35 = new Card();
    card35->init("Dash", "Draw 3 cards. Consume.", "Common", "0", "effect", "draw|3|0|self|;");
    card35->set_consume();
    common_cards.push_back(card35);
    card_name_map.insert(std::make_pair("Dash", card35));

    Card *card36 = new Card();
    card36->init("Twitch fibers", "Gain 8 agility. Consume.", "Common", "0", "effect", "agility|8|0|self|;");
    card36->set_consume();
    common_cards.push_back(card36);
    card_name_map.insert(std::make_pair("Twitch fibers", card36));

    Card *card37 = new Card();
    card37->init("First move", "Draw 4 cards and gain 3 stamina. Consume.", "Common", "3", "effect", "draw|1|0|self|;stamina|1|0|self|;");
    card37->set_consume();
    common_cards.push_back(card37);
    card_name_map.insert(std::make_pair("First move", card37));

    Card *card38 = new Card();
    card38->init("Dodge and weave", "Gain 2 armor and 2 agility", "Common", "1", "effect", "armor|2|0|self|;agility|2|0|self|;");
    common_cards.push_back(card38);
    card_name_map.insert(std::make_pair("Dodge and weave", card38));

    Card *card39 = new Card();
    card39->init("Limber", "Discard a card and gain 4 agility.", "Common", "1", "effect", "discard|1|0|self|;agility|4|0|self|;");
    common_cards.push_back(card39);
    card_name_map.insert(std::make_pair("Limber", card39));

    Card *card40 = new Card();
    card40->init("Unshackle", "Gain 1 stamina and discard 1 card.", "Common", "0", "effect", "discard|1|0|self|;stamina|1|0|self|;");
    common_cards.push_back(card40);
    card_name_map.insert(std::make_pair("Unshackle", card40));

    Card *card41 = new Card();
    card41->init("Headbutt", "Deal 15 damage and discard a card.", "Common", "1", "attack", "discard|1|0|self|;none|0|15|choose|;");
    common_cards.push_back(card41);
    card_name_map.insert(std::make_pair("Headbutt", card41));


    Card *cardEx1 = new Card(); //only to be added, don't add to list
    cardEx1->init("Brace", "Gain 1 armor, twice. Consume.", "Common", "0", "effect", "armor|1|0|self|;armor|1|0|self|;");
    cardEx1->set_consume();
    card_name_map.insert(std::make_pair("Brace", cardEx1));

    Card *cardEx2 = new Card(); //only to be added, don't add to list
    cardEx2->init("Pummel", "Deal 1 damage, twice. Consume.", "Common", "0", "attack", "none|0|1|choose|;none|0|1|choose|;");
    cardEx2->set_consume();
    card_name_map.insert(std::make_pair("Pummel", cardEx2));

}
void Player::init_all_rare_cards() {
    //name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
    Card *card = new Card();
    card->init("Lunge", "Gain 10 rage and 5 vulnerable", "Rare", "1", "effect", "rage|10|0|self|;vulnerable|5|0|self|;");
    rare_cards.push_back(card);
    card_name_map.insert(std::make_pair("Lunge", card));

    Card *card1 = new Card();
    card1->init("Kill kill kill", "Deal 8X damage.", "Rare", "X", "effect", "none|0|8|choose|;");
    rare_cards.push_back(card1);
    card_name_map.insert(std::make_pair("Kill kill kill", card1));

    //add cards to hand
    Card *card2 = new Card();
    card2->init("Die die die", "Gain 2 rage and add 3 Pummels to your hand", "Rare", "3", "effect", "add_card=Pummel|3|0|self|;rage|2|0|self|;");
    rare_cards.push_back(card2);
    card_name_map.insert(std::make_pair("Die die die", card2));

    Card *card3 = new Card();
    card3->init("Tear off", "Convert your armor into rage. Consume.", "Rare", "0", "effect", "convert_armor_to_rage|0|0|self|;");
    rare_cards.push_back(card3);
    card_name_map.insert(std::make_pair("Tear off", card3));

    Card *card4 = new Card();
    card4->init("Taunt", "Give all enemies 3 rage and 6 vulnerable.", "Rare", "1", "effect", "rage|3|0|all enemies|;vulnerable|6|0|all enemies|;");
    rare_cards.push_back(card4);
    card_name_map.insert(std::make_pair("Taunt", card4));

    Card *card5 = new Card();
    card5->init("Iron will", "Gain 15 armor. Retain your armor for 1 turn.", "Rare", "2", "effect", "armor|15|0|self|;retain armor|1|0|self|;");
    rare_cards.push_back(card5);
    card_name_map.insert(std::make_pair("Iron will", card5));

    Card *card6 = new Card();
    card6->init("Be like water", "Gain 5 rage and 3 agility.", "Rare", "2", "effect", "rage|5|0|self|;agility|3|0|self|;");
    rare_cards.push_back(card6);
    card_name_map.insert(std::make_pair("Be like water", card6));

    Card *card7 = new Card();
    card7->init("Clear mind", "Remove all negative effects.", "Rare", "1", "effect", "cleanse|1|0|self|;");
    rare_cards.push_back(card7);
    card_name_map.insert(std::make_pair("Clear mind", card7));

    Card *card8 = new Card();
    card8->init("Flying knee", "Deal 30 damage to the closest enemy.", "Rare", "3", "attack-closest", "none|0|35|closest enemy|;");
    rare_cards.push_back(card8);
    card_name_map.insert(std::make_pair("Flying knee", card8));

    Card *card9 = new Card();
    card9->init("Mixup", "Your attacks for the rest of the turn deal double damage.", "Rare", "1", "effect", "2x|1|0|self|;");
    rare_cards.push_back(card9);
    card_name_map.insert(std::make_pair("Mixup", card9));

    /*//conditional
    Card *card10 = new Card();
    card10->init("Overhead|Deal 5 damage. Deal 10 more if the enemy does not intend to attack.|Rare|1|;none|0|5|choose|;none|0|10|choose|;");
    rare_cards.push_back(card10);
    card_name_map.insert(std::make_pair("Overhead", card10));*/

    /*
    Card *card11 = new Card();
    card11->init("Parry|For the next turn, whenever you successfully block an attack, give the attacker 2 vulnerable.|Rare|1|;vulnerable|2|0|attacker|;");
    rare_cards.push_back(card11);
    card_name_map.insert(std::make_pair("Parry", card11));
    */

    /*
    Card *card12 = new Card();
    card12->init("Hard read|If the enemy intends to attack, gain 10 armor. Otherwise, deal 10 damage.|Rare|1|;armor|10|0|self|;none|0|10|choose|;");
    rare_cards.push_back(card12);
    card_name_map.insert(std::make_pair("Hard read", card12));*/

    /*
    Card *card13 = new Card(); //conditional
    card13->init("Study opponent|If the enemy intends to attack, gain 2 armor and 1 retaliate. Otherwise, gain 2 rage and 1 agility.|Rare|0|;none|0|0|choose|;");
    rare_cards.push_back(card13);
    card_name_map.insert(std::make_pair("Study opponent", card13));*/

    Card *card14 = new Card();
    card14->init("Calculated strike", "Deal 8 damage and draw 1 card.", "Rare", "1", "effect", "draw|1|0|self|;none|0|8|choose|;");
    rare_cards.push_back(card14);
    card_name_map.insert(std::make_pair("Calculated strike", card14));

    Card *card15 = new Card();
    card15->init("Blood deal", "Lose 5 hp and gain 2 stamina.", "Rare", "1", "effect", "stamina|2|0|self|;none|0|4|self|;");
    rare_cards.push_back(card15);
    card_name_map.insert(std::make_pair("Blood deal", card15));

    Card *card16 = new Card();
    card16->init("Cursed blood", "Lose 5 hp and draw 3 cards.", "Rare", "0", "effect", "draw|3|0|self|;none|0|5|self|;");
    rare_cards.push_back(card16);
    card_name_map.insert(std::make_pair("Cursed blood", card16));

    Card *card17 = new Card();
    card17->init("Safeguard", "Retain your armor for 2 turns.", "Rare", "2", "effect", "retain armor|2|0|self|;");
    rare_cards.push_back(card17);
    card_name_map.insert(std::make_pair("Safeguard", card17));

    Card *card18 = new Card();
    card18->init("Toxic armor", "Gain 8 armor and 2 retaliate. Consume.", "Rare", "1", "effect", "armor|8|0|self|;retaliate|2|0|self|;");
    card18->set_consume();
    rare_cards.push_back(card18);
    card_name_map.insert(std::make_pair("Toxic armor", card18));

    Card *card19 = new Card();
    card19->init("Blood barrier", "Lose X health and retain armor for X turns.", "Rare", "X", "effect", "retain armor|1|0|self|;none|0|1|self|;");
    rare_cards.push_back(card19);
    card_name_map.insert(std::make_pair("Blood barrier", card19));

    Card *card20 = new Card();
    card20->init("Batter", "Deal 2X damage to all enemies, twice.", "Rare", "X", "attack", "none|2|0|all enemies|;none|0|2|all enemies|;");
    rare_cards.push_back(card20);
    card_name_map.insert(std::make_pair("Blood barrier", card20));

    Card *card21 = new Card();
    card21->init("Patience", "Draw 2 cards at the end of your turn.", "Rare", "1", "effect", "end_turn_draw|2|0|self|;");
    rare_cards.push_back(card21);
    card_name_map.insert(std::make_pair("Blood barrier", card21));


    /*Card *card19 = new Card();
    card19->init("Utilize skill|Draw cards equal to your agility.|Rare|0|;draw|1|0|self|;");
    rare_cards.push_back(card19);
    card_name_map.insert(std::make_pair("Utilize skill", card18));
    */
}
