/* 
 * File:   drawChunk.h
 * Author: geth2k
 *
 * Created on 24 May 2014, 16:01
 */

#ifndef SID_DRAWVBO_H
#define	SID_DRAWVBO_H

#define GL_GLEXT_PROTOTYPES
#include "SID_draw_delegate.h"
#include "SID_vbo.h"

namespace sid {

    class drawVBO : public drawDelegate {
    public:

        drawVBO() {
        }

        drawVBO(const drawVBO& orig) {
        }

        virtual ~drawVBO() {
        }

        virtual void draw(void const* arg);
    private:

    };
}
#endif	/* DRAWVBO_H */

