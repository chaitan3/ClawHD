#ifndef LEVEL_H
#define LEVEL_H
#include "util.hpp"
#include "animate.hpp"
#include "kdtree.hpp"

#define DATA_PREFIX string ("../Images/")
#define DATA2_PREFIX string ("../Extracted/")
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
    string anim;
    coords c_pos;
    int z;
    
    public:
    int frame;
    int last_update_time;
    bool mirrored;
    
    coords* get_coords ();
    int get_z ();
    dynamic_tile (string, string, string, coords*, int);
    bool x_compare (dynamic_tile*);
    bool y_compare (dynamic_tile*);
    string get_image ();
    string get_anim ();
    void set_anim (string);
    dynamic_tile () {};
};

bool z_compare (dynamic_tile*, dynamic_tile*);    

class level {
    coords c_start_loc;
    vector <plane*> planes;
    int num_planes, num_objects;
    plane* action_plane;
    string folder_tiles, folder_palette;
    map <string, string> image_sources;
    
    // More game related than level
    kdtree <dynamic_tile*> d_tiles;
    map <string, vector <string>*> image_file_lists;
    map <string, animation*> a_loaded;
    
    char* get_compressed_data (ifstream*, int, int);
    
    public:
    level (string);
    int get_num_planes ();
    coords* get_start_location ();
    string get_default_image_file (string);
    void put_image_files (string, string); 
    plane* get_action_plane ();
    plane* get_plane (int);
    kdtree <dynamic_tile*>* get_dynamic_tiles ();
    animation* get_animation (string);
};


#endif

