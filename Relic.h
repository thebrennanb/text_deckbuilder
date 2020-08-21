#pragma once

#include<string>
#include<vector>
using namespace std;

class Relic {
public:

    Relic();
    ~Relic();

    string name;
    int effect; //corresponds to index in array effects in Game class (so no multiple unneeded instances )

};
