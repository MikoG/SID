/* 
 * File:   radio_button.h
 * Author: geth2k
 *
 * Created on 31 March 2014, 00:50
 */

#ifndef SID_RADIO_BUTTON_H
#define	SID_RADIO_BUTTON_H

#include "SID_toggle.h"

namespace sid {

    class radio_button : public toggle {
    public:
        radio_button();
        radio_button(const radio_button& orig);
        virtual ~radio_button();

        void switch_off();

        class rb_group {
        public:

            void add(radio_button* rb) {
                this->m_vrbGroup.push_back(rb);
                rb->set_group(this);
            }

            void switch_to(radio_button* rb) {
                for (std::vector<radio_button*>::iterator it1 = this->m_vrbGroup.begin();
                        it1 != this->m_vrbGroup.end();
                        it1++) {
                    if (*it1 != rb) {
                        (*it1)->switch_off();
                    }
                }
            }
        private:
            std::vector<radio_button*> m_vrbGroup;
        };

        rb_group* m_group;
        rb_group* get_group() const;
        void set_group(rb_group* group);

    protected:

        virtual void _mouse_up(SDL_Event& evt) {
            toggle::_mouse_up(evt);
            if (this->m_group != nullptr) {
                this->m_group->switch_to(this);
            }
        }

    private:

    };
}
#endif	/* RADIO_BUTTON_H */