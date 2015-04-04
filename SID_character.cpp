/* 
 * File:   character.cpp
 * Author: geth2k
 * 
 * Created on 17 November 2013, 17:20
 */

#include "SID_character.h"

sid::character::character() {
    control::t_color ColorForeground = control::t_color(0, 0, 0, 255);
    this->set_colour(ColorForeground);
}

sid::character::character(const character& orig) {
}

sid::character::~character() {
}

void sid::character::paint() {
    control* ctlParent = static_cast<control*>(this->get_parent());
    rectangle_2d* rectParent = nullptr;
    
    rectangle_2d::t_position_atom dblParentTop = 0;
    rectangle_2d::t_position_atom dblParentLeft = 0;
    rectangle_2d::t_position_atom dblParentMargin = 0;
    
    if (this->get_parent()) {
        dblParentMargin = ctlParent->get_margin();
        
        rectParent = ctlParent->get_screenrect();
        dblParentTop = rectParent->get_top();
        dblParentLeft = rectParent->get_left();
    }
    
    m_rectScreen.set_dimensions(
            this->get_top() + dblParentTop + dblParentMargin,
            this->get_left() + dblParentLeft + dblParentMargin,
            this->get_height(),
            this->get_width());
    
    this->m_rectScreen.set_colour(this->get_colour());
    
    this->m_rectScreen.set_texture(this->get_texture_ID());
    
    this->_foreach<control*>(
            [](control* ref, int loops, void* args) {
                ref->paint(); return true;
            });
}