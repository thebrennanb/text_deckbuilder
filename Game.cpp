#include "Player.h"
#include "Enemy.h"
#include "Relic.h"
#include "Event.h"

#include "Game.h"

#include<vector>
#include<map>
#include<string>
#include <sstream>
#include <iostream>
using namespace std;

Game::Game()
{}

Game::~Game()
{}

void Game::init_event_map() {

    string a = "enemies";
    event_chances.insert(make_pair(35, a));
    a = "elite enemy";
    event_chances.insert(make_pair(20, a));
    a = "bonfire";
    event_chances.insert(make_pair(15, a));
    a = "shop";
    event_chances.insert(make_pair(15, a));
    a = "chance";
    event_chances.insert(make_pair(10, a));
    a = "relic";
    event_chances.insert(make_pair(5, a));

    int max_events_per_row = 6;

    vector<Event*> event_row;
    Event *e1 = new Event();
    e1->init("start", 0);
    Event *e2 = new Event();
    e2->init("start", 0);
    event_row.push_back(e1);
    event_row.push_back(e2);
    event_map.push_back(event_row); //2 starting events

    int numLastRow = 2;
    int maxim = 0;
    int minim = 0;
    for(int i = 1; i < num_events; i++) {
        vector<Event*> this_row;
        if(numLastRow > 4) {
            maxim = max_events_per_row;
            minim = numLastRow-2;
        } else if(numLastRow <= 3) {
            maxim = numLastRow+2;
            minim = 2;
        } else {
            maxim = numLastRow+2;
            minim = numLastRow-2;
        }

        if(i == num_events - 2 && numLastRow > 2) { //second to last event needs to be <=4
            maxim = numLastRow-2; // go two lower
        }
        if(i == num_events - 1 && numLastRow > 2) { //last event needs to be <= 2
            maxim = 2;
            minim = 1;
        }

        string eventName;
        int randNum = rand()%(maxim-minim+1) + minim;
        for(int j = 0; j < randNum; j++) { //add this many events on this row.
            Event *event2 = new Event();

            eventName = "enemies"; //set default
            int randNum = rand()%(100); //between 0 and 101 supposedly
            int runningSum = 0; //go thru chance array and get the event
            for( map<int, string>::iterator it = event_chances.begin(); it != event_chances.end(); ++it ) { //go thru chances
                runningSum += it->first;
                if(randNum > runningSum) {
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

            event_row.push_back(event2);
            numLastRow = randNum;
        }
        event_map.push_back(event_row);
    }
    //make last row a boss
    vector<Event*> event_rowBoss;
    Event *boss = new Event();
    boss->init("boss", 1);
    event_rowBoss.push_back(boss);
    event_map.push_back(event_rowBoss);
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
                    if(input[0] == "help") {
                        cout << "Make an action:\n\"card <card>\" to play a card, \"draw pile\" to view your draw pile, \"discard pile\" to view your discard pile, \"end turn\" to end your turn, \"me\" to see everything about you, \"enemies\" to see everything about the current enemies, \"target <enemy>\" to set a target." << endl;
                    } else if(input[0] == "quit") {
                        this->playerQuit = true;
                        madeAction = true;
                        endTurn = true;
                    } else if(input[0] == "me") {
                        this->display_player();
                    } else if(input[0] == "enemies") {
                        this->display_enemies();
                    } else {
                        cout << "Could not understand." << endl;
                    }
                } else if(input.size() == 2) { //all the rest require > 1 input
                    if(input[0] == "end" && input[1] == "turn") {
                        cout << "(temp) ended turn" << endl;
                        madeAction = true;
                        endTurn = true;
                    } else if(input[0] == "target") {
                        if(stoi(input[1]) >= event_map[x][y]->curr_enemies.size() || event_map[x][y]->curr_enemies[stoi(input[1])]->is_dead()) {
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
                            cout << "Not enough energy." << endl;
                        } else {

                            //play card and do effects and stuff here
                            player->stamina-=player->hand[stoi(input[1])]->stamina_cost; //subtract stamina cost from playing
                            cout << "Played " << player->hand[stoi(input[1])]->name << endl;
                            event_map[x][y]->curr_enemies[player->target_enemy]->take_damage(player->hand[stoi(input[1])]->damage);
                            //

                            player->discard_pile.push_back(player->hand[stoi(input[1])]); //put card in discard pile
                            player->hand.erase(player->hand.begin()+stoi(input[1])); //remove card from hand.

                            madeAction = true;
                        }
                    } else {
                        cout << "Could not understand." << endl;
                    }
                } else {
                    cout << "Could not understand" << endl;
                }
            }
        }
    }
    //will print if player quit.
    cout << "Completed turn\n" << endl;
}

bool Game::play_enemy_turn() {
    int cnt_dead = 0;
    int x = player_location.first;
    int y = player_location.second;

    for(int i = 0; i < event_map[x][y]->curr_enemies.size(); i++) {
        if(!event_map[x][y]->curr_enemies[i]->is_dead()) {
            player->take_damage(event_map[x][y]->curr_enemies[i]->attacks[0]->damage);
            cout << "You took " << event_map[x][y]->curr_enemies[i]->attacks[0]->damage << " damage from " << event_map[x][y]->curr_enemies[i]->name << "'s attack: " << event_map[x][y]->curr_enemies[i]->attacks[0]->name << "." << endl;
        } else {
            cnt_dead++;
        }
    }
    return (cnt_dead == event_map[x][y]->curr_enemies.size());
}

void Game::display_enemies() {
    int x = player_location.first;
    int y = player_location.second;

    for(int i = 0; i < event_map[x][y]->curr_enemies.size(); i++) {
        cout << i << "  " << event_map[x][y]->curr_enemies[i]->name << ", hp=" <<event_map[x][y]->curr_enemies[i]->hp << endl;
        cout << "Attacks: ";
        for(int j = 0; j < event_map[x][y]->curr_enemies[i]->attacks.size(); j++) {
            if(j != event_map[x][y]->curr_enemies[i]->attacks.size()-1) {
                cout << event_map[x][y]->curr_enemies[i]->attacks[j]->name << " (damage=" << event_map[x][y]->curr_enemies[i]->attacks[j]->damage << "), ";
            } else {
                cout << event_map[x][y]->curr_enemies[i]->attacks[j]->name << " (damage=" << event_map[x][y]->curr_enemies[i]->attacks[j]->damage << ").";
            }
        }
        cout << endl;
    }
}


void Game::display_player() {
    cout << "hp: " << this->player->hp << endl;
    cout << "Max stamina: " << this->player->max_stamina << endl;
    cout << "Number of cards drawn each turn: " << this->player->num_draw << endl;
    if(!this->player->effects.empty()) {
        cout << "effects:\n";
        for(int i=0; i<this->player->effects.size(); i++) {
            cout << this->player->effects[i] << " " << this->player->effectsMagnitudes[this->player->effects[i]] << endl;
        }
    }
}

void Game::init() {
    player->init_deck();
}

void Game::play_map() {
    while(1) {
        display_map();
        string direction;
        cin >> direction;
        cout << "You went " << direction << endl;
        if(event_map[player_location.first][player_location.second]->fight) { //if it is a fight, initiate combat
            combat();
        }
        cout << "(temp) Going one level up" << endl;
        player_location.second++;
    }

}

void Game::display_map() {
    int spacesBetween = 12; //1: 12 and 12 for two events
    for(int i = 0; i < event_map.size(); i++) { //thru col

        for(int j = 0; j < event_map[i].size(); j++) { //thru row
            cout << event_map[i][j]->classif << " ";
            if(i == player_location.first && j == player_location.second) { //player location
                cout << "(<--you) ";
            }
        }
        cout << endl;

    }

}

void Game::combat() {
    event_map[player_location.first][player_location.second]->init_enemies();
    player->target_enemy = -1;
    player->init_deck_before_combat();
    while(1) {

        play_player_turn();

        if(player->isDead()) { //idk maybe recoil dmg
            cout << "You died" << endl;
            end_game = true;
            break;
        }
        if(playerQuit) {
            cout << "You quit" << endl;
            end_game = true;
            break;
        }

        if(play_enemy_turn()) {
            cout << "You won this battle!" << endl;
            break;
        }

        if(player->isDead()) {
            cout << "You died" << endl;
            end_game = true;
            break;
        }

    }
}
