#ifndef LEVEL_H
#define LEVEL_H
#include "util.hpp"
#include "kdtree.hpp"

#define DATA_PREFIX string ("../Images/")
#define SEPARATOR string ("/")

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
    ~plane () {};
    
};

class dynamic_tile {
    string name;
    string image;
    string animation;
    coords c_pos;
    int z;
    
    public:
    coords* get_coords ();
    int get_z ();
    dynamic_tile (string, string, string, coords*, int);
    bool x_compare (dynamic_tile*);
    bool y_compare (dynamic_tile*);
    bool z_compare (dynamic_tile*);
    string get_image ();
    dynamic_tile () {};
};
    

class level {
    coords c_start_loc;
    kdtree <dynamic_tile*> d_tiles;
    vector <plane*> planes;
    int num_planes;
    plane* action_plane;
    string folder_tiles, folder_palette;
    map <string, string> image_sources;
    map <string, list <string>*> image_file_lists;
    
    char* get_compressed_data (ifstream*, int, int);
    
    public:
    level (string);
    int get_num_planes ();
    coords* get_start_location ();
    string get_image_file (string, int);
    void put_image_files (string, string); 
    plane* get_action_plane ();
    plane* get_plane (int);
    kdtree <dynamic_tile*>* get_dynamic_tiles ();
};


#endif

