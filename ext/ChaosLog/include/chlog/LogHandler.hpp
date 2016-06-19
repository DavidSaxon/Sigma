/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSLOG_LOGHANDLER_HPP_
#define CHAOSLOG_LOGHANDLER_HPP_

#include <map>
#include <memory>
#include <vector>

#include "chlog/Profile.hpp"
#include "chlog/Verbosity.hpp"

namespace chlog
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class AbstractOutput;
class Input;

/*!
 * \brief Object that handles the association of logging inputs and outputs.
 *
 * A default chlog::LogHandler instance is provided with ChaosLog:
 * chlog::default_handler.
 */
class LogHandler
{
public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief std::vector containing unique pointers to chlog::Input objects.
     */
    typedef std::vector<std::unique_ptr<chlog::Input>> InputVector;
    /*!
     * \brief std::vector containing unique pointers to chlog::AbstractOutput
     *        objects.
     */
    typedef std::vector<std::unique_ptr<chlog::AbstractOutput>> OutputVector;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new Logging object.
     *
     * LogHandlers are initialised with no input or outputs associated.
     */
    LogHandler();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~LogHandler();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates and returns a new chlog::Input associated with and owned
     *        by this log handler.
     *
     * \param profile The logging profile to use with this input.
     */
    chlog::Input* vend_input(const chlog::Profile& profile = chlog::Profile());

    /*!
     * \brief Returns a vector containing the chlog::Output objects associated
     *        with this log handler.
     */
    const OutputVector& get_outputs() const;

    /*!
     * \brief Adds a new output writer to this LogHandler.
     *
     * \note The LogHandler will take ownership of the given output writer and
     *       will handle deleting it.
     *
     * \return The pointer to the given output.
     *
     * \throws chaos::ex::ValueError If this LogHandler already holds a pointer
     *                               to the given writer.
     */
    chlog::AbstractOutput* add_output(chlog::AbstractOutput* output);

    /*!
     * \brief Removes the given output from this LogHandler.
     *
     * \param output The pointer to the output to remove.
     *
     * \returns Whether the output exists in this log handler to be removed or
     *          not.
     */
    bool remove_output(chlog::AbstractOutput* output);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The input streams attached to this LogHandler.
     */
    InputVector m_inputs;

    /*!
     * \brief The output writers of this log LogHandler.
     */
    OutputVector m_outputs;
};

} // namespace chlog

#endif
