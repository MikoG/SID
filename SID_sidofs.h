/* 
 * File:   SID_sidofs.h
 * Author: geth2k
 *
 * Created on March 11, 2015, 6:39 PM
 */

#ifndef SID_SIDOFS_H
#define	SID_SIDOFS_H

#include <map>
#include "SID_sidof.h"

namespace sid {
    class sidofs {
    public:
        
        typedef std::map<uint, sidof> t_SidofMap;
        typedef t_SidofMap::iterator t_SidofMapIter;
        typedef t_SidofMap::const_iterator t_SidofMapConstIter;
        
        sidofs();
        sidofs(const sidofs& orig);
        virtual ~sidofs();
        
        void add_sidof(std::string const& strPath);

        void erase_all();

        uint get_sido_id_at(uint index);
        uint get_sido_id_by_ref(uint gameRef);

        inline sidof const& operator[](std::string const& index) {
            return this->__m_vSidofMap[index];
        }
        
    private:

        std::map<std::string, sidof> __m_vSidofMap;
    };
}
#endif	/* SID_SIDOFS_H */

