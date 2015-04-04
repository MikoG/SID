/* 
 * File:   selectElementArray.cpp
 * Author: geth2k
 * 
 * Created on 24 May 2014, 18:34
 */

#include "SID_select_vbo.h"

void sid::select_vbo::draw_select(int name, void const* arg) {
    sid::vbo const* VBO = static_cast<sid::vbo const*>(arg);
    
    if (!VBO->is_vbo_complete()) {
        return;
    }
    glPushName(name);
    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, VBO->get_vbo_id());
    
    glVertexPointer(3, GL_FLOAT, 0, (void*)VBO->get_vertarray_offset());
    glDrawArrays(GL_TRIANGLES, 0, VBO->get_vertarray_size());
    
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopName();
}
