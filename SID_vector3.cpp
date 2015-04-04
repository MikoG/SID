/*
 * File:   vector.cpp
 * Author: geth2k
 *
 * Created on 14 April 2013, 00:14
 */

#include "SID_vector3.h"

sid::vector3::vector3() {
    __m_fMagnitude = 1;
}

sid::vector3::vector3(float fDim1, float fDim2, float fDim3) :
euclid(fDim1, fDim2, fDim3) {
    
}

sid::vector3::vector3(euclid orig) :
euclid(orig) {
    __m_fMagnitude = 1;
}

sid::vector3::~vector3() {
    
}

float sid::vector3::get_magnitude() const {
    return __m_fMagnitude;
}

void sid::vector3::set_magnitude(float fMag) {
    __m_fMagnitude = fMag;
}

void sid::vector3::normalise() {
    vector3 Vector1(*this);
    euclid::normalise();
    vector3 Vector2(*this);

    set_magnitude(Vector1[euclid::DIMENSION_X] / Vector2[euclid::DIMENSION_X]);
}

sid::euclid sid::vector3::get_vector_normal(vector3& p0, vector3& p1, vector3& p2) {
    float fVNormal1;
    float fVNormal2;
    float fVNormal3;

    euclid eucA = p0 - p1;
    euclid eucB = p1 - p2;

    fVNormal1 = (eucA[1] * eucB[2]) - (eucA[2] * eucB[1]);
    fVNormal2 = (eucA[2] * eucB[0]) - (eucA[0] * eucB[2]);
    fVNormal3 = (eucA[0] * eucB[1]) - (eucA[1] * eucB[0]);

    euclid vNormal(fVNormal1, fVNormal2, fVNormal3);
    vNormal.normalise();

    return vNormal;
}