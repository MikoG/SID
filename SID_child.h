/* 
 * File:   child.h
 * Author: geth2k
 *
 * Created on June 21, 2014, 10:17 AM
 */

#ifndef SID_CHILD_H
#define	SID_CHILD_H

#include "SID_parent.h"

namespace sid {

    template <typename T>
    class child : public parent<T> {
    public:

        child() :
        __m_objParent(nullptr) {

        }

        child(const child& orig) :
        parent<T>(orig),
        __m_objParent(orig.__m_objParent) {
        }

        virtual ~child() {

        }

        inline virtual void set_parent(T parent) {
            this->__m_objParent = parent;
        }

        inline T get_parent() const {
            return this->__m_objParent;
        }

    protected:

    private:

        T __m_objParent;

    };
}
#endif	/* CHILD_H */

