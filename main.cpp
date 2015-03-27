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
#include "game.hpp"
#include "display.hpp"

#define CLAW "CLAW_IMAGES"

int main (int argc, char **argv)
{   
    display disp;
    memory_manager mm;
    level l1 ("../Extracted/LEVEL14/WORLDS/WORLD.WWD", &mm);
    coords* start_state = l1.get_start_location ();
    dynamic_tile* d_claw = new dynamic_tile ("Captain Claw", CLAW, "CLAW_ANIS_STAND", start_state, 3000);
    mm.insert_dynamic_tile (d_claw);
    mm.put_image_files (CLAW);
    
    coords* state = d_claw -> get_coords ();
    SDL_Event event;
    int scroll_speed = 9;
    int latency = 10;
    int count;
    const uint8_t* key_state;
    const uint8_t* prev_key_state = NULL;
    
    while (1) {
        while (SDL_PollEvent (&event));
        key_state = SDL_GetKeyboardState (&count);
        
        string anim = d_claw -> get_anim ();
        
        if (key_state [SDL_SCANCODE_ESCAPE])
            exit (0);
        else if (key_state [SDL_SCANCODE_LCTRL]) {
            if ((!prev_key_state [SDL_SCANCODE_LCTRL]) && (anim [0] != '!')) {
                d_claw -> prev_anim = anim;
                d_claw -> set_anim ("!CLAW_ANIS_SWIPE");
            }
        }
        else if (key_state [SDL_SCANCODE_LALT]) {
            if ((!prev_key_state [SDL_SCANCODE_LALT]) && (anim [0] != '!')) {
                d_claw -> prev_anim = anim;
                d_claw -> set_anim ("!CLAW_ANIS_PISTOL");
            }
        }
        
        if (anim == "CLAW_ANIS_STAND") {
            if (key_state [SDL_SCANCODE_RIGHT]) {
                d_claw -> mirrored = false;
                d_claw -> set_anim ("CLAW_ANIS_WALK");
            }
            else if (key_state [SDL_SCANCODE_LEFT]) {
                d_claw -> mirrored = true;
                d_claw -> set_anim ("CLAW_ANIS_WALK");
            }
            
        }
        else if (anim == "CLAW_ANIS_WALK") {
            if ((key_state [SDL_SCANCODE_RIGHT]) && !(d_claw -> mirrored)) {
                mm.remove_dynamic_tile (d_claw);
                state -> x += scroll_speed;
                mm.insert_dynamic_tile (d_claw);
            }
            else if (key_state [SDL_SCANCODE_LEFT] && (d_claw -> mirrored)) {
                mm.remove_dynamic_tile (d_claw);
                state -> x -= scroll_speed;
                mm.insert_dynamic_tile (d_claw);
            }
            else if (key_state [SDL_SCANCODE_LCTRL]) {
                d_claw -> prev_anim = anim;
                d_claw -> set_anim ("!CLAW_ANIS_SWIPE");
            }
            else
                d_claw -> set_anim ("CLAW_ANIS_STAND");
        }
        disp.render_screen (&mm, &l1, state);
        SDL_Delay (latency);
        
        prev_key_state = new uint8_t [count];
        memcpy ((void *)prev_key_state, key_state, count);
    }
    
    delete prev_key_state;
    return 0;
}

