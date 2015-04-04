/* 
 * File:   SID_biomes.cpp
 * Author: geth2k
 * 
 * Created on July 6, 2014, 11:08 PM
 */

#include "SID_biomes.h"

sid::biomes::biomes() :
__m_Regions() {

}

sid::biomes::biomes(const biomes& orig) {
}

sid::biomes::~biomes() {
    for (biomes::t_biome_map_iter iter = this->__m_vBiomes.begin();
            iter != this->__m_vBiomes.end();
            ++iter) {
        delete iter->second;
    }
    
    this->__m_vBiomes.clear();
}

void sid::biomes::clear() {
    this->__m_vBiomes.clear();
    this->__m_Regions.clear();
}

sid::biome const* sid::biomes::get_biome(int x, int z) {
    int iBiomeID = this->__m_Regions.get_biome_id(x, z);
    
    if (iBiomeID >= 0) {
        return this->__m_vBiomes[iBiomeID];
    } else {
        return this->__m_vBiomes[1];
    }
}