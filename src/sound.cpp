#include "util.hpp"
#include "memory.hpp"

const string SOUND_FILE_TYPE = ".wav";

int memory_manager::sound_play_file (const string& c_file) {
    string file = c_file;
    if (this -> waves.count(file) == 0) {
        if (file.find("LEVEL") == 0) {
            file.insert(5, string("1/SOUNDS"));
        }
        string path = SOUNDS_PREFIX + convert_folder_path_to_unix (file) + SOUND_FILE_TYPE;
        this -> waves[file] = Mix_LoadWAV (path.c_str()); 
        if (this -> waves[file] == nullptr) {
            cout << "Could not load " << path << endl;
            exit (1);
        }
    }
    Mix_Chunk *wave =  this -> waves [file];
    return Mix_PlayChannel (-1, wave, 0);
}

void memory_manager::delete_sound () {
    for (auto& kv: this -> waves) {
        Mix_FreeChunk (kv.second);
    }
}
