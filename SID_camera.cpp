/* 
 * File:   camera.cpp
 * Author: geth2k
 * 
 * Created on 15 June 2013, 23:49
 */

#include "SID_camera.h"

sid::keyboard* sid::camera::Keyboard = nullptr;

sid::camera::camera(float defaultX, float defaultY, float defaultZ, 
        float minY, float maxY) :
__m_fCamOffsetX_Def(defaultX),
__m_fCamOffsetY_Def(defaultY),
__m_fCamOffsetZ_Def(defaultZ),
__m_fCamOffsetY_Min(minY),
__m_fCamOffsetY_Max(maxY),
fFocusOffsetY(0.0),
m_vtxPosition(),
m_vtxCamera(),
m_vctFocusVect(),
m_GestureMode(sid::camera::eGestureModes::NONE),
Mouse() {

    this->m_vtxPosition.move(1000.0, 150, 1000.0);
    this->m_vtxCamera = m_vtxPosition;
    this->m_vtxCamera.move(__m_fCamOffsetX_Def, __m_fCamOffsetY_Def, __m_fCamOffsetZ_Def);
    this->_set_focus_dist();

    this->Actions.forward = 25;
    this->Actions.back = 39;
    this->Actions.left = 38;
    this->Actions.right = 40;
    this->Actions.zoomIn = 111;
    this->Actions.zoomOut = 116;
    this->Actions.rotateLeft = 113;
    this->Actions.rotateRight = 114;
    this->Actions.cameraUp = 112;
    this->Actions.cameraDown = 117;
    this->Actions.quickGrabToggle = 50;
    this->Actions.attackClickToggle = 37;
}

sid::camera::camera(const camera& orig) : Mouse(orig.Mouse) {
    this->m_vtxPosition = orig.m_vtxPosition;
    this->m_vtxCamera = orig.m_vtxCamera;
}

sid::camera::~camera() {

}

void sid::camera::process_keyboard(double timeDiff) {
    double dblMoveSpeed = 50.0;
    double dblClimbSpeed = 50.0 * timeDiff;
    double dblRotateSpeed = 45.0 * timeDiff;
    double dblZoomSpeed = 70.0 * timeDiff;
    int iKeyPressCount = 0;

    SDL_PumpEvents();
    const Uint8* KeyState = SDL_GetKeyboardState(NULL);

    if (KeyState[SDL_SCANCODE_S] || KeyState[SDL_SCANCODE_W]) {
        iKeyPressCount++;
    }

    if (KeyState[SDL_SCANCODE_A] || KeyState[SDL_SCANCODE_D]) {
        iKeyPressCount++;
    }
    if (iKeyPressCount == 1) {
        dblMoveSpeed = dblMoveSpeed * timeDiff;
    } else {
        dblMoveSpeed = dblMoveSpeed * 0.707 * timeDiff;
    }

    if (KeyState[SDL_SCANCODE_W]) {
        move(0.0f, dblMoveSpeed);
    } else if (KeyState[SDL_SCANCODE_S]) {
        move(180.0f, dblMoveSpeed);
    }

    if (KeyState[SDL_SCANCODE_A]) {
        move(90.0f, dblMoveSpeed);
    } else if (KeyState[SDL_SCANCODE_D]) {
        move(270.0f, dblMoveSpeed);
    }

    if (KeyState[SDL_SCANCODE_LEFT]) {
        this->m_vtxCamera.rotate(m_vtxPosition, -dblRotateSpeed, 0.0, 0.0);
        this->_set_focus_dist();
    } else if (KeyState[SDL_SCANCODE_RIGHT]) {
        this->m_vtxCamera.rotate(m_vtxPosition, dblRotateSpeed, 0.0, 0.0);
        this->_set_focus_dist();
    }

    if (KeyState[SDL_SCANCODE_PAGEDOWN] &&
            this->__m_fDistance < 300) {
        this->m_vtxCamera.move(0.0, dblClimbSpeed, 0.0);
        this->_set_focus_dist();
    } else if (KeyState[SDL_SCANCODE_PAGEUP] &&
            this->__m_fDistance > 10 &&
            this->__m_fDistance < 300) {
        this->m_vtxCamera.move(0.0, -dblClimbSpeed, 0.0);
        this->_set_focus_dist();
    }

    if (KeyState[SDL_SCANCODE_UP] &&
            this->__m_fDistance > 10 &&
            this->__m_fHorizDistance > 10) {
        this->zoom(dblZoomSpeed);
    } else if (KeyState[SDL_SCANCODE_DOWN] &&
            this->__m_fDistance < 300) {
        this->zoom(-dblZoomSpeed);
    }
}

void sid::camera::process_mouse(SDL_Event& evt) {
    const double dblZoomSpeed = 10.0;
    if (evt.type == SDL_MOUSEWHEEL) {
        this->zoom(dblZoomSpeed * evt.wheel.y);
    }
    this->_gesture_rotate(evt);
    this->_gesture_grab_terrain(evt);
}

void sid::camera::_gesture_rotate(SDL_Event& evt) {
    screen_coords coordsCursor;
    coordsCursor.x = evt.button.x;
    coordsCursor.y = evt.button.y;
    if (evt.type == SDL_MOUSEBUTTONDOWN &&
            evt.button.button == mouse::eMouseButtons::RGHT_BTN) {

        double dblAngle = this->_get_cursor_angle(coordsCursor, this->m_coordsCenterScreen);
        this->m_dblCursorAngle_Current =
                this->m_dblCursorAngle_Prev =
                360.0 - dblAngle;
        this->m_dblCursorAngle_Delta = 0.0;
    } else if (evt.type == SDL_MOUSEMOTION) {
        if (this->Mouse[mouse::eMouseButtons::RGHT_BTN]->is_pressed()) {
            mouse::set_cursor(SDL_SYSTEM_CURSOR_HAND);
            double dblAngle = this->_get_cursor_angle(coordsCursor, this->m_coordsCenterScreen);

            this->m_dblCursorAngle_Current = 360.0 - dblAngle;
            if ((this->m_dblCursorAngle_Prev < 90.0 &&
                    this->m_dblCursorAngle_Current > 270.0) ||
                    (this->m_dblCursorAngle_Prev > 270.0 &&
                    this->m_dblCursorAngle_Current < 90.0)) {
                this->m_dblCursorAngle_Delta =
                        (this->m_dblCursorAngle_Current - 360.0) -
                        this->m_dblCursorAngle_Prev;
            } else {
                this->m_dblCursorAngle_Delta = this->m_dblCursorAngle_Current -
                        this->m_dblCursorAngle_Prev;
            }
            this->m_dblCursorAngle_Prev = this->m_dblCursorAngle_Current;

            this->m_vtxCamera.rotate(m_vtxPosition,
                    this->m_dblCursorAngle_Delta, 0.0, 0.0);
            this->m_GestureMode = sid::camera::eGestureModes::ROTATE;
            this->_set_focus_dist();
        }
    } else if (evt.type == SDL_MOUSEBUTTONUP) {
        if (evt.button.button == mouse::eMouseButtons::RGHT_BTN) {
            mouse::set_cursor(SDL_SYSTEM_CURSOR_ARROW);
            this->m_GestureMode = sid::camera::eGestureModes::NONE;
        }
    }
}

void sid::camera::_gesture_grab_terrain(SDL_Event& evt) {
    screen_coords coordsCursor;
    coordsCursor.x = evt.button.x;
    coordsCursor.y = evt.button.y;

    sid::triplet<GLdouble> dvObjectCoords;
    dvObjectCoords = sid::get_3d_from_2d(coordsCursor.x, coordsCursor.y, plot::SEA_LEVEL);

    this->m_objVertPrev = sid::get_3d_from_2d(this->m_coordsScreenPrev.x,
            this->m_coordsScreenPrev.y, plot::SEA_LEVEL);

    sid::triplet<GLdouble> dvObjectDelta = dvObjectCoords - m_objVertPrev;

    if (evt.type == SDL_MOUSEBUTTONDOWN &&
            evt.button.button == mouse::eMouseButtons::LEFT_BTN) {

        this->m_coordsScreenPrev = coordsCursor;
    } else if (evt.type == SDL_MOUSEMOTION) {
        if (this->Mouse[mouse::eMouseButtons::LEFT_BTN]->is_pressed()) {
            this->move(-dvObjectDelta[0], -dvObjectDelta[1], -dvObjectDelta[2]);
            this->m_GestureMode = sid::camera::eGestureModes::MOVE;
            this->m_coordsScreenPrev = coordsCursor;
        }
    } else if (evt.type == SDL_MOUSEBUTTONUP) {
        if (evt.button.button == mouse::eMouseButtons::LEFT_BTN) {
            mouse::set_cursor(SDL_SYSTEM_CURSOR_ARROW);
            this->m_GestureMode = sid::camera::eGestureModes::NONE;
        }
    }
}

double sid::camera::_get_cursor_angle(screen_coords cursor, screen_coords ref, double* hyp) {
    screen_coords coordsCursorDelta;
    coordsCursorDelta.x = ref.x - cursor.x;
    coordsCursorDelta.y = ref.y - cursor.y;

    double dblDeltaHypot = hypot(coordsCursorDelta.x, coordsCursorDelta.y);
    if (hyp != nullptr) {
        *hyp = dblDeltaHypot;
    }
    return sid::true_asin(coordsCursorDelta.x, coordsCursorDelta.y, dblDeltaHypot);
}

void sid::camera::set_view() {
    gluLookAt(this->m_vtxCamera[0],
            this->m_vtxCamera[1],
            this->m_vtxCamera[2],
            this->m_vtxPosition[0],
            this->m_vtxPosition[1],
            this->m_vtxPosition[2],
            0.0,
            1.0,
            0.0);
}

void sid::camera::set_center_screen(int width, int height) {
    this->m_coordsCenterScreen.x = width / 2;
    this->m_coordsCenterScreen.y = height / 2;
}

void sid::camera::set_zoom_to(vertex3 vtxZoomTo) {
    this->m_vtxZoomTo = vtxZoomTo;
}

sid::vector3 const& sid::camera::get_focus_vector() const {
    return m_vctFocusVect;
}

float sid::camera::get_horizontal_distance() const {
    return __m_fHorizDistance;
}

float sid::camera::get_distance() const {
    return __m_fDistance;
}

sid::vertex3 const& sid::camera::get_camera_pos() const {
    return m_vtxCamera;
}

void sid::camera::move(float fValue_X, float fValue_Y, float fValue_Z) {
    this->m_vtxPosition.move(fValue_X, fValue_Y, fValue_Z);
    this->m_vtxCamera.move(fValue_X, fValue_Y, fValue_Z);
    
    if (this->m_vtxCamera.data()[euclid::DIMENSION_Y] < this->__m_fCamOffsetY_Min) {
        this->m_vtxCamera.data()[euclid::DIMENSION_Y] = this->__m_fCamOffsetY_Min;
    }
    
    this->_set_focus_dist();
}

void sid::camera::move(double offsetAngle, double dblMag) {
    vertex3 vctCamFocus = vertex3();
    vctCamFocus = this->m_vtxCamera - this->m_vtxPosition;

    double radiusXZ;
    radiusXZ = hypot(vctCamFocus[euclid::DIMENSION_X],
            vctCamFocus[euclid::DIMENSION_Z]);
    this->__m_dblHeading = sid::true_asin(vctCamFocus[euclid::DIMENSION_X],
            vctCamFocus[euclid::DIMENSION_Z],
            radiusXZ);

    const double hypot = 1.0 * dblMag;
    float fRads = sid::degrees_to_rads(this->__m_dblHeading + offsetAngle);
    float vfMoveVector[3];
    vfMoveVector[0] = sin(fRads) * hypot;
    vfMoveVector[1] = 0;
    vfMoveVector[2] = cos(fRads) * hypot;
    this->move(vfMoveVector[0], vfMoveVector[1], vfMoveVector[2]);
}

void sid::camera::zoom(double dblSpeed) {
    vector3 vctCamFocus2;
    vector3 vctCamFocus3;
    vertex3 vtxPrevCamera = this->m_vtxCamera;

    double dblNewDistance = this->__m_fDistance - dblSpeed;
    double dblMoveRatio = (dblNewDistance) / this->__m_fDistance;
    
    vctCamFocus2 = this->m_vctFocusVect * dblMoveRatio;
    vctCamFocus3 = this->m_vctFocusVect - vctCamFocus2;

    this->m_vtxCamera -= vctCamFocus3;
    this->_set_focus_dist();
    
    if (this->__m_fVertDistance < 10) {
        this->m_vtxCamera = vtxPrevCamera;
        this->_set_focus_dist();
    }
}

void sid::camera::set_height(float heightVal) {
    vertex3 vtxPosition = vertex3(this->m_vtxPosition[0], 
            heightVal, 
            this->m_vtxPosition[2]);
    
    this->m_vtxPosition = vtxPosition;
    this->m_vtxCamera = this->m_vtxPosition + this->m_vctFocusVect;
    this->_set_focus_dist();
    
    this->_set_focus_dist();
}

sid::vertex3 const& sid::camera::get_focus_pos() const {
    return m_vtxPosition;
}

void sid::camera::_set_focus_dist() {
    this->m_vctFocusVect = this->m_vtxCamera - this->m_vtxPosition;
    this->__m_fDistance = sqrt(m_vctFocusVect.dot(&m_vctFocusVect));
    this->_set_horizontal_dist();
    this->_set_vertical_dist();
}

void sid::camera::_set_horizontal_dist() {
    this->__m_fHorizDistance = hypot(this->m_vctFocusVect[euclid::DIMENSION_X],
            this->m_vctFocusVect[euclid::DIMENSION_Z]);
}

void sid::camera::_set_vertical_dist() {
    float fCamHeight = this->get_camera_pos().data()[euclid::DIMENSION_Y];
    float fFocusHeight = this->get_focus_pos().data()[euclid::DIMENSION_Y];
    this->__m_fVertDistance = fCamHeight - fFocusHeight;
}

bool sid::camera::zoom_to(double dblSpeed) {
    bool blRetFocus(false);
    bool blRetCamera(false);
    
    vertex3 vtxCameraZoomTo = this->m_vtxZoomTo + this->m_vctFocusVect;
    if (vtxCameraZoomTo[euclid::DIMENSION_Y] < this->__m_fCamOffsetY_Min) {
        float fMaxHeightRatio = this->__m_fCamOffsetY_Min / 
                vtxCameraZoomTo[euclid::DIMENSION_Y];
        
        vtxCameraZoomTo[euclid::DIMENSION_Y] = this->__m_fCamOffsetY_Min;
        
        vtxCameraZoomTo[euclid::DIMENSION_X] = 
                this->m_vtxPosition[euclid::DIMENSION_X] + 
                (this->m_vctFocusVect[euclid::DIMENSION_X] * fMaxHeightRatio);
        
        vtxCameraZoomTo[euclid::DIMENSION_Z] = 
                this->m_vtxPosition[euclid::DIMENSION_Z] + 
                (this->m_vctFocusVect[euclid::DIMENSION_Z] * fMaxHeightRatio);
    } else {
        vtxCameraZoomTo = this->m_vtxZoomTo + this->m_vctFocusVect;
    }

    vector3 vctFocusZoomDelta = static_cast<vector3> (this->m_vtxZoomTo - this->m_vtxPosition);
    vctFocusZoomDelta.normalise();
    float fMagFocus = vctFocusZoomDelta.get_magnitude();
    if (fMagFocus >= 1.0 && !blRetFocus) {

        vctFocusZoomDelta = vctFocusZoomDelta * 
                (vctFocusZoomDelta.get_magnitude() * dblSpeed);
        
        this->m_vtxPosition.move(vctFocusZoomDelta[euclid::DIMENSION_X],
                vctFocusZoomDelta[euclid::DIMENSION_Y],
                vctFocusZoomDelta[euclid::DIMENSION_Z]);
    } else {
        blRetFocus = true;
    }
    
    vector3 vctCameraZoomDelta = static_cast<vector3> (vtxCameraZoomTo - this->m_vtxCamera);
    vctCameraZoomDelta.normalise();
    float fMagCamera = vctCameraZoomDelta.get_magnitude();
    if (fMagCamera >= 1.0 && !blRetCamera) {

        vctCameraZoomDelta = vctCameraZoomDelta * 
                (vctCameraZoomDelta.get_magnitude() * dblSpeed);
        
        if (fMagCamera > 150) {
            float fMagOSRatio = fMagCamera / 150;
            vctCameraZoomDelta = vctCameraZoomDelta / fMagOSRatio;
        }

        this->m_vtxCamera.move(vctCameraZoomDelta[euclid::DIMENSION_X],
                vctCameraZoomDelta[euclid::DIMENSION_Y],
                vctCameraZoomDelta[euclid::DIMENSION_Z]);
    } else {
        blRetCamera = true;
    }    

    return (blRetFocus && blRetCamera);
}

sid::camera::eGestureModes sid::camera::get_gesture_mode() const {
    return m_GestureMode;
}