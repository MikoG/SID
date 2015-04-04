/* 
 * File:   display.cpp
 * Author: geth2k
 * 
 * Created on 05 May 2013, 17:05
 */

#include "SID_display.h"

sid::display::display() :
    __m_Window(nullptr),
    __m_displayIndex(-1),
    __m_strTitle("") {
}

sid::display::display(const std::string title, 
        int width, 
        int height, 
        int refreshRate, 
        bool fullscreen) :
    __m_Window(nullptr),
    __m_displayIndex(-1),
    __m_strTitle("") {
    
    this->_init_window(title, width, height, refreshRate, fullscreen);
}

sid::display::display(const display& orig) {
}

sid::display::~display() {
    close();
}

bool sid::display::_init_window(const std::string title, 
        int width, 
        int height, 
        int refreshRate, 
        bool fullscreen) {

    this->__m_displayIndex = 0;
    
    SDL_DisplayMode RequestedMode;
    Uint32 uiScreenFlags = 0;
    int iPosX, iPosY;
    
    RequestedMode.format = 0;
    RequestedMode.w = width;
    RequestedMode.h = height;
    RequestedMode.refresh_rate = refreshRate;
    
    if (fullscreen) {
        if (SDL_GetNumVideoDisplays() > 1) {
            this->__m_Window = SDL_CreateWindow(title.c_str(),
                    SDL_WINDOWPOS_CENTERED,
                    SDL_WINDOWPOS_CENTERED,
                    100, 100,
                    SDL_WINDOW_BORDERLESS);
            if (this->__m_Window != nullptr) {
                this->__m_displayIndex = SDL_GetWindowDisplayIndex(this->__m_Window);
                SDL_DestroyWindow(this->__m_Window);
            } else {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
                        "Display Error",
                        "Unable to create test window.",
                        NULL);
                
                return false;
            }
        }
        
        if (SDL_GetClosestDisplayMode(this->__m_displayIndex, 
                &RequestedMode, 
                &this->__m_Mode) == nullptr) {
        
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
                    "Display Error",
                    "Unable to find an appropriate full screen display mode.\nPossibly because the requested dimensions are too large",
                    NULL);
            return false;
        }
        
        uiScreenFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
        iPosX = SDL_WINDOWPOS_UNDEFINED_MASK | this->__m_displayIndex;
        iPosY = SDL_WINDOWPOS_UNDEFINED_MASK | this->__m_displayIndex;
    } else {
        this->__m_Mode = RequestedMode;
        uiScreenFlags = SDL_WINDOW_OPENGL;
        iPosX = SDL_WINDOWPOS_CENTERED_MASK | this->__m_displayIndex;
        iPosY = SDL_WINDOWPOS_CENTERED_MASK | this->__m_displayIndex;
    }
    
    std::string strRatio;
    for (int i = 0; i < SDL_GetNumDisplayModes(this->__m_displayIndex); i++) {
        SDL_DisplayMode possible_mode;
        SDL_GetDisplayMode(this->__m_displayIndex, i, &possible_mode);
        
        float iScreenRatio = (float)possible_mode.h / (float)possible_mode.w;
        int iNumerator = 1;
        int iDenominator = 0;
        float fDenominator = 0;
        while (true) {
            fDenominator = iScreenRatio * iNumerator;
            iDenominator = fDenominator;
            if (iDenominator == fDenominator) {  
                strRatio = std::to_string(iNumerator) + 
                        ":" + 
                        std::to_string(iDenominator);
                
                break;
            }
            if (iNumerator > possible_mode.w) {
                strRatio = "#Err";
                break;
            }
            iNumerator ++;   
        }      
        std::cout << i << ": " << possible_mode.format << ", " << 
                possible_mode.w << " x " << possible_mode.h << 
                " (" << strRatio << ")" << ", " << 
                possible_mode.refresh_rate << std::endl;
    }
    
    this->__m_strTitle = title;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    this->__m_Window = SDL_CreateWindow(title.c_str(),
            iPosX,
            iPosY,
            this->__m_Mode.w, this->__m_Mode.h,
            uiScreenFlags);
    if (this->__m_Window == nullptr) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
                "Display Error",
                "Unable to create a window.",
                NULL);
        
        return false;
    } else {                 
        this->__m_GLContext = SDL_GL_GetCurrentContext();    
    }
    
    if (SDL_SetWindowDisplayMode(this->__m_Window, &this->__m_Mode) != 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
                "Display Error",
                "Unable to set a display mode.",
                NULL);
        
        return false;
    }

    SDL_Renderer* Renderer = SDL_CreateRenderer(this->__m_Window, 
            -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (Renderer == nullptr) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
                "Display Error",
                "Unable to create renderer.",
                NULL);
        
        return false;
    }
    
    int iDB;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &iDB);
    std::cout << "Double Buffer Value: " << iDB << std::endl;   

    return true;
}

void sid::display::swap_buffers() {
//    glFlush();
//    glFinish();
    SDL_GL_SwapWindow(this->__m_Window);
}