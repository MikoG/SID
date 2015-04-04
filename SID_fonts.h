/* 
 * File:   fonts.h
 * Author: geth2k
 *
 * Created on 17 November 2013, 18:25
 */

#ifndef SID_FONTS_H
#define	SID_FONTS_H

#include <vector>
#include "SID_font.h"
#include "SID_textures.h"

namespace sid {

    class fonts {
    public:
        fonts();
        fonts(const fonts& orig);
        virtual ~fonts();

        inline void add_font(std::string path, uint fontID, uint texID) {
            this->m_fonts.push_back(font(path, fontID, texID));
        }

        const font* get_font(int index) const;

    private:
        std::vector<font> m_fonts;
    };
}
#endif	/* FONTS_H */

