/* 
 * File:   tuplets.h
 * Author: geth2k
 *
 * Created on 26 May 2014, 15:05
 */

#ifndef SID_TUPLETS_H
#define	SID_TUPLETS_H

#include <array>

#define QUADRUPLET_SIZE 4
#define TRIPLET_SIZE 3
#define DUPLET_SIZE 2

namespace sid {

    template <typename U, std::size_t size>
    class tuplet {
    public:

        tuplet() :
        __m_data() {

        }

        tuplet(tuplet const& orig) :
        __m_data(orig.__m_data) {

        }

        tuplet<U, size>& operator=(const tuplet<U, size>& rhs) {
            if (this != &rhs) {
                for (int i(0); i < size; ++i)
                    (*this)[i] = rhs[i];
            }
            return *this;
        }

        inline U const& operator[](const std::size_t Index) const {
            return (this->__m_data[Index]);
        }

        inline U& operator[](const std::size_t Index) {
            return (this->__m_data[Index]);
        }

        inline U const* data() const {
            return this->__m_data.data();
        }

        inline U* data() {
            return this->__m_data.data();
        }

        // ---------------------------------------------------------------

        template <typename _T>
        inline tuplet<U, size>& operator+=(tuplet<_T, size> const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] += static_cast<U>(rhs[i]);

            return *this;
        }

        template <typename _T>
        inline tuplet<U, size>& operator+=(tuplet<_T, size>& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] += static_cast<U>(rhs[i]);

            return *this;
        }
        
        inline tuplet<U, size>& operator+=(tuplet<U, size> const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] += rhs[i];

            return *this;
        }

        inline tuplet<U, size>& operator+=(tuplet<U, size>& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] += rhs[i];

            return *this;
        }

        inline tuplet<U, size>& operator+=(U const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] += rhs;

            return *this;
        }

        inline tuplet<U, size>& operator+=(U& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] += rhs;

            return *this;
        }

        // ---------------------------------------------------------------
        
        template <typename _T>
        inline tuplet<U, size>& operator-=(tuplet<_T, size> const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] -= static_cast<U>(rhs[i]);

            return *this;
        }

        template <typename _T>
        inline tuplet<U, size>& operator-=(tuplet<_T, size>& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] -= static_cast<U>(rhs[i]);

            return *this;
        }        

        inline tuplet<U, size>& operator-=(tuplet<U, size> const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] -= rhs[i];

            return *this;
        }

        inline tuplet<U, size>& operator-=(tuplet<U, size>& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] -= rhs[i];

            return *this;
        }

        inline tuplet<U, size>& operator-=(U const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] -= rhs;

            return *this;
        }

        inline tuplet<U, size>& operator-=(U& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] -= rhs;

            return *this;
        }

        // ---------------------------------------------------------------
        
        
        template <typename _T>
        inline tuplet<U, size>& operator*=(tuplet<_T, size> const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] *= static_cast<U>(rhs[i]);

            return *this;
        }

        template <typename _T>
        inline tuplet<U, size>& operator*=(tuplet<_T, size>& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] *= static_cast<U>(rhs[i]);

            return *this;
        }        

        inline tuplet<U, size>& operator*=(tuplet<U, size> const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] *= rhs[i];

            return *this;
        }

        inline tuplet<U, size>& operator*=(tuplet<U, size>& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] *= rhs[i];

            return *this;
        }

        inline tuplet<U, size>& operator*=(U const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] *= rhs;

            return *this;
        }

        inline tuplet<U, size>& operator*=(U& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] *= rhs;

            return *this;
        }

        // ---------------------------------------------------------------
        
        template <typename _T>
        inline tuplet<U, size>& operator/=(tuplet<_T, size> const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] /= static_cast<U>(rhs[i]);

            return *this;
        }

        template <typename _T>
        inline tuplet<U, size>& operator/=(tuplet<_T, size>& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] /= static_cast<U>(rhs[i]);

            return *this;
        }        

        inline tuplet<U, size>& operator/=(tuplet<U, size> const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] /= rhs[i];

            return *this;
        }

        inline tuplet<U, size>& operator/=(tuplet<U, size>& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] /= rhs[i];

            return *this;
        }

        inline tuplet<U, size>& operator/=(U const& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] /= rhs;

            return *this;
        }

        inline tuplet<U, size>& operator/=(U& rhs) {
            for (int i(0); i < size; ++i)
                (*this)[i] /= rhs;

            return *this;
        }

    protected:

        tuplet(std::array<U, size> const& data) :
        __m_data(data) {

        }

    private:
        std::array<U, size> __m_data;
    };

    template <typename U>
    struct quadruplet : public tuplet<U, QUADRUPLET_SIZE> {
        static const std::size_t SIZE = QUADRUPLET_SIZE;

        quadruplet() :
        tuplet<U, SIZE>() {
        }

        quadruplet(quadruplet const& orig) :
        tuplet<U, SIZE>(orig) {

        }

        quadruplet(tuplet<U, SIZE> const& orig) :
        tuplet<U, SIZE>(orig) {

        }

        quadruplet(U const& val0, U const& val1, U const& val2, U const& val3) :
        tuplet<U, SIZE>(std::array<U, SIZE>{{val0, val1, val2, val3}}) {
        }

        quadruplet<U>& operator=(quadruplet<U> const& rhs) {
            this->tuplet<U, SIZE>::operator=(rhs);
            return *this;
        }

        quadruplet<U>& operator=(tuplet<U, SIZE> const& rhs) {
            this->tuplet<U, SIZE>::operator=(rhs);
            return *this;
        }
    };

    template <typename U>
    struct triplet : public tuplet<U, TRIPLET_SIZE> {
        static const std::size_t SIZE = TRIPLET_SIZE;

        triplet() :
        tuplet<U, SIZE>() {
        }

        triplet(triplet const& orig) :
        tuplet<U, SIZE>(orig) {

        }

        triplet(tuplet<U, SIZE> const& orig) :
        tuplet<U, SIZE>(orig) {

        }

        triplet(U const& val0, U const& val1, U const& val2) :
        tuplet<U, SIZE>(std::array<U, SIZE>{{val0, val1, val2}}) {
            
        }

        triplet<U>& operator=(triplet<U> const& rhs) {
            this->tuplet<U, SIZE>::operator=(rhs);
            return *this;
        }

        triplet<U>& operator=(tuplet<U, SIZE> const& rhs) {
            this->tuplet<U, SIZE>::operator=(rhs);
            return *this;
        }
    };

    template <typename U>
    struct duplet : public tuplet<U, DUPLET_SIZE> {
        static const std::size_t SIZE = DUPLET_SIZE;

        duplet() :
        tuplet<U, SIZE>() {
        }

        duplet(duplet const& orig) :
        tuplet<U, SIZE>(orig) {

        }

        duplet(tuplet<U, SIZE> const& orig) :
        tuplet<U, SIZE>(orig) {

        }

        duplet(U const& val0, U const& val1) :
        tuplet<U, SIZE>(std::array<U, SIZE>{
            {val0, val1}
        }) {
        }

        duplet<U>& operator=(duplet<U> const& rhs) {
            this->tuplet<U, SIZE>::operator=(rhs);
            return *this;
        }

        duplet<U>& operator=(tuplet<U, SIZE> const& rhs) {
            this->tuplet<U, SIZE>::operator=(rhs);
            return *this;
        }
    };

    // ---------------------------------------------------------------
    
    template <typename _T, typename _U>
    static sid::quadruplet<_T> operator+(sid::quadruplet<_T> lhs, _U const& rhs) {
        return lhs += rhs;
    }

    template <typename _T, typename _U>
    static sid::quadruplet<_T> operator-(sid::quadruplet<_T> lhs, _T const& rhs) {
        return lhs -= rhs;
    }

    template <typename _T, typename _U>
    static sid::quadruplet<_T> operator*(sid::quadruplet<_T> lhs, _U const& rhs) {
        return lhs *= rhs;
    }

    template <typename _T, typename _U>
    static sid::quadruplet<_T> operator/(sid::quadruplet<_T> lhs, _U const& rhs) {
        return lhs /= rhs;
    }

    // ---------------------------------------------------------------    

    template <typename _T, typename _U>
    static sid::triplet<_T> operator+(sid::triplet<_T> lhs, _U const& rhs) {
        return lhs += rhs;
    }

    template <typename _T, typename _U>
    static sid::triplet<_T> operator-(sid::triplet<_T> lhs, _T const& rhs) {
        return lhs -= rhs;
    }

    template <typename _T, typename _U>
    static sid::triplet<_T> operator*(sid::triplet<_T> lhs, _U const& rhs) {
        return lhs *= rhs;
    }

    template <typename _T, typename _U>
    static sid::triplet<_T> operator/(sid::triplet<_T> lhs, _U const& rhs) {
        return lhs /= rhs;
    }

    // ---------------------------------------------------------------

    template <typename _T, typename _U>
    static sid::duplet<_T> operator+(sid::duplet<_T> lhs, _U const& rhs) {
        return lhs += rhs;
    }

    template <typename _T, typename _U>
    static sid::duplet<_T> operator-(sid::duplet<_T> lhs, _T const& rhs) {
        return lhs -= rhs;
    }

    template <typename _T, typename _U>
    static sid::duplet<_T> operator*(sid::duplet<_T> lhs, _U const& rhs) {
        return lhs *= rhs;
    }

    template <typename _T, typename _U>
    static sid::duplet<_T> operator/(sid::duplet<_T> lhs, _U const& rhs) {
        return lhs /= rhs;
    }

    // ---------------------------------------------------------------

    template <typename _T, typename _U, std::size_t size>
    static sid::tuplet<_T, size> operator+(sid::tuplet<_T, size> lhs, _U const& rhs) {
        return lhs += rhs;
    }

    template <typename _T, typename _U, std::size_t size>
    static sid::tuplet<_T, size> operator-(sid::tuplet<_T, size> lhs, _U const& rhs) {
        return lhs -= rhs;
    }

    template <typename _T, typename _U, std::size_t size>
    static sid::tuplet<_T, size> operator*(sid::tuplet<_T, size> lhs, _U const& rhs) {
        return lhs *= rhs;
    }

    template <typename _T, typename _U, std::size_t size>
    static sid::tuplet<_T, size> operator/(sid::tuplet<_T, size> lhs, _U const& rhs) {
        return lhs /= rhs;
    }

} // End namespace sid


#endif	/* TUPLETS_H */

