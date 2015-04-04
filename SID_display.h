/* 
 * File:   display.h
 * Author: geth2k
 *
 * Created on 05 May 2013, 17:05
 */

#ifndef SID_DISPLAY_H
#define	SID_DISPLAY_H

#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>


namespace sid {

    class display {
    public:

        display();

        display(const std::string title,
                int width,
                int height,
                int refreshRate,
                bool fullscreen);

        display(const display& orig);

        virtual ~display();

        /* @brief Closes the current window.
         * 
         * Note: This does not terminate the application. 
         */
        inline void close() {
            if (this->__m_Window)
                SDL_GL_DeleteContext(this->__m_GLContext);
            SDL_DestroyWindow(this->__m_Window);
        }

        void swap_buffers();

        inline float get_aspect_ratio() const {
            return get_window_width() / get_window_height();
        }

        inline int get_screen_center_x() const {
            return this->get_window_width() / 2;
        }

        inline int get_screen_center_y() const {
            return this->get_window_height() / 2;
        }

        inline int get_window_width() const {
            return this->__m_Mode.w;
        }

        inline int get_window_height() const {
            return this->__m_Mode.h;
        }

        inline int get_refresh_rate() const {
            return this->__m_Mode.refresh_rate;
        }

        inline bool is_initialised() const {
            return (this->__m_Window != nullptr) ? true : false;
        }

    private:

        /*
         * @brief Creates an SDL window setup for OpenGL output.
         * 
         * @param Title        - Title of new window.
         * @param Width        - Width of the new window in pixels
         * @param Height       - Height of the new window in pixels
         */
        bool _init_window(const std::string title,
                int width,
                int height,
                int refreshRate,
                bool fullscreen);

        SDL_Window* __m_Window;

        SDL_DisplayMode __m_Mode;

        SDL_GLContext __m_GLContext;

        int __m_displayIndex;

        std::string __m_strTitle;

    };
}
#endif	/* SID_DISPLAY_H */

