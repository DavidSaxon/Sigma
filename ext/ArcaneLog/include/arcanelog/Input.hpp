/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANELOG_INPUT_HPP_
#define ARCANELOG_INPUT_HPP_

#include <arcanecore/base/Preproc.hpp>

#include "arcanelog/Stream.hpp"

namespace arclog
{

/*!
 * \brief Log input object that provides streams for writing log messages with
 *        varying severity.
 *
 * Inputs cannot be manually constructed, see arclog::LogHandler::vend_input()
 * for creating inputs.
 */
class Input
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(Input);

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class LogHandler;

public:

    //--------------------------------------------------------------------------
    //                             PUBLIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief For reporting imminent catastrophic failure.
     */
    arclog::Stream critical;
    /*!
     * \brief For reporting an unexpected error which may cause failure.
     */
    arclog::Stream error;
    /*!
     * \brief For reporting an unexpected error which may cause the application
     *        to perform unexpectedly.
     */
    arclog::Stream warning;
    /*!
     * \brief For reporting high level notices about the runtime of the
     *        application.
     */
    arclog::Stream notice;
    /*!
     * \brief For reporting explicit information about the the runtime of the
     *        application.
     */
    arclog::Stream info;
    /*!
     * \brief For reporting implementation specific information to aid in
     *        development and debugging.
     */
    arclog::Stream debug;

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new Input.
     *
     * \param log_handler Pointer to the log_handler that owns this Input.
     * \param profile The logging profile to use for any messages reported
     *                through this input.
     */
    Input(arclog::LogHandler* log_handler, const arclog::Profile& profile);
};

} // namespace arclog

#endif
