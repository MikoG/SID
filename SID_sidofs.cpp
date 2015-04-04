/* 
 * File:   SID_sidofs.cpp
 * Author: geth2k
 * 
 * Created on March 11, 2015, 6:39 PM
 */

#include "SID_sidofs.h"

sid::sidofs::sidofs() {
}

sid::sidofs::sidofs(const sidofs& orig) {
}

sid::sidofs::~sidofs() {
}

void sid::sidofs::add_sidof(std::string const& strPath) {
    std::string strFileName = sid::file::get_file_from_filepath(strPath);
    this->__m_vSidofMap.emplace(std::make_pair(strFileName, sid::sidof(strPath)));
}

void sid::sidofs::erase_all() {
    this->__m_vSidofMap.clear();
}

uint sid::sidofs::get_sido_id_at(uint index) {
}

uint sid::sidofs::get_sido_id_by_ref(uint gameRef) {
}