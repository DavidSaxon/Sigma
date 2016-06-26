/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANELOG_VERBOSITY_HPP_
#define ARCANELOG_VERBOSITY_HPP_

namespace arclog
{

/*!
 * \brief Enumerator representing the possible verbosity levels of logging
 *        messages.
 */
enum Verbosity
{
    /// Reports of imminent catastrophic failure.
    VERBOSITY_CRITICAL = 1UL << 0,
    /// Reports of an unexpected error which may cause failure.
    VERBOSITY_ERROR    = 1UL << 1,
    /// Reports of an unexpected error which may cause the application to
    /// perform unexpectedly.
    VERBOSITY_WARNING  = 1UL << 2,
    /// Reports high level notices about the runtime of the application.
    VERBOSITY_NOTICE   = 1UL << 3,
    /// Reports explicit information about the the runtime of the application.
    VERBOSITY_INFO     = 1UL << 4,
    /// Reports implementation specific information to aid in development and
    /// debugging.
    VERBOSITY_DEBUG    = 1UL << 5
};

} // namespace arclog

#endif
