#include "physics.hpp"

bool bounding_box::intersecting (bounding_box* B) {
   bounding_box* A = this; 
   if ((A -> bottom_right -> y < B -> top_left -> y) ||
       (B -> bottom_right -> y < A -> top_left -> y) ||
       (A -> bottom_right -> x < B -> top_left -> x) ||
       (B -> bottom_right -> x < A -> top_left -> x))
        return false;
    return true;
}


bounding_box::bounding_box (coords* top_left, coords* bottom_right) {
    this -> top_left = top_left;
    this -> bottom_right = bottom_right;
}

bounding_box::~bounding_box () {
    delete this-> top_left;
    delete this-> bottom_right;
}

#include "memory.hpp"

void collision_detection (memory_manager* mm, dynamic_tile* claw, vector <dynamic_tile*>* d_tiles) {
    bounding_box* A = claw -> get_bounding_box (); 
    for (auto d_tile: *d_tiles) {
        bounding_box* B = d_tile -> get_bounding_box ();
        action_tile* attributes = d_tile -> get_tile_attributes ();
        string image = d_tile -> get_image ();

        if (A == B) {
            continue; 
        }
        if (attributes != nullptr) {
            B -> top_left -> x += attributes -> left;
            B -> top_left -> y += attributes -> top;
            B -> bottom_right -> x += attributes -> right;
            B -> bottom_right -> y += attributes -> bottom;
            if (A -> intersecting (B)) {
                cout << image << " " << attributes -> tile_type << endl;
            }
        }
        else if (A -> intersecting (B)) {
            cout << image << endl;
            if (image == "GAME_IMAGES_SOUNDICON") {
                string anim = d_tile -> get_anim ();    
                mm -> sound_play_file (anim);
                mm -> remove_dynamic_tile (d_tile);
            }       
        }
    }
}


