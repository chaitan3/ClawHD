#ifndef ANIMATE_H
#define ANIMATE_H
#include "util.hpp"

const string ANIMATION_FILE_TYPE = ".ANI";

class animation_state {
    public:
    int frame;
    int last_update_time;
    
    animation_state ();
    void reset ();
};


class animation {
    string image;
    int num_frames;
    vector <int> frames;
    vector <int> duration;
    
    public:
    animation (const string&);
    int get_next_frame (animation_state*);
};



#endif
