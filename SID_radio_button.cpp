/* 
 * File:   radio_button.cpp
 * Author: geth2k
 * 
 * Created on 31 March 2014, 00:50
 */

#include "SID_radio_button.h"

sid::radio_button::radio_button() : m_group(nullptr) {
}

sid::radio_button::radio_button(const radio_button& orig) : m_group(orig.m_group) {
}

sid::radio_button::~radio_button() {
}

void sid::radio_button::switch_off() {
    uint uiTexID_off = object::_s_Textures.get_tex_id_by_ref(
            this->StatusImages.get_image_ID(
            button::state_images::eStates::OFF));

    if (this->get_state() == button::state_images::ON) {
        this->set_texture_ID(uiTexID_off);
        this->set_state(button::state_images::OFF);
    }

    this->paint();
}

sid::radio_button::rb_group* sid::radio_button::get_group() const {
    return m_group;
}

void sid::radio_button::set_group(rb_group* group) {
    this->m_group = group;
}

