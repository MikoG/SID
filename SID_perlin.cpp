/*
 * File:   perlin.cpp
 * Author: geth2k
 *
 * Created on 07 July 2012, 20:43
 */

#include "SID_perlin.h"

sid::perlin::perlin() {
    this->__m_fAmplitude = 0;
    this->__m_fFrequency = 0;
    this->__m_fPersistance = 0;
    this->__m_uiOctaves = 0;

}

sid::perlin::perlin(float freq, float amp, float persist, int octaves, Uint32 seed) {
    this->__m_fAmplitude = amp;
    this->__m_fFrequency = freq;
    this->__m_fPersistance = persist;
    this->__m_uiOctaves = octaves;
    this->_m_uiSeed = seed;
}

sid::perlin::perlin(const perlin& orig) {
    this->__m_fAmplitude = orig.__m_fAmplitude;
    this->__m_fFrequency = orig.__m_fFrequency;
    this->__m_fPersistance = orig.__m_fPersistance;
    this->__m_uiOctaves = orig.__m_uiOctaves;
}

sid::perlin::~perlin() {

}

float sid::perlin::__noise(int x, int y) {
    int n = x + y * 57;
    n = (n<<13) ^ n;

    // seed was 1376312589
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + this->_m_uiSeed) & 0x7fffffff) / 1073741824.0);
}

float sid::perlin::__smoothed_noise(float x, float y) {
    float corners = ( __noise(x-1, y-1)+__noise(x+1, y-1)+__noise(x-1, y+1)+__noise(x+1, y+1) ) / 16;
    float sides   = ( __noise(x-1, y)  +__noise(x+1, y)  +__noise(x, y-1)  +__noise(x, y+1) ) /  8;
    float center  =  __noise(x, y) / 4;

    return corners + sides + center;
}

float sid::perlin::__interpolate(float x, float y, float a) {
    float value = (1 - cos(a * M_PI)) * 0.5;
    return x * (1 - value) + y * value;
}

float sid::perlin::__interpolated_noise(float x, float y) {
    int integer_X    = floor(x);
    float fractional_X = x - integer_X;

    int integer_Y    = floor(y);
    float fractional_Y = y - integer_Y;

    float v1 = __smoothed_noise(integer_X,     integer_Y);
    float v2 = __smoothed_noise(integer_X + 1, integer_Y);
    float v3 = __smoothed_noise(integer_X,     integer_Y + 1);
    float v4 = __smoothed_noise(integer_X + 1, integer_Y + 1);

    float i1 = __interpolate(v1 , v2 , fractional_X);
    float i2 = __interpolate(v3 , v4 , fractional_X);

    return __interpolate(i1 , i2 , fractional_Y);
}

float sid::perlin::get_height(int x, int y) {
    float total = 0.0;
    float freq = this->__m_fFrequency;
    float amp  = this->__m_fAmplitude;
    float persistance = this->__m_fPersistance;
    int octaves = this->__m_uiOctaves;
    
    for (int i = 0; i < octaves; ++i) {
        total = total + __interpolated_noise(x * freq, y * freq) * amp;
        freq *= 2;
        amp *= persistance;
    }
    
    if (total > 1) total = 1;
    else if (total < -1) total = -1;
    
    return total;
}

// ---- Static Functions --------------------------------------------

bool sid::perlin::__is_prime(int val) {
    float fSqrt;
    fSqrt = sqrt(val);

    for (int i=2; i <= fSqrt; i++) {
        if ((val % i) == 0) {
            return false;
        }
    }

    return true;
}

Uint32 sid::perlin::gen_seed() {
    srand(time(nullptr));
    uint uiSeed = rand();
    
    if (!__is_prime(uiSeed)) {
        bool blPrimeFound = false;
        while (!blPrimeFound) {
            uiSeed += 1;
            if (__is_prime(uiSeed)) {
                blPrimeFound = true;
            }
        }
    }
    
    return uiSeed;
}