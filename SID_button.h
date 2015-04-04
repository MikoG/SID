/* 
 * File:   button.h
 * Author: geth2k
 *
 * Created on 28 November 2013, 19:57
 */

#ifndef SID_BUTTON_H
#define	SID_BUTTON_H

#include "SID_control.h"
#include "SID_character_strip.h"

namespace sid {

    class button : public control {
    public:
        button();
        button(const button& orig);
        virtual ~button();

        void set_title(std::string title_text) const;

        character_strip* title;

        class state_images {
        public:

            enum eStates {
                NORMAL = 0,
                OFF = 0,
                ON = 1,
                PRESSED = 1,
                HOVER = 2,
                DISABLED = 3,
            };

            state_images();

            state_images(const state_images& orig) {
            }

            virtual ~state_images() {
            }

            void set_image_ID(eStates state, int value);
            int get_image_ID(eStates state) const;

        private:
            std::array<uint, 4> m_iImagIDs;

        } StatusImages;

        button::state_images::eStates get_state() const;
        button::state_images::eStates set_state(button::state_images::eStates state);

        virtual void cancel_click(SDL_Event& evt);

    protected:
        virtual void setup_ui();

        virtual void _mouse_down(SDL_Event& evt);
        virtual void _mouse_up(SDL_Event& evt);

    private:
        static const int DEFAULT_HEIGHT;

        button::state_images::eStates m_iState;

    };
}
#endif	/* BUTTON_H */

