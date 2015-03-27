#ifndef ANIMATE_H
#define ANIMATE_H
#include "util.hpp"

#define ANIMATION_FILE_TYPE string (".ANI")

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
    animation (string);
    int get_next_frame (animation_state*);
};



#endif
