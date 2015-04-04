/* 
 * File:   layout.h
 * Author: geth2k
 *
 * Created on 11 December 2013, 18:28
 */

#ifndef SID_LAYOUT_H
#define	SID_LAYOUT_H

#include <vector>

#include "SID_globals.h"

namespace sid {

    class layout {
    public:
        layout(int height, std::vector<int>& cellWidths, int padding);
        layout(const layout& orig);
        virtual ~layout();

        void set_cols(uint cols);
        uint get_cols() const;

        int get_col_width(int colID) const;
        void set_cell_height(int iCellHeight);
        int get_cell_height() const;

        int get_padding() const;

    private:
        uint m_uiCols;
        uint m_uiRows;
        int m_iCellHeight;
        int __m_iPadding;
        std::vector<int> m_viCellWidths;
    };
}
#endif	/* LAYOUT_H */