#ifndef CO_H
#define CO_H
#include <iostream>
#include "prototypes.h"
#include <vector> 

using namespace std;

class CO: public point {
    public: 
    CO() {};
    vector<obstacle>getContent() {return shared;}

    bool checkShare(CO);

    // Member variables 
    vector<obstacle>shared;

};

#endif