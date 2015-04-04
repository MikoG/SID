/*
 * File:   audio.cpp
 * Author: geth2k
 *
 * Created on 02 April 2014, 18:36
 */

#include "SID_audio.h"

int sid::audio::__s_iInstances = 0;
bool sid::audio::__s_blAudioInit = false;

sid::audio::audio() {

    if (!sid::audio::__s_blAudioInit) {
        sid::audio::__s_blAudioInit = sid::audio::__init_sound();
    }
    sid::audio::__s_iInstances++;
}

sid::audio::audio(const audio& orig) :
__m_Music(orig.__m_Music),
__m_Sounds(orig.__m_Sounds) {

    sid::audio::__s_iInstances++;
}

sid::audio::~audio() {
    this->__m_Music.clear_songs();
    this->__m_Sounds.clear_sounds();

    sid::audio::__s_iInstances--;

    if (sid::audio::__s_iInstances == 0 && sid::audio::__s_blAudioInit) {
        while (Mix_Init(0))
            Mix_Quit();
        Mix_CloseAudio();
    }
}

bool sid::audio::__init_sound() {
    // load support for the MP3 music format
    int flags = MIX_INIT_OGG | MIX_INIT_MP3;
    int iMixInitted = Mix_Init(flags);
    if (iMixInitted & flags != flags) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "SID Error",
                "Failed to initialise required OGG (vorbis) and MP3 (smpeg) support.",
                NULL);
        return false;
    }

    // start SDL with audio support
    if (SDL_Init(SDL_INIT_AUDIO) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        return false;
    }

    // open 22.05KHz, signed 16bit, system byte order,
    // stereo audio, using 512 byte chunks
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 1, 512) == -1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        return false;
    }
    int freq;
    int channels;
    Uint16 format;
    Mix_AllocateChannels(2);
    Mix_QuerySpec(&freq, &format, &channels);
    std::cout << freq << ", " << channels << ", " << format << std::cout;


    return true;
}

bool sid::audio::add_song(std::string fileName, uint const id) {
    return this->__m_Music.add_song(fileName, id);
}

bool sid::audio::play_song(int const id) {
    return this->__m_Music.play_song(id);
}

bool sid::audio::fade_in_song(uint const id, int ms) {
    return this->__m_Music.fade_in_song(id, ms);
}

bool sid::audio::add_sound(std::string fileName, uint const id) {
    return this->__m_Sounds.add_sound(fileName, id);
}

bool sid::audio::play_sound(int const id) {
    return this->__m_Sounds.play_sound(id);
}

bool sid::audio::music::add_song(std::string fileName, uint id) {
    this->__m_vsng[id] = song(fileName);
    return this->__m_vsng[id].is_init();
}

sid::audio::music::music() {
    
}

sid::audio::music::music(music const& orig) :
__m_vsng(orig.__m_vsng) {
    
}

sid::audio::music::~music() {
    this->clear_songs();
}

bool sid::audio::music::play_song(uint id) {
    return __m_vsng[id].play();
}

bool sid::audio::music::fade_in_song(uint const id, int ms) {
    return this->__m_vsng[id].fade_in(ms);
}

void sid::audio::music::clear_songs() {
    for (std::map<int, song>::iterator it1 = this->__m_vsng.begin();
            it1 != this->__m_vsng.end();
            it1++) {
        it1->second.clear_data();
    }
    this->__m_vsng.clear();
}

sid::audio::music::song::song() :
__m_SongData(nullptr),
__m_blInitialised(false) {

}

sid::audio::music::song::song(std::string file) :
__m_SongData(nullptr),
__m_blInitialised(false) {
    this->__m_SongData = Mix_LoadMUS(file.c_str());
    if (this->__m_SongData) {
        this->__m_FileName = __m_FileName;
        this->__m_blInitialised = true;
    }
}

sid::audio::music::song::song(song const& orig) :
__m_FileName(orig.__m_FileName),
__m_SongData(orig.__m_SongData),
__m_blInitialised(orig.__m_blInitialised) {

}

sid::audio::music::song::~song() {

}

void sid::audio::music::song::operator=(song const& orig) {
    this->__m_blInitialised = orig.__m_blInitialised;
    this->__m_FileName = orig.__m_FileName;
    if (this->__m_SongData) {
        Mix_FreeMusic(this->__m_SongData);
    }
    this->__m_SongData = orig.__m_SongData;
}

bool sid::audio::music::song::play() {
    Mix_VolumeMusic(128);
    if (Mix_PlayMusic(this->__m_SongData, 1) == -1) {
        return false;
    }

    return true;
}

bool sid::audio::music::song::fade_in(int ms) {
    Mix_VolumeMusic(128);
    if (Mix_FadeInMusic(this->__m_SongData, 1, ms) == -1) {
        return false;
    }

    return true;    
}

void sid::audio::music::song::clear_data() {
    if (this->__m_SongData != nullptr) {
        Mix_FreeMusic(this->__m_SongData);
    }
}

sid::audio::sounds::sounds() {
    
}

sid::audio::sounds::sounds(const sounds& orig) :
__m_vsnd(orig.__m_vsnd) {

}

sid::audio::sounds::~sounds() {
    this->clear_sounds();
}

bool sid::audio::sounds::add_sound(std::string fileName, uint const id) {
    this->__m_vsnd[id] = sound(fileName);
    return this->__m_vsnd[id].is_init();
}

bool sid::audio::sounds::play_sound(uint id) {
    return this->__m_vsnd[id].play();
}

void sid::audio::sounds::set_volume(int const volume) {
    Mix_Volume(-1, volume);
}

int sid::audio::sounds::get_volume() const {
    return Mix_Volume(-1, -1);
}

void sid::audio::sounds::clear_sounds() {
    for (int i = 0; i < this->__m_vsnd.size(); i++) {
        this->__m_vsnd[i].clear_data();
    }
    this->__m_vsnd.clear();
}

sid::audio::sounds::sound::sound() :
__m_Sample(nullptr),
__m_blInitialised(false) {

}

sid::audio::sounds::sound::sound(std::string file) :
__m_Sample(nullptr),
__m_FileName(file),
__m_blInitialised(false) {
    
    this->__m_Sample = Mix_LoadWAV(file.c_str());
    if (this->__m_Sample) {
        this->__m_FileName = file;
        this->__m_blInitialised = true;
    }

}

sid::audio::sounds::sound::sound(sound const& orig) :
__m_Sample(orig.__m_Sample),
__m_FileName(orig.__m_FileName),
__m_blInitialised(orig.__m_blInitialised) {

}

sid::audio::sounds::sound::~sound() {
}

bool sid::audio::sounds::sound::play() {
    if (Mix_PlayChannel(-1, this->__m_Sample, 0) == -1)
        return false;

    return true;
}

void sid::audio::sounds::sound::clear_data() {
    if (this->__m_Sample != nullptr) {
        Mix_FreeChunk(this->__m_Sample);
    }
}

void sid::audio::sounds::sound::operator=(sound const& orig) {
    if (this->__m_Sample)
        Mix_FreeChunk(this->__m_Sample);
    this->__m_FileName = orig.__m_FileName;
    this->__m_Sample = orig.__m_Sample;
}