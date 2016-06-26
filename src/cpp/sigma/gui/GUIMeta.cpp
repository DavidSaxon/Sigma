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

DataPtr logging;

DataPtr resource_locations;
DataPtr fonts;

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \breif Handles the generic actions of loading MetaEngine Data objects.
 *
 * \param path The file path to the JSON metadata to read.
 * \param memory Fallback JSON to read from memory if reading from the path
 *               fails.
 * \param schema Schema to validate provided metadata against.
 *
 * \returns The MetaEngine data that was loaded as a result of this function.
 */
DataPtr load_meta_data(
        const arc::io::sys::Path& path,
        const arc::str::UTF8String* const memory,
        const arc::str::UTF8String& schema);

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void init()
{
    //----------------------------RESOURCE LOCATIONS----------------------------
    logger->debug << "Loading resource location metadata" << std::endl;
    arc::io::sys::Path resource_locations_path;
    resource_locations_path
        << "meta" << "gui" << "resources" << "locations.json";
    meta::resource_locations = load_meta_data(
        resource_locations_path,
        &meta_comp::resource_locations,
        meta_comp::resource_locations
    );
    //----------------------------------FONTS-----------------------------------
    logger->debug << "Loading fonts metadata" << std::endl;
    arc::io::sys::Path fonts_path;
    fonts_path
        << "meta" << "gui" << "resources" << "fonts.json";
    meta::fonts = load_meta_data(
        fonts_path,
        &meta_comp::fonts,
        meta_comp::fonts
    );
}


DataPtr load_meta_data(
        const arc::io::sys::Path& path,
        const arc::str::UTF8String* const memory,
        const arc::str::UTF8String& schema)
{
    try
    {
        return DataPtr(new metaeng::Data(path, schema));
    }
    catch(const arc::ex::ArcException& exc)
    {
        logger->error << "Failed to load meta data from \"" << path
                      << "\" with error:\n" << exc.what() << "\nReverting to "
                      << "loading metadata from memory." << std::endl;

        // load from memory, no point in using a scheme since the memory path
        // was what we previously used as schema
        return meta::DataPtr(new metaeng::Data(memory));
    }
}

} // namespace meta
} // namespace gui
} // namespace sigma
