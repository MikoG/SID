/*
 * File:   euclid.cpp
 * Author: geth2k
 *
 * Created on 10 April 2013, 21:43
 */

#include "SID_euclid.h"

sid::euclid::euclid() : 
__m_blNormalised(false) {

}

sid::euclid::euclid(const euclid& orig) :
sid::t_vertset(orig),
__m_blNormalised(orig.__m_blNormalised) {

}

sid::euclid::~euclid() {

}

sid::euclid::euclid(float fDim1, float fDim2, float fDim3) :
__m_blNormalised(false),
sid::t_vertset(fDim1, fDim2, fDim3) {

}

void sid::euclid::normalise() {
    float fLength = sqrt(this->dot(this));
    fLength = (fLength == 0.0) ?  1 : fLength;
    *this /= fLength;
    this->__m_blNormalised = true;
}

float sid::euclid::dot(euclid* euc) const {
    float fDot;
    euclid eucTemp = *euc;
    fDot = (*this)[DIMENSION_X] * eucTemp[DIMENSION_X];
    fDot += (*this)[DIMENSION_Y] * eucTemp[DIMENSION_Y];
    fDot += (*this)[DIMENSION_Z] * eucTemp[DIMENSION_Z];
    return fDot;
}

string sid::euclid::data_to_string() const {
    string strTemp;
    strTemp = "X:=";
    strTemp += sid::float_to_text((*this)[DIMENSION_X]);
    strTemp += " Y:=";
    strTemp += sid::float_to_text((*this)[DIMENSION_Y]);
    strTemp += " Z:=";
    strTemp += sid::float_to_text((*this)[DIMENSION_Z]);
    return strTemp;
}

