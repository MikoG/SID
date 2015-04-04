/*
 * File:   render.h
 * Author: geth2k
 *
 * Created on 06 May 2013, 11:00
 */

#ifndef SID_RENDER_H
#define	SID_RENDER_H


#include <iostream>
#include <math.h>
#include <cmath>
#include <array>

#include <GL/gl.h>
#include <GL/glu.h>

#include "SID_tuplets.h"


namespace sid {
    
    void init_GL(GLdouble perspective, GLfloat drawDistance,
            GLfloat clearColorR, GLfloat clearColorG,
            GLfloat clearColorB, GLfloat clearColorA,
            GLint width, GLint height, GLfloat aspect);
    
    void resizeGL(int width, int height, float fov, float drawDistance);
    
    void init_frame_3d(float fov, float drawDistance);
    void set_fogging(float start, float end);
    
    void init_frame_2d();

    GLint get_screen_width();
    GLint get_screen_height();

    std::array<GLdouble, 3> get_2d_from_3d(double x, double y, double z);
    sid::triplet<GLdouble> get_3d_from_2d(int x, int y, double z);
    
}
#endif	/* RENDER_H */

