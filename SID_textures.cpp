/* 
 * File:   textures.cpp
 * Author: geth2k
 * 
 * Created on 04 July 2013, 23:30
 */

#include "SID_textures.h"

sid::textures::textures() { 

}

sid::textures::textures(const textures& orig) {
}

sid::textures::~textures() {
    this->erase_all();
}

void sid::textures::add_texture(std::string strPath, uint gameRef, uint flags) {
    texture tex;
    tex.loadTexture(strPath, gameRef, flags);
    __m_vTexture.push_back(tex);
}

void sid::textures::erase_all() {
    for (std::vector<texture>::iterator it1 = this->__m_vTexture.begin(); 
            it1 != this->__m_vTexture.end(); it1++) {
        
        (*it1).remove_texture();
    }
    this->__m_vTexture.clear();
}

GLuint sid::textures::get_tex_id_at(uint index) {
    return (__m_vTexture[index].get_tex_id());
}

GLuint sid::textures::get_tex_id_by_ref(uint gameRef) {
    for (std::vector<texture>::iterator it1 = this->__m_vTexture.begin(); 
            it1 != this->__m_vTexture.end(); it1++) {
        
        if ((*it1).get_game_ref() == gameRef) {
            return (*it1).get_tex_id();
        }
    }
    return 0;
}