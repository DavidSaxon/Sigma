#include "sigma/gui/GUILogging.hpp"

#include <chrono>
#include <ctime>
#include <iostream>

#include <arcanelog/Logging.hpp>
#include <arcanelog/outputs/FileOutput.hpp>
#include <arcanelog/outputs/StdOutput.hpp>

#include "sigma/gui/GUIMeta.hpp"
#include "sigma/gui/GUIMetaCompiled.hpp"

// allows us to use std::localtime, without warning it's unsafe, maybe this is
// bad idea, but I can't imagine it being too detrimental since it's only being
// used to determine log file names
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

namespace metaeng
{

// metadata override for logging verbosity
template<>
arclog::Verbosity& Data::get(
        const arc::str::UTF8String& key,
        arclog::Verbosity& value) const
{
    const Json::Value* j_value = resolve_key(key);

    if(j_value->isString())
    {
        arc::str::UTF8String str(j_value->asCString());
        if(str == "critical")
        {
            value = arclog::VERBOSITY_CRITICAL;
            return value;
        }
        if(str == "error")
        {
            value = arclog::VERBOSITY_ERROR;
            return value;
        }
        if(str == "warning")
        {
            value = arclog::VERBOSITY_WARNING;
            return value;
        }
        if(str == "notice")
        {
            value = arclog::VERBOSITY_NOTICE;
            return value;
        }
        if(str == "info")
        {
            value = arclog::VERBOSITY_INFO;
            return value;
        }
        if(str == "debug")
        {
            value = arclog::VERBOSITY_DEBUG;
            return value;
        }
    }

    arc::str::UTF8String error_message;
    error_message << "Unable to convert value for key: \"" << key
                  << "\" to a value of type: <arclog::Verbosity>";
    throw arc::ex::TypeError(error_message);
}

} // namespace metaeng

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

    // TODO: can this utilize the meta data function?
    // logging meta data needs to be loaded first, this is loaded before all
    // other meta data since we want to initialise logging as early as possible.
    arc::io::sys::Path logging_meta_path;
    // TODO: could this live in a more general data file?
    logging_meta_path << "meta" << "gui" << "logging.json";
    try
    {
        meta::logging = meta::DataPtr(
                new metaeng::Data(logging_meta_path, meta_comp::logging)
        );
    }
    catch(const arc::ex::ArcException& exc)
    {
        // this report must be done through std::cerr since logging isn't active
        // yet
        std::cerr << "Failed to load GUI logging meta data from \""
                  << logging_meta_path << "\" with error:\n" << exc.what()
                  << "\nReverting to loading metadata from memory."
                  << std::endl;

        // load from memory, no point in using a scheme since the memory path
        // was what we previously used as schema
        meta::logging = meta::DataPtr(new metaeng::Data(&meta_comp::logging));
    }

    // create a logging profile
    arc::str::UTF8String profile_name;
    arclog::Profile profile(meta::logging->get("profile", profile_name));

    // vend the input from the default handler
    logger = arclog::default_handler.vend_input(profile);

    // set up loggers
    init_std_output();
    init_file_output();
}

void init_std_output()
{
    // stdoutput
    std_output = new arclog::StdOutput();
    // enabled?
    bool enabled = meta::logging->get("outputs.StdOutput.enabled", enabled);
    std_output->set_enabled(enabled);
    // verbosity
    arclog::Verbosity verbosity =
        meta::logging->get("outputs.StdOutput.verbosity_level", verbosity);
    std_output->set_verbosity_level(verbosity);
    // add to handler
    arclog::default_handler.add_output(std_output);

    // if enabled alert that we're writing to std out
    if(enabled)
    {
        logger->info << "Sigma session logging to stdout and stderr"
                     << std::endl;
    }
}

void init_file_output()
{
    // get the logging path
    arc::io::sys::Path log_path;
    meta::logging->get("outputs.FileOutput.base_path", log_path);
    // get time
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    // get the syntax to read the date as
    arc::str::UTF8String date_syntax;
    meta::logging->get(
        "outputs.FileOutput.path_date_syntax",
        date_syntax
    );
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
    arc::str::UTF8String time_syntax;
    meta::logging->get(
        "outputs.FileOutput.path_time_syntax",
        time_syntax
    );
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
    arc::str::UTF8String file_extension;
    meta::logging->get(
        "outputs.FileOutput.file_extension",
        file_extension
    );
    file_name << "." << file_extension;
    // update the path
    log_path << file_name;

    // create the file output
    file_output = new arclog::FileOutput(log_path, false);

    // enabled?
    bool enabled = meta::logging->get("outputs.FileOutput.enabled", enabled);
    // need to test this since enabling the file writer opens it
    try
    {
        file_output->set_enabled(enabled);
    }
    catch(const arc::ex::ArcException& exc)
    {
        // report the error using std::cerr since logging is not setup yet
        std::cerr << "Failed to open the file output to: \"" << log_path
                  << "\". With error: \"" << exc.what() << "\". Logging to "
                  << "file will be disabled." << std::endl;
        return;
    }

    // verbosity
    arclog::Verbosity verbosity =
        meta::logging->get("outputs.FileOutput.verbosity_level", verbosity);
    file_output->set_verbosity_level(verbosity);
    // add to handler
    arclog::default_handler.add_output(file_output);

    // if the enabled write the file we're logging to
    if(enabled)
    {
        logger->info << "Sigma session logging to file: \"" << log_path << "\""
                     << std::endl;
    }
}

} // namespace gui
} // namespace sigma
