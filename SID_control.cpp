/*
 * File:   control.cpp
 * Author: geth2k
 *
 * Created on 27 October 2013, 01:44
 */

#include "SID_control.h"

sid::fonts sid::control::_s_Fonts = fonts();

sid::control::control() :
interactive_object(VISIBLE | ENABLED | RECURSE_EVENTS),
m_layout(nullptr) {

    this->set_front_flag(false);
    this->set_draw_delegate(new drawControl());
}

sid::control::control(Uint32 setFlags) :
interactive_object(setFlags),
m_layout(nullptr) {

    this->set_front_flag(false);
    this->paint();
    this->set_draw_delegate(new drawControl());
}

sid::control::control(const control& orig) :
interactive_object(orig),
m_layout(orig.m_layout) {

    this->__m_DisplayParams = orig.__m_DisplayParams;
    this->m_strData = orig.m_strData;
    this->set_parent(orig.get_parent());
    this->paint();
}

sid::control::~control() {
    if (this->m_layout != nullptr) {
        delete this->m_layout;
    }
}

void sid::control::add_child(object* child) {
    this->_add_child_back(static_cast<control*> (child));
}

void sid::control::set_front_flag(bool status) {
    this->_m_Data.set_subdata_mask(FRONT);
}

bool sid::control::get_front_flag() const {
    return this->_m_Data.is_subdata_mask(FRONT);
}

sid::rectangle_2d* sid::control::get_screenrect() {
    return &this->m_rectScreen;
}

void sid::control::paint() {

    // Declare buffers and set default values
    rectangle_2d* rectParent = nullptr;
    double dblParentTop = 0;
    double dblParentLeft = 0;
    double dblParentWidth = 0;
    double dblParentHeight = 0;
    double dblParentMargin = 0;

    // Set upper left bound to parent object's top and left values, or if parent,
    // is not set, use zeros.
    if (this->get_parent()) {
        control* Parent = static_cast<control*> (this->get_parent());
        dblParentMargin = Parent->get_margin();
        rectParent = &Parent->m_rectScreen;
        dblParentTop = rectParent->get_top();
        dblParentLeft = rectParent->get_left();
        dblParentWidth = rectParent->get_width();
        dblParentHeight = rectParent->get_height();
    } else {
        dblParentWidth = sid::get_screen_width();
        dblParentHeight = sid::get_screen_height();
    }

    // Set the rectangle size and position according to the stretch mode setting.
    switch (this->get_stretch_mode()) {
        case 1:
            this->set_left(0);
            this->set_width(dblParentWidth);
            break;
        case 2:
            this->set_top(0);
            this->set_height(dblParentHeight);
            break;
        case 3:
            this->set_width(dblParentWidth - this->get_left());
            break;
        case 4:
            this->set_height(dblParentHeight - this->get_top());
            break;
        case 5:
            this->set_left(0);
            this->set_width(dblParentWidth);
            this->set_top(0);
            this->set_height(dblParentHeight);
    }

    // Set this object's left value according to the Horizontal Justify setting.
    int iLeft = 0;
    switch (this->get_hjustify_mode()) {
        case sid::control::eHJustifyTypes::left:
            iLeft = this->get_left() + dblParentLeft + dblParentMargin;
            break;
        case sid::control::eHJustifyTypes::right:
            iLeft = dblParentWidth -
                    dblParentMargin -
                    this->get_width() -
                    this->get_left();
            break;
        case sid::control::eHJustifyTypes::center:
            iLeft = dblParentLeft +
                    dblParentMargin +
                    dblParentWidth -
                    dblParentWidth / 2 - this->get_width() / 2;
            break;
    }

    // Set this object's top value according to the Vertical Justify Setting.
    int iTop = 0;
    switch (this->get_vjustify_mode()) {
        case sid::control::eVJustifyTypes::top:
            iTop = this->get_top() + dblParentTop + dblParentMargin;
            break;
        case sid::control::eVJustifyTypes::bottom:
            iTop = dblParentTop + dblParentHeight -
                    dblParentMargin -
                    this->get_height() -
                    this->get_top();
            break;
        case sid::control::eVJustifyTypes::middle:
            iTop = dblParentTop +
                    dblParentMargin +
                    dblParentHeight -
                    dblParentHeight / 2 - this->get_height() / 2;
            break;
    }

    m_rectScreen.set_dimensions(
            iTop,
            iLeft,
            this->get_height(),
            this->get_width());

    this->m_rectScreen.set_colour(this->get_colour());

    // Set the texture U and V coordinates for all four corners.
    rectangle_2d::t_uv tuvTopLeft(0, 0);
    rectangle_2d::t_uv tuvBottomLeft(0, 1);
    rectangle_2d::t_uv tumBottomRight(1, 1);
    rectangle_2d::t_uv tuvTopRight(1, 0);
    rectangle_2d::t_uv_set tuvSet(tuvTopLeft,
            tuvBottomLeft,
            tumBottomRight,
            tuvTopRight);

    m_rectScreen.set_texture(this->get_texture_ID(), tuvSet);

    this->_foreach_if_then_always<control*, control*, control*>(
            // Predicate
            [](control* ref, int loops, void* args) {
                control* ctlParent = static_cast<control*> (ref->get_parent());
                return ctlParent->get_layout() ? true : false;
            },
    //Then
    [](control* ref, int loops, void* args) {
        ref->apply_layout(loops); return true;
    },
    // Always
    [](control* ref, int loops, void* args) {
        ref->paint();
    });
}

void sid::control::apply_layout(int loops) {
    control* ctlParent = static_cast<control*> (this->get_parent());
    if (ctlParent->m_layout) {
        static control* ctlLastSibling = nullptr;

        this->set_height(
                ctlParent->m_layout->get_cell_height());

        uint uiColPos = loops % ctlParent->get_layout()->get_cols();
        int iColWidth = ctlParent->get_layout()->get_col_width(uiColPos);

        if (loops > 0) {
            int iLastSiblingTop = ctlLastSibling->get_top();
            int iLastSiblingHeight = ctlLastSibling->get_height();
            int iLastSiblingLeft = ctlLastSibling->get_left();
            int iLastSiblingWidth = ctlLastSibling->get_width();


            if (uiColPos == 0) {
                this->set_top(iLastSiblingTop +
                        iLastSiblingHeight +
                        ctlParent->m_layout->get_padding());
                this->set_left(
                        ctlParent->m_layout->get_padding());
                this->set_width(iColWidth);
            } else {
                this->set_top(iLastSiblingTop);
                this->set_left(iLastSiblingLeft +
                        ctlParent->m_layout->get_padding() +
                        iLastSiblingWidth);

                this->set_width(iColWidth);
            }
        } else {
            ctlLastSibling = nullptr;
            this->set_top(ctlParent->m_layout->get_padding());
            this->set_left(ctlParent->m_layout->get_padding());
            this->set_width(iColWidth);
        }
        ctlLastSibling = this;
    }
}

bool sid::control::_test_event(SDL_Event& evt, int* succesLevel, int maxRecurse) {
    bool blRetVal = true;
    if (evt.type == SDL_MOUSEBUTTONDOWN ||
            evt.type == SDL_MOUSEBUTTONUP ||
            evt.type == SDL_MOUSEMOTION) {

        struct event_args {
            SDL_Event* Event;
            int* SuccesLevel;
            int MaxRecurse;
        };

        rectangle_2d* rect = this->get_screenrect();
        double dblClickX = evt.button.x;
        double dblClickY = evt.button.y;
        bool blClickInRect = rect->test_click_position(dblClickX, dblClickY);
        if (blClickInRect && this->_m_Data.is_subdata_mask(object::ENABLED)) {
            if (maxRecurse != 0) {
                bool blSuccess = false;
                if (this->_m_Data.is_subdata_mask(object::RECURSE_EVENTS)) {

                    event_args* EventArgs = new event_args();
                    EventArgs->Event = &evt;
                    EventArgs->MaxRecurse = maxRecurse;
                    EventArgs->SuccesLevel = succesLevel;

                    char chRetVal =
                            this->_forfirst<control*>(
                            [](control* ref, int loops, void* args) {
                                event_args* EventArgs =
                                        static_cast<event_args*> (args);

                                return ref->_test_event(*(EventArgs->Event),
                                        EventArgs->SuccesLevel,
                                        EventArgs->MaxRecurse - 1);
                            },
                    EventArgs);

                    delete EventArgs;
                    if (chRetVal == 1) {
                        blSuccess = true;
                    }

                } else {
                    blRetVal = true;
                }
                if (!blSuccess) {
                    this->_process_event(evt);
                    *succesLevel = INT_MAX - maxRecurse;
                    blRetVal = true;
                }
            } else {
                this->_process_event(evt);
                *succesLevel = INT_MAX - maxRecurse;
            }
        } else {
            blRetVal = false;
        }
    } else if (evt.type == SDL_WINDOWEVENT) {
        this->_process_event(evt);
        blRetVal = false;
    }
    return blRetVal;
}

void sid::control::set_layout(layout* layout) {
    this->m_layout = layout;
}

sid::layout* sid::control::get_layout() const {
    return m_layout;
}

void sid::control::render_this(uint flags) {
    this->_draw(this->get_screenrect());
}

void sid::control::set_data(std::string strData) {
    this->m_strData = strData;
}

std::string sid::control::get_data() const {
    return m_strData;
}

sid::control::__display_parameters::__display_parameters() :
m_Color(255, 255, 255, 255),
m_iHeight(100.0),
m_iLeft(0.0),
m_iTop(0.0),
m_iWidth(100.0),
m_uiTexID(0),
m_iMargin(4),
m_iHJustifyMode(sid::control::eHJustifyTypes::left),
m_iVJustifyMode(sid::control::eVJustifyTypes::top),
m_iStretchMode(0) {

}

sid::control::__display_parameters::__display_parameters(const sid::control::__display_parameters& orig) :
m_Color(orig.m_Color),
m_iHeight(orig.m_iHeight),
m_iLeft(orig.m_iLeft),
m_iTop(orig.m_iTop),
m_iWidth(orig.m_iWidth),
m_uiTexID(orig.m_uiTexID),
m_iMargin(orig.m_iMargin),
m_iHJustifyMode(orig.m_iHJustifyMode),
m_iVJustifyMode(orig.m_iVJustifyMode),
m_iStretchMode(orig.m_iStretchMode) {

}

sid::control::__display_parameters::~__display_parameters() {

}
