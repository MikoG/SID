/* 
 * File:   character_strip.h
 * Author: geth2k
 *
 * Created on 17 November 2013, 17:53
 */

#ifndef SID_CHARACTER_STRIP_H
#define	SID_CHARACTER_STRIP_H

#include "SID_control.h"
#include "SID_character.h"
#include "SID_font.h"

namespace sid {

    class character_strip : public control {
    public:
        character_strip();
        character_strip(const character_strip& orig);
        virtual ~character_strip();

        void set_font(const font* fnt);
        const font* get_font() const;

        virtual void set_data(std::string strData);

        void set_text_colour(control::t_color const& color);
        control::t_color get_text_colour() const;

    private:
        const font* m_fnt;
        control::t_color m_Color;
    };
}
#endif	/* CHARACTER_STRIP_H */

