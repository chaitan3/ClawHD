#ifndef DTILE_H
#define DTILE_H
#include "util.hpp"
#include "physics.hpp"
#include "animate.hpp"

#define CLEAR 0
#define SOLID 1
#define GROUND 2
#define CLIMB 3
#define DEATH 4

typedef struct {
    int tile_type = -1;
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
} action_tile;

enum posture {
    STANDING,
    WALKING,
    FALLING,
    JUMPING,
    CROUCHING
};

class dynamic_tile {
    string name;
    string image;
    string anim;

    coords c_pos;
    int z;
    action_tile* attributes;
    bounding_box* box;

    animation_state anim_state;
    posture pos;
    static map<posture, string> default_anim;
    
    public:
    bool mirrored;
    

    action_tile* get_tile_attributes ();
    void set_tile_attributes (action_tile*);
    coords* get_coords ();
    bounding_box* get_bounding_box ();
    void set_bounding_box (bounding_box*);
    int get_z ();
    animation_state* get_animation_state ();
    dynamic_tile (const string&, const string&, const string&, coords*, int);
    dynamic_tile (const string&, coords*, int);
    bool x_compare (dynamic_tile*);
    bool y_compare (dynamic_tile*);
    string get_image ();
    string get_name ();
    string get_anim ();
    string get_default_anim ();
    void set_anim (const string&);
    void reset_anim ();
    dynamic_tile () {};
};

bool z_compare (dynamic_tile*, dynamic_tile*);    


#endif
