/* 
 * File:   drawControl.h
 * Author: geth2k
 *
 * Created on 24 May 2014, 16:50
 */

#ifndef SID_DRAWCONTROL_H
#define	SID_DRAWCONTROL_H

#include "SID_draw_delegate.h"
#include "SID_rectangle_2d.h"

namespace sid {

    class drawControl : public drawDelegate {
    public:
        drawControl();
        drawControl(const drawControl& orig);
        virtual ~drawControl();

        virtual void draw(void const* arg);
    private:

    };
}

#endif	/* DRAWCONTROL_H */

