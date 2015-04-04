#include <math.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SID_globals.h"

double sid::rads_to_degrees(double rads) {
    const double ONEEIGHTY = 180.0;
    return rads*ONEEIGHTY/M_PI;
}

double sid::degrees_to_rads(double degrees) {
    return degrees*M_PI/180.0;
}

double sid::true_asin(double vectorAxis1, double vectorAxis2, double hypot) {
    double angle;
    if (hypot == 0.0) {
        angle = 0.0;
    } else {
        angle = rads_to_degrees(asin(vectorAxis1 / hypot));
    }
    if (vectorAxis2 <= 0.0) {
        angle = 180 - angle;
    }
    if (vectorAxis1 <= 0.0) {
        angle = 360 + angle;
    }
    angle = fmod(angle, 360);
    return angle;
}

std::string sid::int_to_text(int number) {
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

std::string sid::float_to_text(float number) {
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
