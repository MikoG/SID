/* 
 * File:   select3DRect.h
 * Author: geth2k
 *
 * Created on 24 May 2014, 17:54
 */

#ifndef SID_SELECT_3DRECT_H
#define	SID_SELECT_3DRECT_H

#include "SID_select_delegate.h"
#include "SID_rectangle_3d.h"

namespace sid {

    class select_3Drect : public select_delegate {
    public:

        select_3Drect() {
        }

        select_3Drect(const select_3Drect& orig) {
        }

        virtual ~select_3Drect() {
        }

        virtual void draw_select(int name, void const* arg);
    private:

    };
}
#endif	/* SID_SELECT_3DRECT_H */

