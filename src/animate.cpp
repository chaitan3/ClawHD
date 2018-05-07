#include "animate.hpp"
#include "memory.hpp"

animation::animation (const string& anim_file) {
    string a_file = DATA_PREFIX + convert_folder_path_to_unix (anim_file) +
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
    cout << anim_file << " ";
    for (auto i: this->frames)
          cout << i << ' ';
    cout << endl;
    
    f_ani.close ();
}

int animation::get_next_frame (animation_state* anim_state) {
    int curr_time = SDL_GetTicks ();
    if (curr_time > anim_state -> last_update_time + this -> duration [anim_state -> frame]) {
        (anim_state -> frame) ++;
        if (anim_state -> frame >= this -> num_frames) {
            return -1;
        }
        anim_state -> last_update_time = curr_time;
    }
    return this -> frames [anim_state -> frame];
}

animation_state::animation_state () {
    this -> reset ();
}
void animation_state::reset () {
    this -> frame = -1;
    this -> last_update_time = -0xFF;
}

animation* memory_manager::get_animation (const string& anim) {
    if (anim.length() == 0)
        return nullptr;
    if ((anim.find("GAME_SOUND") == 0) || (anim.find("LEVEL") == 0)) {
            return nullptr;
        }
    if (this -> animations.count (anim) == 0) {
            this -> animations [anim] = new animation (anim);
        }
    return this -> animations [anim];
}

string memory_manager::get_default_image (const string& image) {
    if (this -> animation_image_list.count (image) == 0) {
        //this -> load_image_list(image);
        //return this -> get_default_image(image);
        return image;
    }
    else
        return this -> animation_image_list [image] -> begin()->second;
}


void memory_manager::load_image_list (const string& image) {
    if (this -> animation_image_list.count (image) == 0) {
        vector<string>* dir_list = get_directory_list (image);
        this -> animation_image_list[image] = new map<string, string>;
        for (const string& path: *dir_list) {
            int index = path.rfind('_');
            string key = path.substr(index + 1);
            cout << key << " " << path << endl;
            (*this -> animation_image_list [image])[key] = path;
        }
        delete dir_list;
    }
}

string memory_manager::get_image_from_list (const string& image, int frame) {
    return this -> animation_image_list [image] -> at ("FRAME" + convert_to_three_digits(frame));
}
