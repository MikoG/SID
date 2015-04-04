/* 
 * File:   SID_biomes.h
 * Author: geth2k
 *
 * Created on July 6, 2014, 11:08 PM
 */

#ifndef SID_BIOMES_H
#define	SID_BIOMES_H

#include <map>
#include <iostream>

#include "SID_globals.h"
#include "SID_biome.h"
#include "SID_regions.h"

namespace sid {

    class biomes {
    public:
        
        typedef regions::region t_region;
        
        biomes();
        biomes(const biomes& orig);
        virtual ~biomes();
        
        void add_biome(biome* value) {
            this->__m_vBiomes[value->get_index()] = value;
        }
        
        biome const* operator[](uchar index) const {
            return this->__m_vBiomes.at(index);
        }
        
        biome const* get_biome(int x, int z);

        inline void set_seed(int seed) {
            this->__m_Regions.set_seed(seed);
        }

        inline int get_seed() const {
            return this->__m_Regions.get_seed();
        }
        
        void clear();
        
    private:
        
        typedef std::map<uchar, biome*> t_biome_map;
        typedef t_biome_map::iterator t_biome_map_iter;
        
        t_biome_map __m_vBiomes;
        
        regions __m_Regions;
        
        int __m_iSeed;
    };    

}



#endif	/* SID_BIOMES_H */
