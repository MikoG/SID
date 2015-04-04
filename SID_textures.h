/* 
 * File:   textures.h
 * Author: geth2k
 *
 * Created on 04 July 2013, 23:30
 */

#ifndef SID_TEXTURES_H
#define	SID_TEXTURES_H

#include <vector>

#include "SID_texture.h"

namespace sid {

    class textures {
    public:
        textures();
        textures(const textures& orig);
        virtual ~textures();


        void add_texture(std::string strPath, uint gameRef, uint flags);

        void erase_all();

        GLuint get_tex_id_at(uint index);
        GLuint get_tex_id_by_ref(uint gameRef);

        inline texture const& operator[](std::size_t const index) const {
            return this->__m_vTexture[index];
        }

    private:

        std::vector <texture>__m_vTexture;

    };
}
#endif	/* TEXTURES_H */

