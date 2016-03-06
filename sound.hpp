#ifndef SOUND_HPP
#define SOUND_HPP
#include "util.hpp"

#define SOUND_FILE_TYPE string (".wav")

class sound_manager {
    map <string, Mix_Chunk*> waves;    

    public:
    string level;

    int play_file (string);
    //sound_manager ();
    ~sound_manager ();
};
#endif
