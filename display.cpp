#include "display.h"
#include <SDL2/SDL_image.h>

display::display() {
    this -> height = 600;
    this -> width = 1000;
    
    SDL_Init(SDL_INIT_VIDEO);
    this -> window = SDL_CreateWindow ("Captain Claw",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this -> width, this -> height, SDL_WINDOW_SHOWN);
    if (this -> window == NULL) {
        cout << "Unable to create Window" << endl;
        exit (1);
    }
    this -> renderer = SDL_CreateRenderer (this -> window, -1, SDL_RENDERER_ACCELERATED);
    if (this -> renderer == NULL) {
        cout << "Unable to create Renderer" << endl;
        exit (1);
    }
    SDL_RenderClear (renderer);
}

display::~display () {
    SDL_DestroyRenderer (this -> renderer);
    SDL_DestroyWindow (this -> window);
    SDL_Quit ();
}

int display::copy_tile_to_display (string tile, coords* pos) {
    SDL_Rect src, dest;
    src.x = 0;
    src.y = 0;
    src.w = TILE_SIZE;
    src.h = TILE_SIZE;
    dest.w = TILE_SIZE;
    dest.h = TILE_SIZE;
    dest.x = pos -> x;
    dest.y = pos -> y;
    if (!(this -> tmm.contains_tile (tile)))
        this -> import_tile_texture (tile);
    SDL_Texture* texture = this -> tmm.get_tile_texture (tile);
    SDL_RenderCopy (this -> renderer, texture, &src, &dest);
    return 0;
}

void display::import_tile_texture (string file) {
    SDL_Surface* surface = IMG_Load (file.c_str());
    if (surface == NULL) {
        cout << "Failed to load surface: " << file << endl;
        exit(1);
    }    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this -> renderer, surface);
    this -> tmm.put_tile_texture (file, texture);
    SDL_FreeSurface (surface);
}

void display::render_screen (string prefix, int32_t** tiles, coords* pos) {
    coords pos_draw;
    int i, j, i_start, j_start;
    for (i = 0, pos_draw.y = 0; pos_draw.y < this -> height; i++, pos_draw.y += TILE_SIZE) {
        for (j = 0, pos_draw.x = 0; pos_draw.x < this -> width; j++, pos_draw.x += TILE_SIZE) {
            int tileID = tiles [i][j];
            if (tileID >= 0) {
                string tile = prefix + convert_to_three_digits (tileID) + TEXTURE_FILE_TYPE;
                this -> copy_tile_to_display (tile, &pos_draw);
            }
        }
    }
    SDL_RenderPresent(this -> renderer);
    SDL_Delay(1000);
}

SDL_Texture* tile_memory_manager::get_tile_texture (string tile) {
    return this -> textures [tile];
}

void tile_memory_manager::put_tile_texture (string tile, SDL_Texture* texture) {
    this -> textures [tile] = texture;
}

bool tile_memory_manager::contains_tile (string tile) {
    return this -> textures.count (tile) > 0;
}

tile_memory_manager::~tile_memory_manager () {
    map <string, SDL_Texture*>::iterator it;
    for (it = this -> textures.begin (); it != this -> textures.end (); it++)
        SDL_DestroyTexture (it -> second);
}


