/*
 * File:   chunk.h
 * Author: geth2k
 *
 * Created on 18 April 2013, 20:09
 */

#ifndef SID_CHUNK_H
#define	SID_CHUNK_H

#include <iostream>
#include <float.h>
#include <math.h>

#include <GL/gl.h>

#include "SID_biomes.h"
#include "SID_plots.h"
#include "SID_vector3.h"
#include "SID_vbo.h"
#include "SID_object_ingame.h"
#include "SID_rectangle_3d.h"
#include "SID_draw_vbo.h"
#include "SID_select_3drect.h"

namespace sid {

    class chunk : public object_ingame {
    public:

        class chunkoffset {
        public:

            chunkoffset() :
            __m_iX(0),
            __m_iZ(0),
            __m_Init(false),
            __m_Chunk(nullptr) {

            }

            chunkoffset(int x, int z) :
            __m_iX(x),
            __m_iZ(z),
            __m_Init(false),
            __m_Chunk(nullptr) {

            }

            chunkoffset(chunkoffset const& orig) :
            __m_iX(orig.__m_iX),
            __m_iZ(orig.__m_iZ),
            __m_Init(orig.__m_Init),
            __m_Chunk(orig.__m_Chunk) {

            }

            virtual ~chunkoffset() {

            }

            void set_chunk(chunk* chnk) {
                if (chnk) {
                    this->__m_Chunk = chnk;
                    this->__m_Init = true;
                }
            }

            inline chunk* get_chunk() const {
                return this->__m_Chunk;
            }

            inline bool is_init() const {
                return this->__m_Init;
            }

            inline int get_x() const {
                return this->__m_iX;
            }

            inline int get_z() const {
                return this->__m_iZ;
            }

            chunkoffset& operator=(chunkoffset const& orig) {
                this->__m_iX = orig.__m_iX;
                this->__m_iZ = orig.__m_iZ;

                return *this;
            }

            chunkoffset& operator-=(chunkoffset const& orig) {
                this->__m_iX -= orig.__m_iX;
                this->__m_iZ -= orig.__m_iZ;

                return *this;
            }

            chunkoffset& operator+=(chunkoffset const& orig) {
                this->__m_iX += orig.__m_iX;
                this->__m_iZ += orig.__m_iZ;

                return *this;
            }

            bool operator==(chunkoffset check) const {
                return (__m_iX == check.__m_iX && __m_iZ == check.__m_iZ);
            }

            bool operator!=(chunkoffset check) const {
                return (__m_iX != check.__m_iX || __m_iZ != check.__m_iZ);
            }

        private:

            int __m_iX;
            int __m_iZ;

            bool __m_Init;
            chunk* __m_Chunk;

        };

        chunk& operator[](int idx);

        static const int TRI_POINTS = 3;
        static const int VERT_DIMS = 3;
        static const int PIX_BYTES = 4;
        static const uint CHUNK_TIMEOUT = 64;
        
        // @TODO Encapsulate s_Biomes.
        static sid::biomes s_Biomes;

        chunk(int chunkOffsetX, int chunkOffsetZ);

        chunk(const chunk& orig);
        virtual ~chunk();

        virtual chunk& operator=(const chunk& orig) {
        }

        void reset_chunk();
        void set_chunk();

        plots* get_plots();

        chunkoffset get_offset();

        virtual void tick();
        void set_rectangle(rectangle_3d* rect3d);
        rectangle_3d get_rectangle() const;

        virtual bool process_event(SDL_Event& evt, bool recurse = true);
        bool is_ready() const;
        void set_unready();

        bool build_chunk() {
            if (this->m_vboOpaque) {
                this->m_vboOpaque->clear();
                this->set_unready();
            }

            if (this->m_vboTransparent) {
                this->m_vboTransparent->clear();
                this->set_unready();
            }

            if (!this->is_ready()) {
                this->set_chunk();
                float fOffsetHeight = this->get_plots()->at(0)->get_height();
                chunk::chunkoffset ofs = this->get_offset();
                vertex3 vtxPosition(ofs.get_x() * 4, fOffsetHeight, ofs.get_z() * 4);
                this->set_position(vtxPosition);
                this->m_blReady = true;
                return true;
            }
            return false;
        }

        bool make_new_vbo() {
            if (this->is_ready()) {
                bool blRetVal(false);
                if (!this->m_vboOpaque->is_vbo_complete()) {
                    this->m_vboOpaque->make_vbo(GL_DYNAMIC_DRAW);
                    blRetVal = true;
                }
                if (!this->m_vboTransparent->is_vbo_complete()) {
                    this->m_vboTransparent->make_vbo(GL_DYNAMIC_DRAW);
                    blRetVal = true;
                }
                return blRetVal;
            }
            return false;
        }

    protected:


    private:

        chunkoffset m_offset;

        plots* m_Plots;

        rectangle_3d* m_rect3d;

        void make_land_vertices();
        void make_land_indices();
        void make_land_verts_array();
        void make_land_normals_array();
        void make_land_colors_array();
        void make_land_tex_coords();

        void make_sea_vertices();
        void make_sea_indices();
        void make_sea_verts_array();
        void make_sea_normals_array();
        void make_sea_colors_array();

        static sid::t_index triangle_point_to_index(int Strip, int Point, bool* Flip);

        void coords_from_index(sid::vbo& VBO,
                GLuint index,
                sid::triplet<GLfloat>& returnArray);
        
        float __get_fade_ratio(int value) const;
        float __get_compound_ratio(
                int x, int z, std::array<bool, 8> const& boolArray) const;

        bool m_blReady;

    };

    inline chunk::chunkoffset operator+(chunk::chunkoffset lhs,
            chunk::chunkoffset const& rhs) {

        return lhs += rhs;
    }

    inline chunk::chunkoffset operator-(chunk::chunkoffset lhs,
            chunk::chunkoffset const& rhs) {

        return lhs -= rhs;
    }
}
#endif	/* CHUNK_H */