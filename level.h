#ifndef LEVEL_H
#define LEVEL_H
#include "util.h"

class plane {
    int height;
    int width;
    int32_t **tiles;
    
    public:
    char* import_tile_ids (char *);
    int32_t** get_tile_ids ();
    char* set_width_and_height (char*);
    int get_width ();
    int get_height ();
};

class level {
    coords c_start_loc;
    
    public:
    plane p_action, p_bg, p_fg;
    level (string);
    coords* get_start_location ();
};


#endif

