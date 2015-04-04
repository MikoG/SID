/*
 * File:   vertex3.h
 * Author: geth2k
 *
 * Created on 13 April 2013, 18:50
 */

#ifndef SID_VERTEX3_H
#define	SID_VERTEX3_H

#include <iostream>

#include "SID_euclid.h"

namespace sid {

    struct vertex3 : public euclid {
        
        vertex3();
        vertex3(float fDim1, float fDim2, float fDim3);
        vertex3(euclid orig);
        virtual ~vertex3();

        void move(float fValue_X, float fValue_Y, float fValue_Z);

        void rotate(vertex3 const& vtxCenter,
                float angleX, float angleY, float angleZ);

        vertex3& operator=(const vertex3& rhs) {
            this->sid::euclid::operator =(rhs);
            return *this;
        }
    };

    template <typename _T>
    static sid::vertex3 operator-(sid::vertex3 lhs, _T const& rhs) {
        return lhs -= rhs;
    }
    
    template <typename _T>
    static sid::vertex3 operator+(sid::vertex3 lhs, _T const& rhs) {
        return lhs += rhs;
    }
    
    template <typename _T>
    static sid::vertex3 operator*(sid::vertex3 lhs, _T const& rhs) {
        return lhs *= rhs;
    }
    
    template <typename _T>
    static sid::vertex3 operator/(sid::vertex3 lhs, _T const& rhs) {
        return lhs /= rhs;
    }    
}
#endif	/* VERTEX3_H */

