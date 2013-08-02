#include "game.hpp"

void memory_manager::insert_dynamic_tile (dynamic_tile* d_tile) {
    this -> d_tiles.insert (d_tile);
}

void memory_manager::remove_dynamic_tile (dynamic_tile* d_tile) {
    this -> d_tiles.remove (d_tile);
}

kdtree <dynamic_tile*>* memory_manager::get_dynamic_tiles () {
    return &(this -> d_tiles);
}

animation* memory_manager::get_animation (string anim) {
    if (this -> a_loaded.count (anim) == 0) {
        animation* a_new = new animation (anim);
        this -> a_loaded [anim] = a_new;
    }
    return this -> a_loaded [anim];
}

string memory_manager::get_default_image_file (string image) {
    return this -> image_file_lists [image] -> at (0);
}

void memory_manager::put_image_files (string image) {
    if (this -> image_file_lists.count (image) == 0)
        this -> image_file_lists [image] = get_directory_list (image);
}

