#include "level.hpp"
#include "dtile.hpp"
#include "miniz.h"


level::level (const string& filename, memory_manager* mm) {
    ifstream f_wwd (filename.c_str(), ios::in | ios::binary);
    
    this -> num_planes = f_read_integer (&f_wwd, 0x8);
    this -> planes.reserve (num_planes);
    
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
        replace (val.begin(), val.end(), '\\', '_');
        this -> image_sources [key] = val;
        cursor += 0x80;
        num_sources ++;
    }
    
    // Get  compressed data
    char* inflated_data = this -> get_compressed_data (&f_wwd, offset, inflated_len);
    save_to_file (filename + ".ext", inflated_data, inflated_len);

    
    // Get resolution of the 3 planes
    char* ptr_data = inflated_data;
    //char *tmp = inflated_data;
    int width, height, tile_width, tile_height, x_move_speed, y_move_speed, fill_color;
    int32_t c_z;
    for (int i = 0; i < this -> num_planes; i++) {
        ptr_data += 0x58;
        memcpy (&tile_width, ptr_data, 4);
        ptr_data += 0x4;
        memcpy (&tile_height, ptr_data, 4);
        ptr_data += 0x4;
        memcpy (&width, ptr_data, 4);
        ptr_data += 0x4;
        memcpy (&height, ptr_data, 4);
        ptr_data += 0x0c;
        

        memcpy (&x_move_speed, ptr_data, 4);
        ptr_data += 0x04;
        memcpy (&y_move_speed, ptr_data, 4);
        ptr_data += 0x04;

        memcpy (&fill_color, ptr_data, 4);
        ptr_data += 0x08;

        if (i == 1)
            memcpy (&(this -> num_objects), ptr_data, 4);

        ptr_data += 0x10;
        memcpy (&c_z, ptr_data, 4);
        ptr_data += 0x10;

        //cout << i << " " << num_objects << " " << fill_color << " " << c_z << endl;
        plane* p = new plane (tile_width, tile_height, width, height, x_move_speed, y_move_speed, fill_color, c_z);
        this -> planes.push_back (p);
    }
    // Copy tile IDs for each plane
    for (int i = 0; i < this -> num_planes; i++) {
        ptr_data = this -> planes [i] -> import_tile_ids (ptr_data);
    }
    
    // Set folder names for each of the planes
    action_plane = planes [1];
    this -> planes [0] -> folder_prefix = this -> folder_tiles + string ("_BACK_");
    this -> planes [0] -> x_wrapping = true;
    this -> planes [0] -> y_wrapping = true;
    this -> planes [1] -> folder_prefix = this -> folder_tiles + string ("_ACTION_");
    this -> planes [2] -> folder_prefix = this -> folder_tiles + string ("_FRONT_");
    this -> planes [2] -> x_wrapping = true;
    
    // Start dynamic tiles
    ptr_data += 0x1A;
    int name_len, image_len, anim_len, z;
    coords c_pos;
    
    for (int i = 0; i < num_objects; i++)  {
        // copy basic data
        memcpy (&name_len, ptr_data, 4);
        memcpy (&image_len, ptr_data + 4, 4);
        memcpy (&anim_len, ptr_data + 8, 4);
        memcpy (&(c_pos.x), ptr_data + 12, 4);
        memcpy (&(c_pos.y), ptr_data + 16, 4);
        memcpy (&z, ptr_data + 20, 4);
        
        // copy info data
        ptr_data += 0x114;
        char name [name_len + 1], image [image_len + 1], anim [anim_len + 1];
        memcpy (name, ptr_data, name_len);
        name [name_len] = '\0';
        memcpy (image, ptr_data + name_len, image_len);
        image [image_len] = '\0';
        memcpy (anim, ptr_data + image_len + name_len, anim_len);
        anim [anim_len] = '\0';
        ptr_data += name_len + image_len + anim_len + 8;
        
        char key [200], path [200];
        char *pos = strchr (image, '_');
        int len = pos - image;
        strncpy (key, image, len);
        strcpy (path, image + len + 1);
        key [len] = '\0';
        //cout << image_sources[key] << " " << path << endl;
        string folder_images = image_sources [key] + string ("_") + string (path);
        
        // ARCHESFRONT HACK
        if (strcmp ("LEVEL_ARCHESFRONT", image) == 0)
            continue;
        
        string sname(name);
        string sanim(anim);

        dynamic_tile* d_tile = new dynamic_tile (sname, folder_images, sanim, &c_pos, z);
        mm -> load_image_list (folder_images);
        mm -> insert_dynamic_tile (d_tile);
    }
    
    int num_action_tiles;
    memcpy (&num_action_tiles, ptr_data, 4);
    ptr_data += 24;

    char *start_data = ptr_data;
    int num = 0, temp;
    vector<int> tile_ids;
    vector<action_tile> tile_attributes;
    while (ptr_data < inflated_data + inflated_len) {
        memcpy (&temp, ptr_data, 4);
        if (temp == 2) {
            int id = (ptr_data-start_data)/20 - num;
            action_tile attributes;
            ptr_data += 20;
            memcpy (&attributes.tile_type, ptr_data, 4);
            ptr_data += 4;
            memcpy (&attributes.left, ptr_data, 4);
            ptr_data += 4;
            memcpy (&attributes.top, ptr_data, 4);
            ptr_data += 4;
            memcpy (&attributes.right, ptr_data, 4);
            ptr_data += 4;
            memcpy (&attributes.bottom, ptr_data, 4);
            ptr_data += 4;
            num ++;
            tile_ids.push_back (id);
            tile_attributes.push_back (attributes);
        }
        else {
            ptr_data += 20;
        }
    }
    
    set <int> action_tiles = this -> planes[1] -> get_unique_tiles ();
    for (auto tile: action_tiles) {
        if (tile > 0) {
            int lower = distance (tile_ids.begin (), upper_bound (tile_ids.begin (), tile_ids.end (), tile) - 1);
            if (tile_ids [lower] <= 0) {
                lower = tile_ids.size () - 1;
            }
            this -> tile_attributes [tile] = tile_attributes [lower];
            //cout << tile << " " << tile_ids [lower] << endl;
        }
    }

    delete[] inflated_data;
    string palette_file = DATA_PREFIX + "LEVEL" + mm->l_curr + "/PALETTES/MAIN.PAL";
    ifstream palette_data(palette_file.c_str(), ios::binary);
    if (!palette_data.is_open()) {
        cout << "palette data not found "  << palette_file << endl;
        exit(1);
    }
    int n = 0;
    for (int i = 0; i < 768; i+= 3) {
            int r = f_read_byte(&palette_data, i);
            int g = f_read_byte(&palette_data, i + 1);
            int b = f_read_byte(&palette_data, i + 2);
            //if (r == 255 && g == 0 && b == 132) palette[n] = Color(0, 0, 0, 0);
            //else if (r == 252 && g == 2 && b == 132) palette[n] = Color(0, 0, 0, 0);
            //else if (r == 0 && g == 0 && b == 0) palette[n] = Color(0, 0, 0, 0);
            if (0) palette[n] = Color(0, 0, 0, 0);
            //else if (r == 0 && g == 0 && b == 0) palette[n] = Color(0, 0, 0, 0);
            else palette[n] = Color(r, g, b);
            n++;
    }
    palette_data.close();

    //exit(1);
}

char* level::get_compressed_data (ifstream* file, int offset, int inflated_len) {
    file -> seekg (0, ios::end);
    int end = file -> tellg ();
    int deflated_len = end - offset;
    char deflated_data [deflated_len];
    file -> seekg (offset, ios::beg);
    file -> read (deflated_data, deflated_len);
    file -> close ();
    
    // Extract data using zlib
    char* inflated_data = new char [inflated_len];
    int ret = uncompress ((Bytef*) inflated_data, (uLong*) &inflated_len,
        (Bytef*) deflated_data, deflated_len);
    if (ret != Z_OK) cout << "ERROR " << ret << endl;
    return inflated_data;
    
}

level::~level () {
    for (int i = 0; i < this -> num_planes; i++) {
        delete this -> planes [i];
    }
}

int level::get_num_planes () {
    return this -> num_planes;
}

coords* level::get_start_location () {
    return &(this -> c_start_loc);
}

plane* level::get_action_plane () {
    return this -> action_plane;
}

plane* level::get_plane (int i) {
    return this -> planes [i];
}

action_tile* level::get_tile_attributes (int tile_id) {
    return &(this -> tile_attributes [tile_id]);
}


set<int> plane::get_unique_tiles () {
    set <int> uniq_tiles;
    for (int i = 0; i < this -> height; i++) {
        for (int j = 0; j < this -> width; j++) {
            uniq_tiles.insert (this -> tiles [i][j]);
        }
    }
    return uniq_tiles;
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

int plane::get_z () {
    return this -> c_z;
}

tuple<int, int> plane::get_dimensions () {
    return make_tuple (this -> width, this -> height);
}
tuple<int, int> plane::get_movement_speeds () {
    return make_tuple (this -> x_move_speed, this -> y_move_speed);
}

int32_t** plane::get_tile_ids () {
    return this -> tiles;
}

plane::plane (int t_w, int t_h, int w, int h, int x_ms, int y_ms, int f_c, int32_t c_z) {
    this -> height = h;
    this -> width = w;
    this -> tile_height = t_h;
    this -> tile_width = t_w;
    this -> x_move_speed = x_ms;
    this -> y_move_speed = y_ms;
    this -> x_wrapping = false;
    this -> y_wrapping = false;
    this -> fill_color = f_c;
    this -> c_z = c_z;
}

plane::~plane () {
    
    for (int i = 0; i < this -> height; i++) {
        delete[] this -> tiles [i];
    }
    delete[] this -> tiles;
}
