#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <list>
#include <vector>
#include <map>

using namespace std;

int f_read_integer (ifstream *, int );
int f_read_short_int (ifstream *, int);
string convert_to_three_digits (int);
string convert_int_to_string (int);
bool f_exists (string);
string f_read_path (ifstream *, int, int);
string convert_folder_path_to_unix (char*);
list <string>* get_directory_list (string);

class coords {
    public:
    int x;
    int y;
    coords ();
    coords (int, int);
};

#endif
