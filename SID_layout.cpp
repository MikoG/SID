/* 
 * File:   layout.cpp
 * Author: geth2k
 * 
 * Created on 11 December 2013, 18:28
 */

#include "SID_layout.h"

sid::layout::layout(int height, std::vector<int>& cellWidths, int padding) :
m_uiCols(cellWidths.size()),
m_iCellHeight(height),
__m_iPadding(padding) {
    this->m_viCellWidths = cellWidths;
}

sid::layout::layout(const layout& orig) :
m_uiCols(orig.m_uiCols),
m_uiRows(orig.m_uiRows),
m_iCellHeight(orig.m_iCellHeight),
__m_iPadding(orig.__m_iPadding) {

}

sid::layout::~layout() {
}

void sid::layout::set_cols(uint cols) {
    this->m_uiCols = cols;
}

uint sid::layout::get_cols() const {
    return m_uiCols;
}

int sid::layout::get_col_width(int colID) const {
    return this->m_viCellWidths[colID];
}

void sid::layout::set_cell_height(int iCellHeight) {
    this->m_iCellHeight = iCellHeight;
}

int sid::layout::get_cell_height() const {
    return m_iCellHeight;
}

int sid::layout::get_padding() const {
    return this->__m_iPadding;
}


