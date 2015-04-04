/* 
 * File:   object_templates.h
 * Author: geth2k
 *
 * Created on March 18, 2015, 6:12 PM
 */

#ifndef OBJECT_TEMPLATES_H
#define	OBJECT_TEMPLATES_H

#include <map>
#include "SID_object_template.h"

namespace sid {
    class object_templates {
    public:
        object_templates();
        object_templates(const object_templates& orig);
        virtual ~object_templates();
        
        inline void add_template(uint pos, sid::sidof const* Sidof) {
            this->__m_vTemplates[pos] = sid::object_template(Sidof);
        }
        
        inline void clear_templates() {
            this->__m_vTemplates.clear();
        }
        
    private:

        std::map<uint, sid::object_template> __m_vTemplates;
    };
}

#endif	/* OBJECT_TEMPLATES_H */

