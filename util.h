#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>

using namespace std;

int f_read_integer (ifstream *, int );
string convert_to_three_digits (int);
string convert_int_to_string (int);
bool f_exists (string);

class coords {
    public:
    int x;
    int y;
    coords ();
};

#endif
