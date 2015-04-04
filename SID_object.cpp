/*
 * File:   object.cpp
 * Author: geth2k
 *
 * Created on 14 April 2013, 18:49
 */

#include "SID_object.h"

const sid::object::t_object_data sid::object::ENABLED = 0x80000000; //        10000000000000000000000000000000
const sid::object::t_object_data sid::object::VISIBLE = 0x40000000; //        01000000000000000000000000000000
const sid::object::t_object_data sid::object::DELETE = 0x20000000; //         00100000000000000000000000000000
const sid::object::t_object_data sid::object::RECURSE_EVENTS = 0x10000000; // 00010000000000000000000000000000
const sid::object::t_object_data sid::object::FRONT = 0x8000000; //           00001000000000000000000000000000
const sid::object::t_object_data sid::object::KILL_TIMER = 0x3F; //           00000000000000000000000000111111

sid::textures sid::object::_s_Textures;
sid::audio sid::object::_s_Audio;

sid::object::object() :
_m_Data(0),
_m_Draw(nullptr),
_m_Select(nullptr),
__m_strName("") {

    this->_m_Data.set_subdata_mask(VISIBLE | ENABLED | RECURSE_EVENTS);
}

sid::object::object(t_object_data setFlags) :
_m_Data(setFlags),
_m_Draw(nullptr),
_m_Select(nullptr),
__m_strName("") {

}

sid::object::object(const object& orig) :
child<object*>(orig),
_m_Data(orig._m_Data),
_m_Draw(orig._m_Draw),
_m_Select(orig._m_Select) {

    this->__m_strName = orig.__m_strName;
}

sid::object::~object() {
    this->_clear_children();
    if (this->_m_Draw)
        delete this->_m_Draw;

    if (this->_m_Select)
        delete this->_m_Select;
}

sid::object* sid::object::get_child(std::string name) {
    return this->_find_if([&name](object* ref, int loop, void* args) {
                std::string* strCompName = static_cast<std::string*>(args);
                return (ref->get_name() == name);
            });
}

void sid::object::tick() {
    this->_foreach<object*>(
            [](object* ref, int loop, void* arg) { 
                ref->tick(); 
                return true;
            });
}

void sid::object::render_all(int maxRecurse, uint flags) {
    this->render_this(flags);
        
    struct render_args {
        object* Caller;
        int MaxRecurse;
        uint Flags;
    };
        
    render_args* RenderArgs = new render_args();
    RenderArgs->Caller = this;
    RenderArgs->Flags = flags;
    RenderArgs->MaxRecurse = maxRecurse - 1;

    this->_foreach_if_then(
            [](object* ref, int loop, void* args) {
                render_args* RenderArgs_ = static_cast<render_args*>(args);
                return RenderArgs_->Caller->is_visible();
            },
            [](object* ref, int loop, void* args) {
                render_args* RenderArgs_ = static_cast<render_args*>(args);
                ref->render_all(RenderArgs_->MaxRecurse, RenderArgs_->Flags);
                return true;
            },
            RenderArgs);

    delete RenderArgs;
}

void sid::object::set_visible(bool const value) {
    if (value)
        this->_m_Data.set_subdata_mask(VISIBLE);
    else
        this->_m_Data.unset_subdata_mask(VISIBLE);
}

bool sid::object::is_visible() const {
    return this->_m_Data.is_subdata_mask(VISIBLE);
}

void sid::object::set_enabled(bool const value) {
    if (value)
        this->_m_Data.set_subdata_mask(ENABLED);
    else
        this->_m_Data.unset_subdata_mask(ENABLED);
}

bool sid::object::get_enabled() const {
    return this->_m_Data.is_subdata_mask(ENABLED);
}

void sid::object::reset_kill_timer() {
    this->_m_Data.set_subdata_value(KILL_TIMER, 0);
    this->_m_Data.unset_subdata_mask(DELETE);
}

sid::object::t_object_data sid::object::inc_kill_timer(t_object_data const incValue) {
    t_object_data uiTemp = this->_m_Data.inc_subdata_value(KILL_TIMER, incValue);
    t_object_data uiMax = bits<t_object_data>::get_valuemask_max(KILL_TIMER);

    if (uiTemp >= uiMax) {
        this->reset_kill_timer();
        this->_m_Data.set_subdata_mask(DELETE);
        return 0;
    } else {
        return uiTemp;
    }
}

bool sid::object::is_deletable() const {
    return this->_m_Data.is_subdata_mask(DELETE);
}


bool sid::object::is_frontflag_set() const {
    return this->_m_Data.is_subdata_mask(FRONT);
}

bool sid::compare_objects(sid::object* obj1, sid::object* obj2) {
    return (obj1->is_frontflag_set() < obj2->is_frontflag_set());
}