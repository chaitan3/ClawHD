
#include "util.hpp"
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>

void save_to_file (const string& filename, char* ptr_data, int len) {
   ofstream file (filename, ios::out | ios::binary);
   file.write (ptr_data, len);
   file.close ();
}

int f_read_byte (ifstream *file, int offset) {
    uint8_t val;
    file -> seekg (offset, ios::beg);
    file -> read ((char *) &val, sizeof (val));
    return val;
}

int f_read_integer (ifstream *file, int offset) {
    uint32_t val;
    file -> seekg (offset, ios::beg);
    file -> read ((char *) &val, sizeof (val));
    //printf("integer: %x %d\n", val, offset);
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
    string str = string (buffer);
    delete[] buffer;
    return str;
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

bool f_exists (const string& name) {
  struct stat buffer;   
  return stat (name.c_str(), &buffer) == 0; 
}

vector <string>* get_directory_list (const string& dir) {
    string read_dir = DATA_PREFIX + convert_folder_path_to_unix (dir);
    vector <string>* list_dir = new vector <string>;
    DIR* ptr_dir = opendir (read_dir.c_str ());
    
    struct dirent* file_dir;
    
    if (ptr_dir != nullptr) {
        while ((file_dir = readdir (ptr_dir)) != nullptr) {
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

string convert_folder_path_to_unix (const string& str) {
    string path (str);
    replace (path.begin (), path.end (), '\\', '/');
    replace (path.begin (), path.end (), '_', '/');
    // ARROWSIGN HACK
    string arrow = "ARROWSIGN/";
    int i = path.find (arrow);
    if (i >= 0) 
        path [i + 9] = '_';
    return path;
}

//PID HACK
coords* get_offset_from_pid (const string& tile) {
    coords* c_off = new coords ();
    string p_tile = DATA_PREFIX + convert_folder_path_to_unix (tile) + string (".PID");
    ifstream f_pid (p_tile.c_str ());
    if (f_pid.is_open ()) {
        c_off -> x = f_read_integer (&f_pid, 16);
        c_off -> y = f_read_integer (&f_pid, 20);
    }
    return c_off;
}
coords::coords () {
    this -> x = -1;
    this -> y = -1;
}

coords::coords (int a, int b) {
    this -> x = a;
    this -> y = b;
}

int true_mod (int n, int M) {
    return ((n % M) + M) % M;
}
