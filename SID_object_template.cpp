/* 
 * File:   object_initialiser.cpp
 * Author: geth2k
 * 
 * Created on March 17, 2015, 9:12 PM
 */

#include "SID_object_template.h"

STATIC_STRING_DEF::STR_OBJ_TYPE       = "type";
STATIC_STRING_DEF::STR_MIN_CREW       = "min_crew";
STATIC_STRING_DEF::STR_MAX_CREW       = "max_crew";
STATIC_STRING_DEF::STR_HITPOINTS      = "hitpoints";
STATIC_STRING_DEF::STR_SHIELDPOINTS   = "shieldpoints";
STATIC_STRING_DEF::STR_WEPONS_SLOTS   = "weapon_slots";
STATIC_STRING_DEF::STR_MASS           = "mass";
STATIC_STRING_DEF::STR_FORCE          = "force";
STATIC_STRING_DEF::STR_TRACTION       = "traction";
STATIC_STRING_DEF::STR_WIDTH          = "width";
STATIC_STRING_DEF::STR_LENGTH         = "length";
STATIC_STRING_DEF::STR_MAX_GRAD       = "max_gradient";
STATIC_STRING_DEF::STR_LAND_ENABLED   = "land_enabled";
STATIC_STRING_DEF::STR_WATER_ENABLED  = "water_enabled";
STATIC_STRING_DEF::STR_MOUNTING_POINT = "mounting_point";
STATIC_STRING_DEF::STR_VBO            = "vbo";

sid::object_template::object_template() : 
__m_VBO(new sid::vbo()) {
}

sid::object_template::object_template(std::shared_ptr<sid::sido const> Sido) : 
__m_VBO(new sid::vbo()) {    

    this->__m_shptrSido = Sido;
    build_template();
}

sid::object_template::object_template(const object_template& orig) :
__m_VBO(orig.__m_VBO) {
}

sid::object_template::object_template(object_template&& orig) : 
child(std::move(orig))  {
    
}

sid::object_template::~object_template() {
    if (this->__m_VBO)
        delete this->__m_VBO;
}

sid::object_template& sid::object_template::operator =(const object_template& orig) {
    this->__copy(orig);
    return *this;
}

void sid::object_template::__copy(const object_template& orig) {
        this->__m_shptrSido = orig.__m_shptrSido;
        this->__m_strName = orig.__m_strName;
        this-> __m_objType = orig.__m_objType;
        this->__m_MinCrew = orig.__m_MinCrew;
        this->__m_MaxCrew = orig.__m_MaxCrew;
        this->__m_HitPoints = orig.__m_HitPoints;
        this->__m_ShieldPoints = orig.__m_ShieldPoints;
        this->__m_WeaponsSlots = orig.__m_WeaponsSlots;
        this->__m_Mass = orig.__m_Mass;
        this->__m_Force = orig.__m_Force;
        this->__m_Traction = orig.__m_Traction;
        this->__m_Width = orig.__m_Width;
        this->__m_Length = orig.__m_Length;
        this->__m_MaxGradient = orig.__m_MaxGradient;
        this->__m_LandEnabled = orig.__m_LandEnabled;
        this->__m_WaterEnabled = orig.__m_WaterEnabled;
        this->__m_MountingPoint = orig.__m_MountingPoint;
        *this->__m_VBO = *orig.__m_VBO;
}

void sid::object_template::build_template() {
    this->__get_template_values();
    
    this->__get_vbo_data(this->__m_VBO);
    
    if (this->__m_shptrSido->has_child("sub_models")) {
        sid::sido const* sidoSubTemplates = &(*this->__m_shptrSido)["sub_models"];
        std::cout << sidoSubTemplates->get_name() << std::endl;
        for(int iii = 0; iii < sidoSubTemplates->size(); ++iii) {
            
        }
    }
}

char sid::object_template::__get_template_values() {
    this->__m_strName = this->__m_shptrSido->get_name();
    
    this->__m_objType = this->__get_int_val<uchar>(STR_OBJ_TYPE);
    this->__m_MinCrew = this->__get_int_val<uchar>(STR_MIN_CREW);
    this->__m_MaxCrew = this->__get_int_val<uchar>(STR_MAX_CREW);
    this->__m_HitPoints = this->__get_int_val<uchar>(STR_HITPOINTS);
    this->__m_ShieldPoints = this->__get_int_val<uchar>(STR_SHIELDPOINTS);
    this->__m_WeaponsSlots = this->__get_int_val<uchar>(STR_WEPONS_SLOTS);
    
    this->__m_Mass = this->__get_float_val<float>(STR_MASS);
    this->__m_Force = this->__get_float_val<float>(STR_FORCE);
    this->__m_Traction = this->__get_float_val<float>(STR_TRACTION);
    this->__m_Width = this->__get_float_val<float>(STR_WIDTH);
    this->__m_Length = this->__get_float_val<float>(STR_LENGTH);
    this->__m_MaxGradient = this->__get_float_val<float>(STR_MAX_GRAD);
    
    this->__m_LandEnabled = this->__get_bool_val(STR_LAND_ENABLED);
    this->__m_WaterEnabled = this->__get_bool_val(STR_WATER_ENABLED);
    
#ifdef DEBUG
    std::cout << std::endl;
    std::cout << "Object: " << this->__m_strName << std::endl;
    std::cout << "Type: " << static_cast<int>(this->__m_objType) << std::endl;
    std::cout << "Min Crew: " << static_cast<int>(this->__m_MinCrew) << std::endl;
    std::cout << "Max Crew: " << static_cast<int>(this->__m_MaxCrew) << std::endl;
    std::cout << "Hit Points: " << static_cast<int>(this->__m_HitPoints) << std::endl;
    std::cout << "Shield Points: " << static_cast<int>(this->__m_ShieldPoints) << std::endl;
    
    std::cout << "Mass: " << this->__m_Mass << std::endl;
    std::cout << "Force: " << this->__m_Force << std::endl;
    std::cout << "Traction: " << this->__m_Traction << std::endl;
    std::cout << "Width: " << this->__m_Width << std::endl;
    std::cout << "Length: " << this->__m_Length << std::endl;
    std::cout << "Max Gradient: " << this->__m_MaxGradient << std::endl;
    
    std::cout << std::boolalpha;
    std::cout << "Land: " << this->__m_LandEnabled << std::endl;
    std::cout << "Water: " << this->__m_WaterEnabled << std::endl;
    std::cout << std::noboolalpha;
    
    const int LINEWIDTH = 60;
    char hr[LINEWIDTH + 1];
    int iii = 0;
    while (iii < LINEWIDTH) {
        hr[iii] = '-';
        ++iii;
    }
    hr[iii] = '\0';
    std::cout << hr << std::endl;
#endif
    
}

char sid::object_template::__get_vbo_data(sid::vbo* VBO) {
    
    bool blAllOK = false;
    char chVBOIDOK = 0;
    char chParamsOK = 0;
    char chVertsOK = 0;
    char chIndicesOK = 0;
    char chColorsOK = 0;
    char chUVsOK = 0;
    
    std::string strModel = "model";
    std::string strParams = "params";
    std::string strMesh = "mesh";
    
    int vboid;
    int iPrimitive;
    int iColorMode;
    int iNormalsMode;
    float fScale;
    
    sid::t_vertset vertCenter;
    std::vector<sid::t_vertset> vVertMesh;
    std::vector<sid::t_index> vIndices;
    std::vector<sid::t_color> vColors;
    std::vector<sid::t_uv> vUV;

    t_variant varTemp;    
    
    sid::sido const* sidoMesh;    
    
    if (this->__m_shptrSido->has_child(strModel)) {
        sidoMesh = &(*this->__m_shptrSido)[strModel];
        blAllOK = true;
    } else {
        blAllOK = false;
        std::cerr << "Error: Could not find \"model\" section in sido file." << std::endl;
    }

    if (blAllOK) {
        if (sidoMesh->has_value("vboid")) {
            varTemp = sidoMesh->get_value("vboid");
            get_val(vboid, varTemp);
            if (vboid > 0) {
                chVBOIDOK = 1;
            } else {
                chVBOIDOK = -1;
            }
        } else {
            chVBOIDOK = -1;
        }
    }

    (chVBOIDOK == 1) ? blAllOK = true : blAllOK = false;

    if (blAllOK) {
        if (sidoMesh->has_child(strParams)) {
            sid::sido const* sidoParams = &(*sidoMesh)[strParams];

            chParamsOK = __get_vbo_params(sidoParams,
                    iPrimitive,
                    iColorMode,
                    iNormalsMode,
                    fScale,
                    vertCenter);

        } else {
            chParamsOK = -1;
        }
    }

    if (chParamsOK != 1) {
        std::cerr << "Error: Failed to load parameters" << std::endl;
    }

    (chParamsOK == 1) ? blAllOK = true : blAllOK = false;

    if (blAllOK) {
        if (sidoMesh->has_child(strMesh)) {
            sidoMesh = &(*sidoMesh)[strMesh];
            chVertsOK = __get_vbo_verts(sidoMesh, VBO);

            if (chVertsOK == -1) {
                std::cerr << "Error: Failed to load mesh vertices." << std::endl;
            }

            (chVertsOK == 1) ? blAllOK = true : blAllOK = false;

            if (blAllOK) {
                chIndicesOK = __get_vbo_indices(sidoMesh, VBO);
            }

            if (chIndicesOK == -1) {
                std::cerr << "Error: Failed to load mesh indices." << std::endl;
            }

            (chIndicesOK == 1) ? blAllOK = true : blAllOK = false;

            if (blAllOK) {
                if (iColorMode == 0) {
                    chColorsOK = __get_vbo_single_color(sidoMesh, VBO);
                } else if (iColorMode == 1) {
                    chColorsOK = __get_vbo_colors(sidoMesh, VBO);
                }
            }

            if (chColorsOK == -1) {
                std::cerr << "Error: Failed to load mesh colors." << std::endl;
            }

            (chColorsOK == 1) ? blAllOK = true : blAllOK = false;

            if (blAllOK) {
                chUVsOK = __get_vbo_uv(sidoMesh, VBO);
            }

        } else {
            chVertsOK = false;
            std::cerr << "Error: Failed to find mesh object." << std::endl;
        }
    }

    for (int iii = 0; iii < VBO->get_vertset_size(); ++iii) {
        std::cout << VBO->get_vertset(iii)[0] << ", ";
        std::cout << VBO->get_vertset(iii)[1] << ", ";
        std::cout << VBO->get_vertset(iii)[2] << std::endl;
    }

    for (int iii = 0; iii < VBO->get_indeces_size(); ++iii) {
        std::cout << VBO->get_index(iii) << std::endl;
    }
    if (VBO->is_single_color()) {
        std::cout << static_cast<int> (VBO->get_default_colorset()[0]) << ", ";
        std::cout << static_cast<int> (VBO->get_default_colorset()[1]) << ", ";
        std::cout << static_cast<int> (VBO->get_default_colorset()[2]) << ", ";
        std::cout << static_cast<int> (VBO->get_default_colorset()[3]) << std::endl;
    } else {
        for (int iii = 0; iii < VBO->get_colors_size(); ++iii) {
            std::cout << static_cast<int> (VBO->get_color(iii)[0]) << ", ";
            std::cout << static_cast<int> (VBO->get_color(iii)[1]) << ", ";
            std::cout << static_cast<int> (VBO->get_color(iii)[2]) << ", ";
            std::cout << static_cast<int> (VBO->get_color(iii)[3]) << std::endl;
        }
    }

    for (int iii = 0; iii < VBO->get_texcoords_size(); ++iii) {
        std::cout << VBO->get_texset(iii)[0] << ", ";
        std::cout << VBO->get_texset(iii)[1] << std::endl;
    }

    VBO->build_verts_array();
    VBO->make_vbo(GL_STATIC_DRAW);
}

char sid::object_template::__get_vbo_params(sid::sido const* ptr,
        int& primitive,
        int& color_mode,
        int& normals_mode,
        float& scale,
        sid::t_vertset& center) {

    const std::string strPrimitive = "primitive";
    const std::string strColorMode = "color_mode";
    const std::string strNormalsMode = "normals_mode";
    const std::string strScale = "scale";
    const std::string strCenter = "center";

    char chContinue = 0;
    sid::t_variant varTemp;

    if (ptr->has_value(strPrimitive)) {
        std::string strPrimitiveVal;
        varTemp = ptr->get_value(strPrimitive);
        get_val(strPrimitiveVal, varTemp);
        chContinue = 1;
        if (strPrimitiveVal == "POINTS") {
            primitive = 0x0000;
        } else if (strPrimitiveVal == "LINES") {
            primitive = 0x0001;
        } else if (strPrimitiveVal == "LINE_LOOP") {
            primitive = 0x0002;
        } else if (strPrimitiveVal == "LINE_STRIP") {
            primitive = 0x0003;
        } else if (strPrimitiveVal == "TRIANGLES") {
            primitive = 0x0004;
        } else if (strPrimitiveVal == "TRIANGLE_STRIP") {
            primitive = 0x0005;
        } else if (strPrimitiveVal == "TRIANGLE_FAN") {
            primitive = 0x0006;
        } else if (strPrimitiveVal == "QUADS") {
            primitive = 0x0007;
        } else if (strPrimitiveVal == "QUAD_STRIP") {
            primitive = 0x0008;
        } else if (strPrimitiveVal == "POLYGON") {
            primitive = 0x0009;
        } else {
            primitive = 0xFFFF;
            chContinue = -1;
        }
    } else {
        chContinue = -1;
    }

    if (chContinue == 1 && ptr->has_value(strColorMode)) {
        std::string strColorVal;
        varTemp = ptr->get_value(strColorMode);
        get_val(strColorVal, varTemp);

        if (strColorVal == "SINGLE") {
            color_mode = 0x0000;
        } else if (strColorVal == "PERPOINT") {
            color_mode = 0x0001;
        } else if (strColorVal == "PERPOLY") {
            color_mode = 0x0002;
        } else {
            color_mode = 0xFFFF;
            chContinue = -1;
        }
    } else {
        chContinue = -1;
    }

    if (chContinue == 1 && ptr->has_value(strNormalsMode)) {
        std::string strNormalsVal;
        varTemp = ptr->get_value(strNormalsMode);
        get_val(strNormalsVal, varTemp);

        chContinue = 1;
        if (strNormalsVal == "RUNTIME") {
            normals_mode = 0x0000;
        } else if (strNormalsVal == "PRE_PERPOINT") {
            normals_mode = 0x0001;
        } else if (strNormalsVal == "PRE_PERPOLY") {
            normals_mode = 0x0002;
        } else {
            normals_mode = 0xFFFF;
            chContinue = -1;
        }
    } else {
        chContinue = -1;
    }

    if (chContinue == 1 && ptr->has_value(strScale)) {
        float fScaleVal;
        varTemp = ptr->get_value(strScale);
        get_val(scale, varTemp);
    } else {
        scale = 1.0f;
    }

    if (chContinue == 1 && ptr->has_value(strCenter)) {
        std::vector<float> vCenter;
        varTemp = ptr->get_value(strCenter);
        get_val(vCenter, varTemp);

        center[0] = vCenter[0];
        center[1] = vCenter[1];
        center[2] = vCenter[2];
    } else {
        chContinue = -1;
    }

    return chContinue;
}

char sid::object_template::__get_vbo_verts(const sid::sido* ptr,
        sid::vbo * const VBO) {

    std::string strVerts = "vertices";
    std::vector<float> vVertsVals;

    char chContinue = 0;

    if (ptr->has_value(strVerts)) {
        sid::t_variant varTemp = ptr->get_value(strVerts);
        sid::get_val(vVertsVals, varTemp);

        for (int iii = 0; iii < vVertsVals.size(); iii += 3) {
            sid::t_vertset vertTemp(vVertsVals[iii],
                    vVertsVals[iii + 1],
                    vVertsVals[iii + 2]);
            VBO->push_vertset(vertTemp);
        }

        if (VBO->get_vertset_size() > 0) {
            chContinue = 1;
        }

    } else {
        chContinue = -1;
    }

    return chContinue;
}

char sid::object_template::__get_vbo_indices(const sid::sido* ptr,
        sid::vbo * const VBO) {

    std::string strIndices = "indices";
    std::vector<int> vVertsVals;

    char chContinue = 0;

    if (ptr->has_value(strIndices)) {
        sid::t_variant varTemp = ptr->get_value(strIndices);
        sid::get_val(vVertsVals, varTemp);

        for (int iii = 0; iii < vVertsVals.size(); ++iii) {
            VBO->push_index(vVertsVals[iii]);
        }

        if (VBO->get_indeces_size() > 0) {
            chContinue = 1;
        }

    } else {
        chContinue = -1;
    }

    return chContinue;
}

char sid::object_template::__get_vbo_colors(const sid::sido* ptr,
        sid::vbo * const VBO) {

    std::string strColors = "colors";
    std::vector<int> vColorsVals;

    char chContinue = 0;

    if (ptr->has_value(strColors)) {
        sid::t_variant varTemp = ptr->get_value(strColors);
        sid::get_val(vColorsVals, varTemp);

        for (int iii = 0; iii < vColorsVals.size(); iii += 4) {
            sid::t_color colorTemp(vColorsVals[iii],
                    vColorsVals[iii + 1],
                    vColorsVals[iii + 2],
                    vColorsVals[iii + 3]);

            VBO->push_color(colorTemp);
        }

        if (VBO->get_colors_size() > 0) {
            chContinue = 1;
        }

    } else {
        chContinue = -1;
    }

    return chContinue;
}

char sid::object_template::__get_vbo_uv(const sid::sido* ptr,
        sid::vbo * const VBO) {

    std::string strUV = "uv";
    std::vector<float> vUVVals;

    char chContinue = 0;

    if (ptr->has_value(strUV)) {
        sid::t_variant varTemp = ptr->get_value(strUV);
        sid::get_val(vUVVals, varTemp);

        for (int iii = 0; iii < vUVVals.size(); iii += 2) {
            sid::t_uv vertTemp(vUVVals[iii], vUVVals[iii + 1]);

            VBO->push_texset(vertTemp);
        }

        if (VBO->get_texcoords_size() > 0) {
            chContinue = 1;
        }

    } else {
        chContinue = -1;
    }

    return chContinue;
}

char sid::object_template::__get_vbo_single_color(const sid::sido* ptr,
        sid::vbo* VBO) {

    std::string strColors = "colors";
    std::vector<int> vColorsVals;

    char chContinue = 0;

    if (ptr->has_value(strColors)) {
        sid::t_variant varTemp = ptr->get_value(strColors);
        sid::get_val(vColorsVals, varTemp);


        sid::t_color colorTemp(vColorsVals[0],
                vColorsVals[1],
                vColorsVals[2],
                vColorsVals[3]);

        VBO->set_default_color(colorTemp);

        if (VBO->is_single_color() > 0) {
            chContinue = 1;
        }

    } else {
        chContinue = -1;
    }

    return chContinue;
}
