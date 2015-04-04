/* 
 * File:   structure.h
 * Author: geth2k
 *
 * Created on 24 May 2014, 14:53
 */

#ifndef SID_STRUCTURE_H
#define	SID_STRUCTURE_H

#include "SID_object_ingame.h"

namespace sid {

    class structure : public object_ingame {
    public:
        structure();
        structure(uint sidofid);
        structure(const structure& orig);
        virtual ~structure();
    private:

    };
}
#endif	/* STRUCTURE_H */
