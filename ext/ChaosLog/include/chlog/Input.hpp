/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSLOG_INPUT_HPP_
#define CHAOSLOG_INPUT_HPP_

#include <chaoscore/base/Preproc.hpp>

#include "chlog/Stream.hpp"

namespace chlog
{

/*!
 * \brief Log input object that provides streams for writing log messages with
 *        varying severity.
 *
 * Inputs cannot be manually constructed, see chlog::LogHandler::vend_input()
 * for creating inputs.
 */
class Input
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN(Input);

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
    chlog::Stream critical;
    /*!
     * \brief For reporting an unexpected error which may cause failure.
     */
    chlog::Stream error;
    /*!
     * \brief For reporting an unexpected error which may cause the application
     *        to perform unexpectedly.
     */
    chlog::Stream warning;
    /*!
     * \brief For reporting high level notices about the runtime of the
     *        application.
     */
    chlog::Stream notice;
    /*!
     * \brief For reporting explicit information about the the runtime of the
     *        application.
     */
    chlog::Stream info;
    /*!
     * \brief For reporting implementation specific information to aid in
     *        development and debugging.
     */
    chlog::Stream debug;

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
    Input(chlog::LogHandler* log_handler, const chlog::Profile& profile);
};

} // namespace chlog

#endif
