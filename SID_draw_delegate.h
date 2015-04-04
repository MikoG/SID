/* 
 * File:   drawDelegate.h
 * Author: geth2k
 *
 * Created on 24 May 2014, 15:51
 */

#ifndef SID_DRAW_DELEGATE_H
#define	SID_DRAW_DELEGATE_H

#include <GL/gl.h>

namespace sid {

    class drawDelegate {
    public:

        drawDelegate() {
        }

        drawDelegate(const drawDelegate& orig) {
        }

        virtual ~drawDelegate() {
        }

        virtual void draw(void const* arg) = 0;

    private:

    };
}
#endif	/* DRAWDELEGATE_H */

