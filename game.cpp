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
    if (anim.length() == 0)
        return NULL;
    if ((anim.find("GAME_SOUND") == 0) || (anim.find("LEVEL") == 0)) {
        return NULL;
    }
    if (this -> animations.count (anim) == 0) {
        this -> animations [anim] = new animation (anim);
    }
    return this -> animations [anim];
}

string memory_manager::get_default_image (string image) {
    return this -> image_list [image] -> at (0);
}


void memory_manager::load_image_list (string image) {
    if (this -> image_list.count (image) == 0)
        this -> image_list [image] = get_directory_list (image);
}

memory_manager::~memory_manager () {
    map <string, vector <string>*>::iterator file_list;
    for (file_list = this -> image_list.begin (); file_list != this -> image_list.end (); file_list++) {
        delete file_list -> second;
    }
    
    map <string, animation*>::iterator a_curr;
    for (a_curr = this -> animations.begin (); a_curr != this -> animations.end (); a_curr++) {
        delete a_curr -> second;
    }
}
