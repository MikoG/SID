/* 
 * File:   vbo.cpp
 * Author: geth2k
 * 
 * Created on 22 May 2013, 23:58
 */

#include "SID_vbo.h"

sid::vbo::vbo() :
__m_gluiVBO(0),
__m_gluiTexture(0),
__m_blVBOComplete(false),
__m_blSingleColor(true),
__m_blTextured(false),
__m_csDefault(1.0, 0.0, 1.0, 1.0),
__m_blUsesDepthBuffer(true),
__m_blUsesLighting(false) {

}

sid::vbo::vbo(const vbo& orig) :
__m_gluiVBO(0),
__m_gluiTexture(orig.__m_gluiTexture),
__m_blVBOComplete(false),
__m_blSingleColor(orig.__m_blSingleColor),
__m_blTextured(orig.__m_blTextured),
__m_csDefault(orig.__m_csDefault),
__m_vglfVertIndex(orig.__m_vglfVertIndex),
__m_vgluiIndices(orig.__m_vgluiIndices),
__m_vglfVertArray(orig.__m_vglfVertArray),
__m_vglfNormals(orig.__m_vglfNormals),
__m_vglfColors(orig.__m_vglfColors),
__m_vglfTexCoords(orig.__m_vglfTexCoords),
__m_blUsesDepthBuffer(orig.__m_blUsesDepthBuffer),
__m_blUsesLighting(orig.__m_blUsesLighting) {
}

sid::vbo::~vbo() {
    this->erase();
}

GLuint sid::vbo::make_vbo(GLenum usage) {
    if (__m_gluiVBO == 0)
        glGenBuffers(1, &__m_gluiVBO);

    glBindBuffer(GL_ARRAY_BUFFER, __m_gluiVBO);

    glBufferData(GL_ARRAY_BUFFER, this->__get_vbo_memsize(), nullptr, usage);

    // Buffer vertex data.
    glBufferSubData(GL_ARRAY_BUFFER,
            this->get_vertarray_offset(),
            this->__get_vertarray_memsize(),
            this->__m_vglfVertArray.data());

    // Buffer normals data.
    glBufferSubData(GL_ARRAY_BUFFER,
            this->get_normals_offset(),
            this->__get_normals_memsize(),
            this->__m_vglfNormals.data());

    // Buffer texture mapping data.
    glBufferSubData(GL_ARRAY_BUFFER,
            this->get_texcoords_offset(),
            this->__get_texcoords_memsize(),
            this->__m_vglfTexCoords.data());

    // Buffer color data.
    glBufferSubData(GL_ARRAY_BUFFER,
            this->get_colors_offset(),
            this->__get_colors_memsize(),
            this->__m_vglfColors.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->__m_blVBOComplete = true;
}

void sid::vbo::reserve(std::size_t size) {
    this->__m_vglfColors.reserve(size);
    this->__m_vglfNormals.reserve(size);
    this->__m_vglfTexCoords.reserve(size);
    this->__m_vglfVertArray.reserve(size);
    this->__m_vglfVertIndex.reserve(size);
}

void sid::vbo::clear() {
    this->__m_vglfColors.clear();
    this->__m_vglfNormals.clear();
    this->__m_vglfTexCoords.clear();
    this->__m_vglfVertArray.clear();
    this->__m_vglfVertIndex.clear();
    this->__m_vgluiIndices.clear();
    __m_blVBOComplete = false;
}

void sid::vbo::erase() {
    this->clear();
    this->__m_gluiTexture = 0;
    if (glIsBuffer(this->__m_gluiVBO)) {
        glDeleteBuffers(1, &this->__m_gluiVBO);
        this->__m_gluiVBO = 0;
    }
}