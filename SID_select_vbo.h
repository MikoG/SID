/* 
 * File:   selectElementArray.h
 * Author: geth2k
 *
 * Created on 24 May 2014, 18:34
 */

#ifndef SID_SELECTVBO_H
#define	SID_SELECTVBO_H

#define GL_GLEXT_PROTOTYPES

#include "SID_select_delegate.h"
#include "SID_vbo.h"

namespace sid {

    class select_vbo : public select_delegate {
    public:

        select_vbo() {
        }

        select_vbo(const select_vbo& orig) {
        }

        virtual ~select_vbo() {
        }

        virtual void draw_select(int name, void const* arg);

    private:

    };
}
#endif	/* SELECTVBO_H */

