/* 
 * File:   vbo.h
 * Author: geth2k
 *
 * Created on 22 May 2013, 23:58
 */

#ifndef VBO_H
#define	VBO_H

#define GL_GLEXT_PROTOTYPES
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>

#include "SID_tuplets.h"
#include "SID_textures.h"

namespace sid {

    typedef sid::triplet<GLfloat> t_vertset;
    typedef GLushort t_index;
    typedef sid::triplet<GLfloat> t_normal;
    typedef sid::duplet<GLfloat> t_uv;
    typedef sid::quadruplet<GLubyte> t_color;

    class vbo {
    public:

        vbo();
        vbo(const vbo& orig);
        virtual ~vbo();

        // --------------------------------------------------------------

        GLuint make_vbo(GLenum usage);

        inline GLuint get_vbo_id() const {
            return this->__m_gluiVBO;
        }

        inline bool is_vbo_complete() const {
            return this->__m_blVBOComplete;
        }

        void reserve(std::size_t size);

        void clear();

        void erase();

        // --------------------------------------------------------------

        inline void set_texture_id(GLuint id) {
            this->__m_gluiTexture = id;
        }

        inline GLuint get_texture_id() const {
            return this->__m_gluiTexture;
        }

        // --------------------------------------------------------------

        inline void push_vertset(t_vertset const& vs) {
            this->__m_vglfVertIndex.push_back(vs);
        }

        inline t_vertset const& get_vertset(int const index) const {
            return this->__m_vglfVertIndex[index];
        }

        inline int get_vertset_size() const {
            return this->__m_vglfVertIndex.size();
        }

        // --------------------------------------------------------------

        inline void push_index(t_index const index) {
            this->__m_vgluiIndices.push_back(index);
        }

        inline t_index const& get_index(int const index) const {
            return this->__m_vgluiIndices[index];
        }

        inline int get_indeces_size() const {
            return this->__m_vgluiIndices.size();
        }

        // --------------------------------------------------------------

        inline void push_arrayvert(t_vertset const& vs) {
            this->__m_vglfVertArray.push_back(vs);
        }

        inline t_vertset const& get_arrayvert(int const index) const {
            return this->__m_vglfVertArray[index];
        }

        inline int get_vertarray_size() const {
            return this->__m_vglfVertArray.size();
        }

        // --------------------------------------------------------------

        inline void push_normal(t_normal const& vs) {
            this->__m_vglfNormals.push_back(vs);
            this->__m_blUsesLighting = true;
        }
        
        inline t_normal const& get_normal(int const index) const {
            return this->__m_vglfNormals[index];
        }        

        inline int get_normals_size() const {
            return this->__m_vglfNormals.size();
        }

        // --------------------------------------------------------------

        inline void push_color(t_color const& cs) {
            this->__m_vglfColors.push_back(cs);
            this->__m_blSingleColor = false;
        }

        inline int get_colors_size() const {
            return this->__m_vglfColors.size();
        }
        
        inline t_color const& get_color(int const index) const {
            return this->__m_vglfColors[index];
        }

        // --------------------------------------------------------------

        inline void push_texset(t_uv const& ts) {
            this->__m_vglfTexCoords.push_back(ts);
            this->__m_blTextured = true;
        }

        inline int get_texcoords_size() const {
            return this->__m_vglfTexCoords.size();
        }
        
        inline t_uv const& get_texset(int const index) const {
            return this->__m_vglfTexCoords[index];
        }

        inline bool is_textured() const {
            return this->__m_blTextured;
        }

        // --------------------------------------------------------------

        inline void set_default_color(t_color const& cs) {
            this->__m_csDefault = cs;
        }

        inline t_color const& get_default_colorset() const {
            return this->__m_csDefault;
        }

        inline bool is_single_color() const {
            return this->__m_blSingleColor;
        }

        // --------------------------------------------------------------

        inline GLintptr get_vertarray_offset() const {
            return static_cast<GLintptr> (0);
        }

        inline GLintptr get_normals_offset() const {
            return static_cast<GLintptr> (this->get_vertarray_offset() +
                    (this->__m_vglfVertArray.size() *
                    sizeof (this->__m_vglfVertArray.front())));
        }

        inline GLintptr get_texcoords_offset() const {
            return static_cast<GLintptr> (this->get_normals_offset() +
                    (this->__m_vglfNormals.size() *
                    sizeof (this->__m_vglfNormals.front())));
        }

        inline GLintptr get_colors_offset() const {
            return static_cast<GLintptr> (this->get_texcoords_offset() + 
                    (this->__m_vglfTexCoords.size() * 
                    sizeof (this->__m_vglfTexCoords.front())));
        }

        // --------------------------------------------------------------

        inline t_vertset const& coords_from_index(GLuint index) const {
            return this->get_vertset(this->get_index(index));
        }
        
        // --------------------------------------------------------------
        
        inline void set_depth_buffer_usage(bool value) {
            this->__m_blUsesDepthBuffer = value;
        }
        
        inline bool get_depth_buffer_usage() const {
            return this->__m_blUsesDepthBuffer;
        }
        
        inline void set_lighting_usage(bool value) {
            this->__m_blUsesLighting = value;
        }
        
        inline bool get_lighting_usage() const {
            return this->__m_blUsesLighting;
        }
        
        // --------------------------------------------------------------
        
        inline void build_verts_array() {
            for (int iIndex = 0; iIndex < this->get_indeces_size(); iIndex++) {
                sid::t_vertset fvPoint;
                fvPoint = this->coords_from_index(iIndex);
                this->push_arrayvert(fvPoint);
            }
        }

    private:
        bool __m_blVBOComplete;
        GLuint __m_gluiVBO;

        GLuint __m_gluiTexture;

        std::vector<t_vertset> __m_vglfVertIndex;

        std::vector<t_index> __m_vgluiIndices;

        std::vector<t_vertset> __m_vglfVertArray;

        std::vector<t_normal> __m_vglfNormals;

        std::vector<t_color> __m_vglfColors;

        bool __m_blTextured;
        std::vector <t_uv> __m_vglfTexCoords;

        bool __m_blSingleColor;
        t_color __m_csDefault;

        bool __m_blUsesDepthBuffer;
        bool __m_blUsesLighting;

        // --------------------------------------------------------------

        inline GLsizeiptr __get_vertarray_memsize() const {
            return static_cast<GLsizeiptr> (
                    this->__m_vglfVertArray.size() *
                    sizeof (this->__m_vglfVertArray.front()));
        }

        inline GLsizeiptr __get_normals_memsize() const {
            return static_cast<GLsizeiptr> (
                    this->__m_vglfNormals.size() *
                    sizeof (this->__m_vglfNormals.front()));
        }

        inline GLsizeiptr __get_texcoords_memsize() const {
            return static_cast<GLsizeiptr> (
                    this->__m_vglfTexCoords.size() *
                    sizeof (this->__m_vglfTexCoords.front()));
        }

        inline GLsizeiptr __get_colors_memsize() const {
            return static_cast<GLsizeiptr> (
                    this->__m_vglfColors.size() *
                    sizeof (this->__m_vglfColors.front()));
        }

        inline GLsizeiptr __get_vbo_memsize() const {
            return static_cast<GLsizeiptr> (
                    this->__get_vertarray_memsize() +
                    this->__get_normals_memsize() +
                    this->__get_texcoords_memsize() +
                    this->__get_colors_memsize());
        }

    };

}
#endif	/* VBO_H */

