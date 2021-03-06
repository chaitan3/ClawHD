/*
 * test.cpp
 * 
 * Copyright 2013 Chaitanya <chaitan3@virtual>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "util.hpp"
#include "level.hpp"
#include "memory.hpp"
#include "display.hpp"

const string CLAW = "CLAW_IMAGES";

int main (int argc, char **argv)
{   
    display disp;
    memory_manager mm;

    string l_curr_str = "1";
    if (argc > 1) {
        l_curr_str = string(argv[1]);
    }
    mm.l_curr = l_curr_str;
    level l_curr (DATA_PREFIX + "LEVEL" + l_curr_str + "/WORLDS/WORLD.WWD", &mm);
    coords* start_state = l_curr.get_start_location ();
    dynamic_tile* d_claw = new dynamic_tile ("Captain Claw", CLAW, "CLAW_ANIS_STAND", start_state, 8999);
    mm.insert_dynamic_tile (d_claw);
    mm.load_image_list (CLAW);
    
    coords* state = d_claw -> get_coords ();
    SDL_Event event;
    int scroll_speed = 7;
    int jump_speed = 5;
    int latency = 10;
    int count;
    const uint8_t* key_state;
    const uint8_t* prev_key_state = nullptr;
    
    while (1) {
        while (SDL_PollEvent (&event));
        key_state = SDL_GetKeyboardState (&count);
        if (key_state [SDL_SCANCODE_ESCAPE])
            exit (0);
        
        string anim = d_claw -> get_anim ();
        if (anim == d_claw -> get_default_anim ()) {
            if (key_state [SDL_SCANCODE_LCTRL]) {
                d_claw -> set_anim("CLAW_ANIS_SWIPE");
                mm.sound_play_file ("CLAW_SOUNDS_SWORDSWISH");
            } else if (key_state [SDL_SCANCODE_LALT]) {
                d_claw -> set_anim("CLAW_ANIS_PISTOL");
                mm.sound_play_file ("CLAW_SOUNDS_GUNSHOT");
            } else if (key_state [SDL_SCANCODE_RIGHT]) {
                mm.remove_dynamic_tile(d_claw);
                state -> x += scroll_speed;
                mm.insert_dynamic_tile(d_claw);
                d_claw -> mirrored = false;
                d_claw -> set_anim ("CLAW_ANIS_WALK");
            } else if (key_state [SDL_SCANCODE_LEFT]) {
                mm.remove_dynamic_tile(d_claw);
                state -> x += scroll_speed;
                mm.insert_dynamic_tile(d_claw);
                d_claw -> mirrored = true;
                d_claw -> set_anim ("CLAW_ANIS_WALK");
            } else if (key_state [SDL_SCANCODE_SPACE]) {
                mm.remove_dynamic_tile(d_claw);
                animation_state *jump = d_claw -> get_animation_state();
                if (jump -> frame > 7)
                    state -> y += jump_speed;
                else
                    state -> y -= jump_speed;
                if ((key_state [SDL_SCANCODE_RIGHT]) && !(d_claw -> mirrored)) {
                    state -> x += scroll_speed;
                }
                else if (key_state [SDL_SCANCODE_LEFT] && (d_claw -> mirrored)) {
                    state -> x -= scroll_speed;
                }
                mm.insert_dynamic_tile(d_claw);
                d_claw -> set_anim ("CLAW_ANIS_JUMP");

            }
        }

        vector <dynamic_tile*>* interior_tiles = disp.render_screen (&mm, &l_curr, state);
        collision_detection (&mm, d_claw, interior_tiles);
        delete interior_tiles;

        SDL_Delay (latency);
        
        prev_key_state = new uint8_t [count];
        memcpy ((void *)prev_key_state, key_state, count);
    }
    
    delete prev_key_state;
    return 0;
}

