/*!
 * \file
 * \author David Saxon
 * \brief The main entry point for the Sigma API.
 *
 * Most importantly this file provides the init function which must be called
 * before Sigma can be used.
 */
#ifndef SIGMA_CORE_SIGMA_HPP_
#define SIGMA_CORE_SIGMA_HPP_

namespace sigma
{
/*!
 * \brief The core backend API of Sigma. Sigma core is entirely detached from
 *        any Sigma UI.
 */
namespace core
{

//------------------------------------------------------------------------------
//                                  ENUMERATORS
//------------------------------------------------------------------------------

/*!
 * \brief The built-in API domains of Sigma.
 */
enum APIDomains
{
    /// Represents none of the Sigma API domain.
    API_NONE  = 0L,
    /// The build system API domain. See sigma::core::build
    API_BUILD = 1L << 1,
    /// The test frame work API domain. See sigma::core::test
    API_TEST  = 1L << 2,
    /// The task management API domain. See sigma::core::tasks
    API_TASKS = 1L << 3,
    /// The linter API domain. See sigma::core::lint
    API_LINT  = 1L << 4,
    /// Represents all of the Sigma API domain.
    API_ALL   = 0xFFFFFFFFFFFFFFFF
};

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Initialises Sigma.
 *
 * TODO:
 */
void init(APIDomains api_domains=API_ALL);

} // namespace core
} // namespace sigma

#endif
