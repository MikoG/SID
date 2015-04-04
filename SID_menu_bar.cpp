/* 
 * File:   menu_bar.cpp
 * Author: geth2k
 * 
 * Created on 26 November 2013, 20:59
 */

#include "SID_menu_bar.h"

sid::menu_bar::menu_bar() : m_dblButtonSpacing(3) {
    this->set_stretch_mode(1);
}

sid::menu_bar::menu_bar(const menu_bar& orig) : 
control(orig) {
}

sid::menu_bar::~menu_bar() {
}

void sid::menu_bar::add_child(object* child) {
    control* ctlNew = static_cast<control*>(child);
    ctlNew->set_parent(this);
    if (this->get_child_count() > 0) {
        control* ctlLast = static_cast<control*>(this->_get_child_back());
        double dblLeft = ctlLast->get_left();
        double dblWidth = ctlLast->get_width();

        ctlNew->set_left(dblLeft + dblWidth + m_dblButtonSpacing);
    }
    control::add_child(ctlNew);
}

void sid::menu_bar::set_button_spacing(double dblButtonSpacing) {
    this->m_dblButtonSpacing = dblButtonSpacing;
}

double sid::menu_bar::get_button_spacing() const {
    return m_dblButtonSpacing;
}



