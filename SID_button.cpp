/* 
 * File:   button.cpp
 * Author: geth2k
 * 
 * Created on 28 November 2013, 19:57
 */

#include "SID_button.h"

const int sid::button::DEFAULT_HEIGHT = 24;

sid::button::button() : 
m_iState(sid::button::state_images::OFF),
control(VISIBLE | ENABLED) {
    this->set_height(DEFAULT_HEIGHT);
    this->setup_ui();
}

sid::button::button(const button& orig) : m_iState(orig.m_iState) {
}

sid::button::~button() {
}

void sid::button::setup_ui() {
    control::t_color ColorBackground = control::t_color(220, 220, 220, 255);
    control::t_color ColorTextBackground = control::t_color(255, 0, 255, 0);
    
    this->set_height(20);
    this->set_width(80);
    this->set_margin(0);
    
    this->set_colour(ColorBackground);
    title = new character_strip();
    title->set_parent(this);
    this->title->set_font(control::_s_Fonts.get_font(0));
    this->title->set_data("Click");
    this->title->set_margin(1);
    this->title->set_height(20);
    this->title->set_width(60);
    this->title->set_colour(ColorTextBackground);
    this->title->set_left((this->get_width() / 2) -
            (this->title->get_width() / 2));
    this->title->paint();
    this->add_child(title);
}

void sid::button::set_title(std::string title_text) const {
    this->title->set_data(title_text);
}

sid::button::state_images::eStates sid::button::get_state() const {
    return m_iState;
}

sid::button::state_images::eStates sid::button::set_state(
        sid::button::state_images::eStates state) {
    
    this->m_iState = state;
}

void sid::button::_mouse_down(SDL_Event& evt) {
    uint uiTexID_on = object::_s_Textures.get_tex_id_by_ref(
            this->StatusImages.get_image_ID(
            sid::button::state_images::eStates::PRESSED));

    if (this->m_iState == sid::button::state_images::NORMAL) {
        this->set_texture_ID(uiTexID_on);
        this->m_iState = sid::button::state_images::PRESSED;
    }
    this->paint();
}

void sid::button::_mouse_up(SDL_Event& evt) {
    uint uiTexID_off = object::_s_Textures.get_tex_id_by_ref(
        this->StatusImages.get_image_ID(
        sid::button::state_images::eStates::OFF));
    
    this->set_texture_ID(uiTexID_off);
    this->m_iState = sid::button::state_images::NORMAL;
    this->paint();
}

void sid::button::cancel_click(SDL_Event& evt) {
    uint uiTexID_off = object::_s_Textures.get_tex_id_by_ref(
            this->StatusImages.get_image_ID(
            sid::button::state_images::eStates::NORMAL));

    if (this->m_iState == sid::button::state_images::PRESSED) {
        this->set_texture_ID(uiTexID_off);
        this->m_iState = sid::button::state_images::NORMAL;
    }
    
    this->paint();
}

sid::button::state_images::state_images() {
    this->m_iImagIDs.fill(0);
}

void sid::button::state_images::set_image_ID(eStates state, int value) {
    this->m_iImagIDs[state] = value;
}

int sid::button::state_images::get_image_ID(eStates state) const {
    return this->m_iImagIDs[state];
}