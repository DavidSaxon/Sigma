/*!
 * \file
 * \author David Saxon
 */
#ifndef ARCANELOG_OUTPUTS_FILEOUTPUT_HPP_
#define ARCANELOG_OUTPUTS_FILEOUTPUT_HPP_

#include <arcanecore/io/sys/FileWriter.hpp>

#include "arcanelog/AbstractOutput.hpp"

namespace arclog
{

/*!
 * \brief Implementation of arclog::AbstractOutput that writes logging message to
 *        the file system.
 *
 * The file handle is only opened when the output is first enabled (which by
 * default happens at construction time). If the output is later disabled the
 * file handle will be closed, and reopened the next time the output is enabled.
 */
class FileOutput : public arclog::AbstractOutput
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new opened FileOutput.
     *
     * If the ```open_now``` is set to ```false``` the file handle will not be
     * opened immediately, instead the FileOutput will initially be disabled,
     * and opened for the first time when the output is enabled.
     *
     * \param path The file path to write the log to.
     * \param open_now Whether this output will initially be enabled and the
     *                 file handle will be opened on construction.
     * \param verbosity_level The initial level of verbosity of this output.
     *
     * \throws arc::ex::InvalidPathError If the path cannot be opened for
     *                                     writing.
     */
    FileOutput(
            arc::io::sys::Path& path,
            bool open_now = true,
            arclog::Verbosity verbosity_level = arclog::VERBOSITY_NOTICE);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void set_enabled(bool enabled);

    // override
    virtual void write(
            arclog::Verbosity verbosity,
            const arclog::Profile& profile,
            const arc::str::UTF8String& message);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The path to the file to write to.
     */
    arc::io::sys::Path m_path;

    /*!
     * \brief The file writer used to write the log messages to disk.
     */
    arc::io::sys::FileWriter m_writer;

    /*!
     * \brief Whether the file handle has been opened before.
     */
    bool m_opened_once;
};

} // namespace arclog

#endif
