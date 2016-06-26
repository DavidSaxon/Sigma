/*!
 * \file
 * \brief Generic globals for logging. Most simple programs will only need to
 *        include this file and use the provided log inputs.
 * \author David Saxon
 */
#ifndef ARCANELOG_LOGGING_HPP_
#define ARCANELOG_LOGGING_HPP_

#include "arcanelog/Input.hpp"
#include "arcanelog/LogHandler.hpp"

/*!
 * \mainpage ArcaneLog C++ Documentation.
 *
 * ArcaneLog is designed to be a generic log facility which provides support for
 * multiple inputs connected to multiple outputs.
 *
 * \par Overview
 *
 * ArcaneLog uses the arclog::LogHandler object to connect arclog::Input objects
 * and arclog::AbstractOutput objects. An input will write to all outputs
 * connected to the same arclog::LogHandler. A pre-existing arclog::LogHandler
 * is provided with ArcaneLog: arclog::default_handler. This handler initially
 * has no inputs or outputs connected. Providing a default handler means that
 * multiple libraries or sections of an application can link against ArcaneLog
 * and share the same logging facility and add their own inputs and outputs.
 *
 * \par Example Usage
 *
 * In this example we have an application that requires two logging inputs, one
 * for the application core, and one for the GUI. In this example will use a
 * global pointer for each input and set up the logging facilities in a function
 * called ```init_logging```.
 *
 * The first step is to vend pointers to the input objects using two different
 * arclog::Profile objects.
 *
 * \code
 * #include <arclog/Logging.hpp>
 *
 * arclog::Input* core_logger;
 * arclog::Input* gui_logger;
 *
 * void init_logging()
 * {
 *     // add an input for the core application
 *     arclog::Profile core_profile("MyApp::core");
 *     core_logger = arclog::default_handler.vend_input(core_profile);
 *
 *     // add an input for the application's GUI
 *     arclog::Profile gui_profile("MyApp::gui");
 *     gui_logger = arclog::default_handler.vend_input(gui_profile);
 * }
 * \endcode
 *
 * At this stage we could write to our newly created inputs, but it would have
 * no effect since there are no outputs connected. So next two logging outputs
 * will be connected. The first is an arclog::StdOutput which outputs logging
 * messages to ```stdout``` (notice, info, and debug) and to ```stderr```
 * (critical, error, and warning). The second will be an arclog::FileOutput
 * which writes logging messages to a file on disk.
 *
 * \code
 * #include <arclog/Logging.hpp>
 * #include <arclog/outputs/StdOutput.hpp>
 * #include <arclog/outputs/StdOutput.hpp>
 *
 * ...
 *
 * void init_logging()
 * {
 *     ...
 *
 *     // add a library provided output that will write messages to std::cout
 *     // and std::cerr. The log handler will handle deleting the stdout_writer.
 *     arclog::StdOutput* stdout_writer = new arclog::StdOutput();
 *     arclog::default_handler.add_output(stdout_writer);
 *
 *     // create the path the FileOutput will write to
 *     arc::io::sys::Path log_path;
 *     log_path << "logs" << "example.log";
 *     // add a library provided output that will write messages to a file on
 *     // disk.
 *     arclog::FileOutput* file_writer =
 *         new arclog::FileOutput(log_path, false, arclog::VERBOSITY_DEBUG);
 *     arclog::default_handler.add_output(file_writer);
 *
 *     // will write the following message to std::cout and disk:
 *     // {MyApp::core} - [NOTICE]: Hello world!
 *     core_logger->notice << "Hello world!" << std::endl;
 *
 *     // will write the following message to std::cerr and disk:
 *     // {MyApp::gui} - [ERROR]: Example error.
 *     gui_logger->error << "Example error." << std::endl;
 * }
 * \endcode
 *
 * Output writers can individually have their verbosity changed or be completely
 * disabled:
 *
 * \code
 *
 * ...
 *
 * void init_logging()
 * {
 *     ...
 *
 *     stdout_writer.set_verbosity_level(arclog::VERBOSITY_INFO);
 *
 *     // this message will be output to std::cout
 *     core_logger->info << "Hello world!" << std::endl;
 *
 *     // this message will not be output std::cout
 *     gui_logger->debug << "Some debug information." << std::endl;
 *
 *     // turn off all output
 *     stdout_writer.set_enabled(false);
 * }
 * \endcode
 */

/*!
 * \brief Global namespace that contains all ArcaneLog functionality.
 */
namespace arclog
{

/*!
 * \brief Library provided arclog::LogHandler instance.
 *
 * In most cases this is should be the only arclog::LogHandler needed. Using
 * this handler means multi-library applications can use the same handler
 * without libraries need to provide access to their own handler instances.
 */
extern arclog::LogHandler default_handler;

} // namespace arclog

#endif
