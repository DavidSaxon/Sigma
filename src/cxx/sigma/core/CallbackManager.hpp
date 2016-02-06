/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_CALLBACKMANAGER_HPP_
#define SIGMA_CORE_CALLBACKMANAGER_HPP_

#include <map>

#include <chaoscore/base/BaseExceptions.hpp>

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
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new callback manager.
     */
    CallbackManager()
    {
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
        m_callback_funcs[id] = callback_function;
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
        return m_callback_funcs[id];
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
            (*it)(params)
        }
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Mapping which stores callback function ids to callback functions
     */
    std::map<chaos::uint64, void (*)(function_parameters...)> m_callback_funcs;
};

} // namespace core
} // namespace sigma

#endif
