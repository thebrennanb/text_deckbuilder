#include "Player.h"
#include "Enemy.h"
#include "Relic.h"
#include "Event.h"

#include "Game.h"

#include<vector>
#include<map>
#include<string>
#include<sstream>
#include<iostream>
#include<algorithm>
using namespace std;

Game::Game()
{}

Game::~Game()
{}

void Game::init_event_map() {

    event_chances.insert(make_pair(35, "enemy"));
    event_chances.insert(make_pair(20+35, "elite enemy"));
    event_chances.insert(make_pair(15+55, "bonfire"));
    event_chances.insert(make_pair(15+70, "shop"));
    event_chances.insert(make_pair(10+85, "chance"));
    event_chances.insert(make_pair(5+95, "treasure"));

    vector<Event*> event_row;
    Event *e1 = new Event();
    e1->init("start", 0);
    event_row.push_back(e1);
    event_map.push_back(event_row); //2 starting events

    int numLastRow = 1;
    int maxim = 0;
    int minim = 1;
    for(int i = 1; i < num_events; i++) {
        vector<Event*> this_row;
        if(numLastRow > 5) {
            maxim = max_events_per_row;
            minim = numLastRow-1;
        } else if(numLastRow <= 3) {
            maxim = numLastRow+1;
            minim = 2;
        } else {
            maxim = numLastRow+1;
            minim = numLastRow-1;
        }

        if( (i == num_events - 5 && numLastRow == 6) || (i == num_events - 2 && numLastRow >= 3) || (i == num_events - 3 && numLastRow >= 4) || (i == num_events - 4 && numLastRow >= 5) ) { //x-to-last needs to be at least x-1
            maxim = numLastRow-1;
            minim = maxim;
        }

        //inserted this to make only one event per row DEBUG
        //maxim = 1;
        //minim = 1;
        //
        if(minim <= 0) {
            minim = 1;
        }

        if(minim > maxim) {
            int temp = minim;
            maxim = minim;
            minim = temp;
        }

        string eventName;
        int randNum = rand()%(maxim-minim+1) + minim;
        int rand2 = rand()%(3); //0 to 2
        if(randNum >= max_events_per_row-2 && randNum != numLastRow-1) {
            if(rand2 == 0) { //1/3 chance
                randNum-=1;
            }
        }
        for(int j = 0; j < randNum; j++) { //add this many events on this row.
            Event *event2 = new Event();

            eventName = "enemies"; //set default
            int randNum2 = rand()%(99); //between 0 and 100 supposedly
            for( map<int, string>::iterator it = event_chances.begin(); it != event_chances.end(); ++it ) { //go thru chances
                if(randNum2 < it->first) {
                    eventName = it->second;
                    break;
                }
            }
            if(eventName == "enemies") {
                event2->init(eventName, (rand()%(3) + 1)); //between 1 and 3 supposedly
            } else if(eventName == "elite enemy") {
                event2->init(eventName, 1); // 1 elite enemy
            } else {
                event2->init(eventName, 0);
            }

            //set lower_right and lower_left of leftmost and rightmost events here
            if(numLastRow > 1) {
                if(j == 0) { //leftmost Event
                    vector<Event*> ev;
                    ev.push_back(event_map[i-1][j]);
                    //ev.push_back(nullptr);
                    event2->set_lower_events(ev);
                } else if(j == randNum-1) { //rightmost Event
                    vector<Event*> ev;
                    //ev.push_back(nullptr);
                    ev.push_back(event_map[i-1][event_map[i-1].size()-1]);
                    event2->set_lower_events(ev);
                }
            }

            this_row.push_back(event2);
            numLastRow = randNum;
        }

        event_map.push_back(this_row);
    }
    //make last row a boss
    vector<Event*> event_bonfires;
    Event *f = new Event();
    f->init("bonfire", 0);
    event_bonfires.push_back(f);
    Event *f2 = new Event();
    f2->init("bonfire", 0);
    event_bonfires.push_back(f2);
    event_map.push_back(event_bonfires);

    vector<Event*> event_rowBoss;
    Event *boss = new Event();
    boss->init("boss", 1);
    event_rowBoss.push_back(boss);
    event_map.push_back(event_rowBoss);

    for(int i = 1; i < event_map.size(); i++) { //thru rows
        for(int j = 0; j < event_map[i].size(); j++) { //go thru all on row

            if(event_map[i-1].size() == event_map[i].size()+1) {
                vector<Event*> lowEv;
                for(int a = 0; a < 2; a++) { //add 0 and 1
                    if(j+a >= 0 && j+a < event_map[i-1].size()) { //within size
                        lowEv.push_back(event_map[i-1][j+a]);
                    }
                }
                event_map[i][j]->set_lower_events(lowEv);
            }
            else if(event_map[i-1].size() == event_map[i].size()) {
                vector<Event*> lowEv;
                for(int a = -1; a < 2; a++) { //add -1, 0, and 1
                    if(j+a >= 0 && j+a < event_map[i-1].size()) { //within size
                        lowEv.push_back(event_map[i-1][j+a]);
                    }
                }
                event_map[i][j]->set_lower_events(lowEv);
            }
            else if(event_map[i-1].size() == event_map[i].size()-1) {
                vector<Event*> lowEv;
                for(int a = -1; a < 1; a++) { //add -1 and 0
                    if(j+a >= 0 && j+a < event_map[i-1].size()) { //within size
                        lowEv.push_back(event_map[i-1][j+a]);
                    }
                }
                event_map[i][j]->set_lower_events(lowEv);
            }

        }
    }
    int randNum = 0;
    //go through all lower events
    for(int i = 1; i < event_map.size()-1; i++) { //thru rows
        vector<Event*> ev;
        for(int j = 0; j < event_map[i].size(); j++) { //go thru all on row
            if(event_map[i][j]->lower_events.size() == 3) { //remove 2 of the 3
                randNum = rand()%(3); //0 to 2
                event_map[i][j]->lower_events.erase(event_map[i][j]->lower_events.begin() + randNum);
                //cout << event_map[i][j]->lower_events.size() << endl;

                /*if(event_map[i].size() > event_map[i-1].size()) { //only remove a second one if the diff b/w the rows is <
                    randNum = rand()%(2); //0 to 1
                    event_map[i][j]->lower_events.erase(event_map[i][j]->lower_events.begin() + randNum);
                }*/
            }/* else if(event_map[i][j]->lower_events.size() == 2) { //else if event contains 2 lower events
                if(j != 0 && j != event_map[i].size()-1) { //index of current event is not on l/r edge
                    randNum = rand()%(3);
                    if(randNum < event_map[i][j]->lower_events.size()) { //gives a 2/3 chance to remove
                        event_map[i][j]->lower_events.erase(event_map[i][j]->lower_events.begin() + randNum); //remove 1 of them
                    }
                } else {
                    if(j == 0) {
                        randNum = rand()%(3);
                        if(randNum < event_map[i][j]->lower_events.size()) { //gives a 2/3 chance to remove
                            event_map[i][j]->lower_events.erase(event_map[i][j]->lower_events.begin() + 1); //remove the rightmost
                        }
                    }
                    else { //j at length-1
                        randNum = rand()%(3);
                        if(randNum < event_map[i][j]->lower_events.size()) { //gives a 2/3 chance to remove
                            event_map[i][j]->lower_events.erase(event_map[i][j]->lower_events.begin()); //remove the leftmost
                        }
                    }
                }
            }*/


            //Go thru all lower connections
            for(int a = 0; a < event_map[i][j]->lower_events.size(); a++) {
                ev.push_back(event_map[i][j]->lower_events[a]);
            }
            //
        }
        for(int c = 0; c < event_map[i].size(); c++) {
            for(int v = 0; v < event_map[i][c]->lower_events.size(); v++) {
                //cout << i << " " << c << " " << v << " " << event_map[i][c]->lower_events[v]->classif << " count: " << count(ev.begin(), ev.end(), event_map[i][c]->lower_events[v]) << endl;
                int cnt = count(ev.begin(), ev.end(), event_map[i][c]->lower_events[v]);
                if(cnt > 1 && event_map[i][c]->lower_events.size() != 1) {
                    //^if count of this event are in the vector > 1:
                    int randNum = rand()%(2); //either 0 or 1
                    //cout << "looking at " << i << " " << c << " " << v << " (" << event_map[i][c]->lower_events[v]->classif << ")" << endl;

                    if(randNum == 1 || cnt == 3) { //50/50 chance
                        //cout << "removing " << i << " " << c << " " << v << " (" << event_map[i][c]->lower_events[v]->classif << ")" << endl;
                        /*if(event_map[i].size() == event_map[i-1].size() && (c == event_map[i-1].size()-1 || c == 0)) {
                            cout << i << " " << c << " " << randNum << " " << cnt << endl;
                        }*/
                        ev.erase(std::remove(ev.begin(), ev.end(), event_map[i][c]->lower_events[v]), ev.end());
                        event_map[i][c]->lower_events.erase(event_map[i][c]->lower_events.begin() + v);
                        v = -1; //reset for safety
                    }
                }


            }
        }
    }
}

void Game::player_choose_starting() {
    cout << "Choose:\nType \"relic\" to gain 1 random relic.\nType \"cards\" to gain 2 random common cards and 1 random rare card." << endl;
    string str;
    while(1) {
        cin >> str;
        if(str == "relic") {

            break;
        } else if(str == "cards") {
            player->get_cards(0, true, 2, 1);
            break;
        } else {
            cout << "Couldn't understand." << endl;
        }
    }
}

void Game::display_player() {
    cout << "hp: " << player->hp << endl;
    cout << "Max stamina: " << player->max_stamina << endl;
    cout << "Number of cards drawn each turn: " << player->num_draw << endl;
    cout << "Number of coins: " << player->coins << endl;
    if(!player->effects.empty()) {
        cout << "Effects:" << endl;
        for(int i=0; i<player->effects.size(); i++) {
            cout << player->effects[i]->name << " " << player->effects[i]->magnitude << endl;
        }
    }
}

void Game::init() {
    player->init_deck();
    init_event_map();
    event_conv.insert(make_pair("enemy", 'e'));
    event_conv.insert(make_pair("elite enemy", 'E'));
    event_conv.insert(make_pair("bonfire", 'f'));
    event_conv.insert(make_pair("shop", '$'));
    event_conv.insert(make_pair("chance", '?'));
    event_conv.insert(make_pair("treasure", 't'));
    event_conv.insert(make_pair("boss", 'B'));
    event_conv.insert(make_pair("start", '^'));
}

void Game::play_map() {
    while(!end_game && !playerQuit && player_location.second != event_map.size()-1) {
        display_map();
        string direction;
        cout << "Where will you go? ";
        string build = "";
        vector<string> ok;
        int x = player_location.first;
        int y = player_location.second;
        int sizeDiff = event_map[y+1].size()-event_map[y].size(); //will be either -1, 0, or 1
        //cout << "Player coord: " << x << "," << y << endl;
        for(int i = 0; i < event_map[y+1].size(); i++) { //go thru the row above player
            for(int j = 0; j < event_map[y+1][i]->lower_events.size(); j++) {
                if(event_map[y+1][i]->lower_events[j] == event_map[y][x]) {
                    //cout << "i -> x: " << i << " " << x << endl;
                    if(event_map[y+1].size() == event_map[y].size()) { //rows are the same length
                        if(i == x) {
                            build += "u/";
                            ok.push_back("u");
                        } else if(i == x-1){
                            build += "l/";
                            ok.push_back("l");
                        } else {
                            build += "r/";
                            ok.push_back("r");
                        }
                    }
                    else if(event_map[y+1].size() > event_map[y].size()){ //row above > this row
                        if(i == x) { //offset 1
                            build += "l/";
                            ok.push_back("l");
                        } else {
                            build += "r/";
                            ok.push_back("r");
                        }
                    }
                    else { //row above < this row
                        if(i == x) { //offset -1
                            build += "r/";
                            ok.push_back("r");
                        } else {
                            build += "l/";
                            ok.push_back("l");
                        }

                    }
                }

            }
        }

        cout << "(" << build.substr(0,build.size()-1) << ")" << endl;
        while(1) {
            cin >> direction;
            if(direction == "me") {
                display_player();
                cin >> direction;
            }
            if(!(std::find(ok.begin(), ok.end(), direction) != ok.end()) && direction != "skip") {
                cout << "Choose a direction." << endl;
            } else {
                //cout << player_location.first << " sizediff: " << sizeDiff << endl;
                if(direction == "r") {
                    if(sizeDiff != -1) {
                        player_location.first++;
                    }

                } else if(direction == "l") {
                    if(sizeDiff != 1) {
                        player_location.first--;
                    }
                }
                //cout << player_location.first << "," << player_location.second << endl;
                break;
            }
        }
        if(direction != "skip") { //add master control "skip" that allows you to skip a room.

        player_location.second++;
        do_event(event_map[player_location.second][player_location.first]);
        //cout << "Event: " << event_map[player_location.second][player_location.first]->classif << "\n" << endl;


        } else { //was skip - move up one floor
            player_location.second++;
        }
    }

}

void Game::display_map() {
    cout << "Map:" << endl;
    int spB = 24; //1: 24 -event- 24 for one event    -- (spaces between)
    string spaces = "                        "; //24 spaces
    for(int i = event_map.size()-1; i >=0; i--) { //thru col
        /*if(event_map[i].size() == 5) {
            cout << "";
        }*/
        for(int j = 0; j < event_map[i].size(); j++) { //thru row
            if(i == 0 || i == event_map.size()-1) {
                cout << "                     ";
            } else {
                if(event_map[i].size() == 5) {
                    cout << spaces.substr(0,spB/event_map[i].size()+2);
                } else {
                    cout << spaces.substr(0,spB/event_map[i].size()+1);
                }

            }
            if(i == player_location.second && j == player_location.first) { //player location
                cout << "Y";
            } else {
                cout << event_conv.find(event_map[i][j]->classif)->second;
            }
        }
        cout << endl;
        //if(i != 0) { cout << "|" << endl;} //since there is only one event per row.

        //displaying from top to bottom
        if(i != 0) {
            int excessChars = 0;
            if(i == event_map.size()-1) {
                excessChars = 5;
            }
            string build = "";
            /*if(event_map[i].size() == 5) { //taken care of by later statement
                cout << " ";
            }*/
            if(event_map[i-1].size() == event_map[i].size()) { //same number of events
                for(int j = 0; j < event_map[i].size(); j++) {
                    if(event_map[i].size() == 5) {
                        excessChars--;
                    }
                    build+=spaces.substr(0,spB/event_map[i].size()+1-excessChars);
                    excessChars = 0;
                    int sz = event_map[i][j]->lower_events.size();
                    if(sz == 1) {
                        if(j == 0) {
                            if(event_map[i][j]->lower_events[0] == event_map[i-1][0]) {
                                build+="|";
                            } else { //if doesn't point to direct lower, skew direction
                                build+="\\";
                            }
                        }
                        else if(j == event_map[i].size()-1) {
                            if(event_map[i][j]->lower_events[sz-1] == event_map[i-1][event_map[i-1].size()-1]) {
                                build+="|";
                            } else { //if doesn't point to direct lower, skew direction
                                build+="/";
                            }
                        }
                        else {
                            if(event_map[i][j]->lower_events[0] == event_map[i-1][j-1]) {
                                build+="/";
                            } else if(event_map[i][j]->lower_events[0] == event_map[i-1][j]) {
                                build+="|";
                            } else { //if doesn't point to direct lower, skew direction
                                build+="\\";
                            }
                        }
                    } else { //number of lower events = 2
                        if(j == 0) {
                            build+="|\\";
                            excessChars++;
                        }
                        else if(j == event_map[i].size()-1) {
                            build = build.substr(0, build.size()-1);
                            build+="/|";
                        }
                        else {
                            if(event_map[i][j]->lower_events[0] == event_map[i-1][j-1]) { //
                                build = build.substr(0, build.size()-1);
                                build+="/|";
                            } else {
                                build+="|\\";
                                excessChars++;
                            }
                        }
                    }
                    //build+=to_string(sz);
                }
            } else if(event_map[i].size() > event_map[i-1].size()) {
                bool alreadyUsed = false;
                for(int j = 0; j < event_map[i].size(); j++) {
                    if(event_map[i].size() == 5) {
                        excessChars--;
                    }
                    build+=spaces.substr(0,spB/event_map[i].size()+1-excessChars);
                    excessChars = 0;
                    int sz = event_map[i][j]->lower_events.size();
                    if(j == 0) {
                        build+="\\";
                    }
                    else if(j == event_map[i].size()-1) {
                        //build = build.substr(0, build.size()-1);
                        build+="/";
                    }
                    else {
                        if(sz == 1) {
                            if(event_map[i][j]->lower_events[0] == event_map[i-1][j-1] && event_map[i][j]->lower_events[0] == event_map[i-1][j]) {
                                excessChars++;
                            }
                            if(event_map[i][j]->lower_events[0] == event_map[i-1][j-1]) { //down left
                                //build = build.substr(0, build.size()-1);
                                build+="/";
                            }
                            if(event_map[i][j]->lower_events[0] == event_map[i-1][j]){
                                build+="\\";
                            }
                        }
                        else if(sz == 2) {
                            build+="/\\";
                            excessChars++;
                        }
                        //excessChars++;
                    }
                    //build+=to_string(sz);

                }

            } else if(event_map[i].size() < event_map[i-1].size()) { //THE SPACING IS WRONG

                for(int j = 0; j < event_map[i].size(); j++) {
                    if(event_map[i].size() == 5) {
                        excessChars--;
                    }
                    build+=spaces.substr(0,spB/event_map[i].size()+1-excessChars);
                    excessChars = 0;
                    int sz = event_map[i][j]->lower_events.size();
                    if(j == 0) { //since pyramid, last row's first event must lead to this
                        build+="/";
                        //excessChars++;
                    }
                    if(sz == 1) {
                        if(j != 0 && j != event_map[i].size()-1) {
                            if(event_map[i][j]->lower_events[0] == event_map[i-1][j]) {
                                //build = build.substr(0, build.size()-1);
                                build+="/";
                            } else {
                                build+="\\";
                                excessChars++;
                            }
                        }
                    }
                    else if(sz == 2 && event_map[i].size() != 1) {
                        if(j == 0) {
                            build+="\\";
                        } else if(j == event_map[i].size()-1) {
                            build = build.substr(0, build.size()-1);
                            build+="/";
                        } else {
                            if(j != event_map[i].size()/2 || j%2 == 1) {
                                build = build.substr(0, build.size()-1);
                            }
                            build+="/\\";
                        }
                    }

                    if(j == event_map[i].size()-1) { //same concept as j==0
                        //build = build.substr(0, build.size()-1);
                        build+="\\";
                    }
                    //build+=to_string(sz);

                }
            }
            cout << build << endl;
        }
    }
}


//event stuff
void Game::do_event(Event* ev) {
    if(ev->is_bonfire) {
        int restoredHp = player->max_hp/3;
        player->hp += player->max_hp/3; //add 1/3 of max hp back
        if(player->hp > player->max_hp) {
            restoredHp -= player->hp-player->max_hp;
            player->hp = player->max_hp;
        }
        cout << "You rest at the bonfire. Restored " << restoredHp << " hp. (hp=" << player->hp << ")\n" << endl;
    }
    else if(event_map[player_location.second][player_location.first]->is_enemy) { //if it is a fight, initiate combat
        event_map[player_location.second][player_location.first]->init_enemies();
        combat();
        if(!playerQuit && !player->isDead()) {
            event_map[player_location.second][player_location.first]->enemy_reward(player);
        }
    }
    else if(event_map[player_location.second][player_location.first]->is_elite_enemy) {
        event_map[player_location.second][player_location.first]->init_elite_enemies();
        combat();
        if(!playerQuit && !player->isDead()) {
            event_map[player_location.second][player_location.first]->elite_enemy_reward(player);
        }
    }
    else if(event_map[player_location.second][player_location.first]->is_boss) {
        event_map[player_location.second][player_location.first]->init_boss();
        combat();
        if(!playerQuit && !player->isDead()) {
            event_map[player_location.second][player_location.first]->boss_reward(player);
        }
    }
    else if(event_map[player_location.second][player_location.first]->is_chance) {
        ev->chance_offer(player);
    }
    else if(event_map[player_location.second][player_location.first]->is_treasure) {
        ev->get_treasure(player);
    }
    else if(event_map[player_location.second][player_location.first]->is_shop) {
        ev->shop_offer(player);
    }
}

void Game::combat() {
    player->target = -2;
    player->init_deck_before_combat();
    while(1) {

        cout << "\n";
        inc_player_effects();

        play_player_turn();

        if(player->isDead()) { //idk maybe recoil dmg
            cout << "You died" << endl;
            end_game = true;
            break;
        }
        if(playerQuit) {
            end_game = true;
            break;
        }

        if(event_map[player_location.second][player_location.first]->all_dead()) {
            cout << "You won this battle!" << endl;
            break;
        }

        play_enemy_turn();

        inc_enemy_effects(event_map[player_location.second][player_location.first]);

        event_map[player_location.second][player_location.first]->check_dead();

        if(player->isDead()) {
            cout << "You died" << endl;
            end_game = true;
            break;
        }
    }
    player->clear_effects();
}

void Game::play_player_turn() {
    string str;
    player->round_start();
    int x = player_location.first;
    int y = player_location.second;

    bool endTurn = false;
    while(!endTurn) {
        event_map[y][x]->display_enemies();
        player->display_hand();
        cout << "\nStamina left: " << player->stamina << ", hp left: " << player->hp << ". What will you do?" << endl;
        bool madeAction = false;
        while(!madeAction) {
            string temp;
            vector<string> input;
            string line;
            getline(cin, line);
            istringstream iss(line);
            while(iss >> temp) {
                input.push_back(temp);
            }
            if(input.empty()) {
                cout << "Input something." << endl;
            } else {
                if(input.size() == 1) { //all inputs with 1 word
                    if(input[0] == "cheatskip") {
                        event_map[y][x]->skip();
                        madeAction = true;
                        endTurn = true;
                    }
                    if(input[0] == "help") {
                        cout << "Make an action:\n\"card <card>\" to play a card, \"draw pile\" to view your draw pile, \"discard pile\" to view your discard pile, \"end turn\" to end your turn, \"me\" to see everything about you, \"enemies\" to see everything about the current enemies, \"target <enemy>\" to set a target." << endl;
                    } else if(input[0] == "quit") {
                        playerQuit = true;
                        madeAction = true;
                        endTurn = true;
                        cout << "You quit." << endl;
                    } else if(input[0] == "me") {
                        display_player();
                    } else if(input[0] == "enemies") {
                        event_map[y][x]->display_enemies();
                    } else {
                        cout << "Could not understand." << endl;
                    }
                } else if(input.size() == 2) { //all the rest require > 1 input
                    if(input[0] == "end" && input[1] == "turn") {
                        madeAction = true;
                        endTurn = true;
                        cout << "Completed turn\n" << endl;
                    } else if(input[0] == "target") {
                        if(input[1] == "self") {
                            player->target = -1;
                            cout << "Setting target: self.";
                        } else {
                            if(stoi(input[1]) >= event_map[y][x]->curr_enemies.size()) {
                                cout << "Choose an existing enemy." << endl;
                            } else {
                                player->target = stoi(input[1]);
                                cout << "Setting target enemy: " << event_map[y][x]->curr_enemies[stoi(input[1])]->name;
                            }
                        }
                    } else if(input[0] == "draw" && input[1] == "pile") {
                       player->display_draw_pile();

                    } else if(input[0] == "discard" && input[1] == "pile") {
                        player->display_discard_pile();

                    } else if(input[0] == "display" && input[1] == "hand") {
                        player->display_hand();

                    } else if(input[0] == "display" && input[1] == "deck") {
                        player->display_deck();

                    } else if(input[0] == "card") { //play card
                        if(stoi(input[1]) >= player->hand.size()) { //card does not exist.
                            cout << "Card does not exist." << endl;
                        } else if(player->target == -2 || player->target >= event_map[y][x]->curr_enemies.size()) {
                            cout << "Set a target first." << endl;
                        } else if(player->hand[stoi(input[1])]->split()[0][3] != "X" && player->stamina-stoi(player->hand[stoi(input[1])]->split()[0][3]) < 0) { //not enough energy

                            cout << "Not enough stamina." << endl;
                        } else {
                            madeAction = true;

                            //play card and do effects and stuff here
                            play_card(stoi(input[1]));
                            //
                            event_map[y][x]->check_dead();
                            if(event_map[y][x]->all_dead()) {
                                endTurn = true;
                                break;
                            }

                        }
                    } else {
                        cout << "Could not understand." << endl;
                    }
                } else {
                    cout << "Could not understand" << endl;
                }
                cout << endl;
            }
        }
    }
    player->return_hand();
    //will print if player quit.
}

void Game::play_card(int idx) {
    //CHANGE

    int x = player_location.first;
    int y = player_location.second;

    //format: name,description,classification,stamina_cost;effect_applied,effect_magnitude,damage,target;same
    vector<vector<string>> does = player->hand[idx]->split();
    string name = does[0][0];
    if(!player->hand[idx]->playable) { //card not playable
        cout << name << " is unplayable." << endl;
        return;
    }

    string description = does[0][1];
    string classification = does[0][2];
    int staminaCost;
    int timesToPlay = 1;
    if(does[0][3] == "X") {
        staminaCost = player->stamina;
        timesToPlay = staminaCost;
    } else {
        staminaCost = stoi(does[0][3]);
    }
    player->stamina-=staminaCost; //subtract stamina cost from playing card
    cout << "Played " << name << endl;

    for(int t = 0; t < timesToPlay; t++) { //play 1 time if not "X", otherwise play staminaCost times
        for(int i = 1; i < does.size(); i++) {
            if(does[i][3] == "choose") {
                if(player->target == -1) { //player targeting self
                    //Do I need this?
                }
                else { //targeting enemy
                    if(stoi(does[i][2]) != 0) { //do damage if != 0
                        event_map[y][x]->curr_enemies[player->target]->take_damage(calc_damage_players_attack(stoi(does[i][2])));
                    }
                    if(does[i][0] != "none") {
                        event_map[y][x]->curr_enemies[player->target]->add_effect(does[i][0], stoi(does[i][1]));
                    }
                    do_retaliate_enemy(event_map[y][x]->curr_enemies[player->target], player->target);
                }

            } else if(does[i][3] == "self") { //target to hit is inherently self - this is different from setting target to self
                //do not calculate damage, this damage to self from cards. Bypasses effects like block, weak, frail, rage, etc.
                if(does[i][0] != "none") {
                    player->add_effect(does[i][0], stoi(does[i][1]));
                }
                if(stoi(does[i][2]) != 0) {
                    player->take_damage(stoi(does[i][2])); //ignore players attack?
                }

            } else if(does[i][3] == "all enemies") {
                for(int a = 0; a < event_map[y][x]->curr_enemies.size(); a++) {
                    if(stoi(does[i][2]) != 0) { //do damage if != 0
                        event_map[y][x]->curr_enemies[a]->take_damage(calc_damage_players_attack(stoi(does[i][2])));
                    }
                    if(does[i][0] != "none") {
                        event_map[y][x]->curr_enemies[a]->add_effect(does[i][0], stoi(does[i][1]));
                    }
                    do_retaliate_enemy(event_map[y][x]->curr_enemies[player->target], player->target);
                }
            } else if(does[i][3] == "closest enemy") { //do same as if player had target but only to 0th enemy
                if(stoi(does[i][2]) != 0) { //do damage if != 0
                    event_map[y][x]->curr_enemies[0]->take_damage(calc_damage_players_attack(stoi(does[i][2])));
                }
                if(does[i][0] != "none") {
                    event_map[y][x]->curr_enemies[0]->add_effect(does[i][0], stoi(does[i][1]));
                }
                do_retaliate_enemy(event_map[y][x]->curr_enemies[player->target], player->target);
            }
        }
    }

    if(!player->hand[idx]->consume) {
        player->discard_pile.push_back(player->hand[idx]); //put card in discard pile
        player->hand.erase(player->hand.begin()+idx); //remove card from hand.
    } else {
        player->hand.erase(player->hand.begin()+idx); //remove card from hand.
        cout << name << " was consumed." << endl;
    }

}

int Game::calc_damage_players_attack(int dmg) { //will currently not work for a player targeting self
    if(dmg == 0) { //Not always the case; maybe relic or card played makes all cards played deal x damage
        return 0;
    }
    int x = player_location.first;
    int y = player_location.second;

    bool doubleD = false;
    for(int i = 0; i < player->effects.size(); i++) { //go thru player effects
        if(player->effects[i]->name == "rage") {
            dmg+=player->effects[i]->magnitude;
        }
        if(player->effects[i]->name == "weak") {
            dmg-=player->effects[i]->magnitude;
        }
        if(player->effects[i]->name == "2x") {
            doubleD = true;
        }
    }

    if(doubleD) {
        dmg*=2;
    }

    if(player->target != -1) { //targeting enemy - factor in enemy effects
        for(int i = 0; i < event_map[y][x]->curr_enemies[player->target]->effects.size(); i++) { //go thru effects
            if(event_map[y][x]->curr_enemies[player->target]->effects[i]->name == "vulnerable") {
                dmg+=event_map[y][x]->curr_enemies[player->target]->effects[i]->magnitude;
            }
            else if(event_map[y][x]->curr_enemies[player->target]->effects[i]->name == "armor") {
                int dmgMitigated = 0;
                event_map[y][x]->curr_enemies[player->target]->effects[i]->magnitude-=dmg;
                if(event_map[y][x]->curr_enemies[player->target]->effects[i]->magnitude > 0) {
                    dmgMitigated = dmg;
                } else if(event_map[y][x]->curr_enemies[player->target]->effects[i]->magnitude == 0) {
                    dmgMitigated = dmg;
                    event_map[y][x]->curr_enemies[player->target]->effects.erase(event_map[y][x]->curr_enemies[player->target]->effects.begin()+i);
                    i--;
                } else if(event_map[y][x]->curr_enemies[player->target]->effects[i]->magnitude < 0) {
                    dmgMitigated = event_map[y][x]->curr_enemies[player->target]->effects[i]->magnitude+dmg; //add back to get pre-dmg magnitude
                    event_map[y][x]->curr_enemies[player->target]->effects.erase(event_map[y][x]->curr_enemies[player->target]->effects.begin()+i);
                    i--;
                }
                dmg-=dmgMitigated;
            }
        }
    }

    if(dmg < 0) {
        dmg = 0;
    }
    return dmg;
}


void Game::play_enemy_turn() {
    int x = player_location.first;
    int y = player_location.second;

    for(int i = 0; i < event_map[y][x]->curr_enemies.size(); i++) {
        if(!event_map[y][x]->curr_enemies[i]->is_dead()) {
            vector<vector<string>> does = event_map[y][x]->curr_enemies[i]->attacks[event_map[y][x]->curr_enemies[i]->card_pos]->split();
            string name = does[0][0];
            cout << event_map[y][x]->curr_enemies[i]->name << " used " << name << "." << endl;

            for(int a = 1; a < does.size(); a++) {
                if(does[a][3] == "player") { //enemy attacks player
                    if(stoi(does[a][2]) != 0) {
                        if(stoi(does[a][2]) != 0) { //do damage if != 0
                            player->take_damage(calc_damage_enemy_attack(stoi(does[a][2]), i));
                        }
                        do_retaliate_player(event_map[y][x]->curr_enemies[i]);
                    }
                    if(does[a][0] != "none") {
                        player->add_effect(does[a][0], stoi(does[a][1]));
                    }

                } else if(does[a][3] == "self") { //enemy hits itself - this will only be with an effect.

                    if(does[a][0] != "none") {
                        event_map[y][x]->curr_enemies[i]->add_effect(does[a][0], stoi(does[a][1]));
                    }
                } else if(does[a][3] == "all enemies") {
                    for(int b = 0; b < event_map[y][x]->curr_enemies.size(); b++) { //will only be effects

                        if(does[a][0] != "none") {
                            event_map[y][x]->curr_enemies[b]->add_effect(does[a][0], stoi(does[a][1]));
                        }
                    }
                }

            }

            event_map[y][x]->curr_enemies[i]->increment_card_pos();
        }
    }
}
int Game::calc_damage_enemy_attack(int dmg, int idx) { //enemy attacking player
    if(dmg == 0) { //Not always the case; maybe relic or card played makes all cards played deal x damage
        return 0;
    }
    int x = player_location.first;
    int y = player_location.second;

    bool doubleD = false;
    for(int i = 0; i < event_map[y][x]->curr_enemies[idx]->effects.size(); i++) { //go thru effects
        if(event_map[y][x]->curr_enemies[idx]->effects[i]->name == "weak") {
            dmg-=event_map[y][x]->curr_enemies[idx]->effects[i]->magnitude;
        }
        if(event_map[y][x]->curr_enemies[idx]->effects[i]->name == "rage") {
            dmg+=event_map[y][x]->curr_enemies[idx]->effects[i]->magnitude;
        }
        if(event_map[y][x]->curr_enemies[idx]->effects[i]->name == "2x") {
            doubleD = true;
        }
    }

    if(doubleD) {
        dmg*=2;
    }

    for(int i = 0; i < player->effects.size(); i++) { //go thru effects

        if(player->effects[i]->name == "armor") {
            int dmgMitigated = 0;
            player->effects[i]->magnitude-=dmg;
            if(player->effects[i]->magnitude > 0) {
                dmgMitigated = dmg;
            } else if(player->effects[i]->magnitude == 0) {
                dmgMitigated = dmg;
                player->effects.erase(player->effects.begin()+i);
                i--;
            } else if(player->effects[i]->magnitude < 0) {
                dmgMitigated = player->effects[i]->magnitude+dmg; //add back to get pre-dmg magnitude
                player->effects.erase(player->effects.begin()+i);
                i--;
            }
            dmg-=dmgMitigated;
        } else if(player->effects[i]->name == "vulnerable") { //add damage equal to magnitude of vulnerable
            dmg+=player->effects[i]->magnitude;
        }
    }

    if(dmg < 0) {
        dmg = 0;
    }
    return dmg;
}

void Game::do_retaliate_player(Enemy* en) { //so this shouldn't be here but idk of another solution
    bool found = false;
    int idx = 0;
    for(int i = 0; i < player->effects.size(); i++) {
        if(player->effects[i]->name == "retaliate") {
            found = true;
            idx = i;
        }
    }
    if(found) {
        cout << "RETALIATE FOUND from player" << endl;
        en->take_damage(calc_damage_players_attack(player->effects[idx]->magnitude)); //take damage equal to magnitude of defender retaliate.
    }
}
void Game::do_retaliate_enemy(Enemy* en, int idx_enemy) { //so this shouldn't be here but idk of another solution
    bool found = false;
    int idx = 0;
    for(int i = 0; i < en->effects.size(); i++) {
        if(en->effects[i]->name == "retaliate") {
            found = true;
            idx = i;
        }
    }
    if(found) {
        player->take_damage(calc_damage_enemy_attack(en->effects[idx]->magnitude, idx_enemy)); //take damage equal to magnitude of defender retaliate.
    }
}

void Game::inc_enemy_effects(Event* ev) {
    for(int en = 0; en < ev->curr_enemies.size(); en++) {
        for(int i = 0; i < ev->curr_enemies[en]->effects.size(); i++) {
            if(!ev->curr_enemies[en]->effects[i]->lasting) { //will nullify single turn or decrements (maybe case of relic??)
                if(ev->curr_enemies[en]->effects[i]->decrements) {
                    ev->curr_enemies[en]->dec_magnitude(i); //decrement this magnitude
                    if(ev->curr_enemies[en]->effects[i]->magnitude == 0) {
                        ev->curr_enemies[en]->effects.erase(ev->curr_enemies[en]->effects.begin()+i);
                        i-=1; //safety
                    }
                } else if(ev->curr_enemies[en]->effects[i]->single_turn) {
                    if(ev->curr_enemies[en]->erase_effect_single_turn(i)) {
                       i-=1; //safety
                    }
                }
            }
        }
    }
}
void Game::inc_player_effects() {
    for(int i = 0; i < player->effects.size(); i++) {
        if(!player->effects[i]->lasting) { //will nullify single turn or decrements (maybe case of relic??)
            if(player->effects[i]->decrements) {
                player->dec_magnitude(i); //decrement this magnitude
                if(player->effects[i]->magnitude == 0) {
                    player->effects.erase(player->effects.begin() + i); //remove this effect from the array entirely
                    i-=1;
                }
            } else if(player->effects[i]->single_turn) {
                if(player->erase_effect_single_turn(i)) { //remove this effect from the array entirely
                    i-=1;
                }
            }
        }
    }
}
