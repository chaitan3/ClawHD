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

#include "game.hpp"

void collision_detection (memory_manager* mm, sound_manager *snd, dynamic_tile* claw, vector <dynamic_tile*>* d_tiles) {
    bounding_box* A = claw -> get_bounding_box (); 
    for (auto d_tile: *d_tiles) {
        //cout << d_tile -> get_image () << endl;
        bounding_box* B = d_tile -> get_bounding_box ();
        if (A == B) {
            continue; 
        }
        if (A -> intersecting (B)) {
            string image = d_tile -> get_image ();
            if (image == "GAME\\IMAGES_SOUNDICON") {
                string anim = d_tile -> get_anim ();    
                snd -> play_file (anim);
                mm -> remove_dynamic_tile (d_tile);
            }
        }
    }
}


