/* 
 * File:   character_strip.cpp
 * Author: geth2k
 * 
 * Created on 17 November 2013, 17:53
 */

#include "SID_character_strip.h"

sid::character_strip::character_strip() :
m_Color(0, 0, 0, 255) {
    this->set_height(22);
    this->set_margin(2);
    this->paint();
}

sid::character_strip::character_strip(const character_strip& orig) {
    this->m_fnt = orig.m_fnt;
}

sid::character_strip::~character_strip() {
}

void sid::character_strip::set_font(const font* fnt) {
    this->m_fnt = fnt;
}

const sid::font* sid::character_strip::get_font() const {
    return m_fnt;
}

void sid::character_strip::set_data(std::string strData) {
    this->_clear_children();
    control::set_data(strData);
    const int iStringLength = strData.length();
    const double dblBoxHeight = this->get_height();
    const double dblMarginSize = this->get_margin();
    const double dblBoxWidth = this->get_width();
    const double dblBoxInnerWidth = dblBoxWidth - dblMarginSize * 2;
    const double dblCharHeight = dblBoxHeight - dblMarginSize * 2;
    const double dblTexWidth = this->m_fnt->get_tex_width();
    const double dblTexHeight = this->m_fnt->get_tex_height();

    int iNextCharPos = 0;

    for (int i = 0; i < iStringLength; i++) {
        char chValue = strData[i];
        const font::glyph* glp = this->m_fnt->get_glyph(chValue);
        double dblSizeRatio = static_cast<double> (dblCharHeight) / glp->height;
        int iCharWidth = glp->width * dblSizeRatio;
        if ((iNextCharPos + iCharWidth) > dblBoxInnerWidth) {
            break;
        }

        character* chr = new character();
        std::string str = "Char";
        chr->set_parent(this);
        chr->set_name(str);
        chr->set_colour(this->get_text_colour());
        chr->set_texture_ID(this->m_fnt->get_texture_ID());
        chr->set_left(iNextCharPos);
        chr->set_width(iCharWidth);
        chr->set_height(dblCharHeight);

        double dblTop = glp->top / dblTexHeight;
        double dblLeft = glp->left / dblTexWidth;
        double dblBottom = (glp->top + glp->height) / dblTexHeight;
        double dblRight = (glp->left + glp->width) / dblTexWidth;

        rectangle_2d::t_uv txcTopLeft = rectangle_2d::t_uv(dblLeft, dblTop);
        rectangle_2d::t_uv txcBottomLeft = rectangle_2d::t_uv(dblLeft, dblBottom);
        rectangle_2d::t_uv txcBottomRight = rectangle_2d::t_uv(dblRight, dblBottom);
        rectangle_2d::t_uv txcTopRight = rectangle_2d::t_uv(dblRight, dblTop);
        rectangle_2d::t_uv_set tuvSet(txcTopLeft,
                txcBottomLeft, 
                txcBottomRight, 
                txcTopRight);
        
        chr->get_screenrect()->set_texcoords(tuvSet);
        this->add_child(chr);
        iNextCharPos += iCharWidth;
    }
    this->paint();
}

sid::control::t_color sid::character_strip::get_text_colour() const {
    return this->m_Color;
}

void sid::character_strip::set_text_colour(
        control::t_color const& color) {

    this->m_Color = color;

}
