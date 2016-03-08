#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

//PID HACK
#define DATA_PREFIX string ("images/")
#define DATA2_PREFIX string ("rez/")
#define SOUNDS_PREFIX string ("sounds/")

#define SEPARATOR string ("/")

using namespace std;

void save_to_file (string, char*, int);
int f_read_integer (ifstream *, int );
int f_read_short_int (ifstream *, int);
string convert_to_three_digits (int);
string convert_int_to_string (int);
bool f_exists (string);
string f_read_path (ifstream *, int, int);
string convert_folder_path_to_unix (string);
vector <string>* get_directory_list (string);

class coords {
    public:
    int x;
    int y;
    coords ();
    coords (int, int);
};

coords* get_offset_from_pid (string);

#endif
