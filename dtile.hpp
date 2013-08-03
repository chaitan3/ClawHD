#ifndef DTILE_H
#define DTILE_H
#include "util.hpp"
#include "animate.hpp"

class dynamic_tile {
    string name;
    string image;
    string anim;
    coords c_pos;
    int z;
    animation_state anim_state;
    
    public:
    string prev_anim;
    bool mirrored;
    
    coords* get_coords ();
    int get_z ();
    animation_state* get_animation_state ();
    dynamic_tile (string, string, string, coords*, int);
    bool x_compare (dynamic_tile*);
    bool y_compare (dynamic_tile*);
    string get_image ();
    string get_anim ();
    void set_anim (string);
    dynamic_tile () {};
};

bool z_compare (dynamic_tile*, dynamic_tile*);    

#endif
