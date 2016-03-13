#ifndef SOUND_HPP
#define SOUND_HPP
#include "util.hpp"

const string SOUND_FILE_TYPE = ".wav";

class sound_manager {
    map <string, Mix_Chunk*> waves;    

    public:
    string level;

    int play_file (string);
    //sound_manager ();
    ~sound_manager ();
};
#endif
