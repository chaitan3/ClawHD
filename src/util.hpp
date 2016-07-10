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

using namespace std;

template<class T> 
void delete_map_objects (T obj) {
    for (auto& i: obj) {
            delete i.second; 
        }
}

//PID HACK
const string DATA_PREFIX = "images/";
const string DATA2_PREFIX = "rez/";
const string SOUNDS_PREFIX = "sounds/";

const string SEPARATOR = "/";

void save_to_file (const string&, char*, int);
int f_read_integer (ifstream *, int );
int f_read_short_int (ifstream *, int);
string convert_to_three_digits (int);
string convert_int_to_string (int);
bool f_exists (const string&);
string f_read_path (ifstream *, int, int);
string convert_folder_path_to_unix (const string&);
vector <string>* get_directory_list (const string&);

class coords {
    public:
    int x;
    int y;
    coords ();
    coords (int, int);
};

coords* get_offset_from_pid (const string&);

#endif
