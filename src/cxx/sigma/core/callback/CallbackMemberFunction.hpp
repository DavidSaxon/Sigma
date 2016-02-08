/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_CALLBACKMEMBERFUNCTION_HPP_
#define SIGMA_CORE_CALLBACKMEMBERFUNCTION_HPP_

namespace sigma
{
namespace core
{

/*!
 * \brief TODO: doc
 */
template<typename... function_parameters>
class CallbackMemberFunction
{
public:

    /*!
     * \brief TODO:
     */
    template<typename owner_type,
             void (owner_type::*member_function)(function_parameters...)>
    static void member_wrapper(void* owner, function_parameters... params)
    {
        (static_cast<owner_type*>(owner)->*member_function)(params...);
    }
};

} // namespace core
} // namespace sigma

#endif
