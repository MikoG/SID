/* 
 * File:   texture.h
 * Author: geth2k
 *
 * Created on 04 July 2013, 23:04
 */

#ifndef SID_TEXTURE_H
#define	SID_TEXTURE_H

#define GL_GLEXT_PROTOTYPES

#include <iostream>
#include <string>

#include <GL/gl.h>
#include <SDL2/SDL_image.h>

#include "SID_file.h"
#include "SID_globals.h"

namespace sid {

    class texture {
    public:

        enum eTexFlags {
            SID_TEX_FLAG_MULTIPLY_ALPHA = 1,
            SID_TEX_FLAG_GEN_MIPMAPS = 2,
        };

        texture();
        texture(const texture& orig);
        virtual ~texture();

        void loadTexture(std::string path, uint gameRef, uint flags);
        void remove_texture();

        inline int get_height() const {
            return __m_iHeight;
        }

        inline int get_width() const {
            return __m_iWidth;
        }

        inline uint get_game_ref() const {
            return __m_uiGameRef;
        }

        inline GLuint get_tex_id() const {
            return __m_gluiTexID;
        }

    private:
        bool _loadTexture(std::string strPath, uint flags);
        
        GLuint __m_gluiTexID;
        uint __m_uiGameRef;
        
        int __m_iWidth, __m_iHeight;

    };
}
#endif	/* TEXTURE_H */

