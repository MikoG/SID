/*
 * File:   control.h
 * Author: geth2k
 *
 * Created on 27 October 2013, 01:44
 */

#ifndef SID_CONTROL_H
#define	SID_CONTROL_H

#include <cstdlib>
#include <string>
#include <limits.h>

#include "SID_object.h"
#include "SID_interactive_object.h"
#include "SID_render.h"
#include "SID_rectangle_2d.h"
#include "SID_fonts.h"
#include "SID_textures.h"
#include "SID_layout.h"
#include "SID_draw_control.h"

namespace sid {

    class control : public interactive_object {
    public:

        typedef rectangle_2d::t_color t_color;

        enum eColourPositions {
            red = 0,
            green = 1,
            blue = 2,
            alpha = 3
        };

        enum eHJustifyTypes {
            left,
            right,
            center
        };

        enum eVJustifyTypes {
            top,
            bottom,
            middle
        };

        control();
        control(object::t_object_data setFlags);
        control(const control& orig);
        virtual ~control();

        void add_child(object* child);

        void set_front_flag(bool status);
        bool get_front_flag() const;

        virtual void paint();

        virtual void apply_layout(int loops);

        rectangle_2d* get_screenrect();

        virtual void render_this(uint flags);

        virtual void set_data(std::string strData);
        virtual std::string get_data() const;

        virtual bool _test_event(
                SDL_Event& evt,
                int* successLevel,
                int maxRecurse = INT_MAX);

        void set_layout(layout* layout);
        layout* get_layout() const;

        inline static void add_font(std::string path,
                uint fontID,
                uint texID) {

            control::_s_Fonts.add_font(path, fontID, texID);
        }

        void set_stretch_mode(int iStretchMode) {
            this->__m_DisplayParams.m_iStretchMode = iStretchMode;
        }

        int get_stretch_mode() const {
            return this->__m_DisplayParams.m_iStretchMode;
        }

        void set_hjustify_mode(
                sid::control::eHJustifyTypes JustifyMode) {

            this->__m_DisplayParams.m_iHJustifyMode = JustifyMode;
        }

        int get_hjustify_mode() const {
            return this->__m_DisplayParams.m_iHJustifyMode;
        }

        void set_vjustify_mode(
                sid::control::eVJustifyTypes JustifyMode) {

            this->__m_DisplayParams.m_iVJustifyMode = JustifyMode;
        }

        int get_vjustify_mode() const {
            return this->__m_DisplayParams.m_iVJustifyMode;
        }

        void set_margin(double margin) {
            this->__m_DisplayParams.m_iMargin = margin;
        }

        double get_margin() const {
            return this->__m_DisplayParams.m_iMargin;
        }

        void set_texture_ID(uint texID) {
            this->__m_DisplayParams.m_uiTexID = texID;
        }

        uint get_texture_ID() const {
            return this->__m_DisplayParams.m_uiTexID;
        }

        int get_ctrlhandle() {
            return this->__m_DisplayParams.m_iCtrlHandle;
        }

        t_color get_colour() const {
            return this->__m_DisplayParams.m_Color;
        }

        void set_colour(t_color const& color) {
            this->__m_DisplayParams.m_Color = color;
        }

        void set_height(double height) {
            this->__m_DisplayParams.m_iHeight = height;
        }

        int get_height() const {
            return this->__m_DisplayParams.m_iHeight;
        }

        void set_width(int width) {
            this->__m_DisplayParams.m_iWidth = width;
        }

        int get_width() const {
            return this->__m_DisplayParams.m_iWidth;
        }

        void set_top(int top) {
            this->__m_DisplayParams.m_iTop = top;
        }

        int get_top() const {
            return this->__m_DisplayParams.m_iTop;
        }

        void set_left(int left) {
            this->__m_DisplayParams.m_iLeft = left;
        }

        int get_left() const {
            return this->__m_DisplayParams.m_iLeft;
        }

    protected:

        static fonts _s_Fonts;

        rectangle_2d m_rectScreen;

        virtual void setup_ui() {
        }

    private:
        struct __display_parameters {
            __display_parameters();
            __display_parameters(__display_parameters const& orig);
            virtual ~__display_parameters();

            int m_iCtrlHandle;

            int m_iTop;
            int m_iLeft;
            int m_iHeight;
            int m_iWidth;

            t_color m_Color;

            int m_iMargin;

            int m_iHJustifyMode;
            int m_iVJustifyMode;

            int m_iStretchMode;

            uint m_uiTexID;
        } __m_DisplayParams;
        
        std::string m_strData;
        layout* m_layout;
    };
}
#endif	/* CONTROL_H */
