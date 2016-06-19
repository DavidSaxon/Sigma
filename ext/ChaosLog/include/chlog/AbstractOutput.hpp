/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSLOG_ABSTRACTOUTPUT_HPP_
#define CHAOSLOG_ABSTRACTOUTPUT_HPP_

#include <chaoscore/base/str/UTF8String.hpp>

#include "chlog/Profile.hpp"
#include "chlog/Verbosity.hpp"

namespace chlog
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Stream;

/*!
 * \brief Abstract base class that represents a logging output.
 *
 * Outputs receive logging messages (along with the verbosity and logging
 * profile) from all chlog::Input objects connected to the parent
 * chlog::LogHandler. It is then up the output implementation on how to handle
 * the message.
 */
class AbstractOutput
{
private:

    CHAOS_DISALLOW_COPY_AND_ASSIGN(AbstractOutput);

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class Stream;

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor.
     *
     * \param verbosity_level The initial level of verbosity of this output.
     */
    AbstractOutput(chlog::Verbosity verbosity_level);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractOutput()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns whether this output is enabled or not.
     */
    bool is_enabled() const;

    /*!
     * \brief Sets whether this output is enabled or not.
     */
    void set_enabled(bool enabled);

    /*!
     * \brief Returns the maximum verbosity level of messages this output will
     *        display.
     */
    chlog::Verbosity get_verbosity_level() const;

    /*!
     * \brief Sets the maximum verbosity level of messages this output will
     *        display.
     */
    void set_verbosity_level(chlog::Verbosity verbosity_level);

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether this output is enabled or not.
     */
    bool m_enabled;

    /*!
     * \brief The maximum verbosity level of messages this output should
     *        display.
     */
    chlog::Verbosity m_verbosity_level;

    //--------------------------------------------------------------------------
    //                        PROTECETED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Is called by inputs streams to handle the outputting of the given
     *        log message.
     *
     * \note This function must be implemented by derived outputs.
     *
     * \param verbosity The verbosity of the message.
     * \param profile The logging profile associated with the message.
     * \param message The message to output.
     */
    virtual void write(
            chlog::Verbosity verbosity,
            const chlog::Profile& profile,
            const chaos::str::UTF8String& message) = 0;
};

} // namespace chlog

#endif
