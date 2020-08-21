#include <iostream>
#include <string>
#include "Game.h"

#include<vector>
#include<string>
using namespace std;

int main()
{
    Game *game = new Game();
    cout << "Game started. Type \"help\" for help and \"quit\" to quit." << endl;
    game->init();
    while(!game->end_game) {
        game->play_map();
    }
    return 0;
}
