/* 
 * File:   t_Clock.h
 * Author: geth2k
 *
 * Created on 22 August 2012, 18:59
 */

#ifndef SID_TIMER_H
#define	SID_TIMER_H

#include <chrono>
#include <ctime>

namespace sid {

    class timer {
    public:

        static constexpr double NANOSCALE = 1000000000.0;

        timer();
        timer(const timer& orig);
        virtual ~timer();

        void update_timediff();

        inline double get_delta() const {
            return this->__m_dblDeltaT;
        }
        
        inline double get_time() const {
            return this->__m_dblTimer;
        }
        
        inline void adjust_time(double adjust) {
            this->__m_dblTimer += adjust;
        }

    private:

        double __m_dblDeltaT;
        double __m_dblTimer;
        
        timespec __m_Time, __m_TimePrev;
    };
}

#endif	/* CLOCK_H */

