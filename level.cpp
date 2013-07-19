#include "level.h"
#include <zlib.h>
#include <cstring>

level::level (string filename) {
    int inflated_len = 1000000;
    char inflated_data [inflated_len];
    //~ int offset = 0x5F4;
    int offset = 0;
    ifstream f_wwd (filename.c_str());
    
    // Get length of the compressed data
    f_wwd.seekg (0, ios::end);
    int end = f_wwd.tellg ();
    int deflated_len = end - offset;
    f_wwd.seekg (offset, ios::beg);
    
    // Extract data using zlib
    char deflated_data [deflated_len];
    f_wwd.read (deflated_data, deflated_len);
    int ret = uncompress ((Bytef*) inflated_data, (uLong*) &inflated_len,
        (Bytef*) deflated_data, deflated_len);
    if (ret != Z_OK) cout << "ERROR " << ret << endl;
    
    // Get resolution of the 3 planes
    char *ptr_data = inflated_data + 0x60;
    ptr_data = p_bg.set_width_and_height (ptr_data);
    ptr_data = p_action.set_width_and_height (ptr_data);
    p_fg.set_width_and_height (ptr_data);
    
    // Copy tile IDs for each plane
    ptr_data = inflated_data + 0x1E0;
    ptr_data = p_bg.import_tile_ids (ptr_data);
    ptr_data = p_action.import_tile_ids (ptr_data);
    p_fg.import_tile_ids (ptr_data);
    
    f_wwd.close ();
}

char* plane::import_tile_ids (char *ptr) {
    int area = this -> height * this -> width;
    tiles = (int **) new int [area];
    memcpy (this -> tiles, ptr, area);
    return ptr + this -> height * this -> width * 4;
}

char* plane::set_width_and_height (char *ptr) {
    memcpy (&this->height, ptr + 4, 4);
    memcpy (&this->width, ptr, 4);
    return ptr + 0xA0;
}
