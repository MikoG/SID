/* 
 * File:   user_events.cpp
 * Author: geth2k
 * 
 * Created on June 8, 2014, 6:34 PM
 */

#include "SID_user_events.h"

Uint32 sid::user_events::SID_CLOSE_FORM = SDL_RegisterEvents(1);
Uint32 sid::user_events::SID_START_GAME = SDL_RegisterEvents(1);
Uint32 sid::user_events::SID_EXIT_GAME  = SDL_RegisterEvents(1);
Uint32 sid::user_events::SID_PAUSE_GAME = SDL_RegisterEvents(1);

sid::user_events::user_events() {
}

sid::user_events::user_events(const user_events& orig) {
}

sid::user_events::~user_events() {
}

