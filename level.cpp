#include "level.hpp"
#include <zlib.h>
#include <cstring>

level::level (string filename) {
    ifstream f_wwd (filename.c_str(), ios::in | ios::binary);
    
    // Get tiles folder
    this -> folder_tiles = f_read_path (&f_wwd, 0x1D0, 0x80);
    
    // Get palette folder
    this -> folder_palette = f_read_path (&f_wwd, 0x250, 0x80);
    
    // Get compression offset
    int offset = f_read_integer (&f_wwd, 0x2E0);
    int inflated_len = f_read_integer (&f_wwd, 0x2E8);
    
    // Get start position
    this -> c_start_loc.x = f_read_integer (&f_wwd, 0x2D0);
    this -> c_start_loc.y = f_read_integer (&f_wwd, 0x2D4);
    
    // Get image sources
    string key, val;
    int cursor = 0x374, num_sources = 0;
    while (1) {
        val = f_read_path (&f_wwd, cursor, 0x80);
        if (val.length () == 0)
            break;
        key = f_read_path (&f_wwd, cursor - num_sources * 0x60 + 0x200, 0x20);
        this -> image_sources [key] = val;
        cursor += 0x80;
        num_sources ++;
    }
    
    // Get  compressed data
    char* inflated_data = this -> get_compressed_data (&f_wwd, offset, inflated_len);
    char *ptr_end = inflated_data + inflated_len;
    
    // Get resolution of the 3 planes
    char *ptr_data = inflated_data + 0x60;
    ptr_data = this -> p_bg.set_width_and_height (ptr_data);
    ptr_data = this -> p_action.set_width_and_height (ptr_data);
    this -> p_fg.set_width_and_height (ptr_data);
    
    // Copy tile IDs for each plane
    ptr_data = inflated_data + 0x1E0;
    ptr_data = this -> p_bg.import_tile_ids (ptr_data);
    ptr_data = this -> p_action.import_tile_ids (ptr_data);
    ptr_data = this -> p_fg.import_tile_ids (ptr_data);
    
    // Set folder names for each of the planes
    this -> p_bg.folder_prefix = DATA_PREFIX + this -> folder_tiles + string ("/BACK/");
    this -> p_action.folder_prefix = DATA_PREFIX + this -> folder_tiles + string ("/ACTION/");
    this -> p_fg.folder_prefix = DATA_PREFIX + this -> folder_tiles + string ("/FRONT/");
    
    // Start dynamic tiles
    ptr_data += 0x1A;
    int name_len, image_len, anim_len, time;
    coords c_pos;
    
    while (ptr_data < ptr_end) {
        if (((int)ptr_data [0] == -86) && ((int)ptr_data [1] == 3))
            break;
        // copy basic data
        memcpy (&name_len, ptr_data, 4);
        memcpy (&image_len, ptr_data + 4, 4);
        memcpy (&anim_len, ptr_data + 8, 4);
        memcpy (&(c_pos.x), ptr_data + 12, 4);
        memcpy (&(c_pos.y), ptr_data + 16, 4);
        memcpy (&time, ptr_data + 20, 4);
        
        // copy info data
        ptr_data += 0x114;
        char name [name_len], image [image_len], anim [anim_len];
        memcpy (name, ptr_data, name_len);
        name [name_len] = '\0';
        memcpy (image, ptr_data + name_len, image_len);
        image [image_len] = '\0';
        memcpy (anim, ptr_data + name_len, anim_len);
        anim [anim_len] = '\0';
        ptr_data += name_len + image_len + anim_len + 8;
        
        if (strcmp ("LEVEL_ARCHESFRONT", image) == 0)
            continue;
        dynamic_tile d_tile(string (name), string (image), string (anim), &c_pos, time);
        
        if (this -> image_file_lists.count (image) == 0) {
            char *c = strchr (image, '_');
            c[0] = '\0';
            string suffix;
            if (strcmp (image, "LEVEL") == 0)
                suffix = string (c + 1);
            else
                suffix = convert_folder_path_to_unix (c + 1);
            string folder_images = DATA_PREFIX + image_sources [image] + 
                SEPARATOR + suffix + SEPARATOR;
            
            this -> image_file_lists [image] = get_directory_list (folder_images);
        }
            
        this -> d_tiles.insert (d_tile);
    }
}

char* level::get_compressed_data (ifstream* file1, int offset, int inflated_len) {
    ifstream* file = new ifstream ("l1"); 
    file -> seekg (0, ios::end);
    int end = file -> tellg ();
    int deflated_len = end - offset;
    file -> seekg (offset, ios::beg);
    char deflated_data [deflated_len];
    file -> read (deflated_data, deflated_len);
    file -> close ();
    
    // Extract data using zlib
    char* inflated_data = new char [inflated_len];
    int ret = uncompress ((Bytef*) inflated_data, (uLong*) &inflated_len,
        (Bytef*) deflated_data, deflated_len);
    if (ret != Z_OK) cout << "ERROR " << ret << endl;
    return inflated_data;
    
}

coords* level::get_start_location () {
    return &(this -> c_start_loc);
}

plane* level::get_action_plane () {
    return &(this -> p_action);
}

kdtree <dynamic_tile>* level::get_dynamic_tiles () {
    return &(this -> d_tiles);
}

string level::get_image_file (string image, int index) {
    return this -> image_file_lists [image] -> front ();
}

char* plane::import_tile_ids (char *ptr) {
    this -> tiles = new int32_t* [this -> height];
    for (int i = 0; i < this -> height; i++)
        this -> tiles [i] = new int32_t [this -> width];
        
    for (int i = 0; i < this -> height; i++) {
        for (int j = 0; j < this -> width; j++) {
            memcpy (&(this -> tiles [i][j]), ptr + (i * this -> width + j) * 4, 4);
        }
    }
    return ptr + this -> height * this -> width * 4;
}

char* plane::set_width_and_height (char *ptr) {
    memcpy (&this -> height, ptr + 4, 4);
    memcpy (&this -> width, ptr, 4);
    return ptr + 0xA0;
}

int plane::get_width () {
    return this -> width;
}
int plane::get_height () {
    return this -> height;
}

int32_t** plane::get_tile_ids () {
    return this -> tiles;
}

bool dynamic_tile::x_compare (dynamic_tile right_operand) {
    coords* c_right = right_operand.get_coords ();
    return this -> c_pos.x < c_right -> x;
}

bool dynamic_tile::y_compare (dynamic_tile right_operand) {
    coords* c_right = right_operand.get_coords ();
    return this -> c_pos.y < c_right -> y;
}

string dynamic_tile::get_image () {
    return this -> image;
}

coords* dynamic_tile::get_coords () {
    return &(this -> c_pos);
}

dynamic_tile::dynamic_tile (string n, string i, string a, coords* c, int t) {
    this -> name = n;
    this -> image = i;
    this -> animation = a;
    this -> c_pos = *c;
    this -> time = t;
}
