/* 
 * File:   drawControl.cpp
 * Author: geth2k
 * 
 * Created on 24 May 2014, 16:50
 */

#include "SID_draw_control.h"
#include "SID_control.h"

sid::drawControl::drawControl() {
}

sid::drawControl::drawControl(const drawControl& orig) {
}

sid::drawControl::~drawControl() {
}

void sid::drawControl::draw(void const* arg) {
    rectangle_2d const* rect = static_cast<rectangle_2d const*> (arg);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rect->get_texbufferid());
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    rectangle_2d::t_uv_set tuvSet(rect->get_texcoords());
    rectangle_2d::t_position_set posSet(rect->get_position_set());
    glColor4ubv(rect->get_color().data());
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2fv(tuvSet[rectangle_2d::corners::TopLeft].data());
        glVertex2iv(posSet[rectangle_2d::corners::TopLeft].data());

        glTexCoord2fv(tuvSet[rectangle_2d::corners::BottomLeft].data());
        glVertex2iv(posSet[rectangle_2d::corners::BottomLeft].data());

        glTexCoord2fv(tuvSet[rectangle_2d::corners::TopRight].data());
        glVertex2iv(posSet[rectangle_2d::corners::TopRight].data());

        glTexCoord2fv(tuvSet[rectangle_2d::corners::BottomRight].data());
        glVertex2iv(posSet[rectangle_2d::corners::BottomRight].data());
    glEnd();
}