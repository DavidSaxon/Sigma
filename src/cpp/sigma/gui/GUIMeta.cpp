#include "sigma/gui/GUIMeta.hpp"

#include "sigma/gui/GUILogging.hpp"
#include "sigma/gui/GUIMetaCompiled.hpp"

namespace sigma
{
namespace gui
{
namespace meta
{

//------------------------------------------------------------------------------
//                                      DATA
//------------------------------------------------------------------------------

DocumentPtr logging;

DocumentPtr resource_locations;
DocumentPtr fonts;

DocumentPtr widgets_startup;

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \brief Reports when a MetaEngine Document has failed to load, and a fallback
 *        protocol must be executed.
 */
static void load_fallback_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);

/*!
 * \brief Reports when retrieving a value from a MetaEngine Document has failed,
 *        and a fallback protocol must be executed.
 */
static void get_fallback_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void init()
{
    // connect fallback reporters
    metaengine::Document::set_load_fallback_reporter(load_fallback_reporter);
    metaengine::Document::set_get_fallback_reporter(get_fallback_reporter);

    //----------------------------RESOURCE LOCATIONS----------------------------
    logger->debug << "Loading MetaEngine data for resource locations."
                  << std::endl;
    arc::io::sys::Path resource_locations_path;
    resource_locations_path
        << "meta" << "gui" << "resources" << "locations.json";
    meta::resource_locations.reset(new metaengine::Document(
        resource_locations_path,
        &meta_comp::logging
    ));
    //----------------------------------FONTS-----------------------------------
    logger->debug << "Loading MetaEngine data for fonts." << std::endl;
    arc::io::sys::Path fonts_path;
    fonts_path
        << "meta" << "gui" << "resources" << "fonts.json";
    meta::fonts.reset(new metaengine::Document(
        fonts_path,
        &meta_comp::fonts
    ));
    //-----------------------------STARTUP WIDGETS------------------------------
    logger->debug << "Loading MetaEngine data for startup widgets."
                  << std::endl;
    arc::io::sys::Path widgets_startup_path;
    widgets_startup_path
        << "meta" << "gui" << "widgets" << "startup.json";
    meta::widgets_startup.reset(new metaengine::Document(
        widgets_startup_path,
        &meta_comp::widgets_startup
    ));
}

static void load_fallback_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    logger->error << "MetaEngine error loading data associated with file \""
                  << file_path << "\": " << message << std::endl;
}

static void get_fallback_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    logger->error << "MetaEngine error accessing data in \"" << file_path
                  << "\": " << message << std::endl;
}

} // namespace meta
} // namespace gui
} // namespace sigma
