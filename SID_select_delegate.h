/* 
 * File:   selectDelegate.h
 * Author: geth2k
 *
 * Created on 24 May 2014, 17:44
 */

#ifndef SID_SELECTDELEGATE_H
#define	SID_SELECTDELEGATE_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <array>

#define BUFFER_SIZE 512

namespace sid {

    class select_delegate {
    public:

        select_delegate() {
        }

        select_delegate(const select_delegate& orig) {
        }

        virtual ~select_delegate() {
        }

        static void init_selframe_3d(int x, int y, float fov, float drawDistance);

        static int process_selhits();

        virtual void draw_select(int name, void const* arg) = 0;

    private:
        static std::array<GLuint, BUFFER_SIZE> __s_selectBuf;
    };
}
#endif	/* SELECTDELEGATE_H */

