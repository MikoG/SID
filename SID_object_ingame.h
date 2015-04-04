/* 
 * File:   object_ingame.h
 * Author: geth2k
 *
 * Created on 01 November 2013, 19:45
 */

#ifndef SID_OBJECT_INGAME_H
#define	SID_OBJECT_INGAME_H

#include "SID_interactive_object.h"
#include "SID_object_template.h"
#include "SID_rectangle_3d.h"
#include "SID_vbo.h"
#include "SID_object_templates.h"

namespace sid {

    class object_ingame : public interactive_object {
    public:
        object_ingame();
        object_ingame(Uint32 setFlags);
        object_ingame(const object_ingame& orig);
        virtual ~object_ingame();

        void set_position(vertex3 const& vtxPosition);
        vertex3& get_position();

        virtual void render_this(uint flags = 0);

        virtual bool process_event(SDL_Event& evt, bool recurse = true) {
            
        }

        // --------------------------------------------------------------
        //
        // --------------------------------------------------------------   

        inline virtual sid::vbo const* get_opaque_vbo() const {
            return this->m_vboOpaque;
        }

        inline virtual sid::vbo const* get_transparent_vbo() const {
            return this->m_vboTransparent;
        }

        void set_zoom_to(bool blZoomTo);
        bool is_zoom_to() const;
        static void set_zoom_object(object_ingame* s_objZoomTo);
        static object_ingame* get_zoom_object();
        void set_as_zoom_object();
        
        // --------------------------------------------------------------
        //
        // -------------------------------------------------------------- 
        
        inline static void add_template(uint pos, sid::sidof const* Sidof) {
            _s_Templates.add_template(pos, Sidof);
        }
        
        inline static void clear_templates() {
            _s_Templates.clear_templates();
        }
        
    protected:

        static sid::object_templates _s_Templates;
        sid::vbo* m_vboOpaque;
        sid::vbo* m_vboTransparent;

    private:
        
        vertex3 m_vtxPosition;
        bool m_blZoomTo;

        static object_ingame* s_objZoomTo;
        

    };
}
#endif	/* OBJECT_INGAME_H */

