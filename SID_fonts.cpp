/* 
 * File:   fonts.cpp
 * Author: geth2k
 * 
 * Created on 17 November 2013, 18:25
 */

#include "SID_fonts.h"

sid::fonts::fonts() {
}

sid::fonts::fonts(const fonts& orig) {
    this->m_fonts = orig.m_fonts;
}

sid::fonts::~fonts() {
}

const sid::font* sid::fonts::get_font(int index) const {
    return &this->m_fonts.data()[index];
}