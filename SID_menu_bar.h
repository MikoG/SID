/* 
 * File:   menu_bar.h
 * Author: geth2k
 *
 * Created on 26 November 2013, 20:59
 */

#ifndef SID_MENU_BAR_H
#define	SID_MENU_BAR_H

#include "SID_control.h"
#include "SID_button.h"

namespace sid {

    class menu_bar : public control {
    public:
        menu_bar();
        menu_bar(const menu_bar& orig);
        virtual ~menu_bar();

        virtual void add_child(object* child);

        void set_button_spacing(double dblButtonSpacing);
        double get_button_spacing() const;

    protected:

    private:
        double m_dblButtonSpacing;
    };
}
#endif	/* MENU_BAR_H */

