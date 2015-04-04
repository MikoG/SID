/* 
 * File:   T_3DOAPI.h
 * Author: geth2k
 *
 * Created on 09 April 2013, 19:59
 */

#ifndef API_H
#define	API_H

#include <SDL2/SDL.h>

#include "SID_display.h"
#include "SID_environs.h"
#include "SID_forms.h"
#include "SID_timer.h"

namespace sid {

    class api {
    public:

        api();
        api(height_map* HeightMap, height_map* TreesMap);
        api(const api& orig);
        virtual ~api();

        /* Get the version description of this API.*/
        static std::string get_version();

        virtual void run();

        virtual void init_engine() = 0;
        virtual void run_gameloop() = 0;

        static const std::string s_strVersion_;

    protected:
        bool process_event(bool recurse = true);
        void render_3d();
        void render_2d();
        void build_terrain();

        inline void stop_game_loop() {
            this->__m_blStopGame = true;
        }

        inline bool is_game_stopped() const {
            return this->__m_blStopGame;
        }

        inline bool is_menu_stopped() const {
            return this->__m_blStopMenus;
        }

        inline bool is_main_stopped() const {
            return this->__m_blStopMain;
        }

        bool is_paused() const {
            return __m_blPaused;
        }

        virtual void build_main_menu() = 0;
        virtual void load_gametextures() = 0;
        virtual void add_ingame_menus() = 0;
        virtual void load_fonts() = 0;
        virtual void add_debug_form() = 0;
        virtual void load_music() = 0;
        virtual void pause_game() = 0;
        virtual void unpause_game() = 0;

    protected:
        audio _m_Audio;
        display* _m_Display;
        forms* _m_Forms;
        environs* _m_Environs;
        camera* _m_Camera;
        keyboard _m_Keyboard;

        timer m_clkFPS;
        timer m_clkTimeDiff;

        void _set_pause_status(bool status) {
            this->__m_blPaused = status;
            this->_m_Environs->set_pause_status(status);
        }

    private:
        void run_main_menu();

        bool __m_blStopMenus;
        bool __m_blStopGame;
        bool __m_blStopMain;
        bool __m_blPaused;
    };

    void SID_Quit();

    int filter_events(void *userdata, SDL_Event * event);
}

#endif	/* API_H */