/* 
 * File:   controls.h
 * Author: geth2k
 *
 * Created on 27 October 2013, 03:13
 */

#ifndef SID_FORMS_H
#define	SID_FORMS_H

#include "SID_control.h"
#include "SID_textures.h"
#include "SID_character_strip.h"
#include "SID_fonts.h"
#include "SID_user_events.h"

namespace sid {

    // @TODO Make forms derive from parent<control*>

    class forms {
    public:
        forms();
        forms(const forms& orig);
        virtual ~forms();

        control* at(std::string name);

        void add_form(control* ctl);
        void remove_form(uint idx);
        void remove_form(std::string name);
        void remove_form(control const* ref);
        void clear_forms();
        void insert_form(uint idx, control* ctl);
        void show_form(std::string name);
        void hide_form(std::string name);
        void control_to_front(std::string name);
        uint size();

        void render_all();

        bool process_event(SDL_Event& evt, int max_recurse = INT_MAX);

        void control_to_front(control* ctl);

        textures Textures;

    protected:

    private:
        std::list<control*> m_vctlChildren;
    };

}
#endif	/* FORMS_H */

