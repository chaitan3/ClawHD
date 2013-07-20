#ifndef ANIMATE_H
#define ANIMATE_H
#include "util.hpp"

class animation {
    string image;
    int num_frames;
    vector <int> frames;
    vector <int> duration;
    
    public:
    animation (string);
};


#endif
