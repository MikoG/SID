/* 
 * File:   basic_object.h
 * Author: geth2k
 *
 * Created on June 20, 2014, 11:04 PM
 */

#ifndef PARENT_H
#define	PARENT_H

#include <list>
#include <algorithm>

#include "SID_globals.h"

namespace sid {

    template <typename T>
    class parent {
    public:

        parent() :
        __m_vChildren() {

        }

        parent(const parent& orig) :
        __m_vChildren(orig.__m_vChildren) {

        }

        virtual ~parent() {

        }

        /** @brief Get the number of nodes currently stored in the child 
         * collection.
         * 
         * @return A std::size_t of the number of child nodes.
         * 
         */
        inline std::size_t get_child_count() const {
            return this->__m_vChildren.size();
        }

    protected:

        /** @brief Push a child node on to the front of the container.
         * 
         * @param child An object of type T.
         */
        inline void _add_child_front(T child) {
            this->__m_vChildren.push_front(child);
        }

        /** @brief Push a child node on to the back of the container.
         * 
         * @param child An object of type T.
         */
        inline void _add_child_back(T child) {
            this->__m_vChildren.push_back(child);
        }

        /** @brief Get a reference to the node at the back of the collection.
         * 
         * @return Reference to node.
         */
        inline T _get_child_back() const {
            return this->__m_vChildren.back();
        }

        /** @brief Get a reference to the node at the front of the collection.
         * 
         * @return Reference to node.
         */
        inline T _get_child_front() const {
            return this->__m_vChildren.front();
        }

        /** @brief Clear the list of child nodes and delete all data from the 
         *  heap.
         * 
         *  Do not call this method in a class where T is not a pointer.
         */
        void _clear_children() {
            __t_childnodes_iter it1 = this->__m_vChildren.begin();
            while (it1 != this->__m_vChildren.end()) {
                T Hook = *it1;
                it1 = this->__m_vChildren.erase(it1);
                delete Hook;
            }
        }

        /** @brief Find the first child whose reference matches @a ref and 
         * delete it.
         * 
         * @param ref A pointer to the child object that needs to be deleted.
         */
        inline void _remove_child(T ref) {
            this->_forfirst_if_delete([](T ref, int loops, void* args) {
                return ref == args;
            }, static_cast<void*> (ref));
        }

        /** @brief Find the first node in the child collection where @a pred
         * returns true and return a reference to it.
         * 
         * @param pred Pointer to predicate function that will be called on each iteration.
         * @param args Void pointer containing user data to be passed to @a pred.
         * 
         * @return A pointer to the selected child node or nullptr if no match
         * was found.
         * 
         * The input parameters of @a pred must be: \n
         *     "(_PredClass ref, int loop, void* args)" \n
         * where _PredClass is defined as the template parameter.
         */
        template <typename _PredClass = T,
        typename _PredFunction>
        T _find_if(_PredFunction pred,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            for (__t_childnodes_iter iter = this->__m_vChildren.begin();
                    iter != this->__m_vChildren.end();
                    ++iter) {

                if (pred(static_cast<_PredClass> (*iter), iLoop, args)) {
                    return (*iter);
                }

                iLoop++;
            }
            return nullptr;
        }

        /** @brief Iteratively call @a func for each node of the child collection.
         * 
         * @param func Pointer to function that will be called on each iteration.
         * @param args Void pointer containing user data to be passed to @a func.
         * 
         * @return A char number indicating the success of the routine. \n
         * 0  = List was empty. \n
         * 1  = @a func was called and returned true at least once. \n
         * -1 = @a func was called and did not return true.
         * 
         * The input parameters of @a func must be: \n
         *     "(_Class ref, int loop, void* args)" \n
         * where _Class is defined as the template parameter.
         */
        template <typename _Class = T,
        typename _Function>
        char _foreach(_Function func,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            for (__t_childnodes_iter iter = this->__m_vChildren.begin();
                    iter != this->__m_vChildren.end();
                    ++iter) {

                if (func(static_cast<_Class> (*iter), iLoop, args))
                    chRetVal = 1;
                else if (chRetVal == 0)
                    chRetVal = -1;

                iLoop++;
            }
            return chRetVal;
        }

        /** @brief Iteratively call @a pred for each node in the child collection
         *  then call @a func each time @a pred returns true.
         * 
         * @param pred Pointer to predicate function that will be called on each iteration.
         * @param func Pointer to function that will be called when pred returns true.
         * @param args Void pointer containing user data to be passed to @a pred
         * and @a func.
         * 
         * @return A char number indicating the success of the routine. \n
         * 0  = List was empty. \n
         * 1  = @a pred passed then @a func was called and returned true at least once. \n
         * -2 = @a pred passed then @a func was called but did not return true. \n
         * -1 = @a pred did not pass.
         * 
         * The input parameters of @a pred must be: \n
         *     "(_PredClass ref, int loop, void* args)" \n
         * where _PredClass is defined as the first template parameter.
         * 
         * The input parameters of @a func must be: \n
         *     "(_Class ref, int loop, void* args)" \n
         * where _Class is defined as the second template parameter.
         */
        template <typename _PredClass = T,
        typename _Class = T,
        typename _PredFunction,
        typename _Function>
        char _foreach_if_then(_PredFunction pred,
                _Function func,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            for (__t_childnodes_iter iter = this->__m_vChildren.begin();
                    iter != this->__m_vChildren.end();
                    ++iter) {

                if (pred(static_cast<_PredClass> (*iter), iLoop, args)) {
                    if (func(static_cast<_Class> (*iter), iLoop, args))
                        chRetVal = 1;
                    else if (chRetVal != 1)
                        chRetVal = -2;
                } else {
                    if (chRetVal == 0) chRetVal = -1;
                }

                iLoop++;
            }
            return chRetVal;
        }

        /** @brief Iteratively call @a pred for each node in the child collection
         *  then call @a func each time @a pred returns true or @a funcElse when
         * it returns false.
         * 
         * @param pred Pointer to predicate function that will be called on each 
         * iteration.
         * @param func Pointer to function that will be called when pred 
         * returns true.
         * @param funcElse Pointer to function that will be called when 
         * pred returns false.
         * @param args Void pointer containing user data to be passed to 
         * @a pred and @a func and @a funcElse.
         * 
         * @return A char number indicating the success of the routine. \n
         * 0  = List was empty. \n
         * 1  = @a pred passed then either @a func or @a funcElse was called and 
         * true was returned by either function at least once. \n
         * -2 = @a pred passed then either @a func or @a funcElse was called and 
         * neither function returned true.
         * least once. \n
         * -1 = @a pred did not pass.
         * 
         * The input parameters of @a pred must be: \n
         * "(_PredClass ref, int loop, void* args)" \n
         * where _PredClass is defined as the first template parameter.
         * 
         * The input parameters of @a func must be: \n
         *     "(_Class ref, int loop, void* args)" \n
         * where _Class is defined as the second template parameter.
         * 
         * The input parameters of @a funcElse must be: \n
         *     "(_ClassElse ref, int loop, void* args)" \n
         * where _ClassElse is defined as the third template parameter.
         */
        template <typename _PredClass = T,
        typename _Class = T,
        typename _ElseClass = T,
        typename _PredFunction,
        typename _Function,
        typename _ElseFunction>
        char _foreach_if_then_else(_PredFunction pred,
                _Function func,
                _ElseFunction funcElse,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            for (__t_childnodes_iter iter = this->__m_vChildren.begin();
                    iter != this->__m_vChildren.end();
                    ++iter) {

                if (pred(static_cast<_PredClass> (*iter), iLoop, args)) {
                    if (func(static_cast<_Class> (*iter), iLoop, args))
                        chRetVal = 1;
                    else if (chRetVal != 1)
                        chRetVal = -2;
                } else {
                    funcElse(static_cast<_ElseClass> (*iter), iLoop, args);
                    if (chRetVal == 0)
                        chRetVal - 1;
                }

                iLoop++;
            }
            return chRetVal;
        }

        /** @brief Iteratively call @a pred for each node in the child 
         * collection then call @a func each time @a pred returns true and
         * finally call funcAlways.
         * 
         * @param pred Pointer to predicate function that will be called on each 
         * iteration.
         * @param func Pointer to function that will be called when @a pred 
         * returns true.
         * @param funcAlways Pointer to function that will be called on 
         * each iteration after @a pred (and @a func if it was called).
         * @param args Void pointer containing user data to be passed to 
         * @a pred, @a func and @a funcAlways.
         * 
         * @return A char number indicating the success of the routine. Note 
         * that the return code ignores the the success of @a funcAlways.\n
         * 0  = List was empty. \n
         * 1  = @a pred passed then @a func was called and returned true 
         * at least once. \n
         * -2 = @a pred passed then @a func was called but did not return
         * true. \n
         * -1 = @a pred did not pass.
         * 
         * The input parameters of @a pred must be: \n
         * "(_PredClass ref, int loop, void* args)" \n
         * where _PredClass is defined as the first template parameter.
         * 
         * The input parameters of @a func must be: \n
         *     "(_Class ref, int loop, void* args)" \n
         * where _Class is defined as the second template parameter.
         * 
         * The input parameters of @a funcAlways must be: \n
         *     "(_ClassAlways ref, int loop, void* args)" \n
         * where _ClassAlways is defined as the third template parameter.
         */
        template <typename _PredClass = T,
        typename _Class = T,
        typename _AlwaysClass = T,
        typename _PredFunction,
        typename _Function,
        typename _AlwaysFunction>
        char _foreach_if_then_always(_PredFunction pred,
                _Function func,
                _AlwaysFunction funcAlways,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            for (__t_childnodes_iter iter = this->__m_vChildren.begin();
                    iter != this->__m_vChildren.end();
                    ++iter) {

                if (pred(static_cast<_PredClass> (*iter), iLoop, args)) {
                    if (func(static_cast<_Class> (*iter), iLoop, args))
                        chRetVal = 1;
                    else if (chRetVal == 0)
                        chRetVal = -2;
                } else if (chRetVal == 0) {
                    chRetVal = -1;
                }

                funcAlways(static_cast<_AlwaysClass> (*iter), iLoop, args);

                iLoop++;
            }
            return chRetVal;
        }

        /** @brief Iteratively call @a pred for each node in the child 
         * collection and break out of the loop when @a pred returns true.
         * 
         * @param pred Pointer to predicate function that will be called on each
         * iteration.
         * @param args Void pointer containing user data to be passed
         * to @a pred.
         * 
         * @return A char number indicating the success of the routine.\n
         * 0  = List was empty. \n
         * 1  = @a pred passed. \n
         * -1 = @a pred did not pass.
         * 
         * The input parameters of @a pred must be: \n
         * "(_PredClass ref, int loop, void* args)" \n
         * where _PredClass is defined as the template parameter.
         */
        template <typename _PredClass = T,
        typename _PredFunction>
        char _forfirst(_PredFunction pred,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            for (__t_childnodes_iter iter = this->__m_vChildren.begin();
                    iter != this->__m_vChildren.end();
                    ++iter) {

                if (pred(static_cast<_PredClass> (*iter), iLoop, args)) {
                    chRetVal = 1;
                    break;
                } else {
                    chRetVal = -1;
                }

                iLoop++;
            }
            return chRetVal;
        }

        /** @brief Iteratively call @a pred for each node in the child 
         * collection then call @a func and break out of the loop when 
         * @a pred returns true.
         * 
         * @param pred Pointer to predicate function that will be called on each 
         * iteration.
         * @param func Pointer to function that will be called when @a pred 
         * returns true.
         * @param args Void pointer containing user data to be passed to 
         * @a pred and @a func.
         * 
         * @return A char number indicating the success of the routine.\n
         * 0  = List was empty. \n
         * 1  = @a pred passed then @a func was called and returned true. \n
         * -2 = @a pred passed then @a func was called and returned false. \n
         * -1 = @a pred did not pass.
         * 
         * The input parameters of @a pred must be: \n
         * "(_PredClass ref, int loop, void* args)" \n
         * where _Class is defined as the first template parameter.
         * 
         * The input parameters of @a func must be: \n
         *     "(_Class ref, int loop, void* args)" \n
         * where _Class is defined as the second template parameter.
         */
        template <typename _PredClass = T,
        typename _Class = T,
        typename _PredFunction,
        typename _Function>
        char _forfirst_then(_PredFunction pred,
                _Function func,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            for (__t_childnodes_iter iter = this->__m_vChildren.begin();
                    iter != this->__m_vChildren.end();
                    ++iter) {

                if (pred(static_cast<_PredClass> (*iter), iLoop, args)) {
                    if (func(static_cast<_Class> (*iter), iLoop, args)) {
                        chRetVal = 1;
                    } else {
                        chRetVal = -2;
                    }
                    break;
                } else {
                    chRetVal = -1;
                }

                iLoop++;
            }
            return chRetVal;
        }

        /** @brief Iteratively call @a pred for each node in the child 
         * collection, removing and deleting any items where @a pred
         * returns true.
         * 
         * @param pred Pointer to predicate function that will be called on each
         * iteration.
         * @param args Void pointer containing user data to be passed
         * to @a pred.
         * 
         * @return A char number indicating the success of the routine.\n
         * 0  = List was empty. \n
         * 1  = @a pred passed and at least one item was deleted. \n
         * -1 = @a pred did not pass and no items were deleted.
         * 
         * The input parameters of @a pred must be: \n
         * "(_PredClass ref, int loop, void* args)" \n
         * where _PredClass is defined as the first template parameter.
         */
        template <typename _PredClass = T,
        typename _ElseClass = T,
        typename _PredFunction>
        char _foreach_if_delete(_PredFunction predDelete,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            __t_childnodes_iter iter = this->__m_vChildren.begin();
            while (iter != this->__m_vChildren.end()) {
                if (predDelete(*iter, iLoop, args)) {
                    T tHook = (*iter);
                    iter = this->__m_vChildren.erase(iter);
                    delete tHook;
                    chRetVal = 1;
                } else {
                    if (chRetVal == 0)
                        chRetVal = -1;
                    ++iter;
                }
            }
            return chRetVal;
        }

        /** @brief Iteratively call @a pred for each node in the child 
         * collection, removing and deleting the first node where @a pred
         * returns true.
         * 
         * @param pred Pointer to predicate function that will be called on each
         * iteration.
         * @param args Void pointer containing user data to be passed
         * to @a pred.
         * 
         * @return A char number indicating the success of the routine.\n
         * 0  = List was empty. \n
         * 1  = @a pred passed and at least one item was deleted. \n
         * -1 = @a pred did not pass and no items were deleted.
         * 
         * The input parameters of @a pred must be: \n
         * "(_PredClass ref, int loop, void* args)" \n
         * where _PredClass is defined as the first template parameter.
         */
        template <typename _PredClass = T,
        typename _ElseClass = T,
        typename _PredFunction>
        char _forfirst_if_delete(_PredFunction predDelete,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            __t_childnodes_iter iter = this->__m_vChildren.begin();
            while (iter != this->__m_vChildren.end()) {
                if (predDelete(*iter, iLoop, args)) {
                    T tHook = (*iter);
                    iter = this->__m_vChildren.erase(iter);
                    delete tHook;
                    chRetVal = 1;
                    break;
                } else {
                    if (chRetVal == 0)
                        chRetVal = -1;
                    ++iter;
                }
            }
            return chRetVal;
        }

        /** @brief Iteratively call @a pred for each node in the child 
         * collection, then removing and deleting any items where @a pred
         * returns true or calling @a funcElse where @a pred returns false.
         * 
         * @param pred Pointer to predicate function that will be called on each 
         * iteration.
         * @param funcElse Pointer to function that will be called when @a pred returns false.
         * @param args Void pointer containing user data to be passed to @a pred.
         * 
         * @return A char number indicating the success of the routine.\n
         * 0  = List was empty. \n
         * 1  = @a pred passed and either an item was deleted or funcElse
         * returned true at least once. \n
         * -1 = @a pred did not pass, no items were deleted and @a funcElse
         * returned true at least once.
         * -2 = @a pred did not pass, no items were deleted and @a funcElse
         * did not return true.
         * 
         * The input parameters of @a pred must be: \n
         * "(_PredClass ref, int loop, void* args)" \n
         * where _PredClass is defined as the first template parameter.
         */
        template <typename _PredClass = T,
        typename _ElseClass = T,
        typename _PredFunction,
        typename _FunctionElse>
        char _foreach_if_delete_else(_PredFunction pred,
                _FunctionElse funcElse,
                void* args = nullptr) {

            int iLoop(0);
            char chRetVal(0);
            __t_childnodes_iter iter = this->__m_vChildren.begin();
            while (iter != this->__m_vChildren.end()) {
                if (pred(static_cast<_PredClass> (*iter), iLoop, args)) {
                    T tHook = (*iter);
                    iter = this->__m_vChildren.erase(iter);
                    chRetVal = 1;
                    delete tHook;
                } else {
                    if (funcElse(static_cast<_ElseClass> (*iter), iLoop, args))
                        if (chRetVal != 1)
                            chRetVal = -1;
                        else if (chRetVal == 0)
                            chRetVal = -2;

                    ++iter;
                }
            }
            return chRetVal;
        }

    private:

        typedef typename std::list<T> __t_childnodes;
        typedef typename __t_childnodes::iterator __t_childnodes_iter;
        typedef typename __t_childnodes::const_iterator __t_childnodes_const_iter;

        __t_childnodes __m_vChildren;

    };
}
#endif	/* PARENT_H */