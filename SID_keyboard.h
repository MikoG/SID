/* 
 * File:   input_set.h
 * Author: geth2k
 *
 * Created on 07 December 2013, 12:00
 */

#ifndef SID_KEYBOARD_H
#define	SID_KEYBOARD_H

#include <string>
#include <array>

namespace sid {

    class keyboard {
    public:

        keyboard();
        keyboard(const keyboard& orig);
        virtual ~keyboard();

        std::array<bool, 256 > inputs;

    private:

    };
}
#endif	/* KEYBOARD_H */

