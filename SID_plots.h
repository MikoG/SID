/* 
 * File:   plots.h
 * Author: geth2k
 *
 * Created on 06 May 2013, 23:10
 */

#ifndef SID_PLOTS_H
#define	SID_PLOTS_H

#include <algorithm>

#include "SID_object.h"
#include "SID_plot.h"
#include "SID_vector3.h"
#include "SID_select_3drect.h"

namespace sid {

    class plots : public object {
    public:

        static const int CHUNK_DIAM = 16;
        static const int CHUNK_CENTER = CHUNK_DIAM / 2;
        static const int CHUNK_VISIBLE_DIAM = CHUNK_DIAM - 1;
        static const int CHUNK_SIZE = CHUNK_DIAM * CHUNK_DIAM;

        typedef std::array<plot*, CHUNK_SIZE> t_plotarray;
        typedef t_plotarray::iterator t_plotarray_iter;
        typedef t_plotarray::const_iterator t_plotarray_const_iter;

        plots(object* objParent);
        plots(const plots& orig);
        virtual ~plots();

        virtual plots& operator=(const plots& orig);

        plot* at(uint index);
        plot* at(uint index_X, uint index_Z);
        plot* at(const plot::plot_offset& pofs);
        plot* first();
        plot* first() const;
        int find(plot* plt);
        int size();
        
        plot* get_plot_at(vertex3 vert);
        plot* get_plot_at(plot::plot_offset pofs);

        virtual void draw_select(int name, void* arg) {
            if ((name % CHUNK_DIAM) != (CHUNK_DIAM - 1) &&
                    name < (CHUNK_SIZE - CHUNK_DIAM)) {

                plot::plot_offset pofs0 = index_to_coords(name);
                plot::plot_offset pofs1 = pofs0 + plot::plot_offset(0, 1);
                plot::plot_offset pofs2 = pofs0 + plot::plot_offset(1, 1);
                plot::plot_offset pofs3 = pofs0 + plot::plot_offset(1, 0);

                vertex3 vtx0 = this->at(pofs0)->get_position();
                vertex3 vtx1 = this->at(pofs1)->get_position();
                vertex3 vtx2 = this->at(pofs2)->get_position();
                vertex3 vtx3 = this->at(pofs3)->get_position();

                rectangle_3d rect = rectangle_3d(vtx0, vtx1, vtx2, vtx3);

                this->_draw_select(name, &rect);
            }
        }

        static uint coords_to_index(uint iX, uint iZ);
        static uint coords_to_index(plot::plot_offset pofs);
        static plot::plot_offset index_to_coords(uint index);

        bool m_blHasWater;
        
    private:

        t_plotarray __m_plots;

        static uint __s_uiPlotSize;
        static uint __s_uiPlotDiam;
    };
}
#endif	/* PLOTS_H */

