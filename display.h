#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include "util.h"
#include <map>

#define TILE_SIZE 64


class tile_memory_manager {
    map <string, SDL_Texture*> textures;
    
    public:
    SDL_Texture* get_tile_texture (string);
    void put_tile_texture (string, SDL_Texture*);
    ~tile_memory_manager ();
};
    
class display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    tile_memory_manager tmm;
    public:
    void import_tile_texture (string);
    int copy_tile_to_display (string tile);
    display ();
    ~display ();
};


#endif
