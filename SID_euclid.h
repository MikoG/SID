/*
 * File:   euclid.h
 * Author: geth2k
 *
 * Created on 10 April 2013, 21:43
 */

#ifndef EUCLID_H
#define	EUCLID_H

#include <math.h>
#include <string>
#include <memory>

#include "SID_globals.h"
#include "SID_tuplets.h"
#include "SID_vbo.h"

using namespace std;

namespace sid {

    class euclid : public sid::t_vertset {
    public:

        enum dimensions {
            DIMENSION_X = 0,
            DIMENSION_Y = 1,
            DIMENSION_Z = 2,
        };

        euclid();
        euclid(float fDim1, float fDim2, float fDim3);
        euclid(const euclid& orig);
        virtual ~euclid();

        string data_to_string() const;

        void normalise();

        inline bool is_normalised() const {
            return this->__m_blNormalised;
        }

        float dot(euclid* euc) const;

        // ---------------------------------------------------------------

        euclid& operator=(const euclid& rhs) {
            this->sid::t_vertset::operator =(rhs);
            return *this;
        }

        // ---------------------------------------------------------------

        euclid& operator+=(euclid const& rhs) {
            this->sid::t_vertset::operator+=(rhs);
            this->__m_blNormalised = false;
            return *this;
        }

        euclid& operator+=(euclid& rhs) {
            this->sid::t_vertset::operator+=(rhs);
            this->__m_blNormalised = false;
            return *this;
        }

        euclid& operator+=(float rhs) {
            this->sid::t_vertset::operator+=(rhs);
            this->__m_blNormalised = false;
            return *this;
        }

        // ---------------------------------------------------------------

        euclid& operator-=(euclid const& rhs) {
            this->sid::t_vertset::operator-=(rhs);
            this->__m_blNormalised = false;
            return *this;
        }

        euclid& operator-=(euclid& rhs) {
            this->sid::t_vertset::operator-=(rhs);
            this->__m_blNormalised = false;
            return *this;
        }

        euclid& operator-=(float rhs) {
            this->sid::t_vertset::operator-=(rhs);
            this->__m_blNormalised = false;
            return (*this);
        }

        // ---------------------------------------------------------------

        euclid& operator*=(euclid const& rhs) {
            this->sid::t_vertset::operator*=(rhs);
            this->__m_blNormalised = false;
            return (*this);
        }

        euclid& operator*=(euclid& rhs) {
            this->sid::t_vertset::operator*=(rhs);
            this->__m_blNormalised = false;
            return (*this);
        }

        euclid& operator*=(float rhs) {
            this->sid::t_vertset::operator*=(rhs);
            this->__m_blNormalised = false;
            return (*this);
        }

        // ---------------------------------------------------------------

        euclid& operator/=(euclid const& rhs) {
            this->sid::t_vertset::operator/=(rhs);
            this->__m_blNormalised = false;
            return (*this);
        }

        euclid& operator/=(euclid& rhs) {
            this->sid::t_vertset::operator/=(rhs);
            this->__m_blNormalised = false;
            return (*this);
        }

        euclid& operator/=(float rhs) {
            this->sid::t_vertset::operator/=(rhs);
            this->__m_blNormalised = false;
            return (*this);
        }

        // ---------------------------------------------------------------

    private:
        bool __m_blNormalised;
    };

}

template <typename _T>
static sid::euclid operator+(sid::euclid lhs, _T const& rhs) {
    return lhs += rhs;
}

template <typename _T>
static sid::euclid operator-(sid::euclid lhs, _T const& rhs) {
    return lhs -= rhs;
}

template <typename _T>
static sid::euclid operator*(sid::euclid lhs, _T const& rhs) {
    return lhs *= rhs;
}

template <typename _T>
static sid::euclid operator/(sid::euclid lhs, _T const& rhs) {
    return lhs /= rhs;
}

#endif	/* EUCLID_H */

