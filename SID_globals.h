/*
 * File:   globfuncs.h
 * Author: geth2k
 *
 * Created on 04 June 2012, 22:03
 */

#ifndef SID_GLOBALS_H
#define	SID_GLOBALS_H

#include <math.h>
#include <sstream>
#include <SDL2/SDL.h>


typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned short ushort;

namespace sid {
    double rads_to_degrees(double rads);
    double degrees_to_rads(double degrees);
    double true_asin(double vectorAxis1, double vectorAxis2, double hypot);
    std::string int_to_text(int number);
    std::string float_to_text(float number);

    template <typename type>
    inline bool get_bit(type value, int mask) {
        type tMaskedVal = value & mask;
        return (tMaskedVal != 0x00);
    }
}

#endif	/* SID_GLOBALS_H */

