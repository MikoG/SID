/* 
 * File:   object_initialiser.cpp
 * Author: geth2k
 * 
 * Created on March 17, 2015, 9:12 PM
 */

#include "SID_object_template.h"

STATIC_STRING_DEF::STR_OBJ_TYPE = "type";
STATIC_STRING_DEF::STR_MIN_CREW = "min_crew";
STATIC_STRING_DEF::STR_MAX_CREW = "max_crew";
STATIC_STRING_DEF::STR_HITPOINTS = "hitpoints";
STATIC_STRING_DEF::STR_SHIELDPOINTS = "shieldpoints";
STATIC_STRING_DEF::STR_WEPONS_SLOTS = "weapon_slots";
STATIC_STRING_DEF::STR_MASS = "mass";
STATIC_STRING_DEF::STR_FORCE = "force";
STATIC_STRING_DEF::STR_TRACTION = "traction";
STATIC_STRING_DEF::STR_WIDTH = "width";
STATIC_STRING_DEF::STR_LENGTH = "length";
STATIC_STRING_DEF::STR_MAX_GRAD = "max_gradient";
STATIC_STRING_DEF::STR_LAND_ENBALED = "land_enabled";
STATIC_STRING_DEF::STR_WATER_ENABLED = "water_enabled";
STATIC_STRING_DEF::STR_MOUNTING_POINT = "mounting_point";
STATIC_STRING_DEF::STR_VBO = "vbo";

sid::object_template::object_template() {
}

sid::object_template::object_template(const sid::sidof* Sidof) {
    build_template(Sidof);
}

sid::object_template::object_template(const object_template& orig) {
}

sid::object_template::~object_template() {
}

void sid::object_template::build_template(sid::sidof const* Sidof) {
    sid::vbo* vboTemp = new sid::vbo();
    sid::sido const* sidoMesh;

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

    sid::sido const* Sido;
    t_variant varTemp;

    bool blAllOK = false;
    char chVBOIDOK = 0;
    char chParamsOK = 0;
    char chVertsOK = 0;
    char chIndicesOK = 0;
    char chColorsOK = 0;
    char chUVsOK = 0;

    sid::sido const* sidoBase = Sidof->at(0);

    this->__get_template_values(sidoBase);

    if (sidoBase->has_child(strModel)) {
        Sido = &(*sidoBase)[strModel];
        blAllOK = true;
    } else {
        blAllOK = false;
        std::cerr << "Error: Could not find \"model\" section in sido file." << std::endl;
    }

    if (blAllOK) {
        if (Sido->has_value("vboid")) {
            varTemp = Sido->get_value("vboid");
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
        if (Sido->has_child(strParams)) {
            sid::sido const* sidoParams = &(*Sido)[strParams];

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
        if (Sido->has_child(strMesh)) {
            sidoMesh = &(*Sido)[strMesh];
            chVertsOK = __get_vbo_verts(sidoMesh, vboTemp);

            if (chVertsOK == -1) {
                std::cerr << "Error: Failed to load mesh vertices." << std::endl;
            }

            (chVertsOK == 1) ? blAllOK = true : blAllOK = false;

            if (blAllOK) {
                chIndicesOK = __get_vbo_indices(sidoMesh, vboTemp);
            }

            if (chIndicesOK == -1) {
                std::cerr << "Error: Failed to load mesh indices." << std::endl;
            }

            (chIndicesOK == 1) ? blAllOK = true : blAllOK = false;

            if (blAllOK) {
                if (iColorMode == 0) {
                    chColorsOK = __get_vbo_single_color(sidoMesh, vboTemp);
                } else if (iColorMode == 1) {
                    chColorsOK = __get_vbo_colors(sidoMesh, vboTemp);
                }
            }

            if (chColorsOK == -1) {
                std::cerr << "Error: Failed to load mesh colors." << std::endl;
            }

            (chColorsOK == 1) ? blAllOK = true : blAllOK = false;

            if (blAllOK) {
                chUVsOK = __get_vbo_uv(sidoMesh, vboTemp);
            }

        } else {
            chVertsOK = false;
            std::cerr << "Error: Failed to find mesh object." << std::endl;
        }
    }

    for (int iii = 0; iii < vboTemp->get_vertset_size(); ++iii) {
        std::cout << vboTemp->get_vertset(iii)[0] << ", ";
        std::cout << vboTemp->get_vertset(iii)[1] << ", ";
        std::cout << vboTemp->get_vertset(iii)[2] << std::endl;
    }

    for (int iii = 0; iii < vboTemp->get_indeces_size(); ++iii) {
        std::cout << vboTemp->get_index(iii) << std::endl;
    }
    if (vboTemp->is_single_color()) {
        std::cout << static_cast<int> (vboTemp->get_default_colorset()[0]) << ", ";
        std::cout << static_cast<int> (vboTemp->get_default_colorset()[1]) << ", ";
        std::cout << static_cast<int> (vboTemp->get_default_colorset()[2]) << ", ";
        std::cout << static_cast<int> (vboTemp->get_default_colorset()[3]) << std::endl;
    } else {
        for (int iii = 0; iii < vboTemp->get_colors_size(); ++iii) {
            std::cout << static_cast<int> (vboTemp->get_color(iii)[0]) << ", ";
            std::cout << static_cast<int> (vboTemp->get_color(iii)[1]) << ", ";
            std::cout << static_cast<int> (vboTemp->get_color(iii)[2]) << ", ";
            std::cout << static_cast<int> (vboTemp->get_color(iii)[3]) << std::endl;
        }
    }

    for (int iii = 0; iii < vboTemp->get_texcoords_size(); ++iii) {
        std::cout << vboTemp->get_texset(iii)[0] << ", ";
        std::cout << vboTemp->get_texset(iii)[1] << std::endl;
    }

    vboTemp->build_verts_array();
    vboTemp->make_vbo(GL_STATIC_DRAW);
    if (vboTemp->is_vbo_complete()) {
        __m_VBO = vboTemp;
    }
}

char sid::object_template::__get_template_values(const sid::sido* ptr) {

    sid::t_variant varTemp;
    this->__m_strName = ptr->get_name();

    if (ptr->has_value(STR_OBJ_TYPE)) {
        varTemp = ptr->get_value(STR_OBJ_TYPE);
        sid::get_val(this->__m_strName, varTemp);
    } else {
        
    }

    if (ptr->has_value(STR_MIN_CREW)) {
        varTemp = ptr->get_value(STR_MIN_CREW);
        sid::get_val(this->__m_MinCrew, varTemp);
    }

    if (ptr->has_value(STR_MAX_CREW)) {
        varTemp = ptr->get_value(STR_MAX_CREW);
        sid::get_val(this->__m_MaxCrew, varTemp);
    }

    if (ptr->has_value(STR_HITPOINTS)) {
        varTemp = ptr->get_value(STR_HITPOINTS);
        sid::get_val(this->__m_HitPoints, varTemp);
    }

    if (ptr->has_value(STR_SHIELDPOINTS)) {
        varTemp = ptr->get_value(STR_SHIELDPOINTS);
        sid::get_val(this->__m_ShieldPoints, varTemp);
    }

    if (ptr->has_value(STR_WEPONS_SLOTS)) {
        varTemp = ptr->get_value(STR_WEPONS_SLOTS);
        sid::get_val(this->__m_WeaponsSlots, varTemp);
    }

    if (ptr->has_value(STR_MASS)) {
        varTemp = ptr->get_value(STR_MASS);
        sid::get_val(this->__m_Mass, varTemp);
    }

    if (ptr->has_value(STR_FORCE)) {
        varTemp = ptr->get_value(STR_FORCE);
        sid::get_val(this->__m_Force, varTemp);
    }

    if (ptr->has_value(STR_TRACTION)) {
        varTemp = ptr->get_value(STR_TRACTION);
        sid::get_val(this->__m_Traction, varTemp);
    }

    if (ptr->has_value(STR_WIDTH)) {
        varTemp = ptr->get_value(STR_WIDTH);
        sid::get_val(this->__m_Width, varTemp);
    }

    if (ptr->has_value(STR_LENGTH)) {
        varTemp = ptr->get_value(STR_LENGTH);
        sid::get_val(this->__m_Length, varTemp);
    }

    if (ptr->has_value(STR_MAX_GRAD)) {
        varTemp = ptr->get_value(STR_MAX_GRAD);
        sid::get_val(this->__m_MaxGradient, varTemp);
    }

    if (ptr->has_value(STR_LAND_ENBALED)) {
        varTemp = ptr->get_value(STR_LAND_ENBALED);
        sid::get_val(this->__m_LandEnabled, varTemp);
    }

    if (ptr->has_value(STR_WATER_ENABLED)) {
        varTemp = ptr->get_value(STR_WATER_ENABLED);
        sid::get_val(this->__m_WaterEnabled, varTemp);
    }
    
    
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
