/* 
 * File:   object_ingame.cpp
 * Author: geth2k
 * 
 * Created on 01 November 2013, 19:45
 */

#include "SID_object_ingame.h"

sid::object_ingame* sid::object_ingame::s_objZoomTo = nullptr;
sid::object_templates sid::object_ingame::_s_Templates; 

sid::object_ingame::object_ingame() :
m_vtxPosition(),
m_vboOpaque(new sid::vbo()),
m_vboTransparent(new sid::vbo()),
interactive_object(ENABLED | VISIBLE | RECURSE_EVENTS) {

    this->m_blZoomTo = false;
}

sid::object_ingame::object_ingame(Uint32 setFlags) :
m_vtxPosition(),
m_vboOpaque(nullptr),
m_vboTransparent(nullptr),
interactive_object(setFlags) {

    this->m_blZoomTo = false;
}

sid::object_ingame::object_ingame(const object_ingame& orig) :
m_vboOpaque(nullptr),
m_vboTransparent(nullptr),
interactive_object(orig) {
    m_vtxPosition = orig.m_vtxPosition;
}

sid::object_ingame::~object_ingame() {

    if (this->m_vboOpaque)
        delete this->m_vboOpaque;

    if (this->m_vboTransparent)
        delete this->m_vboTransparent;

    if (sid::object_ingame::get_zoom_object() == this)
        sid::object_ingame::set_zoom_object(nullptr);
}

sid::vertex3& sid::object_ingame::get_position() {
    return m_vtxPosition;
}

void sid::object_ingame::set_position(vertex3 const& vtxPosition) {
    m_vtxPosition = vtxPosition;
}

void sid::object_ingame::render_this(uint flags) {
    this->reset_kill_timer();

    if (sid::get_bit(flags, draw_flags::TRANSPARENT)) {
        this->_draw(this->m_vboTransparent);
    } else {
        this->_draw(this->m_vboOpaque);
    }
}

void sid::object_ingame::set_zoom_to(bool blZoomTo) {
    this->m_blZoomTo = blZoomTo;
}

bool sid::object_ingame::is_zoom_to() const {
    return m_blZoomTo;
}

void sid::object_ingame::set_zoom_object(object_ingame* s_objZoomTo) {
    sid::object_ingame::s_objZoomTo = s_objZoomTo;
}

sid::object_ingame* sid::object_ingame::get_zoom_object() {
    return sid::object_ingame::s_objZoomTo;
}

void sid::object_ingame::set_as_zoom_object() {
    sid::object_ingame::s_objZoomTo = this;
}
