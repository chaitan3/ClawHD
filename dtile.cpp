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

dynamic_tile::dynamic_tile (string n, string i, string a, coords* c, int c_z) {
    this -> name = n;
    this -> image = i;
    this -> anim = a;
    this -> prev_anim = a;
    this -> c_pos = *c;
    this -> z = c_z;
    
    this -> mirrored = false;
}

animation_state* dynamic_tile::get_animation_state () {
    return & (this -> anim_state);
}

