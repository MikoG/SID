/* 
 * File:   event_calls.h
 * Author: geth2k
 *
 * Created on June 24, 2014, 3:55 PM
 */

#ifndef SID_EVENT_CALLS_H
#define	SID_EVENT_CALLS_H

#include <iostream>
#include <map>

#include <SDL2/SDL_events.h>

namespace sid {

    template <typename T>
    class event_calls {
    public:

        static const int CALL_TYPES_COUNT = 8;

        typedef void (*t_event_call)(T sender, SDL_Event& evt);

        event_calls() :
        m_vfnEventCalls() {
            for (int i = 0; i < event_calls::CALL_TYPES_COUNT; i++) {
                m_vfnEventCalls[i] = [](T sender, SDL_Event & evt) {
                };
            }
        }

        void add_call(int slot, t_event_call func) {
            m_vfnEventCalls[slot] = func;
        }

        void call(int slot, T sender, SDL_Event& evt) {
            m_vfnEventCalls.at(slot)(sender, evt);
        }

    private:
        typedef std::map<char, t_event_call> t_call_map;
        t_call_map m_vfnEventCalls;

    };
}
#endif	/* EVENT_CALLS_H */

