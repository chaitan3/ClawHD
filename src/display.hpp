#ifndef DISPLAY_H
#define DISPLAY_H
#include "util.hpp"
#include "level.hpp"
#include "memory.hpp"
#include "physics.hpp"

const int TILE_SIZE = 64;
const int RENDERER_PADDING = 50;
const string TEXTURE_FILE_TYPE = ".PID";

class texture {
    public:
    SDL_Texture* tx;
    coords* c_off;
    int width, height;
    
    texture (SDL_Texture*, coords*);
    ~texture ();
};

class display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int height;
    int width;
    coords* plane_cursors;
    coords* plane_indices;
    coords cursor;
    map <string, texture*> textures;

    texture* get_tile_texture (const string&);
    void import_tile_texture (const string&);
    bool contains_tile (const string&);
    bounding_box* copy_tile_to_display (const string&, coords*, coords*, bool);

    public:
    vector <dynamic_tile*>* render_screen (memory_manager* , level*, coords*);
    display ();
    ~display ();
};


#endif
