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

    return 0;
}

//BIG TODO WHENEVER YOU ADD A CARD MAKE IT ADD A COPY BECAUSE RIGHT NOW IF YOU UPGRADE A CARD IT UPGRADES ALL CARDS
//ADD A COPY OF THE CARD INSTEAD OF THE ACTUAL CARD

//UPGRADE SHOP: THE +20 and +10 DAMAGES WILL ADD +20X and +10X DAMAGE, FIX this
//ALSO DOES NOT CHANGE THE DESCRIPTION ACCURATELY
