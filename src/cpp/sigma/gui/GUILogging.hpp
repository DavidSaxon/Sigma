/*!
 * \file
 * \brief Logging in relation to Sigma's GUI.
 * \author David Saxon
 */
#ifndef SIGMA_GUI_LOGGING_HPP_
#define SIGMA_GUI_LOGGING_HPP_

#include <arcanelog/Input.hpp>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace arclog
{

class FileOutput;
class StdOutput;

} // namespace arclog

namespace sigma
{
namespace gui
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

/*!
 * \brief The input for logging GUI related messages through.
 */
extern arclog::Input* logger;

/*!
 * \brief The logging output to std::cout and std::cerr.
 */
extern arclog::StdOutput* std_output;
/*!
 * \brief The logging output for writing to the file system.
 */
extern arclog::FileOutput* file_output;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Initialises logging.
 */
void init_logging();

} // namespace gui
} // namespace sigma

#endif
