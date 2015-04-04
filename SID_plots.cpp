/* 
 * File:   plots.cpp
 * Author: geth2k
 * 
 * Created on 06 May 2013, 23:10
 */

#include "SID_plots.h"

uint sid::plots::__s_uiPlotDiam = 15;
uint sid::plots::__s_uiPlotSize = 225;

sid::plots::plots(object* objParent) :
m_blHasWater(false) {
    
    set_parent(objParent);
    for (int i=0; i< CHUNK_SIZE; i++) {
        __m_plots[i] = new plot();
        std::string str = "Plot";
        //__m_plots[i]->set_name(str);
        __m_plots[i]->set_parent(this);
    }
    this->set_select_delegate(new select_3Drect());
}

sid::plots::plots(const plots& orig) : object(orig) {
    for (int i=0; i < CHUNK_SIZE; i++) {
        this->__m_plots[i] = orig.__m_plots[i];
    }
}

sid::plots& sid::plots::operator =(const plots& orig) {
    for (int i=0; i < CHUNK_SIZE; ++i) {
        this->__m_plots[i] = orig.__m_plots[i];
    }
}

sid::plots::~plots() {
    for (sid::plots::t_plotarray_iter iter = this->__m_plots.begin(); 
            iter != this->__m_plots.end();
            iter++) {
        delete *iter;
    }
}

sid::plot* sid::plots::at(uint index) {
    return (__m_plots[index]);
}

sid::plot* sid::plots::at(const plot::plot_offset& pofs) {
    return (__m_plots[coords_to_index(pofs.get_x(), pofs.get_z())]);
}

sid::plot* sid::plots::at(uint index_X, uint index_Z) {
    return (__m_plots[coords_to_index(index_X, index_Z)]);
}

sid::plot* sid::plots::first() {
    return __m_plots.front();
}

sid::plot* sid::plots::first() const {
    return __m_plots.front();
}

int sid::plots::find(plot* plt) {
    for (int i = 0; i < this->__m_plots.size(); i++) {
        if (this->__m_plots[i] == plt) {
           return i; 
        }
    }
}

sid::plot* sid::plots::get_plot_at(vertex3 vert) {
    plot::plot_offset pofsVert(vert.data()[0] / plot::PLOT_WIDTH,
            vert.data()[2] / plot::PLOT_WIDTH);
  
    return this->get_plot_at(pofsVert);
}

sid::plot* sid::plots::get_plot_at(plot::plot_offset pofs) {
    plot* pltFirst = this->first();
    plot::plot_offset pofsFirst = pltFirst->get_plot_offset();
    plot::plot_offset pofsDelta = pofs - pofsFirst;
    plot* pltRetVal = this->__m_plots[coords_to_index(pofsDelta)];

    return pltRetVal;
}

int sid::plots::size() {
    return (CHUNK_SIZE);
}

uint sid::plots::coords_to_index(uint iX, uint iZ) {
    return (iX + (iZ * CHUNK_DIAM));
}

uint sid::plots::coords_to_index(plot::plot_offset pofs) {
    return (pofs.get_x() + (pofs.get_z() * CHUNK_DIAM));
}

sid::plot::plot_offset sid::plots::index_to_coords(uint index) {
    plot::plot_offset pofsRet(index % CHUNK_DIAM, index / CHUNK_DIAM);

    return pofsRet;
}