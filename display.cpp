#include "display.hpp"
#include <SDL2/SDL_image.h>
#include <algorithm>

display::display() {
    this -> height = 600;
    this -> width = 1000;
    
    SDL_Init(SDL_INIT_VIDEO);
    this -> window = SDL_CreateWindow ("Captain Claw",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this -> width, 
        this -> height, SDL_WINDOW_SHOWN);
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

int display::copy_tile_to_display (string tile, coords* c_pos, bool mirrored) {
    SDL_Rect src, dest;
    int width, height;
    
    if (!(this -> tmm.contains_tile (tile)))
        this -> import_tile_texture (tile);
    texture* t_curr = this -> tmm.get_tile_texture (tile);
    SDL_QueryTexture (t_curr -> tx, NULL, NULL, &width, &height);
    
    src.x = 0;
    src.y = 0;
    src.w = width;
    src.h = height;
    dest.w = width;
    dest.h = height;
    dest.x = c_pos -> x + t_curr -> c_off -> x;
    dest.y = c_pos -> y + t_curr -> c_off -> y;
    
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (mirrored)
        flip = SDL_FLIP_HORIZONTAL;
    int ret = SDL_RenderCopyEx (this -> renderer, t_curr -> tx, &src, &dest, 0, NULL, flip);
    if (ret != 0) {
        cout << "Failed to copy texture: " << tile << endl;
        exit (1);
    }
    return 0;
}

void display::import_tile_texture (string file) {
    string t_file = DATA_PREFIX + convert_folder_path_to_unix (file) + TEXTURE_FILE_TYPE;
    SDL_Surface* surface = IMG_Load (t_file.c_str ());
    if (surface == NULL) {
        cout << "Failed to load surface: " << file << endl;
        exit(1);
    }    
    SDL_Texture* tx = SDL_CreateTextureFromSurface(this -> renderer, surface);
    if (tx == NULL) {
        cout << "Failed to load texture: " << file << endl;
        exit(1);
    }
    coords* c_off = get_offset_from_pid (file);
    this -> tmm.put_tile_texture (file, new texture (tx, c_off));
    
    SDL_FreeSurface (surface);
}

void display::render_screen (level* l_current, coords* c_pos) {
    kdtree <dynamic_tile*>* dynamic_tiles = l_current -> get_dynamic_tiles ();
    
    coords c_draw_pos;
    int i, j, i_start, j_start;
    int top = c_pos -> y - this -> height / 2;
    int left = c_pos -> x - this -> width / 2;
    
    SDL_RenderClear (renderer);
    
    // Assume Claw at center of screen
    // Render static tiles
    for (int k = 0; k < l_current -> get_num_planes (); k++) {
        plane* p_curr = l_current -> get_plane (k);
        int32_t **tiles = p_curr -> get_tile_ids ();
        int num_w_tiles =  p_curr -> get_width ();
        int num_h_tiles =  p_curr -> get_height ();
        i_start = top / TILE_SIZE;
        j_start = left / TILE_SIZE;
        for (i = i_start, c_draw_pos.y = i_start * TILE_SIZE - top; c_draw_pos.y < this -> height; i++, c_draw_pos.y += TILE_SIZE) {
            for (j = j_start, c_draw_pos.x = j_start * TILE_SIZE - left; c_draw_pos.x < this -> width; j++, c_draw_pos.x += TILE_SIZE) {
                
                int tileID = tiles [i % num_h_tiles][j % num_w_tiles];
                if (tileID >= 0) {
                    string tile = p_curr -> folder_prefix +
                    convert_to_three_digits (tileID);
                    //~ if (!f_exists (tile))
                        //~ tile = p_curr -> folder_prefix + 
                        //~ convert_int_to_string (tileID);
                    this -> copy_tile_to_display (tile, &c_draw_pos, false);
                }
            }
        }
    }
    // Render Dynamic Tiles
    coords c_top_left (left - RENDERER_PADDING, top - RENDERER_PADDING);
    coords c_bottom_right (left + this -> width + RENDERER_PADDING, top + this -> height + RENDERER_PADDING);
    vector <dynamic_tile*>* interior_elements = dynamic_tiles -> range_search (&c_top_left, &c_bottom_right);
    sort (interior_elements -> begin (), interior_elements -> end (), z_compare);
    
    vector <dynamic_tile*>::iterator it;
    for (it = interior_elements -> begin (); it != interior_elements -> end (); it++) {
        dynamic_tile* d_tile = *it;
        coords* c_tile_pos = d_tile -> get_coords ();
        
        string tile;
        string anim = d_tile -> get_anim ();
        string image = d_tile -> get_image ();
        if ((anim.length () > 0) && (anim [0] == 'C')) {
            int frame = 0;
            animation* a_curr = l_current -> get_animation (anim);
            
            int curr_time = SDL_GetTicks ();
            if (curr_time > d_tile -> last_update_time + a_curr -> get_duration (d_tile -> frame)) {
                (d_tile -> frame) ++;
                if (d_tile -> frame == a_curr -> get_num_frames ())
                    d_tile -> frame = 0;
                d_tile -> last_update_time = curr_time;
            }
            frame = a_curr -> get_frame (d_tile -> frame);
            tile = image + string ("_FRAME") + convert_to_three_digits (frame);
        }
        else {
            tile = l_current -> get_default_image_file (image);
        }
        c_draw_pos.x = c_tile_pos -> x - left;
        c_draw_pos.y = c_tile_pos -> y - top;
        this -> copy_tile_to_display (tile, &c_draw_pos, d_tile -> mirrored);
    }
    SDL_RenderPresent(this -> renderer);
}

texture* tile_memory_manager::get_tile_texture (string tile) {
    return this -> textures [tile];
}

void tile_memory_manager::put_tile_texture (string tile, texture* t_new) {
    this -> textures [tile] = t_new;
}

bool tile_memory_manager::contains_tile (string tile) {
    return this -> textures.count (tile) > 0;
}

tile_memory_manager::~tile_memory_manager () {
    map <string, texture*>::iterator it;
    for (it = this -> textures.begin (); it != this -> textures.end (); it++)
        SDL_DestroyTexture (it -> second -> tx);
}

texture::texture (SDL_Texture* new_tx, coords* c_offset) {
    this -> tx = new_tx;
    this -> c_off = c_offset;
}


