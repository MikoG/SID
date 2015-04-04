/*
 * File:   T_3DOAPI.cpp
 * Author: geth2k
 *
 * Created on 09 April 2013, 19:59
 */

#include "SID_api.h"

std::string const sid::api::s_strVersion_ = "3DO API Version 0.1 (Alpha)";

sid::api::api() :
_m_Audio(),
_m_Display(nullptr),
_m_Camera(new camera(70, 70, 70, plot::SEA_LEVEL + 10, plot::SEA_LEVEL + 500)),
__m_blStopGame(false),
__m_blStopMain(false) {

    SDL_Init(SDL_INIT_EVERYTHING);
    std::cout << user_events::SID_CLOSE_FORM << std::endl;

    int iImageFlags = IMG_INIT_PNG;
    int iImageInitted = IMG_Init(iImageFlags);
    if (iImageInitted & iImageFlags != iImageFlags) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "SID Error",
                "Failed to initialise PNG image support.",
                NULL);
        SID_Quit();
        exit(1);
    }
    
    SDL_SetEventFilter(filter_events, nullptr);

    camera::Keyboard = &this->_m_Keyboard;
}

sid::api::api(const api& orig) :
_m_Display(orig._m_Display),
_m_Environs(orig._m_Environs),
_m_Camera(orig._m_Camera),
__m_blStopGame(false) {

    this->_m_Forms = orig._m_Forms;
}

sid::api::~api() {
    object::clean_up_textures();
    
    SID_Quit();

    if (this->_m_Camera)
        delete this->_m_Camera;

    if (this->_m_Forms)
        delete this->_m_Forms;

    if (this->_m_Environs)
        delete this->_m_Environs;

    if (this->_m_Display)
        delete this->_m_Display;

    std::cout << "API Destructor" << std::endl;
}

/*
 *
 */
std::string sid::api::get_version() {
    return s_strVersion_;
}

void sid::api::run() {

    this->init_engine();
    int i = 0;
    while (!this->is_main_stopped()) {
        this->run_main_menu();
        this->__m_blStopMenus = false;
        this->__m_blPaused = false;

        if (!this->is_game_stopped()) {
            this->run_gameloop();
            this->__m_blStopGame = false;
        }
        i++;
        if (i > 1000) exit(1);
    }
}

void sid::api::run_main_menu() {
    this->build_main_menu();
    while (!this->is_menu_stopped()) {
        SDL_Event Event;
        while (SDL_PollEvent(&Event) == 1) {
            if (Event.type == user_events::SID_START_GAME) {
                this->__m_blStopMenus = true;
            } else if (Event.type == SDL_QUIT) {
                this->stop_game_loop();
                this->_m_Forms->clear_forms();
                this->__m_blStopMain = true;
                this->__m_blStopMenus = true;
            }
            this->_m_Forms->process_event(Event);
        }
        this->render_3d();
        this->render_2d();
        this->_m_Display->swap_buffers();
    }
}

/*
 *
 */
bool sid::api::process_event(bool recurse) {
    SDL_Event Event;

    if (!this->is_paused()) {
        _m_Camera->process_keyboard(this->m_clkTimeDiff.get_delta());
    }

    if (SDL_PollEvent(&Event) == 1) {

        if (Event.type == SDL_MOUSEMOTION) {
            SDL_FlushEvent(SDL_MOUSEMOTION);
        } 

        bool blEventInForm;
        if (_m_Camera->get_gesture_mode() == camera::eGestureModes::NONE) {
            blEventInForm = this->_m_Forms->process_event(Event);
        } else {
            blEventInForm = false;
        }


        if (!blEventInForm && this->_m_Environs && !this->is_paused()) {
            this->_m_Environs->process_keyboard(Event);

            bool blDblClick = false;
            bool blDragClick = false;
            bool blDrag = false;
            object_ingame* objgSelection;
            int iBtnReleaseDelta;
            objgSelection = static_cast<object_ingame*> (object_ingame::get_focus());
            switch (Event.type) {
                case SDL_KEYDOWN:
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    blDblClick = _m_Camera->Mouse[Event.button.button]->press(Event.button.timestamp);
                    if (blDblClick &&
                            _m_Camera->Mouse.get_prev_button_id() == mouse::eMouseButtons::LEFT_BTN) {

                        if (object_ingame::get_mouseover()) {
                            static_cast<object_ingame*> (object_ingame::get_mouseover())->set_as_zoom_object();
                            object_ingame::get_zoom_object()->set_zoom_to(true);
                        }
                    } else {
                        this->_m_Environs->process_event(Event, false, true);
                        this->_m_Camera->process_mouse(Event);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    iBtnReleaseDelta = Event.button.timestamp -
                            _m_Camera->Mouse[_m_Camera->Mouse.get_prev_button_id()]->get_last_release_time();
                    blDragClick = _m_Camera->Mouse[Event.button.button]->release(Event.button.timestamp);
                    if (!blDragClick &&
                            !this->_m_Keyboard.inputs[_m_Camera->Actions.quickGrabToggle] &&
                            iBtnReleaseDelta > 300) {
                        switch (Event.button.button) {
                            case mouse::eMouseButtons::LEFT_BTN:
                                //                                object::set_focus(nullptr);
                                this->_m_Environs->process_event(Event,
                                        false,
                                        true);
                        }
                    }
                    _m_Camera->process_mouse(Event);
                    break;
                case SDL_MOUSEMOTION:
                    blDrag = _m_Camera->Mouse[_m_Camera->Mouse.get_prev_button_id()]->move(Event.mgesture.timestamp);
                    if ((blDrag ||
                            this->_m_Keyboard.inputs[_m_Camera->Actions.quickGrabToggle]) &&
                            !this->_m_Keyboard.inputs[_m_Camera->Actions.attackClickToggle]) {
                        if (this->_m_Environs->get_mouse_mode() != 3) {
                            if (object_ingame::get_zoom_object() != nullptr) {
                                object_ingame::get_zoom_object()->set_zoom_to(false);
                                object_ingame::set_zoom_object(nullptr);
                            }
                            _m_Camera->process_mouse(Event);
                        }
                        this->_m_Environs->process_event(Event, true, true);
                    } else {
                        this->_m_Environs->process_event(Event, false, true);
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    _m_Camera->process_mouse(Event);
                    break;
            }
        }

        if (this->_m_Environs->get_mouse_mode() != 3) {
            if (this->_m_Keyboard.inputs[_m_Camera->Actions.attackClickToggle]) {
                mouse::set_cursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
            } else {
                
                if (mouse::s_eCursorID == SDL_SYSTEM_CURSOR_CROSSHAIR) {
                    mouse::set_cursor(SDL_SYSTEM_CURSOR_ARROW);
                }
                
                _m_Camera->Mouse.timed_cursor_changes(this->m_clkTimeDiff.get_delta(),
                        SDL_SYSTEM_CURSOR_HAND,
                        SDL_SYSTEM_CURSOR_HAND,
                        this->_m_Keyboard.inputs[_m_Camera->Actions.quickGrabToggle]);
                
            }
        }

        switch (Event.type) {
            case SDL_QUIT:
                this->stop_game_loop();
                this->__m_blStopMain = true;
                break;
            case SDL_KEYDOWN:
                switch (Event.key.keysym.sym) {
                    case SDLK_F12:
                        // For debugging only.
                        this->stop_game_loop();
                        break;
                    case SDLK_ESCAPE:
                        if (!this->is_paused()) {
                            SDL_Event PushEvent;
                            PushEvent.user.type = user_events::SID_PAUSE_GAME;
                            SDL_PushEvent(&PushEvent);
                        }
                        break;
                }
                break;
        }
        if (Event.type == user_events::SID_PAUSE_GAME) {
            if (!this->is_paused()) {
                this->_set_pause_status(true);
                this->pause_game();
            } else {
                this->_set_pause_status(false);
                this->unpause_game();
            }
        } else if (Event.type == user_events::SID_EXIT_GAME) {
            this->stop_game_loop();
        }
    }
}

void sid::api::render_3d() {
    sid::init_frame_3d(60, 2000);
    sid::set_fogging(
            this->_m_Camera->get_distance(), 200);

    this->_m_Camera->set_view();
    
    if (this->_m_Environs) {
        this->_m_Environs->make_new_vbos();
        frustum Frustum = frustum();
        this->_m_Environs->render_all(Frustum);
    }
}

void sid::api::render_2d() {
    sid::init_frame_2d();
    this->_m_Forms->render_all();
}

void sid::api::build_terrain() {
    vertex3 vtxCamFocus = this->_m_Camera->get_focus_pos();
    chunk::chunkoffset ofsPlotCoords = environs::get_chunk_offset(vtxCamFocus);
    this->_m_Environs->build_environs(ofsPlotCoords);
}

void sid::SID_Quit() {
    // unload the dynamically loaded image libraries.
    IMG_Quit();

    // Unload mixer Libraries.
    Mix_CloseAudio();
    while (Mix_Init(0))
        Mix_Quit();

    // Quit SDL and close window.
    SDL_Quit();
}

int sid::filter_events(void *userdata, SDL_Event* event) {
    int iRetVal(1);
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        if (event->key.repeat != 0) {
            iRetVal = 0;
        }
    }
    return iRetVal;
}