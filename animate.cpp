#include "animate.hpp"

animation::animation (string ani_file) {
    string a_file = DATA2_PREFIX + convert_folder_path_to_unix (ani_file) +
        ANIMATION_FILE_TYPE;
    
    ifstream f_ani (a_file.c_str ());
    
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
        cout << cursor << endl;
        int frameID = f_read_short_int (&f_ani, cursor + 8);
        int dur = f_read_short_int (&f_ani, cursor + 10);
        //~ cout << frameID << " " << dur << endl;
        this -> frames.push_back (frameID);
        this -> duration.push_back (dur);
        f_ani.seekg (cursor, ios::beg);
        c = f_ani.get ();
        if ((c & 2) == 2) {
            cursor += 20;
            f_ani.seekg (cursor, ios::beg);
            while ((c = f_ani.get ()) != 0);
            cursor = f_ani.tellg ();
        }
        else
            cursor += 20;
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
    
