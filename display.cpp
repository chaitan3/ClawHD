#include "display.h"

display::display() {
    this -> window = SDL_CreateWindow ("Captain Claw",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 580, SDL_WINDOW_SHOWN);
    this -> renderer = SDL_CreateRenderer (this -> window, -1, SDL_RENDERER_ACCELERATED);
}

display::~display () {
    SDL_DestroyRenderer (this -> renderer);
    SDL_DestroyWindow (this -> window);
}

int display::copy_tile_to_display (string tile) {
    SDL_Rect src, dest;
    src.x = 0;
    src.y = 0;
    src.w = TILE_SIZE;
    src.h = TILE_SIZE;
    dest.w = TILE_SIZE;
    dest.h = TILE_SIZE;
    dest.x = 0;
    dest.y = 0;
    SDL_RenderCopy (this -> renderer, this -> tmm.get_tile_texture (tile), &src, &dest);
    return 0;
}

SDL_Texture* tile_memory_manager::get_tile_texture (string tile) {
    return this -> textures [tile];
}

void tile_memory_manager::put_tile_texture (string tile, SDL_Texture* texture) {
    this -> textures [tile] = texture;
}

tile_memory_manager::~tile_memory_manager () {
    map <string, SDL_Texture*>::iterator it;
    for (it = this -> textures.begin (); it != this -> textures.end (); it++)
        SDL_DestroyTexture (it -> second);
}

void display::import_tile_texture (string file) {
    SDL_Surface* surface = SDL_LoadBMP (file.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this -> renderer, surface);
    this -> tmm.put_tile_texture (file, texture);
    SDL_FreeSurface (surface);
}
