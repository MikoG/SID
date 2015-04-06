/* 
 * File:   object_initialiser.h
 * Author: geth2k
 *
 * Created on March 17, 2015, 9:12 PM
 */

#ifndef OBJECT_INITIALISER_H
#define	OBJECT_INITIALISER_H

#include <string>
#include "SID_globals.h"
#include "SID_vbo.h"
#include "SID_vertex3.h"
#include "SID_child.h"
#include "SID_sidof.h"

#define STATIC_STRING     static const std::string
#define STATIC_STRING_DEF        const std::string sid::object_template

namespace sid {

    class object_template : public sid::child<object_template*> {

        enum object_type {
            terrain_object = 0,
            building = 1,
            mob = 2,
            vehicle = 3
        };
    public:
        object_template();
        object_template(std::shared_ptr<sid::sido const> Sido);
        object_template(const object_template& orig);
        object_template(object_template&& orig);
        virtual ~object_template();
        
        object_template& operator=(const object_template& orig);
        
        void build_template();

        STATIC_STRING STR_OBJ_TYPE;
        STATIC_STRING STR_MIN_CREW;
        STATIC_STRING STR_MAX_CREW;
        STATIC_STRING STR_HITPOINTS;
        STATIC_STRING STR_SHIELDPOINTS;
        STATIC_STRING STR_WEPONS_SLOTS;
        STATIC_STRING STR_MASS;
        STATIC_STRING STR_FORCE;
        STATIC_STRING STR_TRACTION;
        STATIC_STRING STR_WIDTH;
        STATIC_STRING STR_LENGTH;
        STATIC_STRING STR_MAX_GRAD;
        STATIC_STRING STR_LAND_ENABLED;
        STATIC_STRING STR_WATER_ENABLED;
        STATIC_STRING STR_MOUNTING_POINT;
        STATIC_STRING STR_VBO;

    private:
        void __copy(const object_template& orig);
        
        char __get_template_values();

        char __get_vbo_params(sid::sido const* ptr,
                int& primitive,
                int& color_mode,
                int& normals_mode,
                float& scale,
                sid::t_vertset& center);

        char __get_vbo_data(sid::vbo * const VBO);

        char __get_vbo_verts(sid::sido const* ptr,
                sid::vbo * const VBO);

        char __get_vbo_indices(sid::sido const* ptr,
                sid::vbo * const VBO);

        char __get_vbo_colors(sid::sido const* ptr,
                sid::vbo * const VBO);

        char __get_vbo_single_color(sid::sido const* ptr,
                sid::vbo * const VBO);

        char __get_vbo_uv(sid::sido const* ptr,
                sid::vbo * const VBO);

        template<typename T>
        T __get_int_val(std::string const& name) {
            return __get_num_val<int, T>(name);
        }
        
        template<typename T>
        T __get_float_val(std::string const& name) {
            return __get_num_val<float, T>(name);
        }
        
        template<typename __Base, typename __Ret>
        __Ret __get_num_val(std::string const& name) {
            sid::t_variant varTemp;
            __Base fTemp = 0;
            __Ret tRetVal = 0;
            if (__m_shptrSido->has_value(name)) {
                varTemp = __m_shptrSido->get_value(name);
                sid::get_val(fTemp, varTemp);
                tRetVal = fTemp;
            }
            return tRetVal;
        }

        inline bool __get_bool_val(std::string const& name) {
            sid::t_variant varTemp;
            bool blRetVal = false;
            if (__m_shptrSido->has_value(name)) {
                varTemp = __m_shptrSido->get_value(name);
                sid::get_val(blRetVal, varTemp);
            }
            return blRetVal;
        }
        
        std::shared_ptr<sid::sido const> __m_shptrSido;
        
        std::string __m_strName;
        uchar __m_objType;
        uchar __m_MinCrew;
        uchar __m_MaxCrew;
        uchar __m_HitPoints;
        uchar __m_ShieldPoints;
        uchar __m_WeaponsSlots;
        double __m_Mass;
        double __m_Force;
        double __m_Traction;
        double __m_Width;
        double __m_Length;
        double __m_MaxGradient;
        bool __m_LandEnabled;
        bool __m_WaterEnabled;
        sid::vertex3 __m_MountingPoint;
        sid::vbo* __m_VBO;

    };
}
#endif	/* OBJECT_INITIALISER_H */