/* 
 * File:   frustum.h
 * Author: geth2k
 *
 * Created on 25 May 2014, 14:19
 */

#ifndef SID_FRUSTUM_H
#define	SID_FRUSTUM_H

#include <GL/gl.h>
#include <array>
#include <math.h>

namespace sid {

    class frustum {
    public:
        frustum();
        frustum(const frustum& orig);
        virtual ~frustum();

        bool testPoint(float x, float y, float z) const;

    private:
        float m_vfFrustum[6][4];
    };
}
#endif	/* FRUSTUM_H */

