#ifndef GAME_H
#define GAME_H

#include "util.hpp"
#include "kdtree.hpp"
#include "animate.hpp"
#include "dtile.hpp"

class memory_manager {
    kdtree <dynamic_tile*> d_tiles;
    map <string, vector <string>*> image_list;
    map <string, animation*> animations;
    
    public:
    void insert_dynamic_tile (dynamic_tile*);
    void remove_dynamic_tile (dynamic_tile*);
    kdtree <dynamic_tile*>* get_dynamic_tiles ();
    
    animation* get_animation (string);
    
    string get_default_image (string);
    void load_image_list (string);
    
    ~memory_manager ();
};

#endif

