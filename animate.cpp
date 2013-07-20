#include "animate.hpp"

animation::animation (string ani_file) {
    ifstream f_ani (ani_file.c_str ());
    
    int cursor = 0x0c;
    this -> num_frames = f_read_integer (&f_ani, cursor);
    cursor += 4;
    
    int image_len = f_read_integer (&f_ani, cursor);
    char buffer [image_len + 1];
    cursor = 0x20;
    f_ani.seekg (cursor, ios::beg);
    f_ani.read (buffer, image_len + 1);
    this -> image = string (buffer);
    
    cursor += image_len + 1;
    for (int i = 0; i < num_frames; i++) {
        cursor += 7;
        int frameID = f_read_short_int (&f_ani, cursor);
        int dur = f_read_short_int (&f_ani, cursor + 2);
        cout << frameID << " " << dur << endl;
        
        this -> frames.push_back (frameID);
        this -> duration.push_back (dur);
        cursor += 13;
    }
    
    f_ani.close ();
}