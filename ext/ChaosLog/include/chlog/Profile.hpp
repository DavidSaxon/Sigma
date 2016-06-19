/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSLOG_PROFILE_HPP_
#define CHAOSLOG_PROFILE_HPP_

#include <chaoscore/base/str/UTF8String.hpp>

namespace chlog
{

/*!
 * \brief Structure used to stored information about a logging profile.
 *
 * Used to define information that should be included with log entries.
 */
struct Profile
{
public:

    //----------------------------PUBLIC ATTRIBUTES-----------------------------

    /*!
     * \brief The name of the application writing logs using this profile.
     *
     * \note If application name is left empty it will not be included in log
     *       entires.
     */
    const chaos::str::UTF8String app_name;
    /*!
     * \brief The version of the application writing logs using this profile.
     *
     * \note If application name is left empty it will not be included in log
     *       entries.
     */
    const chaos::str::UTF8String app_version;

    //-------------------------------CONSTRUCTOR--------------------------------

    /*!
     * \brief Creates a new logging profile with the given values.
     */
    Profile(
            const chaos::str::UTF8String& _app_name = "",
            const chaos::str::UTF8String& _app_version = "")
        :
        app_name   (_app_name),
        app_version(_app_version)
    {
    }
};

} // namespace chlog

#endif
