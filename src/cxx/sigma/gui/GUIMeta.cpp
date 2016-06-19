#include "sigma/gui/GUIMeta.hpp"

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

DataPtr resource_globals;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void init()
{
    // TODO: functionise
    // resource globals
    chaos::io::sys::Path resource_globals_path;
    resource_globals_path << "meta" << "resources" << "globals.json";
    try
    {
        resource_globals = DataPtr(new metaeng::Data(resource_globals_path));
    }
    catch(...)
    {
        // TODO: log error

        // TODO: set up metaengine baking
        // resource_globals = DataPtr(
        //     new metaeng::Data(&meta_bake::resource_globals));
    }
}

} // namespace meta
} // namespace gui
} // namespace sigma
