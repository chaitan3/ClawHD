#include "display.hpp"
#include "dtile.hpp"

display::display() {
    this -> height = 600;
    this -> width = 1000;
    
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
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
    this -> plane_cursors = new coords[3];
    this -> plane_indices = new coords[3];

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        cout << "Unable to initialize Audio" << endl;
        exit (1);
    }
}

display::~display () {
    Mix_CloseAudio ();
    SDL_DestroyRenderer (this -> renderer);
    SDL_DestroyWindow (this -> window);
    SDL_Quit ();
}

bounding_box* display::copy_tile_to_display (string tile, coords* c_pos, coords* c_off, bool mirrored) {
    SDL_Rect src, dest;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    
    if (!(this -> tmm.contains_tile (tile)))
        this -> import_tile_texture (tile);
    texture* t_curr = this -> tmm.get_tile_texture (tile);

    int width = t_curr -> width;
    int height = t_curr -> height;
    int x = c_pos -> x - width / 2;
    if (mirrored) {
        x -= t_curr -> c_off -> x;
        flip = SDL_FLIP_HORIZONTAL;
    }
    else {
        x += t_curr -> c_off -> x;
    }
    int y = c_pos -> y - height / 2 + t_curr -> c_off -> y;

    src.x = 0;
    src.y = 0;
    src.w = width;
    src.h = height;
    dest.w = width;
    dest.h = height;
    dest.x = x - c_off -> x;
    dest.y = y - c_off -> y;
    int ret = SDL_RenderCopyEx (this -> renderer, t_curr -> tx, &src, &dest, 0, NULL, flip);
    if (ret != 0) {
        cout << "Failed to copy texture: " << tile << endl;
        exit (1);
    }
    coords* top_left = new coords (x, y);
    coords* bottom_right = new coords (x + width, y + height);
    bounding_box *box = new bounding_box (top_left, bottom_right);
    return box;
}

void display::import_tile_texture (string file) {
    string t_file = DATA_PREFIX + convert_folder_path_to_unix (file) + TEXTURE_FILE_TYPE;
    // LEVEL1 ACTION 074 HACK
    if (!f_exists (t_file)) {
        t_file.erase (remove (t_file.begin (), t_file.end (), '0'), t_file.end ());
    }
        
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
    cout << "texture " << file << " " << c_off -> x << " " << c_off -> y << endl;
    this -> tmm.put_tile_texture (file, new texture (tx, c_off));
    
    SDL_FreeSurface (surface);
}

vector <dynamic_tile*>* display::render_screen (memory_manager* mm, level* l_current, coords* c_pos) {
    kdtree <dynamic_tile*>* dynamic_tiles = mm -> get_dynamic_tiles ();
    
    coords c_draw_pos;
    int top = c_pos -> y - this -> height / 2;
    int left = c_pos -> x - this -> width / 2;
    static int prev_top = top;
    static int prev_left = left;

    coords c_top_left (left - RENDERER_PADDING, top - RENDERER_PADDING);
    coords c_bottom_right (left + this -> width + RENDERER_PADDING, top + this -> height + RENDERER_PADDING);
    vector <dynamic_tile*>* interior_tiles = dynamic_tiles -> range_search (&c_top_left, &c_bottom_right);
    vector <dynamic_tile*>* interior_dynamic_tiles = new vector<dynamic_tile*>(*interior_tiles);
    vector <dynamic_tile*> static_tiles;
    
    // Assume Claw at center of screen
    // Render static tiles
    for (int k = 0; k < l_current -> get_num_planes (); k++) {
        plane* p_curr = l_current -> get_plane (k);
        int32_t **tiles = p_curr -> get_tile_ids ();
        int num_w_tiles, num_h_tiles, x_move_speed, y_move_speed;
        tie (num_w_tiles, num_h_tiles) = p_curr -> get_dimensions ();
        tie (x_move_speed, y_move_speed) = p_curr -> get_movement_speeds ();

        int i_start, j_start;
        if (this -> plane_cursors[k].y < 0) {
            this -> plane_cursors[k].y = top;
            this -> plane_cursors[k].x = left;
            this -> plane_indices[k].y = (this -> plane_cursors[k].y ) * y_move_speed / (TILE_SIZE * 100);
            this -> plane_indices[k].x = (this -> plane_cursors[k].x ) * x_move_speed / (TILE_SIZE * 100);
        }
        this -> plane_cursors[k].y += (top - prev_top) * y_move_speed/ 100;
        this -> plane_cursors[k].x += (left - prev_left) * x_move_speed/ 100;
        i_start = (this -> plane_cursors[k].y ) * y_move_speed / (TILE_SIZE * 100);
        j_start = (this -> plane_cursors[k].x ) * x_move_speed / (TILE_SIZE * 100);
        int x_start = (j_start * 100 * TILE_SIZE) / x_move_speed - plane_cursors[k].x + left;
        int y_start = (i_start * 100 * TILE_SIZE) / y_move_speed - plane_cursors[k].y + top;

        //cout << k << " " << j_start << " " << prev_j << " " << x_start << " " << endl;
        int i, j;
        for (i = i_start, c_draw_pos.y = y_start + TILE_SIZE/2; c_draw_pos.y < this -> height + y_start + TILE_SIZE / 2; i++, c_draw_pos.y += TILE_SIZE) {
            for (j = j_start, c_draw_pos.x = x_start + TILE_SIZE/2; c_draw_pos.x < this -> width + x_start + TILE_SIZE / 2; j++, c_draw_pos.x += TILE_SIZE) {
                int tileID = tiles [i % num_h_tiles][j % num_w_tiles];
                //cout <<  k << " " << x_move_speed  << " " << j << " " << tileID << endl;
                if (tileID >= 0) {
                    string tile = p_curr -> folder_prefix + convert_to_three_digits (tileID);
                    dynamic_tile* static_tile = new dynamic_tile(tile, &c_draw_pos, p_curr -> get_z ());
                    static_tiles.push_back (static_tile);
                    interior_tiles -> push_back (static_tile);
                }
            }
        }
    }
    prev_top = top;
    prev_left = left;

    // Render Dynamic Tiles
    sort (interior_tiles -> begin (), interior_tiles -> end (), z_compare);
    
    vector <dynamic_tile*>::iterator it;
    int num_d_tiles = 0;
    SDL_RenderClear (renderer);

    for (it = interior_tiles -> begin (); it != interior_tiles -> end (); it++) {
        dynamic_tile* d_tile = *it;
        coords* c_tile_pos = d_tile -> get_coords ();
        //cout << d_tile -> get_name () << " " << d_tile -> get_anim () << " " << d_tile -> get_image () << endl;
        //
        //FIX THIS
        if (d_tile -> get_image () == "GAME\\IMAGES_SOUNDICON") {
            //cout << d_tile -> get_anim () << endl;
            //continue;
        }
        
        string tile;
        string anim = d_tile -> get_anim ();
        string image = d_tile -> get_image ();
            
        animation* a_curr = mm -> get_animation (anim);
        //cout << "display " << d_tile -> get_name () << " " << image << " " << anim << endl;
        if (a_curr != NULL) {
            int frame = a_curr -> get_next_frame (d_tile -> get_animation_state ());
            if (frame == -1) {
                d_tile -> reset_anim (d_tile -> get_prev_anim());
                it--; continue;
            }
            else {
                // CLAW IMAGES HACK
                if (image == "CLAW_IMAGES") {
                    tile = image + string ("_FRAME") +  convert_to_three_digits (frame);
                } else {
                    tile = mm -> get_image_from_list(image, frame);
                }
            }
            
        }
        else {
            tile = mm -> get_default_image (image);
        }
        coords top_left (left, top);
        bounding_box* box = this -> copy_tile_to_display (tile, c_tile_pos, &top_left, d_tile -> mirrored);
        d_tile -> set_bounding_box (box);
        num_d_tiles ++;
    }
    for (it = static_tiles.begin(); it != static_tiles.end(); it++)
        delete *it;
    
    SDL_RenderPresent(this -> renderer);
    return interior_dynamic_tiles;
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
    for (it = this -> textures.begin (); it != this -> textures.end (); it++) {
        SDL_DestroyTexture (it -> second -> tx);
        delete it -> second -> c_off;
        delete it -> second;
    }
}

texture::texture (SDL_Texture* new_tx, coords* c_offset) {
    this -> tx = new_tx;
    SDL_QueryTexture (this -> tx, NULL, NULL, &(this -> width), &(this -> height));
    this -> c_off = c_offset;
}


