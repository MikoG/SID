/* 
 * File:   bits.h
 * Author: geth2k
 *
 * Created on June 24, 2014, 3:50 PM
 */

#ifndef SID_BITS_H
#define	SID_BITS_H

namespace sid {

    template <typename T>
    class bits {
    public:

        bits() : __m_data(0) {
        }

        bits(T const flags) : __m_data(flags) {

        }

        bits(bits const& orig) : __m_data(orig.__m_data) {
        }

        virtual ~bits() {
        }

        void set_subdata_mask(T flags) {
            this->__m_data |= flags;
        }

        inline bool is_subdata_mask(T flags) const {
            T flagsTemp = flags & this->__m_data;
            T zero = 0x0;
            bool blRet = flagsTemp != zero;

            return blRet;
        }

        inline void unset_subdata_mask(T flags) {
            flags &= __m_data;
            this->__m_data ^= flags;
        }

        inline T get_subdata_value(T flags) const {
            return (this->__m_data & flags) >> get_valuemask_offset(flags);
        }

        void set_subdata_value(T const flags, T const value) {
            T uiTemp = (value << get_valuemask_offset(flags)) & flags;
            this->__m_data |= flags;
            this->__m_data ^= flags;
            this->__m_data |= uiTemp;
        }

        inline T inc_subdata_value(T const flags, T const incValue) {
            T tMax = bits::get_valuemask_max(flags);
            T tTemp = this->get_subdata_value(flags);
            tTemp += incValue;
            if (tTemp > tMax)
                tTemp = tMax;

            this->set_subdata_value(flags, tTemp);
            return tTemp;
        }

        inline static int get_valuemask_offset(T flags) {
            int iOffset;
            for (iOffset = 0; iOffset < DATA_BIT_SIZE; iOffset++)
                if ((flags >> iOffset) & 0x01)
                    return iOffset;
            return -1;
        }

        inline static T get_valuemask_max(T const flags) {
            return flags >> bits::get_valuemask_offset(flags);
        }

    private:

        T __m_data;

        static const int DATA_BIT_SIZE = sizeof (T) * 8;
    };
}
#endif	/* BITS_H */

