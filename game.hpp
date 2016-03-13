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
    
    animation* get_animation (const string&);
    
    string get_default_image (const string&);
    void load_image_list (const string&);
    string get_image_from_list(const string&, int);
    
    ~memory_manager ();
};

#endif

