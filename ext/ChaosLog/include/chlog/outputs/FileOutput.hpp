/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSLOG_OUTPUTS_FILEOUTPUT_HPP_
#define CHAOSLOG_OUTPUTS_FILEOUTPUT_HPP_

#include <chaoscore/io/sys/FileWriter.hpp>

#include "chlog/AbstractOutput.hpp"

namespace chlog
{

class FileOutput : public chlog::AbstractOutput
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new FileOutput.
     *
     * \param path The file path to write the log to.
     * \param verbosity_level The initial level of verbosity of this output.
     *
     * \throws chaos::ex::InvalidPathError If the path cannot be opened for
     *                                     writing.
     */
    FileOutput(
            chaos::io::sys::Path& path,
            chlog::Verbosity verbosity_level = chlog::VERBOSITY_NOTICE);

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
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The file writer used to write the log messages to disk.
     */
    chaos::io::sys::FileWriter m_writer;
};

} // namespace chlog

#endif
