/*
 * File:   vector3.h
 * Author: geth2k
 *
 * Created on 14 April 2013, 00:14
 */

#ifndef SID_VECTOR3_H
#define	SID_VECTOR3_H

#include "SID_euclid.h"

namespace sid {

    class vector3 : public euclid {
    public:
        vector3();
        vector3(float fDim1, float fDim2, float fDim3);
        vector3(euclid orig);
        virtual ~vector3();

        void normalise();
        void set_magnitude(float fMag);
        float get_magnitude() const;
        static euclid get_vector_normal(vector3& p1, vector3& p2, vector3& p3);

        vector3& operator=(const vector3& rhs) {
            this->sid::euclid::operator =(rhs);
            return *this;
        }

    private:
        float __m_fMagnitude;

    };

    template <typename _T>
    static sid::vector3 operator-(sid::vector3 lhs, _T const& rhs) {
        return lhs -= rhs;
    }
    
    template <typename _T>
    static sid::vector3 operator+(sid::vector3 lhs, _T const& rhs) {
        return lhs += rhs;
    }
    
    template <typename _T>
    static sid::vector3 operator*(sid::vector3 lhs, _T const& rhs) {
        return lhs *= rhs;
    }
    
    template <typename _T>
    static sid::vector3 operator/(sid::vector3 lhs, _T const& rhs) {
        return lhs /= rhs;
    }    
}
#endif	/* SID_VECTOR3_H */

