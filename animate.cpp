#include "animate.hpp"

animation::animation (string ani_file) {
    cout << ani_file.c_str () << endl;
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
    
    char c;
    cursor += image_len;
    for (int i = 0; i < num_frames; i++) {
        f_ani.seekg (cursor, ios::beg);
        while ((c = f_ani.get ()) != 0x03);
        cursor = (int)f_ani.tellg () + 5;
        
        int frameID = f_read_short_int (&f_ani, cursor);
        int dur = f_read_short_int (&f_ani, cursor + 2);
        //~ cout << frameID << " " << dur << endl;
        this -> frames.push_back (frameID);
        this -> duration.push_back (dur);
        cursor += 4;
    }
    
    f_ani.close ();
}

int animation::get_frame (int index) {
    return this -> frames [index];
}

int animation::get_num_frames () {
    return this -> num_frames;
}

int animation::get_duration (int index) {
    if (index < 0)
        return 0;
    return this -> duration [index];
}
    
