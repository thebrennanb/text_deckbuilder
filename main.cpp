#include <iostream>
#include <string>
#include "Game.h"

#include<vector>
#include<string>
#include<time.h>
using namespace std;

int main()
{
    srand(time(0));

    Game *game = new Game();
    cout << "Game started. Type \"help\" for help and \"quit\" to quit." << endl;
    game->init();
    game->player_choose_starting();
    game->play_map();

    cout << "YOU MADE IT OUT OF THE DUNGEON! CONGRATS! YOU WIN!" << endl;

    return 0;
}
