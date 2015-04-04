/* 
 * File:   camera.h
 * Author: geth2k
 *
 * Created on 15 June 2013, 23:49
 */

#ifndef SID_CAMERA_H
#define	SID_CAMERA_H

#include <cmath>
#include <iostream>
#include <GL/glu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>

#include "SID_vertex3.h"
#include "SID_vector3.h"
#include "SID_keyboard.h"
#include "SID_mouse.h"
#include "SID_tuplets.h"
#include "SID_render.h"
#include "SID_plot.h"

namespace sid {

    class camera {
    public:

        enum eGestureModes {
            NONE   = 0x00,
            MOVE   = 0x01,
            ROTATE = 0x02,
        };

        camera(float defaultX, float defaultY, float defaultZ, 
                float minY, float maxY);
        
        camera(const camera& orig);
        virtual ~camera();

        vertex3 const& get_focus_pos() const;

        vertex3 const& get_camera_pos() const;

        vector3 const& get_focus_vector() const;
        
        float get_distance() const;
        float get_horizontal_distance() const;

        void process_keyboard(double timeDiff);
        void process_mouse(SDL_Event& evt);

        void set_height(float heightVal);

        void move(float fValue_X, float fValue_Y, float fValue_Z);
        void move(double offsetAngle, double dblMag);

        void zoom(double dblSpeed);

        void set_view();

        void set_center_screen(int width, int height);

        void set_zoom_to(vertex3 vtxZoomTo);
        bool zoom_to(double dblSpeed);

        camera::eGestureModes get_gesture_mode() const;

        struct action_keys {
            uchar forward;
            uchar back;
            uchar left;
            uchar right;
            uchar zoomIn;
            uchar zoomOut;
            uchar rotateLeft;
            uchar rotateRight;
            uchar cameraUp;
            uchar cameraDown;
            uchar quickGrabToggle;
            uchar attackClickToggle;
        } Actions;
        
        static keyboard* Keyboard;

        mouse Mouse;

    protected:
        void _set_focus_dist();
        void _set_horizontal_dist();
        void _set_vertical_dist();
        void _gesture_rotate(SDL_Event& evt);
        void _gesture_grab_terrain(SDL_Event& evt);

    private:

        struct screen_coords {
            int x;
            int y;
        };
        
        screen_coords m_coordsCenterScreen;
        screen_coords m_coordsScreenPrev;
        sid::triplet<GLdouble> m_objVertPrev;
        double m_dblCursorAngle_Prev;
        double m_dblCursorAngle_Current;
        double m_dblCursorAngle_Delta;

        vertex3 m_vtxPosition;
        vertex3 m_vtxCamera;
        vector3 m_vctFocusVect;

        float __m_fDistance;
        float __m_fHorizDistance;
        float __m_fVertDistance;
        double __m_dblHeading;

        float __m_fCamOffsetX_Def;
        float __m_fCamOffsetY_Def;
        float __m_fCamOffsetZ_Def;
        float __m_fCamOffsetY_Min;
        float __m_fCamOffsetY_Max;
        
        float fFocusOffsetY;

        vertex3 m_vtxZoomTo;

        camera::eGestureModes m_GestureMode;

        double _get_cursor_angle(screen_coords cursor, screen_coords ref, double* hyp = nullptr);
    };
}
#endif	/* CAMERA_H */

