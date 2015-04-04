/* 
 * File:   user_events.h
 * Author: geth2k
 *
 * Created on June 8, 2014, 6:34 PM
 */

#ifndef SID_USER_EVENTS_H
#define	SID_USER_EVENTS_H

#include <SDL2/SDL.h>

namespace sid {

    class user_events {
    public:
        user_events();
        user_events(const user_events& orig);
        virtual ~user_events();

        static Uint32 SID_CLOSE_FORM;
        static Uint32 SID_START_GAME;
        static Uint32 SID_EXIT_GAME;
        static Uint32 SID_PAUSE_GAME;
    private:

    };
}
#endif	/* USER_EVENTS_H */

