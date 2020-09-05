#pragma once

#include<string>
#include<vector>
using namespace std;

class Card {

public:
    Card();
    ~Card();

    //string split constants
    string d1;
    string d2;

    void init(string info);
    void set_conditional(string cond);
    void set_unplayable();
    void set_consume();
    void set_type(string type); //this is for enemy cards

    vector<vector<string>> split();
    bool playable;
    bool consume = false;

    string info;
    string card_type;
};
