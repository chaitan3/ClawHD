#ifndef DISPLAY_H
#define DISPLAY_H
#include "util.hpp"
#include "level.hpp"
#include "game.hpp"
#include "physics.hpp"

#define TILE_SIZE 64
#define RENDERER_PADDING 50
#define TEXTURE_FILE_TYPE string(".png")

class texture {
    public:
    SDL_Texture* tx;
    coords* c_off;
    int width, height;
    
    texture (SDL_Texture*, coords*);
};

class tile_memory_manager {
    map <string, texture*> textures;
    
    public:
    texture* get_tile_texture (string);
    void put_tile_texture (string, texture*);
    bool contains_tile (string);
    ~tile_memory_manager ();
};
    
class display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    tile_memory_manager tmm;
    int height;
    int width;
    coords* plane_cursors;
    coords* plane_indices;
    coords cursor;
    
    void import_tile_texture (string);
    bounding_box* copy_tile_to_display (string, coords*, coords*, bool);

    public:
    vector <dynamic_tile*>* render_screen (memory_manager* , level*, coords*);
    display ();
    ~display ();
};


#endif
