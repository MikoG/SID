/*
 * File:   rectangle.cpp
 * Author: geth2k
 *
 * Created on 27 October 2013, 17:37
 */

#include "SID_rectangle_2d.h"

sid::rectangle_2d::rectangle_2d() : 
__m_Color(1.0, 0.0, 1.0, 1.0) {
}

sid::rectangle_2d::rectangle_2d(const rectangle_2d& orig) :
__m_Color(orig.__m_Color),
__m_posTop(orig.__m_posTop),
__m_posLeft(orig.__m_posLeft),
__m_posHeight(orig.__m_posHeight),
__m_posWidth(orig.__m_posWidth),
__m_uintTexBufferID(orig.__m_uintTexBufferID),
__m_TexCoords(orig.__m_TexCoords) {
    
}

sid::rectangle_2d::~rectangle_2d() {
}

sid::rectangle_2d& sid::rectangle_2d::operator =(const rectangle_2d& orig) {
    this->__m_posTop = orig.__m_posTop;
    this->__m_posLeft = orig.__m_posLeft;
    this->__m_posHeight = orig.__m_posHeight;
    this->__m_posWidth = orig.__m_posWidth;
    this->__m_uintTexBufferID = orig.__m_uintTexBufferID;
    this->__m_TexCoords = orig.__m_TexCoords;
}

void sid::rectangle_2d::set_dimensions(
        double top, double left, double height, double width) {
    
    this->__m_posTop = top;
    this->__m_posLeft = left;
    this->__m_posHeight = height;
    this->__m_posWidth = width;
}