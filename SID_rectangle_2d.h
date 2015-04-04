/* 
 * File:   rectangle.h
 * Author: geth2k
 *
 * Created on 27 October 2013, 17:37
 */

#ifndef SID_RECTANGLE_H
#define	SID_RECTANGLE_H

#include <iostream>
#include <GL/gl.h>

#include "SID_globals.h"
#include "SID_tuplets.h"

namespace sid {

    class rectangle_2d {
    public:

        typedef GLubyte t_color_atom;
        typedef sid::quadruplet<t_color_atom> t_color;

        typedef GLfloat t_uv_atom;
        typedef sid::duplet<t_uv_atom> t_uv;
        typedef sid::quadruplet<t_uv> t_uv_set;

        typedef GLint t_position_atom;
        typedef sid::duplet<t_position_atom> t_position;
        typedef sid::quadruplet<t_position> t_position_set;

        enum corners {
            TopLeft,
            BottomLeft,
            BottomRight,
            TopRight
        };

        rectangle_2d();
        rectangle_2d(const rectangle_2d& orig);
        virtual ~rectangle_2d();

        rectangle_2d& operator=(const rectangle_2d& orig);

        void set_dimensions(double top, double left, double height, double width);

        rectangle_2d::t_position_set get_position_set() const {
            rectangle_2d::t_position_set PositionSet(
                    rectangle_2d::t_position(this->__m_posLeft, this->__m_posTop),
                    rectangle_2d::t_position(this->__m_posLeft, this->get_bottom()),
                    rectangle_2d::t_position(this->get_right(), this->get_bottom()),
                    rectangle_2d::t_position(this->get_right(), this->__m_posTop));

            return PositionSet;
        }

        inline void set_texture(uint texBufferID,
                rectangle_2d::t_uv_set const& uvSet) {

            this->__m_uintTexBufferID = texBufferID;
            this->set_texcoords(uvSet);
        }

        inline void set_texture(uint texBufferID) {
            this->__m_uintTexBufferID = texBufferID;
        }

        inline void set_texcoords(rectangle_2d::t_uv_set const& uvSet) {
            this->__m_TexCoords = uvSet;
        }

        inline rectangle_2d::t_uv_set const& get_texcoords() const {
            return __m_TexCoords;
        }

        inline uint get_texbufferid() const {
            return __m_uintTexBufferID;
        }

        inline void set_colour(rectangle_2d::t_color const& color) {
            this->__m_Color = color;
        }

        inline rectangle_2d::t_color get_color() const {
            return this->__m_Color;
        }

        inline rectangle_2d::t_position_atom get_width() const {
            return this->__m_posWidth;
        }

        inline rectangle_2d::t_position_atom get_height() const {
            return this->__m_posHeight;
        }

        inline rectangle_2d::t_position_atom get_left() const {
            return this->__m_posLeft;
        }

        inline rectangle_2d::t_position_atom get_top() const {
            return this->__m_posTop;
        }

        inline rectangle_2d::t_position_atom get_right() const {
            return this->__m_posLeft + this->__m_posWidth;
        }

        inline rectangle_2d::t_position_atom get_bottom() const {
            return this->__m_posTop + this->__m_posHeight;
        }

        inline bool test_click_position(int x, int y) const {
            if (x >= this->__m_posLeft && x <= this->get_right() &&
                    y >= this->__m_posTop && y <= this->get_bottom()) {
                return true;
            }
            return false;
        }

    private:
        t_position_atom __m_posTop;
        t_position_atom __m_posLeft;
        t_position_atom __m_posHeight;
        t_position_atom __m_posWidth;
        t_color __m_Color;
        t_uv_set __m_TexCoords;
        uint __m_uintTexBufferID;
    };
}
#endif	/* RECTANGLE_H */

