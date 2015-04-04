/* 
 * File:   sidof.cpp
 * Author: geth2k
 * 
 * Created on 04 May 2014, 14:52
 */

#include <algorithm>

#include "SID_sidof.h"

sid::sidof::sidof(std::string filepath) :
__m_strFielPath(filepath) {

    std::string strContents;
    sid::file::file_to_memory(__m_strFielPath, strContents);

    byte* bytPrev = __m_Bytes[0];
    return_codes rcPrev = return_codes::ILLEGAL;
    byte* bytPrevNWS = __m_Bytes[0];
    return_codes rcPrevNWS = return_codes::ILLEGAL;

    bool blStringOpen = false;
    bool blArrayOpen = false;

    objects::t_component component;
    objects::t_components vComponents;
    int iComponentCount = 0;
    bool blCommentOpen = false;
    const char LINE_FEED = 10;
    const char CARRIAGE_RETURN = 13;
    
    int i = 0;
    while (i < strContents.size()) {
        char cursorValue = strContents[i];
        byte* cursorByteVal = __m_Bytes[strContents[i]];
        return_codes cursorRetCode =
                cursorByteVal->get_return_code(rcPrev, rcPrevNWS, blArrayOpen);

        if (cursorRetCode == return_codes::STRING_DELIMITER) {
            if (blStringOpen)
                blStringOpen = false;
            else
                blStringOpen = true;

            if (iComponentCount > 0) {
                component.compType = rcPrevNWS;
                vComponents.push_back(component);
                component.strComponent.clear();
            }
            component.strComponent += cursorValue;
            iComponentCount++;

        } else if (cursorRetCode == return_codes::COMMENT || blCommentOpen) {
            if (cursorRetCode == return_codes::COMMENT) {
                blCommentOpen = true;
            } else if (cursorRetCode == return_codes::WHITESPACE) {
                if (cursorValue == LINE_FEED || 
                        cursorValue == CARRIAGE_RETURN) {
                    
                    blCommentOpen = false;
                }
            }
        } else {
            if (!blStringOpen) {
                if (cursorRetCode == return_codes::ILLEGAL) {
                    throw std::runtime_error("Illegal character in file");
                }
                if (cursorRetCode != rcPrevNWS &&
                        cursorRetCode != return_codes::WHITESPACE) {

                    if (iComponentCount > 0) {
                        if (component.strComponent == "true" ||
                                component.strComponent == "false") {
                            rcPrevNWS = return_codes::BOOLEAN;
                        }
                        component.compType = rcPrevNWS;
                        vComponents.push_back(component);
                        component.strComponent.clear();
                    }
                    iComponentCount++;
                }

                if (cursorRetCode != return_codes::WHITESPACE) {
                    component.strComponent += cursorValue;
                }

            } else if (blStringOpen) {
                if (std::isprint(cursorValue)) {
                    cursorRetCode = return_codes::STRING_TEXT;

                    if (cursorRetCode != rcPrevNWS) {
                        if (iComponentCount > 0) {
                            component.compType = rcPrevNWS;
                            vComponents.push_back(component);
                            component.strComponent.clear();
                        }
                        iComponentCount++;
                    }

                    component.strComponent += cursorValue;

                } else {
                    throw std::runtime_error(
                            "Non printable character in string");
                }
            }
        }

        if (cursorRetCode != return_codes::COMMENT && !blCommentOpen) {
            bytPrev = cursorByteVal;
            rcPrev = cursorRetCode;
            if (cursorRetCode != return_codes::WHITESPACE) {
                bytPrevNWS = cursorByteVal;
                rcPrevNWS = cursorRetCode;
            }

            switch (cursorRetCode) {
                case return_codes::BLOCK_START:
                case return_codes::LINE_END:
                case return_codes::BLOCK_END:
                case return_codes::ARRAY_START:
                case return_codes::ARRAY_END:
                case return_codes::LIST_SEPERATOR:

                    if (iComponentCount > 0) {
                        component.compType = cursorRetCode;
                        vComponents.push_back(component);
                        component.strComponent.clear();
                    }

                    this->__m_Objects.process_components(vComponents);

                    bytPrev = __m_Bytes[0];
                    rcPrev = return_codes::ILLEGAL;
                    bytPrevNWS = __m_Bytes[0];
                    rcPrevNWS = return_codes::ILLEGAL;

                    component.strComponent.clear();
                    vComponents.clear();
                    iComponentCount = 0;

                    if (cursorRetCode == return_codes::ARRAY_START) {
                        blArrayOpen = true;
                    } else if (cursorRetCode == return_codes::ARRAY_END) {
                        blArrayOpen = false;
                    }

                    break;
            }
        }
        i++;
    };
}

sid::sidof::sidof(const sidof& orig) :
__m_strFielPath(orig.__m_strFielPath),
__m_Objects(orig.__m_Objects) {
}

sid::sidof::~sidof() {
}

void sid::objects::process_components(t_components const& vcomp) {
//    for (int i = 0; i < vcomp.size(); i++) {
//        std::cout << vcomp[i].strComponent << std::endl;
//    }        

    std::string strLValue;
    t_variant varRValue;
    bool blStringOpen = false;

    if (this->__m_blArrayMode && !this->__m_blArrayCreated) {
        this->__push_array_to_stack(vcomp[0]);
    }

    t_constCompIter itComponentsCurr = vcomp.begin();
    t_constCompIter itComponentsEnd = vcomp.end();
    int iComponentCount = 0;

    for (itComponentsCurr;
            itComponentsCurr != itComponentsEnd;
            itComponentsCurr++) {

        switch (itComponentsCurr->compType) {
            case return_codes::OBJECT_NAME:
                if (this->__validate_object_name(vcomp,
                        itComponentsCurr,
                        iComponentCount)) {

                    strLValue = itComponentsCurr->strComponent;
                }
                break;

            case return_codes::BLOCK_START:
                if (this->__validate_block_start(vcomp,
                        itComponentsCurr,
                        iComponentCount)) {

                    t_variantMap Values;
                    this->__m_vVariantMapStack.push_back(Values);
                    this->__m_vstrNameStack.push_back(strLValue);
                    this->__m_iRecursionLevel++;
                }

                break;

            case return_codes::BLOCK_END:
                if (this->__validate_block_end(vcomp,
                        itComponentsCurr,
                        iComponentCount)) {

                    this->__add_object_to_structure();
                }

                break;

            case return_codes::ARRAY_START:
                if (this->__validate_array_start(vcomp,
                        itComponentsCurr,
                        iComponentCount)) {

                    this->__m_blArrayMode = true;
                    this->__m_blArrayCreated = false;
                    this->__m_strArrayName = strLValue;
                }

                break;

            case return_codes::ARRAY_END:
                if (this->__validate_array_end(vcomp,
                        itComponentsCurr,
                        iComponentCount)) {

                    this->__process_list_separator(varRValue);
                    this->__m_blArrayMode = false;
                    this->__m_strArrayName.clear();
                }

                break;

            case return_codes::ASSIGNMENT:
                this->__validate_assignment_operator(vcomp,
                        itComponentsCurr,
                        iComponentCount);

                break;

            case return_codes::STRING_DELIMITER:
                if (blStringOpen) {
                    if (this->__validate_string_close(vcomp,
                            itComponentsCurr,
                            iComponentCount)) {

                        varRValue = (itComponentsCurr - 1)->strComponent;
                    }
                    blStringOpen = false;
                } else {
                    if (this->__validate_string_open(vcomp,
                            itComponentsCurr,
                            iComponentCount)) {

                        blStringOpen = true;
                    }
                }

                break;

            case return_codes::STRING_TEXT:

                break;

            case return_codes::NUMBER:
                if (this->__validate_number(vcomp,
                        itComponentsCurr,
                        iComponentCount)) {

                    int iNumberType = this->__is_float(
                            itComponentsCurr->strComponent);

                    if (iNumberType == 1) {
                        varRValue = std::stof(itComponentsCurr->strComponent);
                    } else if (iNumberType == 0) {
                        varRValue = std::stoi(itComponentsCurr->strComponent);
                    } else {
                        throw std::runtime_error(
                                "Error: Number contains illegal character."
                                );

                    }
                }

                break;

            case return_codes::BOOLEAN:
                if (this->__validate_boolean(vcomp,
                        itComponentsCurr,
                        iComponentCount)) {
                    
                    bool blValue;
                    if (itComponentsCurr->strComponent == "true") {
                        blValue = true;
                    } else if (itComponentsCurr->strComponent == "false") {
                        blValue = false;
                    } else {
                        std::runtime_error(
                                "Internal Error: Boolean value is neither \"true\" nor \"false\"."
                                );

                    }
                    varRValue = blValue;
                }
                break;

            case return_codes::LINE_END:
                if (strLValue.empty()) {
                    std::cout <<
                            "Error: Missing LValue." <<
                            std::endl;
                }
                if (varRValue.which() == variant_types::T_BLANK) {
                    std::cout <<
                            "Error: Missing RValue." <<
                            std::endl;
                }

                this->__m_vVariantMapStack.back()[strLValue] = varRValue;

                break;

            case return_codes::LIST_SEPERATOR:
                if (varRValue.which() != variant_types::T_BLANK)
                    this->__process_list_separator(varRValue);

                break;

        }

        iComponentCount++;
    }
}

bool sid::objects::__validate_object_name(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    int iSize = vComponents.size();
    if (count != 0) {
        throw std::runtime_error(
                "Error: Object names can be l_values only."
                );

    } else if (iSize > 1 && count == 0) {
        t_constCompIter itNext = curr + 1;
        if (itNext->compType != return_codes::BLOCK_START &&
                itNext->compType != return_codes::ASSIGNMENT) {
            throw std::runtime_error(
                    "Error: Object name must be followed by open braces or assignment operator."
                    );
        }
    }
    return true;
}

bool sid::objects::__validate_block_start(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    if (count > 0) {
        t_constCompIter itPrev = curr - 1;
        if (itPrev->compType != return_codes::OBJECT_NAME) {
            throw std::runtime_error(
                    "Error: Open braces must follow an object name."
                    );

        }
    } else {
        throw std::runtime_error(
                "Error: Expected identifier before open braces."
                );
    }

    return true;
}

bool sid::objects::__validate_block_end(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    int iSize = vComponents.size();
    if (iSize != 1) {
        throw std::runtime_error(
                "Error: Unexpected token found before close braces."
                );

    }

    return true;
}

bool sid::objects::__validate_array_start(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    int iSize = vComponents.size();
    if (this->__m_blArrayMode) {
        throw std::runtime_error(
                "Error: Cannot create nested arrays."
                );

    }

    bool blMissingAssign = false;
    if (iSize == 1) {
        blMissingAssign = true;
    } else if (count > 0) {
        t_constCompIter itPrev = curr - 1;
        if (itPrev->compType != return_codes::ASSIGNMENT) {
            blMissingAssign = true;

        }
    }
    if (blMissingAssign) {
        throw std::runtime_error(
                "Error: Expected assignment operator before array start."
                );

    }

    return true;
}

bool sid::objects::__validate_array_end(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    if (!this->__m_blArrayMode) {
        throw std::runtime_error(
                "Error: Attempted to close array when no open array was found."
                );

    }

    return true;
}

bool sid::objects::__validate_assignment_operator(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    int iSize = vComponents.size();
    if (count > 0) {
        t_constCompIter itPrev = curr - 1;
        if (itPrev->compType != return_codes::OBJECT_NAME) {
            throw std::runtime_error(
                    "Error: Assignment operator must follow an object name (LValue)."
                    );

        }
    } else if (count == 0) {
        throw std::runtime_error(
                "Error: Expected identifier before assignment operator."
                );
    }

    return true;
}

bool sid::objects::__validate_string_open(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    if (count > 0) {
        t_constCompIter itPrev = curr - 1;

        if (this->__m_blArrayMode) {
            throw std::runtime_error(
                    "Error: Unexpected identifier before string in array mode."
                    );

        } else {
            if (itPrev->compType != return_codes::ASSIGNMENT) {
                throw std::runtime_error(
                        "Error: Expected assignment operator before string."
                        );

            }
        }
    } else {
        if (!this->__m_blArrayMode) {
            throw std::runtime_error(
                    "Error: Expected assignment operator before string."
                    );

        }
    }

    int iSize = vComponents.size();
    int iMovesToEnd = iSize - count - 1;
    if (iMovesToEnd > 0) {
        if ((curr + 1)->compType != return_codes::STRING_TEXT) {
            std::cout <<
                    "Internal Error: Opening double quotes must be followed by text. " <<
                    std::endl;
        }
    }
    if (iMovesToEnd > 1) {
        if ((curr + 2)->compType != return_codes::STRING_DELIMITER) {
            std::cout <<
                    "Error: String can only be used in assignment. " <<
                    "Expected preceeding \"=\"." <<
                    std::endl;
        }
    }

    return true;
}

bool sid::objects::__validate_string_close(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    if (count > 0) {
        t_constCompIter itPrev = curr - 1;
        if (itPrev->compType != return_codes::STRING_TEXT) {
            throw std::runtime_error(
                    "Error: Double quotes can only contain string data."
                    );

        }
    }
    return true;
}

bool sid::objects::__validate_number(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    if (count > 0) {
        t_constCompIter itPrev = curr - 1;
        if (itPrev->compType != return_codes::ASSIGNMENT) {
            throw std::runtime_error(
                    "Error: Unexpected identifier before number in array mode."
                    );

        }
    } else {
        if (!this->__m_blArrayMode) {
            throw std::runtime_error(
                    "Error: Expected assignment operator before number."
                    );
            
        }
    }

    return true;
}

bool sid::objects::__validate_boolean(t_components const& vComponents,
        t_constCompIter const& curr,
        int const count) const {

    if (count > 0) {
        t_constCompIter itPrev = curr - 1;
        if (itPrev->compType != return_codes::ASSIGNMENT) {
            throw std::runtime_error(
                    "Error: Unexpected identifier before boolean in array mode."
                    );

        }
    } else {
        if (!this->__m_blArrayMode) {
            throw std::runtime_error(
                    "Error: Expected assignment operator before boolean."
                    );
            
        }
    }
    
    return true;
}

void sid::objects::__push_array_to_stack(t_component const& Component) {
    int iNumberType;
    std::vector<t_variantMap>::reference back =
            this->__m_vVariantMapStack.back();
    switch (Component.compType) {
        case return_codes::NUMBER:
            iNumberType = this->__is_float(Component.strComponent);
            if (iNumberType == 1) {
                back[this->__m_strArrayName] =
                        boost::variant<std::vector<float> >();

            } else if (iNumberType == 0) {
                back[this->__m_strArrayName] =
                        boost::variant<std::vector<int> >();

            } else {
                throw std::runtime_error(
                        "Error: Illegal character found in number."
                        );

            }
            this->__m_blArrayCreated = true;

            break;

        case return_codes::BOOLEAN:
            back[this->__m_strArrayName] =
                    boost::variant < std::vector<bool> >();

            this->__m_blArrayCreated = true;

            break;

        case return_codes::STRING_DELIMITER:
            back[this->__m_strArrayName] =
                    boost::variant<std::vector<std::string> >();

            this->__m_blArrayCreated = true;

            break;

        default:
            throw std::runtime_error(
                    "Error: Arrays can only be of types int, float, bool or std::string"
                    );
    }
    if (this->__m_blArrayCreated)
        this->__m_ArrayType = static_cast<variant_types> (
            back[this->__m_strArrayName].which());
}

void sid::objects::__process_list_separator(t_variant const& RValue) {
    if (RValue.which() == variant_types::T_BLANK) {
        throw std::runtime_error(
                "Error: Attempted to assign empty value to array."
                );

    }

    if (!this->__m_blArrayMode) {
        std::cout << "Error: List separator can only be used in arrays." <<
                std::endl;
    } else {
        std::vector<t_variantMap>::reference back =
                this->__m_vVariantMapStack.back();

        switch (this->__m_ArrayType) {
            case variant_types::T_VEC_BOOL:
                sid::push_array_value(
                        boost::get<bool>(RValue),
                        back[this->__m_strArrayName]);

                break;

            case variant_types::T_VEC_FLOAT:
                sid::push_array_value(
                        boost::get<float>(RValue),
                        back[this->__m_strArrayName]);

                break;

            case variant_types::T_VEC_INT:
                sid::push_array_value(
                        boost::get<int>(RValue),
                        back[this->__m_strArrayName]);

                break;

            case variant_types::T_VEC_STRING:
                sid::push_array_value(
                        boost::get<std::string>(RValue),
                        back[this->__m_strArrayName]);

                break;

            default:

                break;

        }
    }
}

void sid::objects::__add_object_to_structure() {
    if (this->__m_iRecursionLevel > 0) {

        t_sidoPair sidoPairTemp;
        sidoPairTemp.m_Level = __m_iRecursionLevel;
        sidoPairTemp.m_Sido = std::make_shared<sido>(
                sido(this->__m_vstrNameStack.back(),
                this->__m_vVariantMapStack.back()));

        this->__m_vstrNameStack.pop_back();
        this->__m_vVariantMapStack.pop_back();

        while (this->__m_vSidoPairStack.size() > 0 &&
                this->__m_vSidoPairStack.back().m_Level >
                this->__m_iRecursionLevel) {

            sidoPairTemp.m_Sido->add_child(
                    this->__m_vSidoPairStack.back().m_Sido);

            this->__m_vSidoPairStack.back().m_Sido->set_parent(
                    sidoPairTemp.m_Sido);

            this->__m_vSidoPairStack.pop_back();
        }

        this->__m_vSidoPairStack.push_back(sidoPairTemp);

        if (this->__m_iRecursionLevel == 1) {
            this->add_child(sidoPairTemp.m_Sido);
        }

        this->__m_iRecursionLevel--;
    } else {
        std::cout <<
                "Internal Error: Extra Close Block \"}\" found." <<
                std::endl;
    }
}

int sid::objects::__is_float(std::string const& strNumber) const {
    int iRetVal = 0;

    for (std::string::const_iterator itString =
            strNumber.begin();
            itString != strNumber.end();
            itString++) {
        if (!std::isdigit(*itString)) {
            if (strcmp(&*itString, ".")) {
                iRetVal = 1;
            } else {
                iRetVal = -1;
            }
        }
    }
    return (iRetVal);
}