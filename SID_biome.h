/* 
 * File:   SID_biome.h
 * Author: geth2k
 *
 * Created on July 5, 2014, 11:42 PM
 */

#ifndef SID_BIOME_H
#define	SID_BIOME_H

#include "SID_height_map.h"
#include "SID_plots.h"
#include "SID_biome_paths.h"

namespace sid {

    class biome {
    public:
        
        biome();
        biome(int index, height_map* height, height_map* trees);
        biome(const biome& orig);
        virtual ~biome();

        virtual float get_height(int x, int y) const = 0;

        template <typename _CheckType>
        bool check_height_over(int start_x, int end_x,
                int start_z, int end_z,
                _CheckType check) const {

            for (int iX = start_x; iX < end_x; iX++) {
                for (int iZ = start_z; iZ < end_z; iZ++) {
                    if (this->get_height(iX, iZ) > check) {
                        return true;
                    }
                }
            }
            
            return false;
        }
        
        template <typename _CheckType>
        bool check_height_under(int start_x, int end_x,
                int start_y, int end_z,
                _CheckType check) const {
            
            for (int iX = start_x; iX < end_x; iX++) {
                for (int iZ = start_y; iZ < end_z; iZ++) {
                    float fHeight = this->get_height(iX, iZ);
                    if (fHeight < check) {
                        return true;
                    }
                }
            }
            
            return false;
        }

        inline sid::t_color get_color() const {
            return __m_Color;
        }

        int get_index() const {
            return __m_iIndex;
        }      

    protected:

        int __m_iIndex;
        height_map* __m_HeightMap;
        height_map* __m_TreeMap;
        sid::t_color __m_Color;

    };
}
#endif	/* SID_BIOME_H */

