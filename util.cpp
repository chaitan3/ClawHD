
#include "util.hpp"
#include <sstream>
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>

int f_read_integer (ifstream *file, int offset) {
    uint32_t val;
    file -> seekg (offset, ios::beg);
    file -> read ((char *) &val, sizeof (val));
    return val;
}

string f_read_path (ifstream *file, int offset, int length) {
    char* buffer = new char [length];
    file -> seekg (offset, ios::beg);
    file -> read (buffer, length);
    return convert_folder_path_to_unix (buffer);
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

list <string>* get_directory_list (string dir) {
    list <string>* list_dir = new list <string>;
    DIR* ptr_dir = opendir (dir.c_str ());
    struct dirent* file_dir;
    
    if (ptr_dir != NULL) {
        while ((file_dir = readdir (ptr_dir)) != NULL) {
            if (file_dir -> d_name [0] != '.')
                list_dir -> push_back (dir + string (file_dir -> d_name));
        }
        list_dir -> sort ();
        closedir (ptr_dir);
    }
    return list_dir;
}

string convert_folder_path_to_unix(char* c_str) {
    string str (c_str);
    replace (str.begin (), str.end (), '\\', '/');
    replace (str.begin (), str.end (), '_', '/');
    return str;
}

coords::coords () {
    this -> x = 0;
    this -> y = 0;
}

coords::coords (int a, int b) {
    this -> x = a;
    this -> y = b;
}
