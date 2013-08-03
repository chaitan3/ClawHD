#ifndef LEVEL_H
#define LEVEL_H
#include "util.hpp"
#include "game.hpp"

class plane {
    int height;
    int width;
    int32_t **tiles;
    int tile_width, tile_height;
    int x_move_speed, y_move_speed;
    
    public:
    string folder_prefix;
    bool x_wrapping, y_wrapping;
    
    char* import_tile_ids (char *);
    int32_t** get_tile_ids ();
    int get_width ();
    int get_height ();
    plane (int, int, int, int, int, int);
    ~plane ();
    
};

class level {
    coords c_start_loc;
    vector <plane*> planes;
    int num_planes, num_objects;
    plane* action_plane;
    string folder_tiles, folder_palette;
    map <string, string> image_sources;
    
    char* get_compressed_data (ifstream*, int, int);
    
    public:
    level (string, memory_manager*);
    ~level ();
    int get_num_planes ();
    coords* get_start_location ();
    plane* get_action_plane ();
    plane* get_plane (int);
};


#endif

