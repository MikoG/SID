/* 
 * File:   plot.h
 * Author: geth2k
 *
 * Created on 12 May 2013, 11:52
 */

#ifndef SID_PLOT_H
#define	SID_PLOT_H

#include "SID_object_ingame.h"
#include "SID_structure.h"

namespace sid {

    class plot : public object_ingame {
    public:
        static const int PLOT_WIDTH = 4;

        class plot_offset {
        public:

            plot_offset() :
            __m_iX(0),
            __m_iZ(0) {
            }

            plot_offset(int x, int z) :
            __m_iX(x),
            __m_iZ(z) {
            }

            plot_offset(const plot_offset& orig) :
            __m_iX(orig.__m_iX),
            __m_iZ(orig.__m_iZ) {
            }

            virtual ~plot_offset() {
            }

            inline int get_z() const {
                return __m_iZ;
            }

            inline int get_x() const {
                return __m_iX;
            }

            plot_offset& operator=(plot_offset const& orig) {
                this->__m_iX = orig.__m_iX;
                this->__m_iZ = orig.__m_iZ;

                return *this;
            }

            plot_offset& operator-=(plot_offset const& orig) {
                this->__m_iX -= orig.__m_iX;
                this->__m_iZ -= orig.__m_iZ;

                return *this;
            }

            plot_offset& operator+=(plot_offset const& orig) {
                this->__m_iX += orig.__m_iX;
                this->__m_iZ += orig.__m_iZ;

                return *this;
            }

            bool operator==(plot_offset check) const {
                return (__m_iX == check.__m_iX && __m_iZ == check.__m_iZ);
            }

            bool operator!=(plot_offset check) const {
                return (__m_iX != check.__m_iX || __m_iZ != check.__m_iZ);
            }

        private:
            int __m_iX;
            int __m_iZ;
        };
        
        static const int SEA_LEVEL = 154;
        static const int BEACH_MIN_HEIGHT = 156;
        static const int SNOW_LEVEL = 220;        

        plot();
        plot(const plot& orig);
        virtual ~plot();

        virtual plot& operator=(const plot& orig);

        uint get_data();
        float get_height();
        uint get_direction();
        uint get_type();
        uint get_subtype();
        uint get_damage();
        plot_offset get_plot_offset();

        void set_height(float Value);
        void set_direction(uint Value);
        void set_type(uint Value);
        void set_subtype(uint Value);
        void set_damage(uint Value);

        void set_structure(structure* Structure);

    protected:

        structure* m_Structure;

    private:

        static const uint DATAMASK_HEIGHT = 0xfff;
        static const uint DATASHIFT_HEIGHT = 0;
        static const uint DATAMASK_TYPE = 0XFF000;
        static const uint DATASHIFT_TYPE = 12;
        static const uint DATAMASK_STYPE = 0XF00000;
        static const uint DATASHIFT_STYPE = 20;
        static const uint DATAMASK_DIRCTN = 0x3000000;
        static const uint DATASHIFT_DIRCTN = 24;
        static const uint DATAMASK_DAMAGE = 0x3C000000;
        static const uint DATASHIFT_DAMAGE = 26;


        int m_Data;
        float m_fHeight;

        void set_data_element(uint DataMask, uint DataShift, uint Value);
        uint get_data_element(uint DataMask, uint DataShift);

    };

    inline plot::plot_offset operator+(plot::plot_offset lhs,
            plot::plot_offset const& rhs) {

        return lhs += rhs;
    }

    inline plot::plot_offset operator-(plot::plot_offset lhs,
            plot::plot_offset const& rhs) {

        return lhs -= rhs;
    }
}
#endif	/* PLOT_H */

