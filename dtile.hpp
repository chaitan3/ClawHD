#ifndef DTILE_H
#define DTILE_H
#include "util.hpp"
#include "physics.hpp"
#include "animate.hpp"

class dynamic_tile {
    string name;
    string image;
    string anim;
    string prev_anim;
    coords c_pos;
    bounding_box* box;
    int z;
    animation_state anim_state;
    
    public:
    bool mirrored;
    
    coords* get_coords ();
    bounding_box* get_bounding_box ();
    void set_bounding_box (bounding_box*);
    int get_z ();
    animation_state* get_animation_state ();
    dynamic_tile (string, string, string, coords*, int);
    dynamic_tile (string, coords*, int);
    bool x_compare (dynamic_tile*);
    bool y_compare (dynamic_tile*);
    string get_image ();
    string get_name ();
    string get_anim ();
    string get_prev_anim ();
    void set_anim (string);
    void reset_anim (string);
    dynamic_tile () {};
};

bool z_compare (dynamic_tile*, dynamic_tile*);    

#endif
