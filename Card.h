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
    vector<vector<string>> split();

    string info;
};
