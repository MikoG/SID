/* 
 * File:   select3DRect.cpp
 * Author: geth2k
 * 
 * Created on 24 May 2014, 17:54
 */

#include "SID_select_3drect.h"

void sid::select_3Drect::draw_select(int name, void const* arg) {
    rectangle_3d const* rect = static_cast<rectangle_3d const*>(arg);
    vertex3 v0, v1, v2, v3;
    v0 = rect->get_vertex(rectangle_3d::top_left);
    v1 = rect->get_vertex(rectangle_3d::bottom_left);
    v2 = rect->get_vertex(rectangle_3d::bottom_right);
    v3 = rect->get_vertex(rectangle_3d::top_right);

    glPushName(name);
    glBegin(GL_TRIANGLES);
        glVertex3fv(v0.data());
        glVertex3fv(v1.data());
        glVertex3fv(v2.data());
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex3fv(v2.data());
        glVertex3fv(v3.data()); 
        glVertex3fv(v0.data());
    glEnd();
    glPopName();
}