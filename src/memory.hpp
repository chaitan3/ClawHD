#ifndef MEMORY_H
#define MEMORY_H

#include "util.hpp"
#include "kdtree.hpp"
#include "animate.hpp"
#include "dtile.hpp"

class memory_manager {
    kdtree <dynamic_tile*> d_tiles;
    map <string, vector <string>*> animation_image_list;
    map <string, animation*> animations;
    map <string, Mix_Chunk*> waves;    
    
    public:
    string l_curr;

    void insert_dynamic_tile (dynamic_tile*);
    void remove_dynamic_tile (dynamic_tile*);
    kdtree <dynamic_tile*>* get_dynamic_tiles ();
    
    animation* get_animation (const string&);
    string get_default_image (const string&);
    void load_image_list (const string&);
    string get_image_from_list(const string&, int);

    
    int sound_play_file (const string&);
    void delete_sound();
    
    ~memory_manager () {
        delete_map_objects (this -> animation_image_list);
        delete_map_objects (this -> animations);
        this -> delete_sound ();
    }
};

#endif

