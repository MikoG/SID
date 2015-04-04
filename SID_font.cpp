/* 
 * File:   font.cpp
 * Author: geth2k
 * 
 * Created on 17 November 2013, 17:58
 */

#include "SID_font.h"

sid::font::font() :
__m_gluiTexID(0),
__m_iTexHeight(0),
__m_iTexWidth(0) {

}

sid::font::font(std::string path, uint fontID, uint texID) :
__m_gluiTexID(texID),
__m_iTexHeight(0),
__m_iTexWidth(0) {

    this->__setup_font(path, fontID, texID);
}

sid::font::font(const font& orig) :
__m_vFontMap(orig.__m_vFontMap),
__m_gluiTexID(orig.__m_gluiTexID),
__m_iTexHeight(orig.__m_iTexHeight),
__m_iTexWidth(orig.__m_iTexWidth) {
    
}

sid::font::~font() {
}

void sid::font::__setup_font(std::string path, uint fontID, uint texID) {
    std::string memblock;
    sid::file::file_to_memory(path, memblock);
    int iBlockSize = memblock.size();

    int iCursorPos = 0;
    int iGlyphSection = 0;
    int iIntCount = 0;

    const int CHAR_SIZE = 8;
    const int INT_SIZE = sizeof (int32_t) * CHAR_SIZE;

    this->__m_vFontMap.resize(256, glyph());

    while (iCursorPos < iBlockSize) {
        int iBitShiftVal = INT_SIZE;
        int32_t iCursorVal = 0;
        for (int iIntBytePos = 0; iIntBytePos < 4; iIntBytePos++) {
            iBitShiftVal -= CHAR_SIZE;
            int iCursorVal_temp = static_cast<uint8_t> (memblock[iCursorPos]);
            iCursorVal_temp <<= iBitShiftVal;
            iCursorVal |= iCursorVal_temp;
            iCursorPos++;
        }

        if (iIntCount < 2) {
            switch (iIntCount) {
                case 0:
                    this->__m_iTexWidth = iCursorVal;
                    break;
                case 1:
                    this->__m_iTexHeight = iCursorVal;
                    break;
            }
        } else {
            int iGlyphVal;
            int iGlyphTop;
            int iGlyphLeft;
            int iGlyphHeight;
            int iGlyphWidth;

            switch (iGlyphSection) {
                case 0:
                    iGlyphVal = iCursorVal;
                    break;
                case 1:
                    iGlyphTop = iCursorVal;
                    break;
                case 2:
                    iGlyphLeft = iCursorVal;
                    break;
                case 3:
                    iGlyphHeight = iCursorVal;
                    break;
                case 4:
                    iGlyphWidth = iCursorVal;
                    //this->__m_vFontMap[iGlyphVal] = glp;                    
                    this->__m_vFontMap[iGlyphVal].top = iGlyphTop;
                    this->__m_vFontMap[iGlyphVal].left = iGlyphLeft;
                    this->__m_vFontMap[iGlyphVal].height = iGlyphHeight;
                    this->__m_vFontMap[iGlyphVal].width = iGlyphWidth;
                    iGlyphSection = -1;
                    break;
            }
            iGlyphSection++;            
        }
        iIntCount++;
    }
}