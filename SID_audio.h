/* 
 * File:   audio.h
 * Author: geth2k
 *
 * Created on 02 April 2014, 18:36
 */

#ifndef SID_AUDIO_H
#define	SID_AUDIO_H

#include <string>
#include <map>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace sid {

    class audio {
    public:
        audio();
        audio(const audio& orig);
        virtual ~audio();

        bool add_song(std::string fileName, uint const id);
        bool play_song(int const id);
        bool fade_in_song(uint const id, int ms);

        void fade_out(int ms) {
            Mix_FadeOutMusic(ms);
        }

        void halt_music() {
            Mix_HaltMusic();
        }

        bool add_sound(std::string fileName, uint const id);
        bool play_sound(int const id);

        class music {
        public:
            music();
            music(music const& orig);
            virtual ~music();

            bool add_song(std::string fileName, uint id);
            bool play_song(uint id);
            bool fade_in_song(uint id, int ms);
            void clear_songs();

            class song {
                std::string __m_FileName;
                Mix_Music* __m_SongData;
                bool __m_blInitialised;

            public:
                song();
                song(std::string file);
                song(song const& orig);
                virtual ~song();
                void operator=(song const& orig);

                bool play();
                bool fade_in(int ms);
                ;
                void clear_data();

                inline bool is_init() const {
                    return this->__m_blInitialised;
                }
            };

        private:
            std::map<int, song> __m_vsng;
        };

        class sounds {
        public:
            sounds();

            sounds(sounds const& orig);

            virtual ~sounds();

            bool add_sound(std::string fileName, uint const id);
            bool play_sound(uint id);

            void set_volume(int const volume);
            int get_volume() const;
            void clear_sounds();

            class sound {
                Mix_Chunk* __m_Sample;
                std::string __m_FileName;
                bool __m_blInitialised;

            public:
                sound();
                sound(std::string file);
                sound(sound const& orig);
                virtual ~sound();

                bool play();
                void clear_data();
                void operator=(sound const& orig);

                inline bool is_init() const {
                    return this->__m_blInitialised;
                }
            };

        private:
            std::map<int, sound> __m_vsnd;

        };

    private:
        static Uint8 __s_Stream;
        static Uint8 *__s_AudioChunk;
        static Uint32 __s_AudioLen;
        static Uint8 *__s_AudioPos;

        sounds __m_Sounds;
        music __m_Music;

        static int __s_iInstances;
        static bool __s_blAudioInit;
        static bool __init_sound();
    };
}
#endif	/* AUDIO_H */

