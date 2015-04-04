/* 
 * File:   drawChunk.cpp
 * Author: geth2k
 * 
 * Created on 24 May 2014, 16:01
 */

#include "SID_draw_vbo.h"

void sid::drawVBO::draw(void const* arg) {
    if (!arg) {
        return;
    }
    sid::vbo const* VBO = static_cast<sid::vbo const*>(arg);
    if (!VBO->is_vbo_complete()) {
        return;
    }
    glPushMatrix();
    
    if (!VBO->get_depth_buffer_usage()) {
        glDisable(GL_DEPTH_TEST);
    }
    
    if (!VBO->get_lighting_usage()) {
        glDisable(GL_LIGHTING);
    }    
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    if (!VBO->is_single_color()) {
        glEnableClientState(GL_COLOR_ARRAY);
    }
    if (VBO->is_textured()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    } 
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO->get_vbo_id());
    
    glVertexPointer(3, GL_FLOAT, 0, (void*)VBO->get_vertarray_offset());
    glNormalPointer(GL_FLOAT, 0, (void*)VBO->get_normals_offset());
    if (VBO->is_textured()) {
        glEnable (GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, VBO->get_texture_id());
        glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)VBO->get_texcoords_offset());
    } else {
        glDisable (GL_TEXTURE_2D);
    }
    if (VBO->is_single_color()) {
        glColor4ubv(&VBO->get_default_colorset()[0]);
    } else {
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, (void*)VBO->get_colors_offset());
    }
    glDrawArrays(GL_TRIANGLES, 0, VBO->get_vertarray_size());
    
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    if (VBO->is_textured()) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    if (!VBO->is_single_color()) {
        glDisableClientState(GL_COLOR_ARRAY);
    }
    
    if (!VBO->get_lighting_usage()) {
        glEnable(GL_LIGHTING);
    }
    
    if (!VBO->get_depth_buffer_usage()) {
        glEnable(GL_DEPTH_TEST);
    }
    
    glPopMatrix();    
        
}