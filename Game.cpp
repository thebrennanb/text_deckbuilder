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
            //PUT THIS BACK IN THIS NEEDS TO GO IN
            /*if(event_map[i][j]->lower_events.size() == 3) { //remove 2 of the 3
                randNum = rand()%(3); //0 to 2
                event_map[i][j]->lower_events.erase(event_map[i][j]->lower_events.begin() + randNum);

                if(event_map[i].size() > event_map[i-1].size()) { //only remove a second one if the diff b/w the rows is <
                    randNum = rand()%(2); //0 to 1
                    event_map[i][j]->lower_events.erase(event_map[i][j]->lower_events.begin() + randNum);
                }
            } else if(event_map[i][j]->lower_events.size() == 2) { //else if event contains 2 lower events
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
            }
            */

            //Go thru all lower connections
            for(int a = 0; a < event_map[i][j]->lower_events.size(); a++) {
                ev.push_back(event_map[i][j]->lower_events[a]);
            }
            //
        }
        for(int c = 0; c < event_map[i].size(); c++) {
            for(int v = 0; v < event_map[i][c]->lower_events.size(); v++) {
                //cout << i << " " << c << " " << v << " " << event_map[i][c]->lower_events[v]->classif << " count: " << count(ev.begin(), ev.end(), event_map[i][c]->lower_events[v]) << endl;
                if(count(ev.begin(), ev.end(), event_map[i][c]->lower_events[v]) > 1 && event_map[i][c]->lower_events.size() != 1) {
                    //^if count of this event are in the vector > 1:
                    int randNum = rand()%(2); //either 0 or 1
                    //cout << "looking at " << i << " " << c << " " << v << " (" << event_map[i][c]->lower_events[v]->classif << ")" << endl;
                    if(randNum == 1) { //50/50 chance
                        //cout << "removing " << i << " " << c << " " << v << " (" << event_map[i][c]->lower_events[v]->classif << ")" << endl;
                        ev.erase(std::remove(ev.begin(), ev.end(), event_map[i][c]->lower_events[v]), ev.end());
                        event_map[i][c]->lower_events.erase(event_map[i][c]->lower_events.begin() + v);
                        v = -1; //reset for safety
                    }
                }


            }
        }
    }
}


void Game::display_enemies() {
    int x = player_location.first;
    int y = player_location.second;

    for(int i = 0; i < event_map[x][y]->curr_enemies.size(); i++) {
        cout << i << "  " << event_map[x][y]->curr_enemies[i]->name << ", hp=" <<event_map[x][y]->curr_enemies[i]->hp << endl;
        if(!event_map[x][y]->curr_enemies[i]->is_dead())  {
           cout << "Attacks: ";
            for(int j = 0; j < event_map[x][y]->curr_enemies[i]->attacks.size(); j++) {
                if(j != event_map[x][y]->curr_enemies[i]->attacks.size()-1) {
                    cout << event_map[x][y]->curr_enemies[i]->attacks[j]->name << " (damage=" << event_map[x][y]->curr_enemies[i]->attacks[j]->damage << "), ";
                } else {
                    cout << event_map[x][y]->curr_enemies[i]->attacks[j]->name << " (damage=" << event_map[x][y]->curr_enemies[i]->attacks[j]->damage << ")." << endl;
                }
            }
            cout << event_map[x][y]->curr_enemies[i]->name << " intends to use " << event_map[x][y]->curr_enemies[i]->attacks[event_map[x][y]->curr_enemies[i]->card_pos]->name << endl;
        }
        cout << endl;
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
    event_conv.insert(make_pair("shop", 's'));
    event_conv.insert(make_pair("chance", 'c'));
    event_conv.insert(make_pair("treasure", 't'));
    event_conv.insert(make_pair("boss", 'B'));
    event_conv.insert(make_pair("start", '^'));
}

void Game::play_map() {
    while(!end_game && !playerQuit && player_location.first != event_map.size()-1) {
        display_map();
        string direction;
        cout << "Where will you go? ";
        string build = "";
        vector<string> ok;
        int x = player_location.first;
        int y = player_location.second;
        int sizeDiff = event_map[y+1].size()-event_map[x].size(); //will be either -1, 0, or 1
        if(event_map[y+1].size() > 1) {
            for(int i = 0; i < event_map[y+1].size(); i++) { //go thru the row above player
                for(int j = 0; j < event_map[y+1][i]->lower_events.size(); j++) {
                    if(event_map[y+1][i]->lower_events[j] == event_map[x][y]) {
                        cout << i << " " << x << endl;
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
                                build += "r/";
                                ok.push_back("r");
                            } else {
                                build += "l/";
                                ok.push_back("l");
                            }
                        }
                        else { //row above < this row
                            if(i == x) { //offset -1
                                build += "l/";
                                ok.push_back("l");
                            } else {
                                build += "r/";
                                ok.push_back("r");
                            }

                        }
                    }

                }
            }
        }
        cout << "(" << build.substr(0,build.size()-1) << ")" << endl;
        while(1) {
            cin >> direction;
            if(!(std::find(ok.begin(), ok.end(), direction) != ok.end()) && direction != "skip") {
                cout << "Choose a direction." << endl;
            } else {
                cout << player_location.second << " ";
                if(direction == "r") {
                    if(sizeDiff != -1) {
                        player_location.second++;
                    }

                } else if(direction == "l") {
                    if(sizeDiff != 1) {
                        player_location.second--;
                    }
                }
                cout << player_location.second << endl;
                break;
            }
        }
        if(direction != "skip") { //add master control "skip" that allows you to skip a room.

        player_location.first++;
        cout << "Event: " << event_map[player_location.first][player_location.second]->classif << "\n" << endl;
        if(event_map[player_location.first][player_location.second]->is_enemy) { //if it is a fight, initiate combat
            enemy_battle();
        }
        else if(event_map[player_location.first][player_location.second]->is_elite_enemy) { //bonfire
            elite_enemy_battle();
        }
        else if(event_map[player_location.first][player_location.second]->is_boss) { //bonfire
            boss_battle();
        }
        else if(event_map[player_location.first][player_location.second]->is_bonfire) { //bonfire
            bonfire();
        }


        } else { //was skip - move up one floor
            player_location.first++;
        }
    }

}

void Game::display_map() {
    cout << "Map:" << endl;
    int spB = 24; //1: 24 -event- 24 for one event    -- (spaces between)
    string spaces = "                        "; //24 spaces
    for(int i = event_map.size()-1; i >=0; i--) { //thru col
        if(event_map[i].size() == 5) {
            cout << "";
        }
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
            if(i == player_location.first && j == player_location.second) { //player location
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
            if(event_map[i].size() == 5) {
                cout << " ";
            }
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
                            build+="|";
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

            } else if(event_map[i].size() < event_map[i-1].size()) { //I AM NOT SURE IF THIS ONE IS RIGHT

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
                                build = build.substr(0, build.size()-1);
                                build+="/";
                            } else {
                                build+="\\";
                                //excessChars++;
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
void Game::bonfire() {
    int restoredHp = player->max_hp/2;
    player->hp += player->max_hp/2; //add half of max hp back
    if(player->hp > player->max_hp) {
        restoredHp -= player->hp-player->max_hp;
        player->hp = player->max_hp;
    }
    cout << "You rest at the bonfire. Restored " << restoredHp << " hp. (hp=" << player->hp << ")\n" << endl;
}

void Game::enemy_battle() {
    event_map[player_location.first][player_location.second]->init_enemies();
    combat();
    if(!playerQuit && !player->isDead()) {
        event_map[player_location.first][player_location.second]->enemy_reward(player);
    }
}

void Game::elite_enemy_battle() {
    event_map[player_location.first][player_location.second]->init_elite_enemies();
    combat();
    if(!playerQuit && !player->isDead()) {
        event_map[player_location.first][player_location.second]->elite_enemy_reward(player);
    }
}

void Game::boss_battle() {
    event_map[player_location.first][player_location.second]->init_boss();
    combat();
    if(!playerQuit && !player->isDead()) {
        event_map[player_location.first][player_location.second]->boss_reward(player);
    }
}

void Game::combat() {
    player->target_enemy = -1;
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

        if(event_map[player_location.first][player_location.second]->all_dead()) {
            cout << "You won this battle!" << endl;
            break;
        }

        inc_enemy_effects();

        play_enemy_turn();

        if(player->isDead()) {
            cout << "You died" << endl;
            end_game = true;
            break;
        }
    }
}

void Game::play_player_turn() {
    string str;
    player->round_start();
    int x = player_location.first;
    int y = player_location.second;

    bool endTurn = false;
    while(!endTurn) {
        display_enemies();
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
                        event_map[x][y]->skip();
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
                        this->display_player();
                    } else if(input[0] == "enemies") {
                        this->display_enemies();
                    } else {
                        cout << "Could not understand." << endl;
                    }
                } else if(input.size() == 2) { //all the rest require > 1 input
                    if(input[0] == "end" && input[1] == "turn") {
                        madeAction = true;
                        endTurn = true;
                        cout << "Completed turn\n" << endl;
                    } else if(input[0] == "target") {
                        if(stoi(input[1]) >= event_map[x][y]->curr_enemies.size()) {
                            cout << "Choose an existing enemy." << endl;
                        } else {
                            player->target_enemy = stoi(input[1]);
                            cout << "Setting target enemy: " << event_map[x][y]->curr_enemies[stoi(input[1])]->name << endl;
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
                        } else if(player->target_enemy == -1) {
                            cout << "Set a target enemy first." << endl;
                        } else if(player->stamina-player->hand[stoi(input[1])]->stamina_cost < 0) { //not enough energy
                            cout << "Not enough stamina." << endl;
                        } else {
                            madeAction = true;

                            //play card and do effects and stuff here
                            play_card(stoi(input[1]));
                            //
                            if(event_map[x][y]->all_dead()) {
                                endTurn = true;
                                break;
                            }
                            player->discard_pile.push_back(player->hand[stoi(input[1])]); //put card in discard pile
                            player->hand.erase(player->hand.begin()+stoi(input[1])); //remove card from hand.

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
    //will print if player quit.
}

void Game::play_card(int idx) {
    int x = player_location.first;
    int y = player_location.second;
    player->stamina-=player->hand[idx]->stamina_cost; //subtract stamina cost from playing
    cout << "Played " << player->hand[idx]->name << endl;
    if(player->hand[idx]->target == "single enemy") {
        event_map[x][y]->curr_enemies[player->target_enemy]->take_damage(player->hand[idx]->damage);
    } else if(player->hand[idx]->target == "self") { //played on self.
        player->add_effect(player->hand[idx]->effect_applied, player->hand[idx]->effect_magnitude);
        if(player->hand[idx]->damage != 0) {
            player->take_damage(player->hand[idx]->damage);
        }
    }

}

void Game::play_enemy_turn() {
    int x = player_location.first;
    int y = player_location.second;

    for(int i = 0; i < event_map[x][y]->curr_enemies.size(); i++) {
        if(!event_map[x][y]->curr_enemies[i]->is_dead()) {
            cout << event_map[x][y]->curr_enemies[i]->name << " used " << event_map[x][y]->curr_enemies[i]->attacks[event_map[x][y]->curr_enemies[i]->card_pos]->name << "." << endl;
            if(!event_map[x][y]->curr_enemies[i]->attacks[event_map[x][y]->curr_enemies[i]->card_pos]->damage == 0) {
                player->take_damage(event_map[x][y]->curr_enemies[i]->attacks[event_map[x][y]->curr_enemies[i]->card_pos]->damage);
            }
            event_map[x][y]->curr_enemies[i]->increment_card_pos();
        }
    }
}

void Game::inc_enemy_effects() {

}
void Game::inc_player_effects() {
    vector<int> removeArr;
    for(int i = 0; i < player->effects.size(); i++) {
        if(!player->effects[i]->lasting) { //will nullify single turn or decrements (maybe case of relic??)
            if(player->effects[i]->decrements) {
                player->effects[i]->magnitude--; //decrement this magnitude
            } else if(player->effects[i]->single_turn) {
                removeArr.push_back(i); //remove this effect from the array entirely
            }
        }
    }
    for(int i = 0; i < removeArr.size(); i++) {
        player->effects.erase(player->effects.begin() + removeArr[i]);
    }
}
