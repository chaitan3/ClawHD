#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>

using namespace std;

int f_read_integer (ifstream *, int );
string convert_to_three_digits (int);

class coords {
    public:
    int x;
    int y;
    coords ();
};

#endif
