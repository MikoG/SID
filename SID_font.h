/* 
 * File:   font.h
 * Author: geth2k
 *
 * Created on 17 November 2013, 17:58
 */

#ifndef FONT_H
#define	FONT_H

#include <algorithm>
#include <stdint.h>
#include <vector>

#include "SID_textures.h"
#include "SID_file.h"
#include "SID_tuplets.h"

namespace sid {

    class font {
    public:

        struct glyph {
            int top;
            int left;
            int height;
            int width;
        };

        font();

        font(std::string path, uint fontID, uint texID);

        font(const font& orig);

        virtual ~font();

        inline const glyph* get_glyph(int index) const {
            return &this->__m_vFontMap[index];
        }

        inline uint get_texture_ID() const {
            return this->__m_gluiTexID;
        };

        inline int get_tex_height() const {
            return __m_iTexHeight;
        }

        inline int get_tex_width() const {
            return __m_iTexWidth;
        }

    private:
        void __setup_font(std::string path, uint fontID, uint texID);

        std::vector<glyph> __m_vFontMap;

        uint __m_gluiTexID;
        int __m_iTexWidth;
        int __m_iTexHeight;

    };
}
#endif	/* FONT_H */

