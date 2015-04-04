/*
 * File:   perlin.h
 * Author: geth2k
 *
 * Created on 07 July 2012, 20:43
 */

#ifndef SID_PERLIN_H
#define	SID_PERLIN_H

#include "SID_height_map.h"
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <iostream>

namespace sid {

    class perlin : public height_map {
    public:
        perlin();
        perlin(const perlin& orig);
        perlin(float freq, float amp, float persist, int octaves, Uint32 seed);
        virtual ~perlin();

        inline void set_seed(uint seed) {
            this->_m_uiSeed = seed;
        }
        
        uint get_seed() const {
            return this->_m_uiSeed;
        }

        float get_height(int x, int y);

        static Uint32 gen_seed();

    private:

        float __noise(int x, int y);
        float __smoothed_noise(float x, float y);
        float __interpolate(float x, float y, float a);
        float __interpolated_noise(float x, float y);

        static bool __is_prime(int val);
        
        uint __m_uiOctaves;
        float __m_fFrequency;
        float __m_fAmplitude;
        float __m_fPersistance;        

    };
}
#endif	/* PERLIN_H */