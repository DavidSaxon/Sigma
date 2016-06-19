/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSLOG_OUTPUTS_STDOUTPUT_HPP_
#define CHAOSLOG_OUTPUTS_STDOUTPUT_HPP_

#include "chlog/AbstractOutput.hpp"

namespace chlog
{

/*!
 * \brief Implementation of chlog::AbstractOutput that writes logging messages
 *        to either ```std::cout``` (notice, info, and debug) or to
 *        ```std::cerr``` (critical, error, and warning).
 */
class StdOutput : public chlog::AbstractOutput
{
public:

    //--------------------------------------------------------------------------
    //                                ENUMERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Enumerator that signifies whether ANSI escape sequence should be
     *        used to decorate text outputted to ```std::cout``` and
     *        ```std::cerr```.
     */
    enum UseANSI
    {
        /// ANSI escape sequences are never used to decorate output text.
        USEANSI_NEVER = 0,
        /// ANSI escape sequences are always used to decorate output text.
        USEANSI_ALWAYS,
        /// ANSI escape sequences are used to decorate output text if the
        /// current operating system's default shell supports them. For example
        /// ANSI escape sequences will be used on Linux systems but not on
        /// Windows systems.
        USEANSI_IF_SUPPORTED
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new StdOutput.
     *
     * \param verbosity_level The initial level of verbosity of this output.
     * \param use_ansi Whether ANSI escape sequences will be used to decorate
     *                 the output messages.
     */
    StdOutput(
            chlog::Verbosity verbosity_level = chlog::VERBOSITY_NOTICE,
            UseANSI use_ansi = USEANSI_IF_SUPPORTED);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void write(
            chlog::Verbosity verbosity,
            const chlog::Profile& profile,
            const chaos::str::UTF8String& message);

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether ANSI escape sequences should be used or not.
     */
    UseANSI m_use_ansi;
};

} // namespace chlog

#endif
