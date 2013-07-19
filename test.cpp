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

#include "level.h"
#include "util.h"
#include "display.h"

int main (int argc, char **argv)
{   
    display disp;
    level l1 ("l1");
    coords state;
    
    state.x = 10;
    state.y = 10;
    
    cout << l1.p_bg.get_height() << " " << l1.p_bg.get_width() << endl;
    cout << l1.p_action.get_height() << " " << l1.p_action.get_width() << endl;
    cout << l1.p_fg.get_height() << " " << l1.p_fg.get_width() << endl;
    
    string prefix = "../Images/LEVEL1/TILES/BACK/";
    disp.render_screen (prefix, l1.p_bg.get_tile_ids (), &state);
    string prefix = "../Images/LEVEL1/TILES/ACTION/";
    disp.render_screen (prefix, l1.p_action.get_tile_ids (), &state);
    return 0;
}

