/* 
 * File:   InteractiveObjet.h
 * Author: geth2k
 *
 * Created on June 24, 2014, 8:09 PM
 */

#ifndef SID_INTERACTIVE_OBJET_H
#define	SID_INTERACTIVE_OBJET_H

#include <limits.h>
#include "SID_object.h"
#include "SID_event_calls.h"

namespace sid {

    class interactive_object : public object {
        typedef event_calls<object*> __t_event_calls;
        static interactive_object* __s_objFocus;
        static interactive_object* __s_objMouseOver;
        static interactive_object* __s_objClickOwner;

    public:

        enum eCallIndex {
            MOUSE_UP,
            MOUSE_DOWN,
            MOUSE_MOVE,
            MOUSE_IN,
            MOUSE_OUT,
            KEY_UP,
            KEY_DOWN,
            CONFIGURE_NOTIFY,
        };

        interactive_object();
        interactive_object(object::t_object_data setFlags);
        interactive_object(const interactive_object& orig);
        virtual ~interactive_object();

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------    

        inline void set_mouse_up_event(__t_event_calls::t_event_call func) {
            this->__m_EventCalls.add_call(interactive_object::eCallIndex::MOUSE_UP, func);
        }

        inline void set_mouse_down_event(__t_event_calls::t_event_call func) {
            this->__m_EventCalls.add_call(interactive_object::eCallIndex::MOUSE_DOWN, func);
        }

        inline void set_mouse_in_event(__t_event_calls::t_event_call func) {
            this->__m_EventCalls.add_call(interactive_object::eCallIndex::MOUSE_IN, func);
        }

        inline void set_mouse_out_event(__t_event_calls::t_event_call func) {
            this->__m_EventCalls.add_call(interactive_object::eCallIndex::MOUSE_OUT, func);
        }

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------

        inline static void set_clickowner(interactive_object* obj) {
            interactive_object::__s_objClickOwner = obj;
        }

        inline static interactive_object* get_clickowner() {
            return interactive_object::__s_objClickOwner;
        }

        inline void set_as_clickowner() {
            interactive_object::__s_objClickOwner = this;
        }

        inline bool is_clickowner() const {
            return interactive_object::__s_objClickOwner == this;
        }

        inline static void set_focus(interactive_object* obj) {
            interactive_object::__s_objFocus = obj;
        }

        inline static interactive_object* get_focus() {
            return interactive_object::__s_objFocus;
        }

        inline void set_as_focus() {
            interactive_object::__s_objFocus = this;
        }

        inline bool is_focus() const {
            return interactive_object::__s_objFocus == this;
        }

        inline static void set_mouseover(interactive_object* obj) {
            interactive_object::__s_objMouseOver = obj;
        }

        inline static interactive_object* get_mouseover() {
            return interactive_object::__s_objMouseOver;
        }

        inline void set_as_mouseover() {
            interactive_object::__s_objMouseOver = this;
        }

        inline bool is_mouseover() const {
            return interactive_object::__s_objMouseOver == this;
        }

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------

        virtual void cancel_click(SDL_Event& evt) {

        }

    protected:

        virtual bool _test_event(
                SDL_Event& evt,
                int* successLevel,
                int max_recurse = INT_MAX) {
            return false;
        }

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------

        virtual bool _process_event(SDL_Event& evt);

        virtual void _mouse_down(SDL_Event& evt) {
        }

        virtual void _mouse_up(SDL_Event& evt) {
        }

        virtual void _mouse_move(SDL_Event& evt) {
        }

        virtual void _mouse_in(SDL_Event& evt) {
        }

        virtual void _mouse_out(SDL_Event& evt) {
        }

    private:
        __t_event_calls __m_EventCalls;

    };
}
#endif	/* INTERACTIVE_OBJET_H */
