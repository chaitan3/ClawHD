#include "dtile.hpp"

bool dynamic_tile::x_compare (dynamic_tile* right_operand) {
    coords* c_right = right_operand -> get_coords ();
    return this -> c_pos.x < c_right -> x;
}

bool dynamic_tile::y_compare (dynamic_tile* right_operand) {
    coords* c_right = right_operand -> get_coords ();
    return this -> c_pos.y < c_right -> y;
}

bool z_compare (dynamic_tile* left, dynamic_tile* right) {
    return left -> get_z () < right -> get_z ();
}

string dynamic_tile::get_name () {
    return this -> name;
}
string dynamic_tile::get_image () {
    return this -> image;
}

string dynamic_tile::get_anim () {
    return this -> anim;
}

string dynamic_tile::get_prev_anim () {
    return this -> prev_anim;
}

void dynamic_tile::set_anim (string anim) {
    this -> anim_state.reset ();
    this -> prev_anim = this->anim;
    this -> anim = anim;
}
    
void dynamic_tile::reset_anim (string anim) {
    this -> anim_state.reset ();
    this -> prev_anim = anim;
    this -> anim = anim;
}

int dynamic_tile::get_z () {
    return this -> z;
}

coords* dynamic_tile::get_coords () {
    return &(this -> c_pos);
}


bounding_box* dynamic_tile::get_bounding_box () {
    return this -> box;
}

void dynamic_tile::set_bounding_box (bounding_box *new_box) {
    if (this -> box != NULL) {
        delete this -> box;
    }
    this -> box = new_box;
}

dynamic_tile::dynamic_tile (string name, string image, string anim, coords* c, int c_z) {
    //if (name == "GlitterlessPowerup") {
    //    anim = "GAME_ANIS_GLITTER1";
    //} else if (name == "TreasurePowerup") {
    //    //anim = "GAME_ANIS_GLITTER2";
    //} else if (name == "FrontAniCandy") {
    //    anim = "GAME_ANIS_GLITTER1";
    //} else if (name == "BehindAniCandy") {
    //    anim = "GAME_ANIS_GLITTER1";
    //}
 
    this -> name = name;
    this -> image = image;
    this -> anim = anim;
    this -> prev_anim = anim;
    this -> c_pos = *c;
    this -> z = c_z;
    this -> mirrored = false;
    this -> box = NULL;
}

dynamic_tile::dynamic_tile (string image, coords *c, int c_z) {
    this -> name = "";
    this -> image = image;
    this -> anim = "";
    this -> c_pos = *c;
    this -> z = c_z;
    this -> mirrored = false;
    this -> box = NULL;
}

animation_state* dynamic_tile::get_animation_state () {
    return & (this -> anim_state);
}

