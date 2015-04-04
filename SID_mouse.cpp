/* 
 * File:   mouse.cpp
 * Author: geth2k
 * 
 * Created on 01 January 2014, 17:44
 */

#include "SID_mouse.h"

sid::mouse::mouse() : m_mbLastPressed(NONE_BTN) {
     sid::mouse::mouse_action::iLastButton = &this->m_mbLastPressed;
     for (int i = 0; i < this->m_Buttons.size(); i++) {
        this->m_Buttons[i].m_ButtonEnum = static_cast<sid::mouse::eMouseButtons>(i);
     }
}

sid::mouse::mouse(const mouse& orig) {
    this->m_Buttons = orig.m_Buttons;
}

sid::mouse::~mouse() {
}

sid::mouse::mouse_action* sid::mouse::operator [](const int index) {
    return &this->m_Buttons[index];
}

bool sid::mouse::mouse_action::press(int time) {
    this->m_blPressed = true;
    bool blRetVal = false;
    int iDblClickTime = time - this->lastPressTime;
    if (iDblClickTime <= sid::mouse::s_iDblClickTime) {
        blRetVal = true;
    }
    *sid::mouse::mouse_action::iLastButton = this->m_ButtonEnum;
    this->lastPressTime = time;
    return blRetVal;
}

bool sid::mouse::mouse_action::release(int time) {
    this->m_blPressed = false;
    bool blRetVal = false;
    int iDragTime = time - this->lastPressTime;
    if (iDragTime > sid::mouse::s_iDragClickSpeed) {
        blRetVal = true;
    }
    this->lastReleaseTime = time;
    return blRetVal;
}

bool sid::mouse::mouse_action::move(int time) {
    int iTimeSincePress = time - this->lastPressTime;
    if (this->m_blPressed)
        if (iTimeSincePress > sid::mouse::s_iDragClickSpeed)
            return true;
    return false;
}

bool sid::mouse::mouse_action::is_pressed() const {
    return m_blPressed;
}

int sid::mouse::mouse_action::get_last_pressed_time() const {
    return lastPressTime;
}

int sid::mouse::mouse_action::get_last_release_time() const {
    return lastReleaseTime;
}

bool sid::mouse::timed_cursor_changes(double td, SDL_SystemCursor cidLMB, 
            SDL_SystemCursor cidRMB, bool force) {
    static double sdblLMBPressDuration = 0.0;
    static double sdblRMBPressDuration = 0.0;
    
    SDL_SystemCursor cidTemp = sid::mouse::s_eCursorID;

    if (this->m_Buttons[sid::mouse::eMouseButtons::LEFT_BTN].is_pressed()) {
        sdblLMBPressDuration += (td * 1000);
    } else {
        sdblLMBPressDuration = 0;
    }
    if (this->m_Buttons[sid::mouse::eMouseButtons::RGHT_BTN].is_pressed()) {
        sdblRMBPressDuration += (td * 1000);
    } else {
        sdblRMBPressDuration = 0;
    }
    
    if ((sdblRMBPressDuration >= sid::mouse::s_iDragClickSpeed ||
            sdblRMBPressDuration > 0 && force) &&
            sid::mouse::s_eCursorID != cidLMB && 
            sid::mouse::s_eCursorID != cidRMB) {
        cidTemp = cidRMB;
    }
    if ((sdblLMBPressDuration >= sid::mouse::s_iDragClickSpeed ||
            sdblLMBPressDuration > 0 && force) && 
            sid::mouse::s_eCursorID != SDL_SYSTEM_CURSOR_HAND) {
        cidTemp = cidLMB;
    }   
    
    if (cidTemp != sid::mouse::s_eCursorID) {
        sid::mouse::set_cursor(cidTemp);
    }
    
  
}

void sid::mouse::set_cursor(SDL_SystemCursor id) {
    SDL_Cursor* cursor;
    cursor = SDL_CreateSystemCursor(id);
    SDL_SetCursor(cursor);
}

sid::mouse::eMouseButtons sid::mouse::get_prev_button_id() const {
    return m_mbLastPressed;
}

sid::mouse::eMouseButtons* sid::mouse::mouse_action::iLastButton = nullptr;
int sid::mouse::s_iDblClickTime = 300;
int sid::mouse::s_iDragClickSpeed = 350;

SDL_SystemCursor sid::mouse::s_eCursorID = SDL_SYSTEM_CURSOR_ARROW;