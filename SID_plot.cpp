/* 
 * File:   plot.cpp
 * Author: geth2k
 * 
 * Created on 12 May 2013, 11:52
 */

#include "SID_plot.h"

sid::plot::plot() :
object_ingame(1),
m_fHeight(0),
m_Structure(nullptr) {
    
    m_Data = 0;
    this->m_fHeight = 0;
}

sid::plot::plot(const plot& orig) : object_ingame(orig) {
    this->m_Data = orig.m_Data;
    this->m_fHeight = orig.m_fHeight;
}

sid::plot& sid::plot::operator =(const plot& orig) {
    this->m_Data = orig.m_Data;
    this->m_fHeight = orig.m_fHeight;
}

sid::plot::~plot() {
    if (this->m_Structure)
        delete this->m_Structure;
}

void sid::plot::set_damage(uint Value) {

}

uint sid::plot::get_damage() {

}

void sid::plot::set_direction(uint Value) {

}

uint sid::plot::get_direction() {

}

void sid::plot::set_height(float value) {
    vertex3 vtxTemp = this->get_position();
    vtxTemp[1] = value;
    this->set_position(vtxTemp);
    this->m_fHeight = value;
}

float sid::plot::get_height() {
    return this->m_fHeight;
}

void sid::plot::set_subtype(uint Value) {
    set_data_element(DATAMASK_STYPE, DATASHIFT_STYPE, Value);
}

uint sid::plot::get_subtype() {
    uint stype = get_data_element(DATAMASK_STYPE, DATASHIFT_STYPE);
    return stype;
}

void sid::plot::set_type(uint Value) {
    set_data_element(DATAMASK_TYPE, DATASHIFT_TYPE, Value);
}

uint sid::plot::get_type() {
    uint type = get_data_element(DATAMASK_STYPE, DATASHIFT_STYPE);
    return type;
}

void sid::plot::set_data_element(uint DataMask, uint DataShift, uint Value) {
    uint uiSetValue = Value << DataShift;
    uint uiCurrValue = m_Data & DataMask;
    uint uiResetValue = m_Data - uiCurrValue;
    m_Data = uiResetValue + uiSetValue;
}

uint sid::plot::get_data_element(uint DataMask, uint DataShift) {
    uint uiReturnValue = (this->m_Data & DataMask) >> DataShift;
    return uiReturnValue;
}

void sid::plot::set_structure(structure* Structure) {
    this->m_Structure = Structure;
}

sid::plot::plot_offset sid::plot::get_plot_offset() {
    vertex3 vtx = this->get_position();
    sid::plot::plot_offset pofs(vtx[0] / sid::plot::PLOT_WIDTH,
            vtx[2] / sid::plot::PLOT_WIDTH);

    return (pofs);
}