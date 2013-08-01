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

#include "level.hpp"
#include "animate.hpp"
#include "kdtree.hpp"
#include "util.hpp"
#include "display.hpp"
#include <SDL2/SDL.h>

#define CLAW "CLAW_IMAGES"

int main (int argc, char **argv)
{   
    display disp;
    level l1 ("../Extracted/LEVEL1/WORLDS/WORLD.WWD");
    coords* start_state = l1.get_start_location ();
    kdtree <dynamic_tile*>* d_tiles = l1.get_dynamic_tiles ();
    dynamic_tile* d_claw = new dynamic_tile ("Captain Claw", CLAW, "CLAW_ANIS_STAND", start_state, 3000);
    d_tiles -> insert (d_claw);
    l1.put_image_files (CLAW);
    
    coords *state = d_claw -> get_coords ();
    SDL_Event event;
    int scroll_speed = 9;
    int latency = 10;
    
    int i = 0;
    while (1) {
        while (SDL_PollEvent (&event));
        const Uint8* key_state = SDL_GetKeyboardState (NULL);
        if (key_state [SDL_SCANCODE_ESCAPE])
            exit (0);
        else if (key_state [SDL_SCANCODE_LCTRL]) {
            d_claw -> set_anim ("CLAW_ANIS_SWIPE");
        }
        string anim = d_claw -> get_anim ();
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
                d_tiles -> remove (d_claw);
                state -> x += scroll_speed;
                d_tiles -> insert (d_claw);
            }
            else if (key_state [SDL_SCANCODE_LEFT] && (d_claw -> mirrored)) {
                d_tiles -> remove (d_claw);
                state -> x -= scroll_speed;
                d_tiles -> insert (d_claw);
            }
            else
                d_claw -> set_anim ("CLAW_ANIS_STAND");
        }
        disp.render_screen (&l1, state);
        SDL_Delay (latency);
    }
    
    return 0;
}

