#include "Player.h"
#include "Enemy.h"
#include "Relic.h"
#include "Card.h"

#include "Event.h"
#include <iostream>

Event::Event()
{}

Event::~Event()
{}

void Event::init(string classif, int num_enemies) {
    this->classif = classif;
    if(classif == "enemy") {
        is_enemy = true;
        fight = true;
    } else if(classif == "elite enemy") {
        is_elite_enemy = true;
        fight = true;
    } else if(classif == "bonfire") {
        is_bonfire = true;
    } else if(classif == "chance") {
        is_chance = true;
    } else if(classif == "shop") {
        is_shop = true;
    } else if(classif == "treasure") { //else
        is_treasure = true;
    } else if(classif == "boss") { //else
        is_boss = true;
        fight = true;
    } else if(classif == "start") { //else
        is_start = true;
    }
    this->num_enemies = num_enemies;
}

void Event::set_lower_events(vector<Event*> lower_events) {
    this->lower_events = lower_events;
}

void Event::init_elite_enemies() {
    curr_enemies.clear();
    int randN = rand()%(3); //0 to 2

    if(randN == 0) {
        //gladiator + henchman
        Enemy *gladiator = new Enemy();
        vector<Card*> v;
        Card *card = new Card();
        //name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
        card->init("Execute|Deal 20 damage|a|0|;none|0|20|player|;");
        card->set_type("attack");
        Card *card1 = new Card();
        card1->init("Strike|Deal 6 damage|a|0|;none|0|3|player|;");
        card1->set_type("attack");
        Card *card2 = new Card();
        card2->init("Mace swing|Deal 10 damage and add 1 wound to the player's discard pile|a|0|;add_status_wound|1|0|player|;");
        card2->set_type("attack");
        v.push_back(card);
        v.push_back(card1);
        v.push_back(card2);
        gladiator->init("Gladiator", v, 180, 0);

        int randNn = rand()%(5)-2; //-2 to 2
        vector<Card*> v2;
        Card *card5 = new Card();
        card5->init("Strike|Deal 3 damage|a|0|;none|0|3|player|;");
        card5->set_type("attack");
        v2.push_back(card5);
        Enemy *henchman = new Enemy();
        henchman->init("Henchman", v2, 20+randNn, 0);

        curr_enemies.push_back(gladiator);
        curr_enemies.push_back(henchman);
    } else if(randN == 1) {
        //illusionist
        vector<Card*> v;
        Card *card = new Card();
        card->init("Blinding flash|Add 3 daze to the player's discard pile|a|0|;add_status_daze|2|0|player|;");
        card->set_type("add_status");
        Card *card2 = new Card();
        card2->init("Trick|Deal 5 damage and add 1 daze to the player's discard pile|a|0|;none|0|5|player|;add_status_daze|1|0|player|;");
        card2->set_type("attack");
        v.push_back(card);
        v.push_back(card2);

        int randNn = rand()%(5)-2; //-2 to 2
        Enemy *enemy1 = new Enemy();
        enemy1->init("Illusionist", v, 33+randNn, 0);
        curr_enemies.push_back(enemy1);

        vector<Card*> v2;
        v2.push_back(card2);
        v2.push_back(card);
        randNn = rand()%(5)-2; //-2 to 2
        Enemy *enemy2 = new Enemy();
        enemy2->init("Illusionist", v2, 33+randNn, 0);
        curr_enemies.push_back(enemy2);


    } else if(randN == 2) {
        //think of one
    }

}

void Event::init_enemies() {
    curr_enemies.clear();
    int randN = rand()%(4); //0 to 3

    if(randN == 0) {
        //bandit
        vector<Card*> v;
        Card *card = new Card();
        card->init("Stab|Deal 5 damage|a|0|;none|0|5|player|;");
        card->set_type("attack");
        v.push_back(card);
        for(int i = 0; i < 2; i++) {
            int randNn = rand()%(5)-2; //-2 to 2
            Enemy *enemy1 = new Enemy();
            enemy1->init("Bandit", v, 30+randNn, 0);
            curr_enemies.push_back(enemy1);
        }

    } else if(randN == 1) {
        //slime
        vector<Card*> v;
        Card *card = new Card();
        card->init("Lick|Give 2 vulnerable|a|0|;vulnerable|2|0|player|;");
        card->set_type("add_effect");
        Card *card2 = new Card();
        card2->init("Phagocytosis|Deal 3 damage|a|0|;none|0|3|player|;");
        card2->set_type("attack");
        v.push_back(card);
        v.push_back(card2);
        for(int i = 0; i < 5; i++) { //5 slimes
            int randNn = rand()%(5)-2; //-2 to 2
            Enemy *enemy1 = new Enemy();
            enemy1->init("Slime", v, 10+randNn, 0);
            curr_enemies.push_back(enemy1);
            randNn = rand()%(2);
            if(randNn == 0) { //swap cards
                v.push_back(v[0]);
                v.erase(v.begin());
            }
        }

    } else if(randN == 2) {
        //demon acolyte
        vector<Card*> v;
        Card *card = new Card();
        card->init("Hellpower|Gain 4 rage|a|0|;rage|3|0|self|;");
        card->set_type("add_effect");
        Card *card5 = new Card();
        card5->init("Swipe|Deal 8 damage|a|0|;none|0|8|player|;");
        card5->set_type("attack");
        Card *card2 = new Card();
        card2->init("Screech|Give 3 weak|a|0|;weak|3|0|player|;");
        card2->set_type("add_effect");
        Card *card4 = new Card();
        card4->init("Brimstone|Deal 30 damage|a|0|;none|0|30|player|;");
        card4->set_type("attack");
        v.push_back(card);
        v.push_back(card5);
        v.push_back(card2);
        v.push_back(card4);
        int randNn = rand()%(11)-5; //-5 to 5
        Enemy *enemy1 = new Enemy();
        enemy1->init("Demon acolyte", v, 70+randNn, 0);
        curr_enemies.push_back(enemy1);


    } else if(randN == 3) {
        //stone guard
        vector<Card*> v;
        Card *card = new Card();
        card->init("Defensive stance|Gain 15 armor|a|0|;armor|15|0|self|;");
        card->set_type("add_effect");
        Card *card2 = new Card();
        card2->init("Thorn armor|Gain 4 retaliate|a|0|;retaliate|4|0|self|;");
        card2->set_type("add_effect");
        Card *card3 = new Card();
        card3->init("Defensive stance|Gain 15 armor|a|0|;armor|15|0|self|;");
        card3->set_type("add_effect");
        Card *card4 = new Card();
        card4->init("Armored strike|Deal 10 damage and gain 5 armor|a|0|;armor|5|0|self|;none|0|10|player|;");
        card4->set_type("attack");
        v.push_back(card);
        v.push_back(card2);
        v.push_back(card3);
        v.push_back(card4);
        int randNn = rand()%(11)-5; //-5 to 5
        Enemy *enemy1 = new Enemy();
        enemy1->init("Stone guard", v, 100+randNn, 0);
        curr_enemies.push_back(enemy1);
    }



}

void Event::init_boss() {
    curr_enemies.clear();
    Enemy *boss = new Enemy();
    vector<Card*> v;

    Card *card1 = new Card();
    card1->init("Empower|Gain 5 rage|a|0|;rage|5|0|self|;");
    card1->set_type("add_status");
    v.push_back(card1);

    Card *card4 = new Card();
    card4->init("Gnash|Deal 1 damage, 4 times|a|0|;none|0|1|player|;none|0|1|player|;none|0|1|player|;none|0|1|player|;");
    card4->set_type("attack");
    v.push_back(card4);

    Card *card = new Card();
    card->init("Heavy strike|Deal 25 damage|a|0|;none|0|25|player|;");
    card->set_type("attack");
    v.push_back(card);

    Card *card2 = new Card();
    card2->init("Double slash|Deal 3 damage, twice|a|0|;none|0|3|player|;none|0|3|player|;");
    card2->set_type("attack");
    v.push_back(card2);

    boss->init("Blood demon", v, 300, 0);
    curr_enemies.push_back(boss);
}

bool Event::all_dead() {
    return (curr_enemies.size() == 0);
}

void Event::check_dead() { //kind of like an update enemy size method.
    for(int i = 0; i < curr_enemies.size(); i++) {
        if(curr_enemies[i]->is_dead()) {
            curr_enemies.erase(curr_enemies.begin()+i);
            i = 0;
        }
    }
}

void Event::skip() {
    for(int i = 0; i < curr_enemies.size(); i++) {
        curr_enemies[i]->hp = 0;
    }
}

void Event::display_enemies() {
    for(int i = 0; i < curr_enemies.size(); i++) {
        cout << i << "  " << curr_enemies[i]->name << ", hp=" << curr_enemies[i]->hp << endl;

        /*cout << "Attacks:" << endl;
        string nameCurrAtk;
        for(int j = 0; j < curr_enemies[i]->attacks.size(); j++) {
            vector<vector<string>> does = curr_enemies[i]->attacks[j]->split();
            if(j == curr_enemies[i]->card_pos) {
                nameCurrAtk = does[0][0];
            }
            if(j != curr_enemies[i]->attacks.size()-1) {
                cout << does[0][0] << ": " << does[0][1] << ", " << endl;
            } else {
                cout << does[0][0] << ": " << does[0][1] << "." << endl;
            }
        }
        cout << curr_enemies[i]->name << " intends to use " << nameCurrAtk << "." << endl;*/
        vector<vector<string>> splt = curr_enemies[i]->attacks[curr_enemies[i]->card_pos]->split();
        cout << curr_enemies[i]->name << " intends to use " << splt[0][0] << ": " << splt[0][1] << "." << endl;
        if(!curr_enemies[i]->effects.empty()) {
            cout << "Effects:" << endl;
            for(int q=0; q<curr_enemies[i]->effects.size(); q++) {
                cout << curr_enemies[i]->effects[q]->name << " " << curr_enemies[i]->effects[q]->magnitude << endl;
            }
        }
        cout << endl;
    }

}

bool Event::enemy_reward(Player *p) {
    p->add_to_coins(50);
    p->get_card(1, 3);
}
bool Event::elite_enemy_reward(Player *p) {
    p->add_to_coins(100);
    p->get_card(2, 5);
}
bool Event::shop_offer(Player *p) {

}
bool Event::chance_offer(Player *p) {

}
bool Event::get_treasure(Player *p) {
    p->add_to_coins(200);
    p->get_card(1, 4);
    p->get_card(1, 4);
}
bool Event::boss_reward(Player *p) {
    //p->add_to_coins(200);
}
