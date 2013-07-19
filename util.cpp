
#include "util.h"
#include <sstream>
#include <sys/stat.h>

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

string convert_int_to_string (int number) {
    stringstream stream;
    stream << number;
    return stream.str ();
}

bool f_exists (string name) {
  struct stat buffer;   
  return stat (name.c_str(), &buffer) == 0; 
}

coords::coords () {
    this -> x = 0;
    this -> y = 0;
}
