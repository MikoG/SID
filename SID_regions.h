/* 
 * File:   SID_regions.h
 * Author: geth2k
 *
 * Created on November 12, 2014, 7:31 PM
 */

#ifndef SID_REGIONS_H
#define	SID_REGIONS_H

#include "SID_biome_paths.h"

namespace sid {
    
    class regions {
    public:
        
        class region {
            
        public:
            
            typedef biome_paths t_biome_paths;

            region(int x, int z, int regionWidth, int seed);
            region(const region& orig);
            virtual ~region(){}

            inline int get_Z() const {
                return __m_iZ;
            }

            inline int get_X() const {
                return __m_iX;
            }

            inline bool operator== (region const& comp) const {
                return (__m_iX == comp.get_X() && __m_iZ == comp.get_Z());
            }
            
            static void set_biome_range(int min, int max);
            
            int get_biome_id(int x, int z) const;
            
        private:
            int __m_iX;
            int __m_iZ;
            const int __m_iRegionWidth;
            static int __s_iMinBiomeID;
            static int __s_iMaxBiomeID;
            t_biome_paths __m_Paths;
            int __m_iSeed;
        };            

        typedef std::vector<region> t_regions;
        typedef t_regions::iterator t_regions_iter;
        typedef t_regions::const_iterator t_regions_const_iter;

        regions();
        regions(regions const& orig);
        virtual ~regions();

        int get_biome_id(int x, int z);

        inline void set_seed(int seed) {
            this->__m_iSeed = seed;
        }

        inline int get_seed() const {
            return __m_iSeed;
        }
        
        void clear();

    private:
        
        region const& __get_region(int x, int z);
        
        const int __m_iRegionWidth;
        t_regions __m_vRegions;
        int __m_iSeed;
    };
}
#endif	/* SID_REGIONS_H */

