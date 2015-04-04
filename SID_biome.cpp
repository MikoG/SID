/* 
 * File:   SID_biome.cpp
 * Author: geth2k
 * 
 * Created on July 5, 2014, 11:42 PM
 */

#include "SID_biome.h"

sid::biome::biome() :
__m_iIndex(0),
__m_HeightMap(nullptr),
__m_TreeMap(nullptr) {
    
}

sid::biome::biome(int index, height_map* height, height_map* trees) :
__m_iIndex(index),
__m_HeightMap(height),
__m_TreeMap(trees) {
}

sid::biome::biome(const biome& orig) :
__m_iIndex(orig.__m_iIndex),
__m_HeightMap(orig.__m_HeightMap),
__m_TreeMap(orig.__m_TreeMap),
__m_Color(orig.__m_Color) {
}

sid::biome::~biome() {
    if (this->__m_HeightMap) 
        delete this->__m_HeightMap;
    if (this->__m_TreeMap) 
        delete this->__m_TreeMap;
}

