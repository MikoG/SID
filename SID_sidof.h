/* 
 * File:   sidof.h
 * Author: geth2k
 *
 * Created on 04 May 2014, 14:52
 */

#ifndef SIDOF_H
#define	SIDOF_H

#include <memory>
#include <array>
#include <map>
#include <boost/variant.hpp>
#include <string.h>
#include "SID_file.h"

namespace sid {

    typedef boost::variant<
    boost::blank,
    bool,
    int,
    float,
    std::string,
    std::vector<bool>,
    std::vector<int>,
    std::vector<float>,
    std::vector<std::string>
    > t_variant;

    typedef std::map<std::string, t_variant> t_variantMap;

    enum variant_types {
        T_BLANK      = 0,
        T_BOOL       = 1,
        T_INT        = 2,
        T_FLOAT      = 3,
        T_STRING     = 4,
        T_VEC_BOOL   = 5,
        T_VEC_INT    = 6,
        T_VEC_FLOAT  = 7,
        T_VEC_STRING = 8
    };

    enum class return_codes {
        ILLEGAL,
        OBJECT_NAME,
        STRING_DELIMITER,
        ASSIGNMENT,
        BLOCK_START,
        BLOCK_END,
        PARENTH_START,
        PARENTH_END,
        LINE_END,
        COMMENT,
        WHITESPACE,
        NUMBER,
        BOOLEAN,
        LIST_SEPERATOR,
        STRING_TEXT,
        ARRAY_START,
        ARRAY_END
    };
    
    template<typename U>
    inline void get_val(U& ret, sid::t_variant const& v) {
        //ret = boost::apply_visitor(retrieve_value_visitor<U>(), v);
        ret = boost::get<U>(v);
    }    

    template <typename U>
    class push_vector_value_visitor : public boost::static_visitor<> {
    public:

        explicit push_vector_value_visitor(U const& value) : __Val(value) {
        }

        void operator ()(std::vector<U>& vec) const {
            vec.push_back(this->__Val);
        }

        template<typename T>
        void operator ()(T const& vec) const {
        }

    private:
        U __Val;

    };

    template <typename T>
    inline void push_array_value(T const& value, sid::t_variant& var) {
        sid::push_vector_value_visitor<T> PushBack(value);
        boost::apply_visitor(PushBack, var);
    }

    template <typename U>
    class basic_object {
    protected:
        typedef std::vector<std::shared_ptr<U> > _t_objVect;
        typedef typename _t_objVect::iterator _t_objIter;
        typedef typename _t_objVect::const_iterator _t_objConstIter;

    public:

        /**@brief Default constructor.
         *  
         * Creates %basic_object with name "unnamed".
         */
        basic_object() :
        __m_strName("unnamed") {
        }

        /**@brief Creates a %basic_object with specified name.
         * 
         * @param name A std:string of the required name
         */
        basic_object(std::string const& name) :
        __m_strName(name) {
        }

        /**@brief %basic_object copy constructor
         * 
         * @param orig A %basic_object object to be copied from.
         */
        basic_object(basic_object const& orig) :
        __m_strName(orig.__m_strName),
        __m_Nodes(orig.__m_Nodes) {
        }

        /**@brief %basic_object move constructor.
         * 
         * @param orig A %basic_object object to be moved.
         */
        basic_object(basic_object&& orig) {
            __m_strName = orig.__m_strName;
            __m_Nodes = orig.__m_Nodes;
            orig.__m_Nodes.clear();
            orig.__m_strName.clear();
        }

        /**@brief %basic_object destructor.
         * 
         */
        virtual ~basic_object() {
            this->__m_Nodes.clear();
        }

        /**@brief Getter for %basic_object name.
         * 
         * @return A const reference to and std::string.
         */
        inline std::string const& get_name() const {
            return (this->__m_strName);
        }

        /**@brief Adds a child object.
         * 
         * @param child A shared_ptr of type %basic_object.
         * 
         * Pushes @a child on to the %basic_object's nodes vector.
         */
        inline void add_child(std::shared_ptr<U> const child) {
            this->__m_Nodes.push_back(child);
        }

        /**@brief %basic_object subscript operator.
         * 
         * @param id A std::string representing the name of the 
         * requested %basic_object.
         * 
         * @return A const reference of type @a U.
         * 
         * The subscript operators can be chained together to
         * access child objects. Can be used in rvalue context only.
         */
        U const& operator[] (std::string const& id) const {
            for (_t_objConstIter itObject = this->__m_Nodes.begin();
                    itObject != this->__m_Nodes.end();
                    itObject++) {
                if ((*itObject)->get_name() == id) {
                    return (**itObject);
                }
            }
            throw std::range_error("Sub object '" + id + "'" +
                    " does not exist in object '" + this->get_name() +
                    "'.");
        }
        
        /**@brief Returns the number of child nodes attached to the 
         * current node.
         * 
         * @return Returns a value of type std::size_t.
         */
        inline std::size_t const size() const {
            return (this->__m_Nodes.size());
        }        

        /**@brief Test for a child node with given name.
         * 
         * @param id A std::string of the %basic_object name to be tested.
         * @return Returns true if %basic_object child with name of @a id
         * exists. Otherwise returns false.
         */
        bool const has_child(std::string const& id) const {
            for (_t_objConstIter itObject = this->__m_Nodes.begin();
                    itObject != this->__m_Nodes.end();
                    itObject++) {
                if ((*itObject)->get_name() == id) {
                    return (true);
                }
            }
            return (false);
        }
        
        /**@brief Returns a pointer to a child node by it's position in the map.
         * 
         * @param id A uint representing the position of the child node.
         * @return Returns a pointer to the child node, nullptr is returned if
         * %pos is greater than the position of the last node.
         */        
        std::shared_ptr<U const> at(std::size_t const pos) const {
            std::shared_ptr<U const> uRet;
            int iii = 0;
            for (_t_objConstIter itObject = this->__m_Nodes.begin();
                    itObject != this->__m_Nodes.end();
                    itObject++) {
                if (iii == pos) {
                    uRet = *itObject;
                    itObject = this->__m_Nodes.end();
                }
                iii++;
            }
            return (uRet);
        }

    private:
        std::string __m_strName; // String representing the Object's name.
        _t_objVect __m_Nodes; // Vector of child node Objects.
    };

    /**Extends class %basic_object and specialises in its self, 
     * allowing child nodes to be of type %sido.
     * 
     * This extension provides a weak pointer to a parent object
     * and a std::map of type t_variant to represent the 
     * object's values.
     */
    class sido : public basic_object<sido> {
    public:

        /**@brief Default constructor.
         * 
         */
        sido() {
        }

        /**@brief Constructor that creates a %sido with specified name 
         * and assigns a pre-populated std::map of values.
         * 
         * @param name A std::string of the required name.
         * @param values A std::map of type t_variant.
         * 
         * The %sido class does not provide any methods for changing 
         * the contents of @a values, meaning that @a values must be 
         * pre-populated before calling this constructor.
         */
        sido(std::string const& name,
                t_variantMap const& values) :
        basic_object(name),
        __m_Values_(values) {
        }

        /**@brief %sido copy constructor.
         * 
         * @param orig A %sido object to be copied from.
         */
        sido(sido const& orig) :
        __m_SidoParent(orig.__m_SidoParent),
        __m_Values_(orig.__m_Values_),
        basic_object(orig) {
        }

        /**@brief %sido move constructor.
         * 
         * @param orig A %sido object to be moved.
         */
        sido(sido&& orig) :
        basic_object(std::move(orig)) {
            this->__m_SidoParent = orig.__m_SidoParent;
            this->__m_Values_ = orig.__m_Values_;
            orig.__m_Values_.clear();
            orig.__m_SidoParent.reset();

        }

        /** @brief %sido destructor.
         * 
         */
        virtual ~sido() {
            __m_Values_.clear();
        }

        /**@brief Set the parent object.
         * 
         * @param parent A std::shared_ptr of type %sido to 
         * the parent object.
         * 
         * To break the circular reference of child to parent and 
         * parent to child, @a parent is converted to a 
         * std::weak_ptr when stored. The equivalent getter (get_parent())
         * will convert the std::weak_ptr back to a std::shared_ptr
         * when returning.
         */
        inline void set_parent(
                std::shared_ptr<sido> const& parent) {
            this->__m_SidoParent = parent;
        }

        /**@brief Get the pointer to the parent object.
         * 
         * @return A std::shared_ptr of type %sido.
         * 
         * The parent object is stored as a std::weak_ptr,
         * this method will convert the pointer to a usable
         * std::shared_ptr during return.
         */
        inline std::shared_ptr<sido> const get_parent() const {
            return (this->__m_SidoParent.lock());
        }

        /**@brief Get the value associated to the given name.
         * 
         * @param id A std::string of the required value's name.
         * @return A t_variant associated to @a id.
         * 
         * An exception will be thrown if @a id is not a valid name.
         * Users should use has_value() to check if a name exists before 
         * trying to access it.
         */
        t_variant get_value(
                std::string const& id) const {
            std::map<std::string, t_variant>::const_iterator itVal =
                    this->__m_Values_.find(id);
            if (itVal == this->__m_Values_.end()) {
                throw std::range_error(
                        "value '" + id + "'" +
                        " does not exist in object '" +
                        this->get_name() + "'.");

            } else {
                return itVal->second;
            }
        }

        /**@brief Check if a value of the requested name exists.
         * 
         * @param id A std::string of represting the required value's name.
         * @return A boolean value of true if @a id exists or false if 
         * it does not exist.
         */
        bool const has_value(
                std::string const& id) const {
            std::map<std::string, t_variant>::const_iterator itVal =
                    this->__m_Values_.find(id);
            if (itVal == this->__m_Values_.end()) {
                return (false);
            } else {
                return (true);
            }
        }

    private:
        std::weak_ptr<sido> __m_SidoParent; // Points to the parent object.
        t_variantMap __m_Values_; // A Map of the object's values.

    };

    /** A %sido collection class that extends %basic_object and specialises
     * in sidos.
     * 
     * The purpose of this is class is to build the %sido objects
     * based on the information it receives from the sidof file 
     * passer and to store the root nodes of the %sido structure.
     */
    class objects : public basic_object<sido> {
    public:

        /** Stores a single word from the input sido file and the
         * type of information that the word represents.
         */
        struct t_component {
            std::string strComponent;
            return_codes compType;
        };
        typedef std::vector<t_component> t_components;
        typedef typename t_components::const_iterator t_constCompIter;

        /**@brief Defualt constructor.
         * 
         * Calls the %basic_object contructor with the name "root".
         */
        objects() :
        __m_iRecursionLevel(0),
        __m_blArrayMode(false),
        __m_blArrayCreated(false),
        basic_object("root") {
        }

        /**@brief Copy constructor for %objects
         * 
         * @param orig A %objects object to be copied.
         */
        objects(
                const objects& orig) :
        __m_iRecursionLevel(orig.__m_iRecursionLevel),
        __m_blArrayMode(orig.__m_blArrayMode),
        __m_blArrayCreated(orig.__m_blArrayCreated),
        __m_vVariantMapStack(orig.__m_vVariantMapStack),
        __m_vstrNameStack(orig.__m_vstrNameStack),
        __m_vSidoPairStack(orig.__m_vSidoPairStack),
        basic_object(orig) {
        }

        /**@brief Move constructor for %objects
         * 
         * @param orig A %objects object to be moved.
         */
        objects(
                objects&& orig) :
        __m_iRecursionLevel(0),
        __m_blArrayMode(false),
        basic_object(std::move(orig)) {
            this->__m_blArrayMode = orig.__m_blArrayMode;
            this->__m_blArrayCreated = orig.__m_blArrayCreated;
            this->__m_iRecursionLevel = orig.__m_iRecursionLevel;
            this->__m_vSidoPairStack = orig.__m_vSidoPairStack;
            this->__m_vVariantMapStack = orig.__m_vVariantMapStack;
            this->__m_vstrNameStack = orig.__m_vstrNameStack;

            orig.__m_blArrayMode = false;
            orig.__m_iRecursionLevel = 0;
            orig.__m_vSidoPairStack.clear();
            orig.__m_vVariantMapStack.clear();
            orig.__m_vstrNameStack.clear();
        }

        /**@brief Destructor for %objects.
         * 
         */
        virtual ~objects() {
            __m_vVariantMapStack.clear();
            __m_vstrNameStack.clear();
            __m_vSidoPairStack.clear();
        }

        /**@brief Process a set of components.
         * 
         * @param vcomp t_components to be processed.
         * 
         * vcomp must end with a t_component of one of the following 
         * %return_codes values:
         *   - BLOCK_START:
         *   - LINE_END:
         *   - BLOCK_END:
         *   - ARRAY_START:
         *   - ARRAY_END:
         *   - LIST_SEPERATOR:
         * 
         * Validations will be applied to @a vcomp and an exception will be 
         * thrown if any of these validations fail.
         */
        void process_components(t_components const& vcomp);

    private:
        int __m_iRecursionLevel; // Monitors the recursion level.

        bool __m_blArrayMode; // Indicates whether an instruction to open.
        bool __m_blArrayCreated; // Indicates whether the array has been created.
        std::string __m_strArrayName; // Holds the name of the array that 
        // needs to be created.
        variant_types __m_ArrayType; // Holds an enum representing the type of
        // data stored in the active array.

        /**
         * 
         * @param Component
         */
        void __push_array_to_stack(t_component const& Component);

        /**
         * 
         * @param RValue
         */
        void __process_list_separator(t_variant const& RValue);

        /**
         * 
         */
        void __add_object_to_structure();

        /**@brief Check if a numeric string represents a float or integer.
         * 
         * @param strNumber A std::string of numeric characters.
         * @return An integer value. 1 = float, 0 = int & -1 if string 
         * is not numeric.
         */
        int __is_float(std::string const& strNumber) const;

        /**@brief Validates an object name component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_object_name(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;

        /**@brief Validates an block start component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_block_start(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;
        
        /**@brief Validates an block end component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_block_end(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;
        
        /**@brief Validates an array start component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_array_start(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;
        
        /**@brief Validates an array end component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_array_end(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;
        
        /**@brief Validates an assignment operator component. Throws an 
         * exception if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_assignment_operator(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;
        
        /**@brief Validates an string open component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_string_open(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;
        
        /**@brief Validates an string close component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_string_close(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;
        
        /**@brief Validates an number component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.c
         */
        bool __validate_number(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;
        
        /**@brief Validates an boolean component. Throws an exception
         * if any validations fail.
         * 
         * @param vComponents A reference to the t_components being 
         * processed.
         * @param curr A const iterator to the current t_component.
         * @param count An integer representing the position of the
         * current.
         * @return Always returns boolean true if completes without
         * throwing an exception.
         */
        bool __validate_boolean(t_components const& vComponents,
                t_constCompIter const& curr,
                int const count) const;

        /** Stores a std::shared_ptr of type %sido that is yet to 
         * be added as a child node to another %sido object and the 
         * recursion level as an integer at which it must be added.
         */
        struct t_sidoPair {
            int m_Level;
            std::shared_ptr<sido> m_Sido;
        };

        // Acts as a LIFO stack of Variant Maps.
        std::vector<t_variantMap> __m_vVariantMapStack;

        // Acts as a LIFO stack of Names.
        std::vector<std::string> __m_vstrNameStack;

        // Acts as a LIFO stack of sidoPairs.
        std::vector<t_sidoPair> __m_vSidoPairStack;
    };

    /** File passer class for .sido files.
     * 
     * This class's public functionality is limited to a const substring 
     * operator and a method to check if a root object exists before 
     * accessing it.
     * 
     * To create the sido structure, the class must be initialised with
     * a std::string parameter representing a path to the required .sido 
     * file.
     */
    class sidof {
    public:

        /**@brief Default constructor.
         * 
         */
        sidof() {
        }

        /**@brief Contructor for %sidof that passes the required file
         * 
         * @param filepath A std::string representing the path to the
         * required file.
         * 
         * After initialising with this constructor, the sido structure 
         * can be accessed by using the subscript( [] ) operators. 
         */
        sidof(std::string filepath);

        /**@brief Copy constructor for %objects.
         * 
         * @param orig A %objects to be copied.
         */
        sidof(const sidof& orig);

        /**@brief Destructor for %sidof objects.
         * 
         */
        virtual ~sidof();

        // To do: Create a move constructor here.

        /**@brief %sidof subscript operator.
         * 
         * @param id A std::string representing the name of the 
         * requested %basic_object.
         * 
         * @return A const reference of type @a sido.
         * 
         * The subscript operators can be chained together to
         * access child objects. Can be used in rvalue context only.
         */
        inline sido const& operator[] (std::string id) const {
            return (this->__m_Objects[id]);
        }

        /**@brief Test for a child node with given name.
         * 
         * @param id A std::string of the %basic_object name to be tested.
         * @return Returns true if %basic_object child with name of @a id
         * exists. Otherwise returns false.
         */
        inline bool const has_child(std::string const& id) const {
            return (this->__m_Objects.has_child(id));
        }
        
        /**@brief Test for a child node with given name.
         * 
         * @param id A std::string of the %basic_object name to be tested.
         * @return Returns true if %basic_object child with name of @a id
         * exists. Otherwise returns false.
         */
        inline std::shared_ptr<sido const> at(std::size_t const pos) const {
            return (this->__m_Objects.at(pos));
        }

    private:

        std::string const __m_strFielPath; // Path to the .sido file.
        objects __m_Objects; // Container class for sidos.

        /**The byte class is used to provide a set of data and 
         * instructions that can be associated to each of the 128 
         * characters in standard utf-8.
         * 
         * Although this class provides default functionality of always
         * returning return_codes::ILLEGAL, it is intended to be used 
         * as a base class to more complex derived classes that can
         * determine the component type for each character based on 
         * the preceeding data.
         * 
         * Each derived class is instantiated and initialised only once
         * and each value in an array of 128 pointers (one for each 
         * character) is associated to one of the %byte derived classes.
         */
        class byte {
        public:

            /**@brief Default constructor.
             * 
             * Sets the return code to ILLEGAL. 
             */
            byte() : _return_code(return_codes::ILLEGAL) {
            }

            /**@brief Constructor for %byte that allows the return 
             * code to be set during initialisation.
             * 
             * @param retcode The default return_codes value.
             */
            byte(return_codes retcode) : _return_code(retcode) {
            }

            /**Copy constructor for %byte.
             * 
             * @param orig A %byte object to be copied.
             */
            byte(const byte& orig) : _return_code(orig._return_code) {
            }

            /**@brief A destructor for %byte.
             * 
             */
            virtual ~byte() {
            }

            /**@brief A virtual function that, by default, returns the
             * return_code set during initialisation.
             * 
             * @param prev A return_codes value for the last byte 
             * that was processed.
             * @param prevNWS A return_codes value for the last 
             * non-whitespace byte that was processed.
             * @param arrayMode A boolean value indicating whether 
             * the bytes are being processed within an array structure.
             * @return A return_codes value indicating the component
             * type for the current byte.
             * 
             * Overridden versions of this function may return a value
             * other than the default return code, based on any of the
             * three input parameters.
             */
            virtual return_codes get_return_code(
                    return_codes& prev,
                    return_codes& prevNWS,
                    bool arrayMode) const {

                return (this->_return_code);
            }

        protected:

            const return_codes _return_code; // Default return code
        };

        /**Class for illegal characters.
         * 
         * Simply extends %byte without changing functionality.
         */
        class byte_illegal : public byte {
        };

        /**Class for object name characters.
         * 
         * Sets OBJECT_NAME as the default return code during 
         * initialisation. Uses default functionality from %byte
         * to always return the default return code.
         */
        class byte_objectname : public byte {
        public:

            byte_objectname() : byte(return_codes::OBJECT_NAME) {
            }

        };

        /**Class for string delimiter characters ( " ).
         * 
         * Sets STRING_DELIMITER as the default return code 
         * during initialisation. Uses default functionality
         *  from %byte to always return the default return code.
         */
        class byte_stringdelimiter : public byte {
        public:

            byte_stringdelimiter() : byte(return_codes::STRING_DELIMITER) {
            }
        };

        /**Class for the assignment operator character ( = ).
         * 
         * Sets ASSIGNMENT as the default return code 
         * during initialisation. Uses default functionality
         * from %byte to always return the default return code.
         */
        class byte_assignment : public byte {
        public:

            byte_assignment() : byte(return_codes::ASSIGNMENT) {
            }
        };

        /**Class for block start character ( { ).
         * 
         * Sets BLOCK_START as the default return code
         * during initialisation.
         * 
         * get_returncode() is overridden to return ARRAY_START
         * if @a prevNWS is ASSIGNMENT, otherwise the default
         * return code is returned.
         */
        class byte_blockstart : public byte {
        public:

            byte_blockstart() : byte(return_codes::BLOCK_START) {
            }

            virtual return_codes get_return_code(
                    return_codes& prev,
                    return_codes& prevNWS,
                    bool arrayMode) const {

                switch (prevNWS) {
                    case return_codes::ASSIGNMENT:
                        return (return_codes::ARRAY_START);
                        break;
                    default:
                        return (this->_return_code);
                        break;
                }
            }
        };

        /**Class for block end character ( } ).
         * 
         * Sets BLOCK_END as the default return code
         * during initialisation.
         * 
         * get_returncode() is overridden to return ARRAY_END
         * if @a arrayMode is true, otherwise the default
         * return code is returned.
         */
        class byte_blockend : public byte {
        public:

            byte_blockend() : byte(return_codes::BLOCK_END) {
            }

            virtual return_codes get_return_code(
                    return_codes& prev,
                    return_codes& prevNWS,
                    bool arrayMode) const {

                if (arrayMode == true) {
                    return (return_codes::ARRAY_END);
                } else {
                    return (return_codes::BLOCK_END);
                }
            }
        };

        /**Class for the open parenthesis operator character ( ( ).
         * 
         * Sets PARENTH_START as the default return code 
         * during initialisation. Uses default functionality
         * from %byte to always return the default return code.
         */
        class byte_parenthstart : public byte {
        public:

            byte_parenthstart() : byte(return_codes::PARENTH_START) {
            }
        };

        /**Class for the close parenthesis operator character ( ) ).
         * 
         * Sets PARENTH_END as the default return code 
         * during initialisation. Uses default functionality
         * from %byte to always return the default return code.
         */
        class byte_parenthend : public byte {
        public:

            byte_parenthend() : byte(return_codes::PARENTH_END) {
            }
        };

        /**Class for the line end character ( ; ).
         * 
         * Sets LINE_END as the default return code 
         * during initialisation. Uses default functionality
         * from %byte to always return the default return code.
         */
        class byte_lineend : public byte {
        public:

            byte_lineend() : byte(return_codes::LINE_END) {
            }
        };

        /**Class for the comment character ( # ).
         * 
         * Sets COMMENT as the default return code 
         * during initialisation. Uses default functionality
         * from %byte to always return the default return code.
         */
        class byte_comment : public byte {
        public:

            byte_comment() : byte(return_codes::COMMENT) {
            }
        };

        /**Class for whitespace characters ( space, tab, lf & cr ).
         * 
         * Sets WHITESPACE as the default return code 
         * during initialisation. Uses default functionality
         * from %byte to always return the default return code.
         */
        class byte_whitespace : public byte {
        public:

            byte_whitespace() : byte(return_codes::WHITESPACE) {
            }
        };

        /**Class for number characters ( 1..9 & . ).
         * 
         * Sets NUMBER as the default return code
         * during initialisation.
         * 
         * get_returncode() is overridden to return OBJECT_NAME
         * if @a prev is OBJECT_NAME, otherwise the default
         * return code is returned.
         */
        class byte_number : public byte {
        public:

            byte_number() : byte(return_codes::NUMBER) {
            }

            virtual return_codes get_return_code(
                    return_codes& prev,
                    return_codes& prevNWS,
                    bool arrayMode) const {

                switch (prev) {
                    case return_codes::OBJECT_NAME:
                        return (return_codes::OBJECT_NAME);
                        break;
                    default:
                        return (this->_return_code);
                        break;
                }
            }
        };

        /**Class for the list separator character ( , ).
         * 
         * get_returncode() is overridden to return ILLEGAL
         * if @a arrayMode is false, otherwise the default
         * return code is returned.
         */
        class byte_listsep : public byte {
        public:

            byte_listsep() : byte(return_codes::LIST_SEPERATOR) {
            }

            virtual return_codes get_return_code(
                    return_codes& prev,
                    return_codes& prevNWS,
                    bool arrayMode) const {

                if (arrayMode == true) {
                    return (_return_code);
                } else {
                    return (return_codes::ILLEGAL);
                }
            }
        };

        class bytes {
        public:

            bytes() {

                __bytes_array[0] = &__illegal;
                __bytes_array[1] = &__illegal;
                __bytes_array[2] = &__illegal;
                __bytes_array[3] = &__illegal;
                __bytes_array[4] = &__illegal;
                __bytes_array[5] = &__illegal;
                __bytes_array[6] = &__illegal;
                __bytes_array[7] = &__illegal;
                __bytes_array[8] = &__illegal;
                __bytes_array[9] = &__whitespace;
                __bytes_array[10] = &__whitespace;
                __bytes_array[11] = &__illegal;
                __bytes_array[12] = &__illegal;
                __bytes_array[13] = &__whitespace;
                __bytes_array[14] = &__illegal;
                __bytes_array[15] = &__illegal;
                __bytes_array[16] = &__illegal;
                __bytes_array[17] = &__illegal;
                __bytes_array[18] = &__illegal;
                __bytes_array[19] = &__illegal;
                __bytes_array[20] = &__illegal;
                __bytes_array[21] = &__illegal;
                __bytes_array[22] = &__illegal;
                __bytes_array[23] = &__illegal;
                __bytes_array[24] = &__illegal;
                __bytes_array[25] = &__illegal;
                __bytes_array[26] = &__illegal;
                __bytes_array[27] = &__illegal;
                __bytes_array[28] = &__illegal;
                __bytes_array[29] = &__illegal;
                __bytes_array[30] = &__illegal;
                __bytes_array[31] = &__illegal;
                __bytes_array[32] = &__whitespace;
                __bytes_array[33] = &__illegal;
                __bytes_array[34] = &__string_delimiter;
                __bytes_array[35] = &__comment;
                __bytes_array[36] = &__illegal;
                __bytes_array[37] = &__illegal;
                __bytes_array[38] = &__illegal;
                __bytes_array[39] = &__illegal;
                __bytes_array[40] = &__parenth_start;
                __bytes_array[41] = &__parenth_end;
                __bytes_array[42] = &__illegal;
                __bytes_array[43] = &__illegal;
                __bytes_array[44] = &__list_seperator;
                __bytes_array[45] = &__number;
                __bytes_array[46] = &__number;
                __bytes_array[47] = &__illegal;
                __bytes_array[48] = &__number;
                __bytes_array[49] = &__number;
                __bytes_array[50] = &__number;
                __bytes_array[51] = &__number;
                __bytes_array[52] = &__number;
                __bytes_array[53] = &__number;
                __bytes_array[54] = &__number;
                __bytes_array[55] = &__number;
                __bytes_array[56] = &__number;
                __bytes_array[57] = &__number;
                __bytes_array[58] = &__illegal;
                __bytes_array[59] = &__line_end;
                __bytes_array[60] = &__illegal;
                __bytes_array[61] = &__assignment;
                __bytes_array[62] = &__illegal;
                __bytes_array[63] = &__illegal;
                __bytes_array[64] = &__illegal;
                __bytes_array[65] = &__object_name;
                __bytes_array[66] = &__object_name;
                __bytes_array[67] = &__object_name;
                __bytes_array[68] = &__object_name;
                __bytes_array[69] = &__object_name;
                __bytes_array[70] = &__object_name;
                __bytes_array[71] = &__object_name;
                __bytes_array[72] = &__object_name;
                __bytes_array[73] = &__object_name;
                __bytes_array[74] = &__object_name;
                __bytes_array[75] = &__object_name;
                __bytes_array[76] = &__object_name;
                __bytes_array[77] = &__object_name;
                __bytes_array[78] = &__object_name;
                __bytes_array[79] = &__object_name;
                __bytes_array[80] = &__object_name;
                __bytes_array[81] = &__object_name;
                __bytes_array[82] = &__object_name;
                __bytes_array[83] = &__object_name;
                __bytes_array[84] = &__object_name;
                __bytes_array[85] = &__object_name;
                __bytes_array[86] = &__object_name;
                __bytes_array[87] = &__object_name;
                __bytes_array[88] = &__object_name;
                __bytes_array[89] = &__object_name;
                __bytes_array[90] = &__object_name;
                __bytes_array[91] = &__illegal;
                __bytes_array[92] = &__illegal;
                __bytes_array[93] = &__illegal;
                __bytes_array[94] = &__illegal;
                __bytes_array[95] = &__object_name;
                __bytes_array[96] = &__illegal;
                __bytes_array[97] = &__object_name;
                __bytes_array[98] = &__object_name;
                __bytes_array[99] = &__object_name;
                __bytes_array[100] = &__object_name;
                __bytes_array[101] = &__object_name;
                __bytes_array[102] = &__object_name;
                __bytes_array[103] = &__object_name;
                __bytes_array[104] = &__object_name;
                __bytes_array[105] = &__object_name;
                __bytes_array[106] = &__object_name;
                __bytes_array[107] = &__object_name;
                __bytes_array[108] = &__object_name;
                __bytes_array[109] = &__object_name;
                __bytes_array[110] = &__object_name;
                __bytes_array[111] = &__object_name;
                __bytes_array[112] = &__object_name;
                __bytes_array[113] = &__object_name;
                __bytes_array[114] = &__object_name;
                __bytes_array[115] = &__object_name;
                __bytes_array[116] = &__object_name;
                __bytes_array[117] = &__object_name;
                __bytes_array[118] = &__object_name;
                __bytes_array[119] = &__object_name;
                __bytes_array[120] = &__object_name;
                __bytes_array[121] = &__object_name;
                __bytes_array[122] = &__object_name;
                __bytes_array[123] = &__block_start;
                __bytes_array[124] = &__illegal;
                __bytes_array[125] = &__block_end;
                __bytes_array[126] = &__illegal;
                __bytes_array[127] = &__illegal;
            }

            bytes(const bytes* orig) {
            }

            virtual ~bytes() {
            }

            byte * const operator[] (std::size_t const id) const {
                if (id < 0 || id > 127) {
                    throw std::range_error(
                            "Character not in range of standard UTF-8");
                } else {
                    return (this->__bytes_array[id]);
                }
            }


        private:

            std::array<byte*, 128> __bytes_array;

            byte_illegal __illegal;
            byte_objectname __object_name;
            byte_stringdelimiter __string_delimiter;
            byte_assignment __assignment;
            byte_blockstart __block_start;
            byte_blockend __block_end;
            byte_parenthstart __parenth_start;
            byte_parenthend __parenth_end;
            byte_lineend __line_end;
            byte_comment __comment;
            byte_whitespace __whitespace;
            byte_number __number;
            byte_listsep __list_seperator;
        } __m_Bytes;
    };

} // namespace sid

#endif	/* SIDOF_H */