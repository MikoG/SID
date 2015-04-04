/* 
 * File:   selectDelegate.cpp
 * Author: geth2k
 * 
 * Created on 24 May 2014, 17:44
 */

#include <cstdlib>

#include "SID_select_delegate.h"

std::array<GLuint, BUFFER_SIZE> sid::select_delegate::__s_selectBuf;

void sid::select_delegate::init_selframe_3d(int x, int y, 
        float fov, float drawDistance) {
    
    glPushMatrix();

    std::array<GLint, 4> viewport;

    glSelectBuffer(BUFFER_SIZE, __s_selectBuf.data());
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glGetIntegerv(GL_VIEWPORT,viewport.data());
    gluPickMatrix(x, viewport[3]-y, 5, 5, viewport.data());
    GLfloat glfAspect = viewport[2] / static_cast<float>(viewport[3]);
    gluPerspective(fov, glfAspect, .1, drawDistance);
    glMatrixMode(GL_MODELVIEW);
    glInitNames();    
}

int sid::select_delegate::process_selhits() {
    // restoring the original projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // returning to normal rendering mode
    int hits = glRenderMode(GL_RENDER);   

    // Process the hits
    if (hits > 0) {
        unsigned int i;
        GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

        ptr = (GLuint *) __s_selectBuf.data();
        minZ = 0xffffffff;
        for (i = 0; i < hits; i++) {	
            names = *ptr;
            ptr++;
            if (*ptr < minZ) {
                numberOfNames = names;
                minZ = *ptr;
                ptrNames = ptr+2;
            }
            ptr += names+2;
        }
        ptr = ptrNames;
        return (*ptr);
    }
    return (-1);      
}
