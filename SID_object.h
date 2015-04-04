/*
 * File:   object.h
 * Author: geth2k
 *
 * Created on 14 April 2013, 18:49
 */

#ifndef SID_OBJECT_H
#define	SID_OBJECT_H

#include <map>
#include <limits.h>
#include "SID_child.h"
#include "SID_bits.h"
#include "SID_event_calls.h"
#include "SID_textures.h"
#include "SID_render.h"
#include "SID_audio.h"
#include "SID_draw_delegate.h"
#include "SID_select_delegate.h"

namespace sid {

    class object : public child<object*> {
    public:

        typedef Uint32 t_object_data;

        static const t_object_data ENABLED;
        static const t_object_data VISIBLE;
        static const t_object_data DELETE;
        static const t_object_data RECURSE_EVENTS;
        static const t_object_data FRONT;
        static const t_object_data KILL_TIMER;

        // @Depracated The type of drawing needs to determined by Data and ???

        enum draw_flags {
            TRANSPARENT = 0x01,
            NO_TEXTURE = 0x04
        };

        object();
        object(t_object_data setFlags);
        object(const object& orig);
        virtual ~object();

        object* get_child(std::string name);

        virtual object& operator=(const object& orig) {
        }

        inline void set_name(std::string Name) {
            __m_strName = Name;
        }

        inline std::string get_name() const {
            return (this->__m_strName);
        }

        virtual void tick();

        virtual void render_all(int maxRecurse, uint flags);

        virtual void render_this(uint flags) {
        }

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------

        void set_visible(bool const value = true);
        bool is_visible() const;

        void set_enabled(bool const value = true);
        bool get_enabled() const;

        bool is_deletable() const;
        bool is_frontflag_set() const;

        void reset_kill_timer();
        t_object_data inc_kill_timer(t_object_data const incValue = 1);

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------    

        inline void set_draw_delegate(drawDelegate* delegate) {
            if (this->_m_Draw) delete this->_m_Draw;
            this->_m_Draw = delegate;
        }

        // @TODO Move set_select_delegate into SID_interactive_object.
        inline void set_select_delegate(select_delegate* delegate) {
            if (this->_m_Select) delete this->_m_Select;
            this->_m_Select = delegate;
        }

        // @TODO Move draw_select into SID_interactive_object.
        inline virtual void draw_select(int name, void const* arg) {
            this->_m_Select->draw_select(name, arg);
        }

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------

        inline static bool add_sound(std::string fileName, uint const id) {
            return object::_s_Audio.add_sound(fileName, id);
        }

        inline static bool add_song(std::string fileName, uint const id) {
            return object::_s_Audio.add_song(fileName, id);
        }

        inline static bool play_sound(int const id) {
            return object::_s_Audio.play_sound(id);
        }

        inline static bool play_song(int const id) {
            return object::_s_Audio.play_song(id);
        }

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------    

        inline static void add_texture(std::string name, uint gameRef, uint flags) {
            object::_s_Textures.add_texture(name, gameRef, flags);
        }

        inline static void clean_up_textures() {
            object::_s_Textures.erase_all();
        }


    protected:

        bits<t_object_data> _m_Data;

        static audio _s_Audio;
        static textures _s_Textures;

        inline void _draw(void const* arg) {
            this->_m_Draw->draw(arg);
        }

        inline void _draw_select(int name, void const* arg) {
            this->_m_Select->draw_select(name, arg);
        }

    private:
        drawDelegate* _m_Draw;
        select_delegate* _m_Select;

        std::string __m_strName;
        
    };

    bool compare_objects(object* obj1, object* obj2);

    void dummy_event(object* sender, SDL_Event& evt);
}
#endif	/* OBJECT_H */