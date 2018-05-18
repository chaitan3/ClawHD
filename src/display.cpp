#include "display.hpp"
#include "dtile.hpp"


void fillPixel(SDL_Surface* buf, int x, int y, const Color& c) {
    int32_t* offset = (int32_t*)((char *)buf->pixels + y*buf->pitch + x*4);
    *offset = c.r + (c.g << 8) + (c.b << 16) + (c.a << 24);
}

SDL_Surface* display::PID_Load(const string& file) {
    SDL_Surface *surface = nullptr;
    
    // Parse info
    ifstream data(file.c_str(), ios::binary);
    if (!data.is_open()) {
        cout << "Failed to open file: " << file << endl;
        return surface;
    }

    int fsize = data.tellg();
    data.seekg(0, ios::end);
    int len = (int)data.tellg() - fsize;
    data.seekg(0, ios::beg);
    
    // Flags
    int flags = f_read_integer(&data, 4);
    bool transparent = (flags & 1) == 1;
    bool rle = (flags & 32) == 32;
    bool ownPalette = (flags & 128) == 128;
    if (rle || transparent || ownPalette) {
        printf("%d %d %d\n", transparent, rle, ownPalette);
        //exit(1);
    }
    //transparent = rle;
    if (rle) transparent = true;
    
    // Extract offset
    //coords fOffset(f_read_integer(data, 16), f_read_integer(data, 20));
    
    // Extract dimensions
    int w = f_read_integer(&data, 8);
    int h = f_read_integer(&data, 12);
    int rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(0, w, h, 32,
                                   rmask, gmask, bmask, amask);
    // Create a new image buffer to fill with pixels
    SDL_LockSurface(surface);
    //SDL_FillRect(surface, NULL, 0);
    //for (int i = 0; i < w; i++) {
    //    for (int j = 0; j < h; j++) {
    //        fillPixel(surface, i, j, Color(0,0,0,0));
    //    }
    //}
    
    // Extract palette
    Color* palette = nullptr;
    int n = 0;
    // Read
    if (!ownPalette) {
        palette = this -> l1 -> palette;
    } else {
        palette = new Color[256];
        for (int i = len - 768; i < len; i+= 3) {
                int r = f_read_byte(&data, i);
                int g = f_read_byte(&data, i + 1);
                int b = f_read_byte(&data, i + 2);
                if (r == 255 && g == 0 && b == 132) palette[n] = Color(0, 0, 0, 0);
                else if (r == 252 && g == 2 && b == 132) palette[n] = Color(0, 0, 0, 0);
                else palette[n] = Color(r, g, b);
                n++;
        }
    }
    
    n = 32;
    int x = 0;
    int y = 0;
    if (rle) {
            while (y < h) {
                    int b = f_read_byte(&data, n);
                    n++;
                    if (b > 128) {
                            int i = b - 128;
                            while (i-- > 0 && y < h) {
                                    fillPixel(surface, x, y, Color(0, 0, 0, transparent ? 0: 255));
                                    x++;
                                    if (x == w) {
                                            x = 0;
                                            y++;
                                    }
                            }
                    } else {
                            int i = b;
                            while (i-- > 0 && y < h) {
                                    b = f_read_byte(&data, n);
                                    n++;
                                    fillPixel(surface, x, y, palette[b]);
                                    x++;
                                    if (x == w) {
                                            x = 0;
                                            y++;
                                    }
                            }
                    }
            }
    } else {
            while (y < h) {
                    int b = f_read_byte(&data, n);
                    n++;
                    int i;
                    if (b > 192) {
                            i = b - 192;
                            b = f_read_byte(&data, n);
                            n++;
                    } else {
                            i = 1;
                    }
                    while (i-- > 0 && y < h) {
                            Color c = palette[b];
                            if (c.r == 0 && c.g == 0 && c.b == 0) {
                                c.a = transparent ? 0 : 255;
                            }
                            fillPixel(surface, x, y, c);
                            x++;
                            if (x == w) {
                                    x = 0;
                                    y++;
                            }
                    }
            }
    }
    SDL_UnlockSurface(surface);
    if (ownPalette) {
        delete palette;
    }

    return surface;
}

display::display() {
    this -> height = 600;
    this -> width = 1000;
    
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    this -> window = SDL_CreateWindow ("Captain Claw",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this -> width, 
        this -> height, SDL_WINDOW_SHOWN);
    if (this -> window == nullptr) {
        cout << "Unable to create Window" << endl;
        exit (1);
    }
    this -> renderer = SDL_CreateRenderer (this -> window, -1, SDL_RENDERER_ACCELERATED);
    if (this -> renderer == nullptr) {
        cout << "Unable to create Renderer" << endl;
        exit (1);
    }
    SDL_RenderClear (renderer);
    this -> plane_cursors = new coords[3];

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        cout << "Unable to initialize Audio" << endl;
        exit (1);
    }
}

display::~display () {
    delete_map_objects (this -> textures);
    Mix_CloseAudio ();
    SDL_DestroyRenderer (this -> renderer);
    SDL_DestroyWindow (this -> window);
    SDL_Quit ();
}

bounding_box* display::copy_tile_to_display (const string& tile, coords* c_pos, coords* c_off, bool mirrored) {
    SDL_Rect src, dest;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    
    if (!(this -> contains_tile (tile)))
        this -> import_tile_texture (tile);
    texture* t_curr = this -> get_tile_texture (tile);

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
    int ret = SDL_RenderCopyEx (this -> renderer, t_curr -> tx, &src, &dest, 0, nullptr, flip);
    if (ret != 0) {
        cout << "Failed to copy texture: " << tile << endl;
        exit (1);
    }
    coords* top_left = new coords (x, y);
    coords* bottom_right = new coords (x + width, y + height);
    bounding_box *box = new bounding_box (top_left, bottom_right);
    return box;
}

void display::import_tile_texture (const string& file) {
    string t_file = DATA_PREFIX + convert_folder_path_to_unix (file) + TEXTURE_FILE_TYPE;
    // LEVEL1 ACTION 074 HACK
    if (!f_exists (t_file)) {
        t_file.erase (remove (t_file.begin (), t_file.end (), '0'), t_file.end ());
    }
        
    //SDL_Surface* surface = IMG_Load (t_file.c_str ());
    SDL_Surface* surface = this->PID_Load(t_file);
    if (surface == nullptr) {
        cout << "Failed to load surface: " << file << endl;
        cout << "Filename: " << t_file << endl;
        exit(1);
    }    
    SDL_Texture* tx = SDL_CreateTextureFromSurface(this -> renderer, surface);
    if (tx == nullptr) {
        cout << "Failed to load texture: " << file << endl;
        exit(1);
    }
    coords* c_off = get_offset_from_pid (file);
    cout << "texture " << file << " " << c_off -> x << " " << c_off -> y << endl;
    this -> textures [file] = new texture (tx, c_off);
    
    SDL_FreeSurface (surface);
}

vector <dynamic_tile*>* display::render_screen (memory_manager* mm, level* l_current, coords* c_pos) {
    coords c_draw_pos;
    coords center = *c_pos;
    //printf("%d %d\n", center.x, center.y);
    coords top_left (center.x - this->width/2, center.y - this->height/2);
    static coords prev_center = center;

    coords c_top_left (top_left.x - RENDERER_PADDING, top_left.y - RENDERER_PADDING);
    coords c_bottom_right (top_left.x + this -> width + RENDERER_PADDING, top_left.y + this -> height + RENDERER_PADDING);
    vector <dynamic_tile*>* visible_tiles = mm -> get_dynamic_tiles () -> range_search (&c_top_left, &c_bottom_right);
    vector <dynamic_tile*>* dynamic_tiles = new vector<dynamic_tile*>(*visible_tiles);
    vector <dynamic_tile*> static_tiles;

    this -> l1 = l_current;
    
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
            this -> plane_cursors[k].y = center.y * y_move_speed / 100;
            this -> plane_cursors[k].x = center.x * x_move_speed / 100;
        }
        this -> plane_cursors[k].y += (center.y - prev_center.y) * y_move_speed/ 100;
        this -> plane_cursors[k].x += (center.x - prev_center.x) * x_move_speed/ 100;
        int y_start = this->plane_cursors[k].y-this->height/2;
        int x_start = this->plane_cursors[k].x-this->width/2;
        i_start = y_start / TILE_SIZE;
        j_start = x_start / TILE_SIZE;
        y_start = center.y + i_start * TILE_SIZE - this->plane_cursors[k].y;
        x_start = center.x + j_start * TILE_SIZE - this->plane_cursors[k].x;

        //cout << k << " " << j_start << " " << prev_j << " " << x_start << " " << endl;
        int i, j;
        for (i = i_start, c_draw_pos.y = y_start + TILE_SIZE/2; c_draw_pos.y < this -> height + y_start + TILE_SIZE; i++, c_draw_pos.y += TILE_SIZE) {
            for (j = j_start, c_draw_pos.x = x_start + TILE_SIZE/2; c_draw_pos.x < this -> width + x_start + TILE_SIZE; j++, c_draw_pos.x += TILE_SIZE) {
                int tileID = tiles [true_mod(i, num_h_tiles)][true_mod(j, num_w_tiles)];
                // TILE IDS 0 and -1 are invalid, find out why!
                if (tileID > 0) {
                    string tile = p_curr -> folder_prefix + convert_to_three_digits (tileID);
                    dynamic_tile* static_tile = new dynamic_tile(tile, &c_draw_pos, p_curr -> get_z ());
                    visible_tiles -> push_back (static_tile);
                    if (k == 1) {
                        static_tile -> set_tile_attributes (l_current -> get_tile_attributes (tileID));
                        dynamic_tiles -> push_back (static_tile);
                    }
                    else {
                        static_tiles.push_back (static_tile);
                    }
                }
            }
        }
    }
    prev_center = center;

    // Render Dynamic Tiles
    sort (visible_tiles -> begin (), visible_tiles -> end (), z_compare);
    
    int num_d_tiles = 0;
    SDL_RenderClear (renderer);

    for (auto it: *visible_tiles) {
        dynamic_tile* d_tile = it;
        //cout << d_tile -> get_name () << " " << d_tile -> get_anim () << " " << d_tile -> get_image () << endl;
        //
        string tile;
        string image = d_tile -> get_image ();
        //FIX THIS
        if (image == "GAME_IMAGES_SOUNDICON") {
            //cout << d_tile -> get_anim () << endl;
            //continue;
        }
         
        // MOVE TO animate.cpp
        bool flag = true;
        while (flag) {
            string anim = d_tile -> get_anim ();
            animation* a_curr = mm -> get_animation (anim);
            if (a_curr != nullptr) {
                int frame = a_curr -> get_next_frame (d_tile -> get_animation_state ());
                cout << d_tile->get_name() << " " << frame << endl;
                if (frame == -1) {
                    d_tile -> reset_anim ();
                    cout << "reset " << d_tile->get_anim() << " " << frame << endl;
                    continue;
                }
                else {
                    tile = mm -> get_image_from_list(image, frame);
                }
                //if (image == "CLAW_IMAGES")
                //cout << "here " << frame << endl;
            }
            else {
                tile = mm -> get_default_image (image);
                //if (image == "CLAW_IMAGES")
                //cout << "here2 " << endl;
            }
            flag = false;
        }

        coords* c_tile_pos = d_tile -> get_coords ();
        bounding_box* box = this -> copy_tile_to_display (tile, c_tile_pos, &top_left, d_tile -> mirrored);
        d_tile -> set_bounding_box (box);
        num_d_tiles ++;
    }
    for (auto& it: static_tiles) {
        delete it;
    }
    
    SDL_RenderPresent(this -> renderer);
    return dynamic_tiles;
}

texture* display::get_tile_texture (const string& tile) {
    return this -> textures [tile];
}

bool display::contains_tile (const string& tile) {
    return this -> textures.count (tile) > 0;
}

texture::texture (SDL_Texture* new_tx, coords* c_offset) {
    this -> tx = new_tx;
    SDL_QueryTexture (this -> tx, nullptr, nullptr, &(this -> width), &(this -> height));
    this -> c_off = c_offset;
}

texture::~texture () {
    SDL_DestroyTexture (this -> tx);
    delete this -> c_off;
}

