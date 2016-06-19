/*!
 * \file
 * \brief Generic globals for logging. Most simple programs will only need to
 *        include this file and use the provided log inputs.
 * \author David Saxon
 */
#ifndef CHAOSLOG_LOGGING_HPP_
#define CHAOSLOG_LOGGING_HPP_

#include "chlog/Input.hpp"
#include "chlog/LogHandler.hpp"

/*!
 * \mainpage ChaosLog C++ Documentation.
 *
 * ChaosLog is designed to be a generic log facility which provides support for
 * multiple inputs connected to multiple outputs.
 *
 * \par Overview
 *
 * ChaosLog uses the chlog::LogHandler object to connect chlog::Input objects
 * and chlog::AbstractOutput objects. An input will write to all outputs
 * connected to the same chlog::LogHandler. A pre-existing chlog::LogHandler is
 * provided with ChaosLog: chlog::default_handler. This handler initially has no
 * inputs or outputs connected. Providing a default handler means that multiple
 * libraries or sections of an application can link against ChaosLog and share
 * the same logging facility and add their own inputs and outputs.
 *
 * \par Example Usage
 *
 * In this example we have an application that requires two logging inputs, one
 * for the application core, and one for the GUI. In this example will use a
 * global pointer for each input and set up the logging facilities in a function
 * called ```init_logging```.
 *
 * The first step is to vend pointers to the input objects using two different
 * chlog::Profile objects.
 *
 * \code
 * #include <chlog/Logging.hpp>
 *
 * chlog::Input* core_logger;
 * chlog::Input* gui_logger;
 *
 * void init_logging()
 * {
 *     // add an input for the core application
 *     chlog::Profile core_profile("MyApp::core");
 *     core_logger = chlog::default_handler.vend_input(core_profile);
 *
 *     // add an input for the application's GUI
 *     chlog::Profile gui_profile("MyApp::gui");
 *     gui_logger = chlog::default_handler.vend_input(gui_profile);
 * }
 * \endcode
 *
 * At this stage we could write to our newly created inputs, but it would have
 * no effect since there are no outputs connected. So next a chlog::StdOutput
 * will be connected to the log handler. chlog::StdOutput is an output type
 * provided by ChaosLog and outputs logging messages to ```std::cout``` (notice,
 * info, and debug) and to ```std::cerr``` (critical, error, and warning).
 *
 * \code
 * #include <chlog/Logging.hpp>
 * #include <chlog/outputs/StdOutput.hpp>
 *
 * ...
 *
 * void init_logging()
 * {
 *     ...
 *
 *     // add an library provided output that will write messages to std::cout
 *     // and std::cerr. The log handler will handle deleting the stdout_writer.
 *     chlog::StdOutput* stdout_writer = new chlog::StdOutput();
 *     chlog::default_handler.add_output(stdout_writer);
 *
 *     // will write the following message to std::cout:
 *     // {MyApp::core} - [NOTICE]: Hello world!
 *     core_logger->notice << "Hello world!" << std::endl;
 *
 *     // will write the following message to std::cerr:
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
 *     stdout_writer.set_verbosity_level(chlog::VERBOSITY_INFO);
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
 * \brief Global namespace that contains all ChaosLog functionality.
 */
namespace chlog
{

/*!
 * \brief Library provided chlog::LogHandler instance.
 *
 * In most cases this is should be the only chlog::LogHandler needed. Using this
 * handler means multi-library applications can use the same handler without
 * libraries need to provide access to their own handler instances.
 */
extern chlog::LogHandler default_handler;

} // namespace chlog

#endif
