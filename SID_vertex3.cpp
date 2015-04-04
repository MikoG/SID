/*
 * File:   vertex3.cpp
 * Author: geth2k
 *
 * Created on 13 April 2013, 18:50
 */

#include "SID_vertex3.h"

sid::vertex3::vertex3() :
euclid() {

}

sid::vertex3::vertex3(float fDim1, float fDim2, float fDim3) : 
euclid(fDim1, fDim2, fDim3) {

}

sid::vertex3::vertex3(euclid orig) :
euclid(orig) {
}

sid::vertex3::~vertex3() {
    
}

void sid::vertex3::move(float fValue_X, float fValue_Y, float fValue_Z) {
    (*this)[DIMENSION_X] += fValue_X;
    (*this)[DIMENSION_Y] += fValue_Y;
    (*this)[DIMENSION_Z] += fValue_Z;
}

void sid::vertex3::rotate(vertex3 const& vtxCenter,
        float angleX, float angleY, float angleZ) {
    // Create a new vertex object to store the results in.
    vertex3 newPoint;

    // Calculate the distances between the centre and the point
    // on all three axes.
    vertex3 pointVector;
    pointVector = *this - vtxCenter;

    // Calculate the difference from the centre to the point from
    // a top down view (x and z axes)
    double radiusXZ;
    radiusXZ = hypot(pointVector[euclid::DIMENSION_X],
            pointVector[euclid::DIMENSION_Z]);

    // Use true_asin function to retrieve angle from centre to point
    // from a top down view (x and z axes) within a 360 degree range.
    double currAngleXZ = sid::true_asin(pointVector[euclid::DIMENSION_X],
            pointVector[euclid::DIMENSION_Z], radiusXZ);

    // Add the requested angle change to the calculated angle using
    // fmod to keep angles with 360 degree range.
    double newAngleXZ = fmod(currAngleXZ + angleX, 360);

    // Calculate the new positions on the x, y and z axes
    newPoint[euclid::DIMENSION_X] = sin(sid::degrees_to_rads(newAngleXZ)) * radiusXZ;
    newPoint[euclid::DIMENSION_Y] = pointVector[euclid::DIMENSION_Y];
    newPoint[euclid::DIMENSION_Z] = cos(sid::degrees_to_rads(newAngleXZ)) * radiusXZ;

    // Now repeat the exercise for the back to front view (axes x and y)
    // but using the values stored in newPoint1.
    double radiusXY = hypot(newPoint[euclid::DIMENSION_X], pointVector[euclid::DIMENSION_Y]);
    double currAngleXY = sid::true_asin(newPoint[euclid::DIMENSION_X], pointVector[euclid::DIMENSION_Y], radiusXY);
    double newAngleXY = fmod(currAngleXY + angleY, 360);
    newPoint[euclid::DIMENSION_X] = sin(sid::degrees_to_rads(newAngleXY)) * radiusXY;
    newPoint[euclid::DIMENSION_Y] = cos(sid::degrees_to_rads(newAngleXY)) * radiusXY;
    newPoint[euclid::DIMENSION_Z] = newPoint[euclid::DIMENSION_Z];

    // Finally, repeat the exercise for the right to left view (axes z and y)
    // but using the values stored in newPoint1.
    double radiusZY = hypot(newPoint[euclid::DIMENSION_Z], newPoint[euclid::DIMENSION_Y]);
    double currAngleZY = sid::true_asin(newPoint[euclid::DIMENSION_Z], newPoint[euclid::DIMENSION_Y], radiusZY);
    double newAngleZY = fmod(currAngleZY + angleZ, 360);
    newPoint[euclid::DIMENSION_X] = newPoint[euclid::DIMENSION_X];
    newPoint[euclid::DIMENSION_Y] = cos(sid::degrees_to_rads(newAngleZY)) * radiusZY;
    newPoint[euclid::DIMENSION_Z] = sin(sid::degrees_to_rads(newAngleZY)) * radiusZY;
    
    // Add the new values to the centre and return the result.
    *this = newPoint + vtxCenter;
}