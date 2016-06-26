/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANECORE_STREAM_HPP_
#define ARCANECORE_STREAM_HPP_

#include <memory>

#include "arcanelog/Profile.hpp"
#include "arcanelog/Verbosity.hpp"

namespace arclog
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class LogHandler;
class Input;

/*!
 * \brief Stream that inherits from ```std::ostream``` for writing log messages
 *        to.
 *
 * All stream objects are owned by and provided by arclog::Input objects.
 */
class Stream : public std::ostream
{
private:

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class Input;

    //--------------------------------------------------------------------------
    //                            FORWARD DECLARATIONS
    //--------------------------------------------------------------------------

    class StreamBuffer;

public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Stream();

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new Stream.
     *
     * \param log_handler The arclog::LogHandler that owns this stream's parent
     *                    arclog::Input.
     * \param verbosity The verbosity of this stream.
     * \param profile The logging profile of this stream's parent arclog::Input.
     */
    Stream(
            arclog::LogHandler* log_handler,
            arclog::Verbosity verbosity,
            const arclog::Profile& profile);

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Sends the given message to the outputs associated with this
     *        Stream's parent arclog::LogHandler.
     */
    void send_to_outputs(const arc::str::UTF8String& message);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The internal buffer of this stream.
     */
    std::unique_ptr<arclog::Stream::StreamBuffer> m_buffer;

    /*!
     * \brief Pointer to the arclog::LogHandler object this Stream's parent is
     *        associated with.
     */
    arclog::LogHandler* m_log_handler;

    /*!
     * \brief The verbosity level of this stream.
     */
    const arclog::Verbosity m_verbosity;

    /*!
     * \brief The logging profile of this Stream's parent input.
     */
    const arclog::Profile m_profile;
};

} // namespace arclog

#endif
