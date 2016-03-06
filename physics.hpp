#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "util.hpp"
#include "sound.hpp"

class bounding_box {
    public:
    coords* top_left;
    coords* bottom_right;

    bool intersecting (bounding_box*);
    bounding_box (coords*, coords*);
    ~bounding_box ();
};

class dynamic_tile;
class memory_manager;

void collision_detection (memory_manager*, sound_manager*, dynamic_tile*, vector <dynamic_tile*>*);

#endif
