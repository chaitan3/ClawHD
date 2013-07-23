#ifndef ANIMATE_H
#define ANIMATE_H
#include "util.hpp"

#define ANIMATION_FILE_TYPE string (".ANI")

class animation {
    string image;
    int num_frames;
    vector <int> frames;
    vector <int> duration;
    
    public:
    animation (string);
    int get_num_frames ();
    int get_frame (int);
    int get_duration (int);
};


#endif
