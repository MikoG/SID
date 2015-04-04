/* 
 * File:   height_map.h
 * Author: geth2k
 *
 * Created on 30 May 2013, 21:36
 */

#ifndef SID_HEIGHT_MAP_H
#define	SID_HEIGHT_MAP_H

#include <SDL2/SDL.h>

namespace sid {

    class height_map {
    public:
        height_map();
        height_map(const height_map& orig);
        virtual ~height_map();

        virtual float get_height(int x, int y) = 0;
        
    protected:
        Uint32 _m_uiSeed;
    private:

    };
}
#endif	/* HEIGHT_MAP_H */

