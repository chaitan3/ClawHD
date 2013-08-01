
#include "util.hpp"
#include <cstring>
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

int f_read_short_int (ifstream *file, int offset) {
    uint16_t val;
    file -> seekg (offset, ios::beg);
    file -> read ((char *) &val, sizeof (val));
    return val;
}

string f_read_path (ifstream *file, int offset, int length) {
    char* buffer = new char [length];
    file -> seekg (offset, ios::beg);
    file -> read (buffer, length);
    return string (buffer);
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

vector <string>* get_directory_list (string dir) {
    string read_dir = DATA_PREFIX + convert_folder_path_to_unix (dir);
    cout << read_dir << endl;
    vector <string>* list_dir = new vector <string>;
    DIR* ptr_dir = opendir (read_dir.c_str ());
    struct dirent* file_dir;
    cout << read_dir << endl;
    
    if (ptr_dir != NULL) {
        while ((file_dir = readdir (ptr_dir)) != NULL) {
            if (file_dir -> d_name [0] != '.') {
                int len = strlen (file_dir -> d_name);
                file_dir -> d_name [len - 4] = '\0'; 
                list_dir -> push_back (dir + string ("_") + string (file_dir -> d_name));
            }
        }
        sort (list_dir -> begin (), list_dir -> end ());
        closedir (ptr_dir);
    }
    return list_dir;
}

string convert_folder_path_to_unix (string str) {
    replace (str.begin (), str.end (), '\\', '/');
    replace (str.begin (), str.end (), '_', '/');
    return str;
}

//PID HACK
coords* get_offset_from_pid (string tile) {
    coords* c_off = new coords ();
    string p_tile = DATA2_PREFIX + convert_folder_path_to_unix (tile) + string (".PID");
    cout << p_tile << endl;
    ifstream f_pid (p_tile.c_str ());
    c_off -> x = f_read_integer (&f_pid, 0);
    cout << c_off -> x << endl;
    c_off -> y = f_read_integer (&f_pid, 4);
    cout << c_off -> y << endl;
    return new coords ();
}
coords::coords () {
    this -> x = 0;
    this -> y = 0;
}

coords::coords (int a, int b) {
    this -> x = a;
    this -> y = b;
}
