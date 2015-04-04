/* 
 * File:   toggle.h
 * Author: geth2k
 *
 * Created on 23 March 2014, 13:01
 */

#ifndef SID_TOGGLE_H
#define	SID_TOGGLE_H

#include "SID_button.h"

namespace sid {

    class toggle : public button {
    public:
        toggle();
        toggle(const toggle& orig);
        virtual ~toggle();

        virtual void cancel_click(SDL_Event& evt);

    protected:
        virtual void _mouse_down(SDL_Event& evt);
        virtual void _mouse_up(SDL_Event& evt);


    private:

        bool m_blToggleStatus;
        
    };
}
#endif	/* TOGGLE_H */

