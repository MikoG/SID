/* 
 * File:   SID_biome_paths.cpp
 * Author: geth2k
 * 
 * Created on November 12, 2014, 6:06 PM
 */

#include "SID_biome_paths.h"
#include "SID_biome.h"

sid::biome_paths::biome_paths() {
    this->__m_vPaths.reserve(MAX_PATHS);
}

sid::biome_paths::biome_paths(
        const biome_paths& orig) :
__m_vPaths(orig.__m_vPaths) {

}

sid::biome_paths::~biome_paths() {

}

void sid::biome_paths::add_paths(
        int quant,
        int x,
        int z,
        int regionWidth,
        int minBiomeID,
        int maxBiomeID,
        int seed) {

    for (int i = 0; i < quant; ++i) {
        
        biome_path pathTemp(i, x, z, regionWidth, minBiomeID, maxBiomeID, seed);
        this->__m_vPaths.push_back(pathTemp);
    }
}

int sid::biome_paths::get_biome_id(int x, int z) const {
    int iRetVal(-3);
    for (t_paths_const_iter iter = this->__m_vPaths.begin();
            iter != this->__m_vPaths.end();
            ++iter) {
        
        int iTempVal = iter->get_biome_id(x, z);
        if (iTempVal >= 0) {
            iRetVal = iTempVal;
        }
    }
    
    return iRetVal;
}

sid::biome_paths::biome_path::biome_path(
        int pos,
        int regionX,
        int regionZ,
        int regionWidth,
        int minBiomeID,
        int maxBiomeID,
        int seed) :
__m_iPosition(pos) {

    int iPathSeed = pos ^ 0xFF;
    iPathSeed = (regionX + iPathSeed) ^ ((regionZ + iPathSeed) << 1);
    iPathSeed = (iPathSeed << 8 + pos) ^ seed;
    iPathSeed >>= 8;

    int iPermissibleWidth = regionWidth - biome_bubble::MAX_RADIUS * 2;

    int iPathX = (iPathSeed ^ seed) % iPermissibleWidth +
            biome_bubble::MAX_RADIUS + regionX;

    int iPathZ = (std::abs(iPathX) ^ iPathSeed) % iPermissibleWidth +
            biome_bubble::MAX_RADIUS + regionZ;

    int iBiomeID;
    int iBiomeRange = maxBiomeID - minBiomeID;
    if (iBiomeRange != 0) {
        iBiomeID = iPathSeed % iBiomeRange + minBiomeID;
    } else {
        iBiomeID = minBiomeID;
    }

    int iBubbleCount = (iPathSeed & 0x7FFFFFFF) % 7 + 1;

    std::cout << "Path Seed: " << pos << ": " << iPathSeed <<
            " X " << iPathX << " Z " << iPathZ << std::endl <<
            "BiomeID " << iBiomeID << std::endl <<
            "Bubble Count " << iBubbleCount << std::endl << std::endl;

    int iBubbleRadius1 = std::abs((iPathX ^ iPathZ) % 
            biome_bubble::RADIUS_RANGE +
            biome_bubble::MIN_RADIUS);
    
//    std::cout << "Bubble Seed 0: " << 0 <<
//            " X " << iPathX << " Z " << iPathZ << 
//            " R " << iBubbleRadius1 << std::endl;    

    biome_bubble bblTemp1(iPathX, iPathZ, iBubbleRadius1, iBiomeID);
    this->__M_vBubbles.push_back(bblTemp1);

    for (int i = 1; i < iBubbleCount; ++i) {
        int iBubbleSeed = (i << 7) ^ iPathSeed;

        int iMaxX = regionX + regionWidth - biome_bubble::MAX_RADIUS;
        int iMaxZ = regionZ + regionWidth - biome_bubble::MAX_RADIUS;

        int iBubbleX = ((iBubbleSeed ^ seed) %
                (DEVIATION_RANGE * 2)) -
                DEVIATION_RANGE;

        if (iBubbleX != 0) {
            iBubbleX += MIN_DEVIATION *
                    (iBubbleX / std::abs(iBubbleX));
        } else {
            iBubbleX = MIN_DEVIATION;
        }

        iBubbleX += iPathX;

        if (iBubbleX > iMaxX) {
            iBubbleX = iMaxX;
        } else if (iBubbleX < regionX + biome_bubble::MAX_RADIUS) {
            iBubbleX = regionX + biome_bubble::MAX_RADIUS;
        }

        int iBubbleZ = ((iBubbleSeed ^ std::abs(iBubbleX - regionX)) %
                (DEVIATION_RANGE * 2)) -
                DEVIATION_RANGE;

        if (iBubbleZ != 0) {
            iBubbleZ += MIN_DEVIATION *
                    (iBubbleZ / std::abs(iBubbleZ));
        } else {
            iBubbleZ = MIN_DEVIATION;
        }

        iBubbleZ += iPathZ;

        if (iBubbleZ > iMaxZ) {
            iBubbleZ = iMaxZ;
        } else if (iBubbleZ < regionZ + biome_bubble::MAX_RADIUS) {
            iBubbleZ = regionZ + biome_bubble::MAX_RADIUS;
        }
        
        int iBubbleRadius2 = (i ^ std::abs(iBubbleX ^ iBubbleZ)) % 
                biome_bubble::RADIUS_RANGE + 
                biome_bubble::MIN_RADIUS;

//        std::cout << "Bubble Seed " << i << ": " << iBubbleSeed <<
//                " X " << iBubbleX << " Z " << iBubbleZ << 
//                " R " << iBubbleRadius2 << std::endl;
        
        biome_bubble bblTemp2(iBubbleX, iBubbleZ, iBubbleRadius2, iBiomeID);
        this->__M_vBubbles.push_back(bblTemp2);
    }
    std::cout << std::endl;
}

sid::biome_paths::biome_path::biome_path(
        const biome_path& orig) :
__m_iPosition(orig.__m_iPosition),
__M_vBubbles(orig.__M_vBubbles) {
}

sid::biome_paths::biome_path::biome_path(
        biome_path&& orig) :
__m_iPosition(std::move(orig.__m_iPosition)),
__M_vBubbles(std::move(orig.__M_vBubbles)) {
}

sid::biome_paths::biome_path::~biome_path() {
    this->__M_vBubbles.clear();
}

int sid::biome_paths::biome_path::get_biome_id(int x, int z) const {
    int iRetVal = -2;
    for (t_bubbles_const_iter iter = this->__M_vBubbles.begin();
            iter != this->__M_vBubbles.end();
            ++iter) {
        
        if (iter->is_hit(x, z)) {
            iRetVal = iter->get_biome_id();
        } else {
            if (iRetVal == -2) {
                iRetVal = -1;
            }
        }
    }
    
    return iRetVal;
}

sid::biome_paths::biome_path::biome_bubble::biome_bubble(
        int x,
        int z,
        int radius,
        int biomeID) :
__m_iX(x),
__m_iZ(z),
__m_iRadius(radius),
__m_iBiomeID(biomeID) {

}

sid::biome_paths::biome_path::biome_bubble::biome_bubble(
        const biome_bubble& orig) :
__m_iRadius(orig.__m_iRadius),
__m_iX(orig.__m_iX),
__m_iZ(orig.__m_iZ),
__m_iBiomeID(orig.__m_iBiomeID) {

}

sid::biome_paths::biome_path::biome_bubble::~biome_bubble() {

}

bool sid::biome_paths::biome_path::biome_bubble::is_hit(int x, int z) const {
    int iRadiusSquared = this->__m_iRadius * this->__m_iRadius;
    
    int iDeltaX = x - this->__m_iX;
    int iDeltaZ = z - this->__m_iZ;
    
    int iDeltaSquared = iDeltaX * iDeltaX + iDeltaZ * iDeltaZ;
    
    return (iDeltaSquared <= iRadiusSquared);
    
}

int sid::biome_paths::biome_path::biome_bubble::get_biome_id() const {
    return this->__m_iBiomeID;
}