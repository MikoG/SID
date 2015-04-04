/* 
 * File:   toggle.cpp
 * Author: geth2k
 * 
 * Created on 23 March 2014, 13:01
 */

#include "SID_toggle.h"

sid::toggle::toggle() : m_blToggleStatus(false) {
}

sid::toggle::toggle(const toggle& orig) {
}

sid::toggle::~toggle() {
}

void sid::toggle::_mouse_down(SDL_Event& evt) {
    uint uiTexID_off = object::_s_Textures.get_tex_id_by_ref(
            this->StatusImages.get_image_ID(
            button::state_images::eStates::OFF));

    uint uiTexID_on = object::_s_Textures.get_tex_id_by_ref(
            this->StatusImages.get_image_ID(
            button::state_images::eStates::ON));

    if (this->get_state() == button::state_images::ON) {
        this->set_texture_ID(uiTexID_off);
    } else if (this->get_state() == button::state_images::OFF) {
        this->set_texture_ID(uiTexID_on);
    }

    this->paint();
}

void sid::toggle::_mouse_up(SDL_Event& evt) {

    if (this->get_state() == button::state_images::OFF) {
        this->set_state(button::state_images::ON);
    } else if (this->get_state() == button::state_images::ON) {
        this->set_state(button::state_images::OFF);
    }
    
    this->paint();
}

void sid::toggle::cancel_click(SDL_Event& evt) {
    uint uiTexID_off = object::_s_Textures.get_tex_id_by_ref(
            this->StatusImages.get_image_ID(
            sid::toggle::state_images::eStates::OFF));
    uint uiTexID_on = object::_s_Textures.get_tex_id_by_ref(
            this->StatusImages.get_image_ID(
            sid::toggle::state_images::eStates::ON));

    if (this->get_state() == sid::toggle::state_images::ON) {
        this->set_texture_ID(uiTexID_on);
        this->set_state(sid::toggle::state_images::ON);
    } else if (this->get_state() == sid::toggle::state_images::OFF) {
        this->set_texture_ID(uiTexID_off);
        this->set_state(sid::toggle::state_images::OFF);
    }
    
    this->paint();
}