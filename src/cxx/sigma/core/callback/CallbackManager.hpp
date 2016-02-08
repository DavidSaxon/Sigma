/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_CALLBACKMANAGER_HPP_
#define SIGMA_CORE_CALLBACKMANAGER_HPP_

#include <cassert>
#include <map>
#include <memory>

#include <chaoscore/base/BaseExceptions.hpp>

// TODO: FIX DOCS

// TODO: Manager class holds callback interface and has the fire function??

namespace sigma
{
namespace core
{

//------------------------------------------------------------------------------
//                            GLOBAL STATIC VARIABLES
//------------------------------------------------------------------------------

namespace {

/*!
 * \brief Counter used to retrieve the next available callback id.
 */
static chaos::uint64 g_next_callback_id = 0;

} // namespace anonymous

/*!
 * \brief Object used to manager a specific callback.
 *
 * This object is instantiated at locations where a callback is required. It
 * handles a list of functions that will be called when the owner of this is
 * triggered.
 *
 * The CallbackManager supports arbitrary function types by using a template.
 * Where the arguments of the callback function should be define through the
 * template.
 *
 * For example to instantiate a CallbackManager for a function that takes a
 * boolean and two integers as its arguments:
 *
 * \code
 * sigma::core::CallbackManager<bool, int, int> my_callback_manager;
 * \endcode
 */
template<typename... function_parameters>
class CallbackManager
{
private:

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
        void* owner;
        PermissiveFunction function;
    };

public:

    //--------------------------------------------------------------------------
    //                               PUBLIC CLASSES
    //--------------------------------------------------------------------------

    class Trigger
    {
        friend class CallbackManager;
    public:

        Trigger()
            :
            m_associated_callback(nullptr)
        {
        }

        void fire(function_parameters... params)
        {
            // TODO: throw exception?
            assert(m_associated_callback != NULL);

            m_associated_callback->trigger(params...);
        }

    protected:

        /*!
         * \brief The CallbackManager this trigger is associated with
         */
        CallbackManager* m_associated_callback;
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new callback manager.
     */
    CallbackManager(Trigger& trigger)
    {
        trigger.m_associated_callback = this;
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Registers the given function in this CallbackManager.
     *
     * When this CallbackManager is triggered the registered function will be
     * called.
     *
     * \param callback_function The function to register in this
     *                          CallbackManager. The function's parameters must
     *                          match the types provided to this object's
     *                          template.
     *
     * \return A globally unique value identifying the provided callback
     *         function. This id is unique across all CallbackManager instances.
     */
    chaos::uint64 register_function(
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
        return id;
    }

    /*!
     * \brief TODO:
     */
    template<typename owner_type,
             void (owner_type::*function_type)(function_parameters...)>
    chaos::uint64 register_member_function(owner_type* owner)
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
        return id;
    }

    /*!
     * \brief Unregisters the function associated with the given id from this
     *        CallbackManager.
     *
     * Once a function has been unregistered it will no longer be called when
     * this object is triggered.
     *
     * \throws chaos::ex::KeyError If there is no function associated with the
     *                             given id in this CallbackManager.
     *
     * \param id The identifier associated with the function to unregister.
     */
    void unregister_function(chaos::uint64 id)
    {
        // ensure the id is associated with this object
        if (!has_function(id))
        {
            chaos::uni::UTF8String error_message;
            error_message << "CallbackManager does not have a callback "
                          << "function with the id: " << id;

            throw chaos::ex::KeyError( error_message );
        }
        // remove the id and function from the map
        m_callback_funcs.erase(id);
    }

    /*!
     * \brief Returns the number of functions currently registered in this
     *        CallbackManager.
     */
    std::size_t get_size() const
    {
        return m_callback_funcs.size();
    }

    /*!
     * \brief Returns whether this CallbackManager has a function associated
     *        with the given id.
     */
    bool has_function(chaos::uint64 id) const
    {
        // is this id stored in the map?
        return m_callback_funcs.find(id) != m_callback_funcs.end();
    }

    /*!
     * \brief Returns the function associated with the given id.
     *
     * \throws chaos::ex::KeyError If there is no function associated with the
     *                             given id in this CallbackManager.
     */
    void (*get_function(chaos::uint64 id) const)(function_parameters...)
    {
        // ensure the id is associated with this object
        if (!has_function(id))
        {
            chaos::uni::UTF8String error_message;
            error_message << "CallbackManager does not have a callback "
                          << "function with the id: " << id;
            throw chaos::ex::KeyError( error_message );
        }
        // return the function
        return m_callback_funcs[id].function;
    }

    // TODO: get function owner??

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

    /*!
     * \brief Calls all functions in this CallbackManager with the given
     *        parameters.
     */
    void trigger(function_parameters... params)
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

} // namespace core
} // namespace sigma

#endif
