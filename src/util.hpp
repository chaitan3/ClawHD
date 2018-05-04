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
const string DATA_PREFIX = "Claw/Assets/";

const string SEPARATOR = "/";

void save_to_file (const string&, char*, int);
int f_read_integer (ifstream *, int );
int f_read_short_int (ifstream *, int);
int f_read_byte (ifstream *, int);
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

class Color {
    public:
        int32_t r, g, b, a;
        Color(int r, int g, int b, int a=0) {
            this -> r = r;
            this -> g = g;
            this -> b = b;
            this -> a = a;
        }
        Color() {
            this -> r = 0;
            this -> g = 0;
            this -> b = 0;
            this -> a = 0;
        }
};

#endif
