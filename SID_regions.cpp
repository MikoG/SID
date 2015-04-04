/* 
 * File:   SID_regions.cpp
 * Author: geth2k
 * 
 * Created on November 12, 2014, 7:31 PM
 */

#include <cmath>

#include "SID_regions.h"

int sid::regions::region::__s_iMinBiomeID = 0;
int sid::regions::region::__s_iMaxBiomeID = 0;

sid::regions::regions() :
__m_iRegionWidth(1020) {
    
    region::set_biome_range(2, 2);
}

sid::regions::regions(regions const& orig) :
__m_iRegionWidth(orig.__m_iRegionWidth),
__m_vRegions(orig.__m_vRegions) {

}

sid::regions::~regions() {
    this->__m_vRegions.clear();
}

int sid::regions::get_biome_id(int x, int z) {
    int iRetVal(-4);
    const region rgnTemp = this->__get_region(x, z);
    iRetVal = rgnTemp.get_biome_id(x, z);
    return iRetVal;
}

sid::regions::region const& sid::regions::__get_region(int x, int z) {
    int iTempX = std::floor(x / static_cast<float>(this->__m_iRegionWidth)) * 
            this->__m_iRegionWidth;
    
    int iTempZ = std::floor(z / static_cast<float>(this->__m_iRegionWidth)) * 
            this->__m_iRegionWidth;
    
    for (regions::t_regions_const_iter iter = this->__m_vRegions.begin();
            iter != this->__m_vRegions.end();
            ++iter) {
        
        if (iTempX == iter->get_X() && 
                iTempZ == iter->get_Z()) {
            
            return *iter;
        }
    }
    
    region rgnTemp(iTempX, iTempZ, this->__m_iRegionWidth, this->__m_iSeed);
    this->__m_vRegions.push_back(rgnTemp);
    return this->__m_vRegions.back();
}

void sid::regions::clear() {
    this->__m_vRegions.clear();
}

sid::regions::region::region(int x, int z, int regionWidth, int seed) :
__m_iX(x),
__m_iZ(z),
__m_iRegionWidth(regionWidth),
__m_iSeed(seed) {
    
    std::cout << "Making Region at " << __m_iX << ", " << __m_iZ << 
            " with seed " << __m_iSeed << std::endl;
    
    int iPaths = std::abs((this->__m_iSeed >> 16) ^ this->__m_iX ^ this->__m_iZ);
    iPaths = iPaths % biome_paths::PATHS_RANGE;
    iPaths += biome_paths::MIN_PATHS;
    
    std::cout << "Creating " << iPaths << " Biome paths." << std::endl;
    
    this->__m_Paths.add_paths(iPaths, x, z, regionWidth, 
            __s_iMinBiomeID, __s_iMaxBiomeID, seed);
}

sid::regions::region::region(const region& orig) :
__m_iX(orig.__m_iX),
__m_iZ(orig.__m_iZ),
__m_iRegionWidth(orig.__m_iRegionWidth),
__m_Paths(orig.__m_Paths) {
    
}

int sid::regions::region::get_biome_id(int x, int z) const {
    return this->__m_Paths.get_biome_id(x, z);
}

void sid::regions::region::set_biome_range(int min, int max) {
    //TODO: Check that min is less than or equal to max.
    region::__s_iMinBiomeID = min;
    region::__s_iMaxBiomeID = max;
}