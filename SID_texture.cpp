/* 
 * File:   texture.cpp
 * Author: geth2k
 * 
 * Created on 04 July 2013, 23:04
 */

#include "SID_texture.h"

sid::texture::texture() :
__m_gluiTexID(0),
__m_uiGameRef(0),
__m_iWidth(0),
__m_iHeight(0) {
}

sid::texture::texture(const texture& orig) :
__m_gluiTexID(orig.__m_gluiTexID),
__m_uiGameRef(orig.__m_uiGameRef),
__m_iWidth(orig.__m_iWidth),
__m_iHeight(orig.__m_iHeight) {
}

sid::texture::~texture() {

}

bool sid::texture::_loadTexture(std::string path, uint flags) {
    // Check that the file exists and throw an exception if not.
    sid::file::file_exists(path);

        // Check that file extension is for a PNG file and throw an 
    // exception if not.
    std::string strExt;
    strExt = path.substr(path.size() - 3, 3);

    // Load the file into an SDL_surface object.
    // This should be enhanced with an appropriate exception but
    // need to investigate the possible SDL errors first.
    SDL_Surface* Surface = IMG_Load(path.c_str());
    if (Surface == NULL) {
        printf("Error: \"%s\"\n", SDL_GetError());
        return false;
    }

    // Get the image dimensions for future use.
    this->__m_iWidth = Surface->w;
    this->__m_iHeight = Surface->h;

    // Determine if the image has an alpha channel and set format and
    // internalFormat accordingly ready for passing to glTexImage2D.
    GLint gliInternalFormat;
    GLint gliFormat;
    if (Surface->format->Amask == 0) {
        gliFormat = GL_RGB;
        gliInternalFormat = GL_RGB8;
    } else {
        gliFormat = GL_RGBA;
        gliInternalFormat = GL_RGBA8;

        // if requested, use alpha pre-multiplying.
        Uint8 BytesPerPixel = Surface->format->BytesPerPixel;
        if (flags & texture::eTexFlags::SID_TEX_FLAG_MULTIPLY_ALPHA && 
                BytesPerPixel == 4) {
            
            int PixelsSize = BytesPerPixel * Surface->w * Surface->h;
            uchar* Pixels = static_cast<uchar*> (Surface->pixels);
            for (int i = 0; i < PixelsSize; i += 4) {
                Pixels[i + 0] = (Pixels[i + 0] * Pixels[i + 3] + 128) >> 8;
                Pixels[i + 1] = (Pixels[i + 1] * Pixels[i + 3] + 128) >> 8;
                Pixels[i + 2] = (Pixels[i + 2] * Pixels[i + 3] + 128) >> 8;
            }
        }
    }

    // Create the texture in the OpenGL context.
    glGenTextures(1, &__m_gluiTexID);
    glBindTexture(GL_TEXTURE_2D, __m_gluiTexID);
    glTexImage2D(GL_TEXTURE_2D,
            0,
            gliInternalFormat,
            Surface->w,
            Surface->h,
            0, gliFormat,
            GL_UNSIGNED_BYTE,
            Surface->pixels);

    // Generate mipmaps if requested and set appropriate min and mag filters.
    // May need to implement client side mipmap generator based on the
    // OpenGL implementation being used.
    if (flags & SID_TEX_FLAG_GEN_MIPMAPS) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_NEAREST);

        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_MAG_FILTER,
                GL_NEAREST);

    } else {
        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_MAG_FILTER,
                GL_LINEAR);
    }

    // Set clamping parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Delete the SDL Surface object as the data is now stored in the
    // OpenGL texture.
    SDL_FreeSurface(Surface);

    return true;
}

void sid::texture::loadTexture(std::string path, uint gameRef, uint flags) {
    _loadTexture(path, flags);

    this->__m_uiGameRef = gameRef;
}

void sid::texture::remove_texture() {
    const GLuint gluiTexID = this->__m_gluiTexID;
    glDeleteTextures(1, &gluiTexID);
}