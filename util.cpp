
#include "util.h"
#include <sstream>

int f_read_integer (ifstream *file, int offset) {
    uint32_t val;
    file -> seekg (offset);
    file -> read ((char *) &val, sizeof (val));
    return val;
}

string convert_to_three_digits (int number) {
    stringstream stream;
    if (number >= 100) {
        stream << number;   
    }
    else if (number >= 10) {
        stream << "0" << number;
    }
    else {
        stream << "00" << number;
    }   
    return stream.str ();
}

coords::coords () {
    this -> x = 0;
    this -> y = 0;
}
