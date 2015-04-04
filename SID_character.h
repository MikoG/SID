/* 
 * File:   character.h
 * Author: geth2k
 *
 * Created on 17 November 2013, 17:20
 */

#ifndef SID_CHARACTER_H
#define	SID_CHARACTER_H

#include "SID_control.h"

namespace sid {

    class character : public control {
    public:
        character();
        character(const character& orig);
        virtual ~character();

        virtual void paint();
    private:

    };
}
#endif	/* CHARACTER_H */

