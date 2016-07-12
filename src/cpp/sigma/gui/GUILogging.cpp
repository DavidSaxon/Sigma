#include "sigma/gui/GUILogging.hpp"

#include <chrono>
#include <ctime>
#include <iostream>

#include <arcanelog/Shared.hpp>
#include <arcanelog/outputs/FileOutput.hpp>
#include <arcanelog/outputs/StdOutput.hpp>

#include <metaengine/visitors/Path.hpp>
#include <metaengine/visitors/Primitive.hpp>
#include <metaengine/visitors/String.hpp>

#include <sigma/core/util/Logging.hpp>

#include "sigma/gui/GUIMeta.hpp"
#include "sigma/gui/GUIMetaCompiled.hpp"

// allows us to use std::localtime, without warning it's unsafe, maybe this is
// bad idea, but I can't imagine it being too detrimental since it's only being
// used to determine log file names
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS


namespace sigma
{
namespace gui
{

arclog::Input* logger = nullptr;
arclog::StdOutput* std_output = nullptr;
arclog::FileOutput* file_output = nullptr;

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \brief Temporary reporter function which will print MetaEngine loading errors
 *        to std::cerr
 */
static void std_load_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);

/*!
 * \brief Temporary reporter function which will print MetaEngine retrieval
 *        errors to std::cerr
 */
static void std_get_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);

/*!
 * \brief Initialises the StdOutut logging writer.
 */
void init_std_output();

/*!
 * \brief Initialises the FileOutput logging writer.
 */
void init_file_output();

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void init_logging()
{
    // TODO: initialize core logging

    // logging MetaEngine data needs to be loaded before all other MetaEngine
    // data since we want to initialise logging as early as possible. Since
    // logging has not yet been initialised we will use some temporary fallback
    // reporter functions that write to std::cerr
    metaengine::Document::set_load_fallback_reporter(std_load_reporter);
    metaengine::Document::set_get_fallback_reporter(std_get_reporter);

    // path to the logging MetaEngine data
    arc::io::sys::Path logging_meta_path;
    logging_meta_path << "meta" << "gui" << "logging.json";

    // load the document
    meta::logging.reset(new metaengine::Document(
        logging_meta_path,
        &meta_comp::logging
    ));

    // create the GUI logging profile
    arclog::Profile profile(*meta::logging->get(
        "profile",
        metaengine::UTF8StringV::instance()
    ));

    // vend the input from the shared handler
    logger = arclog::shared_handler.vend_input(profile);

    // set up loggers
    init_std_output();
    init_file_output();

    // connect new fall-back handlers
}

static void std_load_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    std::cerr << "MetaEngine error loading data associated with file \""
              << file_path << "\": " << message << std::endl;
}

static void std_get_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    std::cerr << "MetaEngine error accessing data in \"" << file_path
              << "\": " << message << std::endl;
}

void init_std_output()
{
    // stdoutput
    std_output = new arclog::StdOutput();
    // enabled?
    bool enabled = *meta::logging->get(
        "outputs.StdOutput.enabled",
        metaengine::BoolV::instance()
    );
    std_output->set_enabled(enabled);
    // verbosity
    std_output->set_verbosity_level(*meta::logging->get(
        "outputs.StdOutput.verbosity_level",
        sigma::core::util::ArcLogVerbosityV::instance()
    ));
    // add to handler
    arclog::shared_handler.add_output(std_output);

    // if enabled alert that we're writing to std out
    if(enabled)
    {
        logger->info << "Sigma session logging to stdout and stderr"
                     << std::endl;
    }
}

void init_file_output()
{
    // build the path to write logs to
    arc::io::sys::Path log_path(*meta::logging->get(
        "outputs.FileOutput.base_path",
        metaengine::PathV::instance()
    ));

    // get time
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    // get the syntax to read the date as
    arc::str::UTF8String date_syntax(*meta::logging->get(
        "outputs.FileOutput.path_date_syntax",
        metaengine::UTF8StringV::instance()
    ));
    // get the date
    char date_buffer[50];
    strftime(
        date_buffer,
        sizeof(date_buffer),
        date_syntax.get_raw(),
        std::localtime(&now_t)
    );
    log_path << date_buffer;
    // get the syntax to read the time as
    arc::str::UTF8String time_syntax(*meta::logging->get(
        "outputs.FileOutput.path_time_syntax",
        metaengine::UTF8StringV::instance()
    ));
    // get the time
    char time_buffer[50];
    strftime(
        time_buffer,
        sizeof(time_buffer),
        time_syntax.get_raw(),
        std::localtime(&now_t)
    );
    arc::str::UTF8String file_name(time_buffer);
    // get the file extension
    arc::str::UTF8String file_extension(*meta::logging->get(
        "outputs.FileOutput.file_extension",
        metaengine::UTF8StringV::instance()
    ));
    file_name << "." << file_extension;
    // update the path
    log_path << file_name;

    // create the file output
    file_output = new arclog::FileOutput(log_path, false);

    // enabled?
    bool enabled = *meta::logging->get(
        "outputs.FileOutput.enabled",
        metaengine::BoolV::instance()
    );
    // need to test this since enabling the file writer opens it
    try
    {
        file_output->set_enabled(enabled);
    }
    catch(const arc::ex::ArcException& exc)
    {
        // report the error using std::cerr since logging is not setup yet
        std::cerr << "Failed to open log file output to: \"" << log_path
                  << "\". With error: \"" << exc.what() << "\". Logging to "
                  << "file will be disabled." << std::endl;
        return;
    }

    // verbosity
    std_output->set_verbosity_level(*meta::logging->get(
        "outputs.StdOutput.verbosity_level",
        sigma::core::util::ArcLogVerbosityV::instance()
    ));
    // add to handler
    arclog::shared_handler.add_output(file_output);

    // if the enabled write the file we're logging to
    if(enabled)
    {
        logger->info << "Sigma session logging to file: \"" << log_path << "\""
                     << std::endl;
    }
}

} // namespace gui
} // namespace sigma
