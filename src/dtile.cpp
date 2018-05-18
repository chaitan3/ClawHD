#include "dtile.hpp"
#include "memory.hpp"

map<posture, string> dynamic_tile::default_anim = {
    {WALKING, "CLAW_ANIS_WALK"},
    {STANDING, "CLAW_ANIS_STAND"},
};

void memory_manager::insert_dynamic_tile (dynamic_tile* d_tile) {
    this -> d_tiles.insert (d_tile);
}

void memory_manager::remove_dynamic_tile (dynamic_tile* d_tile) {
    this -> d_tiles.remove (d_tile);
}

kdtree <dynamic_tile*>* memory_manager::get_dynamic_tiles () {
    return &(this -> d_tiles);
}


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


void dynamic_tile::set_anim (const string& anim) {
    this -> anim_state.reset ();
    this -> anim = anim;
}

string dynamic_tile::get_default_anim () {
    return this->default_anim.at(this->pos);
}
    
void dynamic_tile::reset_anim () {
    this -> set_anim(this->get_default_anim());
}

action_tile* dynamic_tile::get_tile_attributes () {
    return this -> attributes;
}

void dynamic_tile::set_tile_attributes (action_tile* attributes) {
    this -> attributes = attributes;
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
    if (this -> box != nullptr) {
        delete this -> box;
    }
    this -> box = new_box;
}

dynamic_tile::dynamic_tile (const string& name, const string& image, const string& anim, coords* c, int c_z) {
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
    this -> c_pos = *c;
    this -> z = c_z;
    this -> mirrored = false;
    this -> box = nullptr;
    this -> attributes = nullptr;
    this -> pos = STANDING;
}

dynamic_tile::dynamic_tile (const string& image, coords *c, int c_z) {
    this -> name = "";
    this -> image = image;
    this -> anim = "";
    this -> c_pos = *c;
    this -> z = c_z;
    this -> mirrored = false;
    this -> box = nullptr;
    this -> attributes = nullptr;
    this -> pos = STANDING;
}

animation_state* dynamic_tile::get_animation_state () {
    return & (this -> anim_state);
}

