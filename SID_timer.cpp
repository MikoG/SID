/* 
 * File:   t_Clock.cpp
 * Author: geth2k
 * 
 * Created on 22 August 2012, 18:59
 */

#include "SID_timer.h"

sid::timer::timer() :
__m_dblDeltaT(0.0),
__m_dblTimer(0.0) {
    
    clock_gettime(CLOCK_REALTIME, &__m_TimePrev);
    clock_gettime(CLOCK_REALTIME, &__m_Time);
}

sid::timer::timer(const timer& orig) :
__m_dblDeltaT(orig.__m_dblDeltaT),
__m_dblTimer(orig.__m_dblTimer),
__m_Time(orig.__m_Time),
__m_TimePrev(orig.__m_TimePrev) {
}

sid::timer::~timer() {
}

void sid::timer::update_timediff() {
    clock_gettime(CLOCK_REALTIME, &__m_Time);
       
    __m_dblDeltaT = (__m_Time.tv_sec + __m_Time.tv_nsec / NANOSCALE)
               - (__m_TimePrev.tv_sec + __m_TimePrev.tv_nsec / NANOSCALE);
    this->__m_dblTimer += __m_dblDeltaT;
    __m_TimePrev = __m_Time;
}
