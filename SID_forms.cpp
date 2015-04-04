/* 
 * File:   controls.cpp
 * Author: geth2k
 * 
 * Created on 27 October 2013, 03:13
 */

#include "SID_forms.h"

sid::forms::forms() {
    
}

sid::forms::forms(const forms& orig) {
    this->Textures = orig.Textures;
}

sid::forms::~forms() {
}

sid::control* sid::forms::at(std::string name) {
    for (std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
            it1 != this->m_vctlChildren.end();
            it1++) {
        
        if ((*it1)->get_name() == name) {
            return *it1;
        }
    }
    return nullptr;
}

void sid::forms::add_form(control* ctl) {
    this->m_vctlChildren.push_back(ctl);
}

void sid::forms::insert_form(uint idx, control* ctl) {
    uint maxIterate = idx;
    if (this->m_vctlChildren.size() < maxIterate) {
        maxIterate = this->m_vctlChildren.size();
    }
    std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
    for (uint i=0; i <= maxIterate; ++i) {
        if (i == idx) {
            this->m_vctlChildren.insert(it1, ctl);
        }
        it1++;
    }  
}

void sid::forms::remove_form(uint idx) {
    uint maxIterate = idx;
    if (this->m_vctlChildren.size() - 1 < maxIterate) {
        maxIterate = this->m_vctlChildren.size() - 1;
    }
    std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
    for (uint i=0; i <= maxIterate; ++i) {
        if (i == idx) {
            delete (*it1);
            this->m_vctlChildren.erase(it1);
        }
        it1++;
    }
}

void sid::forms::remove_form(std::string name) {
    for (std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
            it1 != this->m_vctlChildren.end();
            it1++) {
        
        if ((*it1)->get_name() == name) {
            delete (*it1);
            it1 = this->m_vctlChildren.erase(it1);  
            break;
        }
    }
}

void sid::forms::show_form(std::string name) {
    for (std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
            it1 != this->m_vctlChildren.end();
            it1++) {
        
        if ((*it1)->get_name() == name) {
            (*it1)->set_visible(true);
            (*it1)->set_enabled(true);
            break;
        }
    }
}

void sid::forms::hide_form(std::string name) {
    for (std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
            it1 != this->m_vctlChildren.end();
            it1++) {
        
        if ((*it1)->get_name() == name) {
            (*it1)->set_visible(false);
            (*it1)->set_enabled(false);
            break;
        }
    }
}

void sid::forms::remove_form(control const* ref) {
    for (std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
            it1 != this->m_vctlChildren.end();
            it1++) {
        
        if ((*it1) == ref) {
            delete (*it1);
            it1 = this->m_vctlChildren.erase(it1);  
            break;
        }
    }
}

void sid::forms::clear_forms() {
    object* objHook;
    std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
    while (it1 != this->m_vctlChildren.end()) {
        objHook = *it1;
        it1 = this->m_vctlChildren.erase(it1);
        delete objHook;
    } 
}

uint sid::forms::size() {
    return this->m_vctlChildren.size();
}

void sid::forms::render_all() {
    this->m_vctlChildren.sort(&compare_objects);
    std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
    for (uint i=0; i < this->m_vctlChildren.size(); ++i) {
        if ((*it1)->is_visible()) {
            (*it1)->render_all(INT_MAX, 0);
        }
        it1++;
    }
}

bool sid::forms::process_event(SDL_Event& evt, int max_recurse) {
    std::list<control*>::iterator it1 = this->m_vctlChildren.end();
    int iEndIndex = this->m_vctlChildren.size() - 1;
    if (evt.type == SDL_MOUSEBUTTONDOWN || 
            evt.type == SDL_MOUSEBUTTONUP || 
            evt.type == SDL_MOUSEMOTION) {
        for (int i = iEndIndex; i >= 0; i--) {
            it1--;
            int iSuccessLevel = -1;
            (*it1)->_test_event(evt, &iSuccessLevel, max_recurse);
            if (iSuccessLevel != -1) {
                if (i < iEndIndex) {
                    this->control_to_front(*it1);
                }
                return true;                
            }
        }
        
        if (evt.type == SDL_MOUSEBUTTONUP && control::get_clickowner() != nullptr) {
            static_cast<control*>(control::get_clickowner())->cancel_click(evt);
        }
    } else if (evt.type == SDL_WINDOWEVENT) {
        for (int i = iEndIndex; i >= 0; i--) {
            it1--;
            int iSuccessLevel = -1;
            if ((*it1)->_test_event(evt, &iSuccessLevel, max_recurse)) {
                return true;
            }
        }
    } else if (evt.type == user_events::SID_CLOSE_FORM) {
        this->remove_form(static_cast<control*>(evt.user.data1));
    }
    
    return false;
}

void sid::forms::control_to_front(control* ctl) {
    ctl->set_front_flag(true);
    this->m_vctlChildren.sort(&compare_objects);
    ctl->set_front_flag(false);
}

void sid::forms::control_to_front(std::string name) {
    for (std::list<control*>::iterator it1 = this->m_vctlChildren.begin();
            it1 != this->m_vctlChildren.end();
            it1++) {
        
        if ((*it1)->get_name() == name) {
            this->control_to_front(*it1);
            break;
        }
    }
}