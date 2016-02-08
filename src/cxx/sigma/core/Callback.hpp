/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_CALLBACK_HPP_
#define SIGMA_CORE_CALLBACK_HPP_

#include <cassert>
#include <memory>

#include <chaoscore/base/BaseExceptions.hpp>

// TODO: REMOVE ME
#include <iostream>

namespace sigma
{
namespace core
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

template<typename... function_parameters>
class CallbackHandler;

//------------------------------------------------------------------------------
//                            GLOBAL STATIC VARIABLES
//------------------------------------------------------------------------------

namespace {

/*!
 * \brief Counter used to retrieve the next available callback id.
 */
static chaos::uint64 g_next_callback_id = 0;

} // namespace anonymous

//------------------------------------------------------------------------------
//                             FORWARD DECELERATIONS
//------------------------------------------------------------------------------

class ScopedCallback;

//------------------------------------------------------------------------------
//                                    CLASSES
//------------------------------------------------------------------------------

// TODO: hide this from doxygen

/*!
 * \brief TODO:
 */
class CallbackBase
{
protected:

    friend class ScopedCallback;

    virtual void unregister_function(chaos::uint64 id) = 0;
};

/*!
 * \brief TODO:
 */
class ScopedCallback
{
private:

    //--------------------------------------------------------------------------
    //                             PRIVATE STRUCTURES
    //--------------------------------------------------------------------------

    struct ReferenceCounter
    {
        bool early_unregister;
        chaos::uint64 count;
        ReferenceCounter() : early_unregister(false), count(1) {}
    };

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    ScopedCallback(CallbackBase* interface, chaos::uint64 id)
        :
        m_ref_counter(new ReferenceCounter()),
        m_interface  (interface),
        m_id         (id)
    {
    }

    /*!
     * \brief TODO:
     */
    ScopedCallback(const ScopedCallback& other)
        :
        m_ref_counter(other.m_ref_counter),
        m_interface  (other.m_interface),
        m_id         (other.m_id)
    {
        // increment the reference counter
        ++m_ref_counter->count;
    }

    /*!
     * \brief TODO:
     */
    ScopedCallback(ScopedCallback&& other)
        :
        m_ref_counter(other.m_ref_counter),
        m_interface  (other.m_interface),
        m_id         (other.m_id)
    {
        // increment the reference counter
        ++m_ref_counter->count;
    }

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    // delete the assignment operators
    ScopedCallback& operator=(const ScopedCallback& other) = delete;

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~ScopedCallback()
    {
        // reference counter should be non-zero
        assert(m_ref_counter->count > 0);

        // decrement the reference counter and check if this is the last
        // reference
        if (--m_ref_counter->count == 0)
        {
            // does the callback need unregistering?
            if (!m_ref_counter->early_unregister)
            {
                m_interface->unregister_function(m_id);
            }
            // clean up the reference counter
            delete m_ref_counter;
        }
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief simple structure used for counting references.
     */
    ReferenceCounter* m_ref_counter;
    /*!
     * \brief The CallbackInterface that owns this callback.
     */
    CallbackBase* m_interface;
    /*!
     * \brief The global id of the callback this is handling.
     */
    chaos::uint64 m_id;
};

/*!
 * \brief TODO:
 */
template<typename... function_parameters>
class CallbackInterface : public CallbackBase
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN( CallbackInterface );

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class ScopedCallback;
    friend class CallbackHandler<function_parameters...>;

    //--------------------------------------------------------------------------
    //                             PRIVATE STRUCTURES
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    union PermissiveFunction
    {
        /*!
         *\brief Represents a standard global or static function which is not
         *       bound to an object.
         */
        void (*standard)(function_parameters...);
        /*!
         * \brief Represents a member function which is bound to it's owner
         *        object.
         */
        void (*member)(void*, function_parameters...);
    };

    /*!
     * \brief TODO:
     */
    struct CallbackData
    {
        /*!
         * \brief The owner of this callback function. Will be null if this
         *        callback is not a member function.
         */
        void* owner;
        /*!
         * \brief The callback function.
         */
        PermissiveFunction function;
    };

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    CallbackInterface()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    ScopedCallback register_function(
            void (*callback_function)(function_parameters...))
    {
        // increment the global callback id and use it as this callback id
        chaos::uint64 id = g_next_callback_id++;
        // this callback shouldn't be registered
        assert(m_callback_funcs.find(id) == m_callback_funcs.end());

        // store this function mapped under the id
        CallbackData f;
        f.owner = NULL;
        f.function.standard = callback_function;
        m_callback_funcs[id] = f;

        // TODO: REMOVE ME
        std::cout << "registering callback: " << id << std::endl;

        // create a scoped callback to return
        return ScopedCallback(this, id);
    }

    /*!
     * \brief TODO:
     */
    template<typename owner_type,
             void (owner_type::*function_type)(function_parameters...)>
    ScopedCallback register_member_function(owner_type* owner)
    {
        // increment the global callback id and use it as this callback id
        chaos::uint64 id = g_next_callback_id++;
        // this callback shouldn't be registered
        assert(m_callback_funcs.find(id) == m_callback_funcs.end());

        // store this function mapped under the id
        CallbackData f;
        f.owner = owner;
        f.function.member = member_wrapper<owner_type, function_type>;
        m_callback_funcs[id] = f;

        // TODO: REMOVE ME
        std::cout << "registering callback: " << id << std::endl;

        // create a scoped callback to return
        return ScopedCallback(this, id);
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Mapping which stores callback function ids to callback functions
     */
    std::map<chaos::uint64, CallbackData> m_callback_funcs;

    //--------------------------------------------------------------------------
    //                          PRIVATE STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    template<typename owner_type,
             void (owner_type::*member_function)(function_parameters...)>
    static void member_wrapper(void* owner, function_parameters... params)
    {
        (static_cast<owner_type*>(owner)->*member_function)(params...);
    }

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    virtual void unregister_function(chaos::uint64 id)
    {
        // TODO: REMOVE ME
        std::cout << "unregistering callback: " << id << std::endl;

        // TODO:
        // ensure the id is associated with this object
        // if (!has_function(id))
        // {
        //     chaos::uni::UTF8String error_message;
        //     error_message << "CallbackManager does not have a callback "
        //                   << "function with the id: " << id;

        //     throw chaos::ex::KeyError( error_message );
        // }
        // remove the id and function from the map
        m_callback_funcs.erase(id);
    }

    /*!
     * \brief TODO
     */
    void emit(function_parameters... params)
    {
        // iterate over the attached functions and call them
        CHAOS_FOR_EACH(it, m_callback_funcs)
        {
            // member function?
            if (it->second.owner != NULL)
            {
                it->second.function.member(it->second.owner, params...);
            }
            // standard function
            else
            {
                it->second.function.standard(params...);
            }
        }
    }
};

/*!
 * \brief TODO:
 */
template<typename... function_parameters>
class CallbackHandler
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    CallbackHandler()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    CallbackInterface<function_parameters...>& get_interface()
    {
        return m_interface;
    }

    /*!
     * \brief TODO
     */
    void emit(function_parameters... params)
    {
        m_interface.emit(params...);
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The internal CallbackInterface
     */
    CallbackInterface<function_parameters...> m_interface;
};

} // namespace core
} // namespace sigma

#endif
