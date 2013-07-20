#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include "util.hpp"
#include "level.hpp"
#include "kdtree.hpp"
#include <map>

#define TILE_SIZE 64
#define TEXTURE_FILE_TYPE string(".png")

class tile_memory_manager {
    map <string, SDL_Texture*> textures;
    
    public:
    SDL_Texture* get_tile_texture (string);
    void put_tile_texture (string, SDL_Texture*);
    bool contains_tile (string);
    ~tile_memory_manager ();
};
    
class display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    tile_memory_manager tmm;
    int height;
    int width;
    
    void import_tile_texture (string);
    int copy_tile_to_display (string, coords*);

    public:
    void render_screen (level*, coords*);
    display ();
    ~display ();
};


#endif
