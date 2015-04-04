/* 
 * File:   InteractiveObjet.cpp
 * Author: geth2k
 * 
 * Created on June 24, 2014, 8:09 PM
 */

#include "SID_interactive_object.h"

sid::interactive_object* sid::interactive_object::__s_objMouseOver = nullptr;
sid::interactive_object* sid::interactive_object::__s_objFocus = nullptr;
sid::interactive_object* sid::interactive_object::__s_objClickOwner = nullptr;

sid::interactive_object::interactive_object() {
}

sid::interactive_object::interactive_object(object::t_object_data setFlags) :
object(setFlags) {
}

sid::interactive_object::interactive_object(interactive_object const& orig) :
object(orig) {
}

sid::interactive_object::~interactive_object() {
    if (sid::interactive_object::__s_objFocus == this) {
        sid::interactive_object::__s_objFocus = nullptr;
    }

    if (sid::interactive_object::get_mouseover() == this) {
        sid::interactive_object::set_mouseover(nullptr);
    }

    if (sid::interactive_object::get_focus() == this) {
        sid::interactive_object::set_focus(nullptr);
    }
}

bool sid::interactive_object::_process_event(SDL_Event& evt) {
    switch (evt.type) {
        case SDL_MOUSEBUTTONDOWN:
            this->_mouse_down(evt);
            this->__m_EventCalls.call(
                    sid::interactive_object::eCallIndex::MOUSE_DOWN,
                    this,
                    evt);
            this->set_as_focus();
            this->set_as_clickowner();
            break;
        case SDL_MOUSEBUTTONUP:
            if (sid::interactive_object::get_clickowner() == this) {
                this->set_as_focus();
                this->_mouse_up(evt);
                this->__m_EventCalls.call(
                        sid::interactive_object::eCallIndex::MOUSE_UP,
                        this,
                        evt);
            } else if (sid::interactive_object::get_focus() != nullptr) {
                sid::interactive_object::get_focus()->cancel_click(evt);
            }
            sid::interactive_object::set_clickowner(nullptr);
            break;
        case SDL_MOUSEMOTION:
            this->_mouse_move(evt);
            if (sid::interactive_object::get_mouseover() == this) {
                this->_mouse_move(evt);
                this->__m_EventCalls.call(
                        sid::interactive_object::eCallIndex::MOUSE_MOVE,
                        this,
                        evt);
            } else if (get_mouseover()) {
                this->_mouse_in(evt);
                this->__m_EventCalls.call(
                        sid::interactive_object::eCallIndex::MOUSE_IN,
                        this,
                        evt);

                get_mouseover()->_mouse_out(evt);
                get_mouseover()->__m_EventCalls.call(
                        sid::interactive_object::eCallIndex::MOUSE_OUT,
                        sid::interactive_object::get_mouseover(),
                        evt);

            }
            this->set_as_mouseover();
            break;
        case SDL_KEYDOWN:
            this->__m_EventCalls.call(
                    sid::interactive_object::eCallIndex::KEY_DOWN,
                    this,
                    evt);
            break;
        case SDL_KEYUP:
            this->__m_EventCalls.call(
                    sid::interactive_object::eCallIndex::KEY_UP,
                    this,
                    evt);
            break;
        case SDL_WINDOWEVENT:
            this->__m_EventCalls.call(
                    sid::interactive_object::eCallIndex::CONFIGURE_NOTIFY,
                    this,
                    evt);
            break;
        default:
            break;
    }
}