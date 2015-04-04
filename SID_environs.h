/* 
 * File:   environs.h
 * Author: geth2k
 *
 * Created on 09 June 2013, 00:18
 */

#ifndef SID_ENVIRONS_H
#define	SID_ENVIRONS_H

#include <map>
#include <array>
#include <climits>
#include <algorithm>

#include "SID_chunks.h"
#include "SID_timer.h"
#include "SID_camera.h"
#include "SID_frustum.h"
#include "SID_select_vbo.h"

namespace sid {

    class environs : public object {
    private:

        typedef chunk::chunkoffset __t_chunkoffset;
        typedef std::map<int, environs::__t_chunkoffset> __t_chunkoffsets;
        typedef environs::__t_chunkoffsets::iterator __t_chunkoffsets_iter;
        typedef environs::__t_chunkoffsets::const_iterator __t_chunkoffsets_const_iter;
        
    public:

        environs(uint gridWidth);
        environs(const environs& orig);
        virtual ~environs();

        inline void set_pause_status(bool status) {
            this->__m_blPauseStatus = status;
        }

        inline bool is_paused() const {
            return this->__m_blPauseStatus;
        }

        void build_environs(chunk::chunkoffset ofsChunkOffset);

        inline chunk* get_chunk(int index) {
            return (this->__m_vRenderOffsets.at(index).get_chunk());
        }

        void render_all(frustum const& frust, uint flags = 0);

        bool process_event(SDL_Event& evt, bool drag, bool recurse = true);

        void select_plots_terrain(plot* start, plot* end);

        void select_plots_build(int width, int length);

        void zoom_to_plot(double td, camera* cam);
        
        plot* get_plot_at(vertex3 vert);

        void get_surrounding_chunks(chunk* chnk, std::vector<chunk*>& vcnhnk);

        bool get_sel_area_grid(plot* plt, std::vector<plot*>& vplt, int width = 1, int length = 1);

        void set_mouse_mode(int iClickMode);

        inline int get_mouse_mode() const {
            return this->__m_iMouseMode;
        }

        void process_keyboard(SDL_Event& evt);

        virtual void tick() {
            this->__m_clkTick.update_timediff();
            for (; this->__m_clkTick.get_time() > 0.25;
                    this->__m_clkTick.adjust_time(-0.25))

                if (!this->__m_blPauseStatus)
                    object::tick();

        }

        inline void make_new_vbos() {
            this->__m_Chunks->make_new_vbos();
        }

        inline int get_chunk_cache_count() {
            return this->__m_Chunks->get_child_count();
        }
        
        chunk* get_cached_chunk(__t_chunkoffset cofs, bool WaitForReady = false);

        void draw_selection2() {
            this->__m_DrawSelectionDelegate->draw(&this->__m_vboSelection);
        }

        static chunk::chunkoffset get_chunk_offset(float PosX, float PosZ);
        static chunk::chunkoffset get_chunk_offset(vertex3 const& vtx);

    private:

        uint const __m_uiGridWidth;
        uint const __m_uiGridSize;

        bool __m_blPauseStatus;

        bool __m_blRenderReflections;

        chunks* __m_Chunks;

        environs::__t_chunkoffsets __m_vRenderOffsets;
        environs::__t_chunkoffset __m_ofsLastChunkOffset;

        void __render_opaque(frustum const& frust, uint flags = 0);
        void __render_transparent(frustum const& frust, uint flags = 0);
        void __populate_offset_list(chunk::chunkoffset ofsChunkOffset);
        bool __load_chunks_from_cache();
        void __load_chunks_from_new();
        bool __crossref_chunks_offsets(chunk* ref);

        sid::vbo __m_vboSelection;
        void __make_selection_vertices(std::vector<plot*>& vplt, int selWidth, int selLength);
        void __make_selection_indices(int selWidth, int selLength);
        void __make_selection_verts_array();

        void __coords_from_index(sid::vbo& elArr,
                GLuint index,
                sid::t_vertset& returnArray);

        static GLuint __triangle_point_to_index(int selWidth, int Strip, int Point, bool* Flip);

        plot* __m_pltDragStart;
        plot* __m_pltDragEnd;
        std::vector<plot*> __m_vpltSelections;

        int __m_iMouseMode;

        timer __m_clkTick;

        drawDelegate* __m_DrawSelectionDelegate;
    };

    bool sort_plot_rows(plot* i, plot* j);
}
#endif	/* ENVIRONS_H */

