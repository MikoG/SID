/* 
 * File:   render.cpp
 * Author: geth2k
 * 
 * Created on 06 May 2013, 11:00
 */

#include "SID_render.h"

void sid::init_GL(GLdouble fov, GLfloat drawDistance,
        GLfloat clearColorR, GLfloat clearColorG,
        GLfloat clearColorB, GLfloat clearColorA,
        GLint width, GLint height, GLfloat aspect) {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    glDisable(GL_MULTISAMPLE);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // values for fogging
    //    GLfloat density = 0.075;
    //    GLfloat fogColor[4] = {0.0, 0.0, 0.0, 1.0};
    //    glEnable (GL_FOG);    
    //    glFogi (GL_FOG_MODE, GL_LINEAR);
    //    glFogi (GL_FOG_HINT, GL_NICEST);
    //    glFogfv (GL_FOG_COLOR, fogColor);
    //    glFogf (GL_FOG_DENSITY, density);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, .1, drawDistance);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(clearColorR, clearColorG, clearColorB, clearColorA);

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //    glDisable (GL_TEXTURE_2D);
    //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);    

}

void sid::resizeGL(int width, int height, float fov, float drawDistance) {
    glViewport(0, 0, width, height);
    float glfAspectRatio = width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, glfAspectRatio,
            .1, drawDistance);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void sid::init_frame_3d(float fov, float drawDistance) {
    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClearStencil(0x0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    GLfloat glfAspect = viewport[2] / static_cast<GLfloat> (viewport[3]);
    gluPerspective(fov, glfAspect, 0.1, drawDistance);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    GLfloat diffuseLight2[] = {0.5, 0.5, 0.55, 1.0};
    GLfloat specularLight2[] = {0.2, 0.2, 0.2, 1.0};
    //GLfloat emission2[] = {0.4, 0.4, 0.4, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight2);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight2);
    //glLightfv(GL_LIGHT0, GL_EMISSION, emission2);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void sid::set_fogging(float start, float fadeDistance) {
    glFogf(GL_FOG_START, start);
    glFogf(GL_FOG_END, start + fadeDistance);
}

/*
 *
 */
void sid::init_frame_2d() {
    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());

    GLdouble dblWindowWidth = viewport[2];
    GLdouble dblWindowHeight = viewport[3];

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, dblWindowWidth, dblWindowHeight, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPolygonMode(GL_FRONT, GL_FILL);
}

std::array<GLdouble, 3> sid::get_2d_from_3d(double x, double y, double z) {
    std::array<GLdouble, 16> projection;
    std::array<GLdouble, 16> modelview;
    std::array<GLint, 4> viewport;
    std::array<GLdouble, 3> screen_coords = {0, 0, 0};

    glGetDoublev(GL_PROJECTION_MATRIX, projection.data());
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview.data());
    glGetIntegerv(GL_VIEWPORT, viewport.data());

    gluProject(x, y, z, modelview.data(), projection.data(), viewport.data(),
            screen_coords.data(),
            screen_coords.data() + 1,
            screen_coords.data() + 2);
    screen_coords[1] = viewport[3] - screen_coords[1];

    return screen_coords;
}

sid::triplet<GLdouble> sid::get_3d_from_2d(int winX, int winY, double objY) {
    std::array<GLdouble, 16> projection;
    std::array<GLdouble, 16> modelview;
    std::array<GLint, 4> viewport;
    sid::triplet<GLdouble> object_coordsN;
    sid::triplet<GLdouble> object_coordsF;
    sid::triplet<GLdouble> object_coords;
    
    glGetDoublev(GL_PROJECTION_MATRIX, projection.data());
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview.data());
    glGetIntegerv(GL_VIEWPORT, viewport.data());

    double dX = (double)winX;
    double dY = (double)viewport[3] - (double) winY;
    
    double dZ = 0;
    gluUnProject(dX, dY, dZ, modelview.data(), projection.data(), viewport.data(),
            object_coordsN.data(),
            object_coordsN.data() + 1,
            object_coordsN.data() + 2);

    dZ = 1;
    gluUnProject(dX, dY, dZ, modelview.data(), projection.data(), viewport.data(),
            object_coordsF.data(),
            object_coordsF.data() + 1,
            object_coordsF.data() + 2);
    
    double dYTarget = objY;
    double dXRange = object_coordsN[0] - object_coordsF[0];
    double dYRange = object_coordsN[1] - object_coordsF[1];
    double dZRange = object_coordsN[2] - object_coordsF[2];
    double dYTargetDeltaN = object_coordsN[1] - dYTarget;
    double dDeltaRatioN = dYTargetDeltaN / dYRange;
    
    sid::triplet<GLdouble> object_coordsOffset(dXRange * dDeltaRatioN, 
            dYRange * dDeltaRatioN, 
            dZRange * dDeltaRatioN);
    
    object_coords = object_coordsN - object_coordsOffset;
    
    return object_coords;
}

GLint sid::get_screen_width() {
    //std::array<GLint, 4> viewport;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    return viewport[2];
}

GLint sid::get_screen_height() {
    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, viewport.data());

    return viewport[3];
}