/* 
 * File:   rectangle_3d.h
 * Author: geth2k
 *
 * Created on 02 November 2013, 16:04
 */

#ifndef SID_RECTANGLE_3D_H
#define	SID_RECTANGLE_3D_H

#include "SID_vertex3.h"

namespace sid {

    class rectangle_3d {
    public:

        enum rectangle_positions {
            top_left,
            bottom_left,
            bottom_right,
            top_right,
        };
        rectangle_3d();
        rectangle_3d(vertex3& p0, vertex3& p1, vertex3& p2, vertex3& p3);
        rectangle_3d(const rectangle_3d& orig);
        virtual ~rectangle_3d();

        vertex3 get_vertex(rectangle_positions pos) const;

    private:
        vertex3 m_vtxP0;
        vertex3 m_vtxP1;
        vertex3 m_vtxP2;
        vertex3 m_vtxP3;
        double m_dblWidth;
        double m_dblLength;
    };
}
#endif	/* RECTANGLE_3D_H */

