#ifndef LEVEL_H
#define LEVEL_H
#include "util.h"

class plane {
    int height;
    int width;
    int **tiles;
    
    public:
    char* import_tile_ids (char *);
    int** get_tile_ids ();
    char* set_width_and_height (char*);
    int get_width ();
    int get_height ();
};

class level {
    public:
    plane p_action, p_bg, p_fg;
    level (string);
};


#endif

