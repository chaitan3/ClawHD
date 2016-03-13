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

animation* memory_manager::get_animation (const string& anim) {
    if (anim.length() == 0)
        return nullptr;
    if ((anim.find("GAME_SOUND") == 0) || (anim.find("LEVEL") == 0)) {
        return nullptr;
    }
    if (this -> animations.count (anim) == 0) {
        this -> animations [anim] = new animation (anim);
    }
    return this -> animations [anim];
}

string memory_manager::get_default_image (const string& image) {
    if (this -> image_list.count (image) == 0)
        return image;
    else
        return this -> image_list [image] -> at (0);
}


void memory_manager::load_image_list (const string& image) {
    if (this -> image_list.count (image) == 0)
        this -> image_list [image] = get_directory_list (image);
}

string memory_manager::get_image_from_list (const string& image, int frame) {
    int num_images = this -> image_list[image] -> size ();
    return this -> image_list [image] -> at (frame % num_images);
}

memory_manager::~memory_manager () {
    for (auto& file_list: this -> image_list) {
        delete file_list.second;
    }
    for (auto& a_curr: this -> animations) {
        delete a_curr.second;
    }
}
