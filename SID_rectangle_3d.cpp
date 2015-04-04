/* 
 * File:   rectangle_3d.cpp
 * Author: geth2k
 * 
 * Created on 02 November 2013, 16:04
 */

#include "SID_rectangle_3d.h"

sid::rectangle_3d::rectangle_3d() {
}

sid::rectangle_3d::rectangle_3d(vertex3& p0, vertex3& p1, vertex3& p2, vertex3& p3) : 
        m_vtxP0(p0), m_vtxP1(p1), m_vtxP2(p2), m_vtxP3(p3) {
    int i = 1;
}

sid::rectangle_3d::rectangle_3d(const rectangle_3d& orig) {
    this->m_vtxP0 = orig.m_vtxP0;
    this->m_vtxP1 = orig.m_vtxP1;
    this->m_vtxP2 = orig.m_vtxP2;
    this->m_vtxP3 = orig.m_vtxP3;
}

sid::rectangle_3d::~rectangle_3d() {
}

sid::vertex3 sid::rectangle_3d::get_vertex(rectangle_positions pos) const {
    switch (pos) {
        case sid::rectangle_3d::top_left:
            return this->m_vtxP0;
        case sid::rectangle_3d::bottom_left:
            return this->m_vtxP1;
        case sid::rectangle_3d::bottom_right:
            return this->m_vtxP2;
        case sid::rectangle_3d::top_right:
            return this->m_vtxP3;
        default:
            break;
    }
}

