/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_CALLBACKMANAGER_HPP_
#define SIGMA_CORE_CALLBACKMANAGER_HPP_

#include <map>

#include <chaoscore/base/Types.hpp>

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

    void unregister_function(chaos::uint64 id)
    {
        // ensure the id is associated with this object
        if (m_callback_funcs.find(id) == m_callback_funcs.end())
        {
            // TODO: throw key error
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
