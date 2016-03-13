#include "sound.hpp"

//void sound_manager::sound_manager () {
//
//}

int sound_manager::play_file (string file) {
    if (this -> waves.count(file) == 0) {
        if (file.find("LEVEL") == 0) {
            file.insert(5, this -> level + "/SOUNDS");
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

sound_manager::~sound_manager () {
    for (auto& kv: this -> waves) {
        Mix_FreeChunk (kv.second);
    }
}
