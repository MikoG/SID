/* 
 * File:   mouse.h
 * Author: geth2k
 *
 * Created on 01 January 2014, 17:44
 */

#ifndef SID_MOUSE_H
#define	SID_MOUSE_H

#include <array>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL_mouse.h>
#include <time.h>

namespace sid {

    class mouse {
    public:

        enum eMouseButtons {
            NONE_BTN = 0x00,
            LEFT_BTN = 0x01,
            MIDL_BTN = 0x02,
            RGHT_BTN = 0x03,
            SCRL_UP_BTN = 0x04,
            SCRL_DN_BTN = 0x05,
            SCRL_LT_BTN = 0x06,
            SCRL_RT_BTN = 0x07,
            SIDE_LT_BTN = 0x08,
            SIDE_RT_BTN = 0x08
        };

        enum eCursorIDs {
            DEFAULT = 0x02,
            CROSS1 = 0x01,
            ARROW_NOCLICK = 0x19,
            CROSSHAIR = 0x23,
            ROTATE = 0x32,
            HAND_GRAB = 0x34,
            HAND_POINT = 0x3A,
        };

        struct mouse_action {
            friend class mouse;

            mouse_action() : lastPressTime(0), lastReleaseTime(0), m_blPressed(false) {
            }

            mouse_action(const mouse_action& orig) :
            lastPressTime(orig.lastPressTime),
            lastReleaseTime(orig.lastReleaseTime),
            m_blPressed(orig.m_blPressed) {

            }

            virtual ~mouse_action() {
            }

            bool press(int time);
            bool release(int time);
            bool move(int time);

            bool is_pressed() const;
            int get_last_pressed_time() const;
            int get_last_release_time() const;

        private:
            bool m_blPressed;
            int lastPressTime;
            int lastReleaseTime;
            eMouseButtons m_ButtonEnum;
            static eMouseButtons* iLastButton;
        };

        mouse();
        mouse(const mouse& orig);
        virtual ~mouse();

        mouse_action* operator[](const int index);

        static void set_cursor(SDL_SystemCursor id);
        eMouseButtons get_prev_button_id() const;

        bool timed_cursor_changes(double td, SDL_SystemCursor cidLMB,
                SDL_SystemCursor cidRMB, bool force = false);
        static SDL_SystemCursor s_eCursorID;

    private:
        std::array<mouse_action, 9> m_Buttons;
        eMouseButtons m_mbLastPressed;

        static int s_iDblClickTime;
        static int s_iDragClickSpeed;
    };
}
#endif	/* MOUSE_H */

