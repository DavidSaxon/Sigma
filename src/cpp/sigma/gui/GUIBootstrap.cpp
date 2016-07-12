#include "sigma/gui/GUIBootstrap.hpp"

#include <algorithm>

#include <arcanecore/base/str/StringOperations.hpp>
#include <arcanecore/io/sys/FileSystemOperations.hpp>

#include <metaengine/visitors/Path.hpp>
#include <metaengine/visitors/String.hpp>

#include <QtGui/QFontDatabase>
#include <QtWidgets/QApplication>

#include "sigma/gui/GUILogging.hpp"
#include "sigma/gui/GUIMeta.hpp"
#include "sigma/gui/startup/SplashScreen.hpp"

namespace sigma
{
namespace gui
{

int bootstrap(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // initialise logging
    sigma::gui::init_logging();

    // initialise MetaEngine data
    sigma::gui::meta::init();

    // load fonts
    load_fonts();

    // TODO: ?

    startup::SplashScreen* s = new startup::SplashScreen();

    s->show();

    return QApplication::exec();
}

void load_fonts()
{
    logger->notice << "Loading fonts" << std::endl;

    // get the directory where fonts are stored
    arc::io::sys::Path font_directory(*meta::resource_locations->get(
        "fonts_resource_path",
        metaengine::PathV::instance()
    ));
    // iterate over each file in the fonts directory and add to the database
    for(const arc::io::sys::Path& path : arc::io::sys::list_rec(font_directory))
    {
        // get the supported font formats
        std::vector<arc::str::UTF8String> supported_formats(*meta::fonts->get(
            "supported_formats",
            metaengine::UTF8StringVectorV::instance()
        ));
        // check that this font is a supporteded format
        arc::str::UTF8String extension(path.get_extension());
        if (std::find(
                supported_formats.begin(),
                supported_formats.end(),
                extension
            ) != supported_formats.end())
        {
            QFontDatabase::addApplicationFont(path.to_native().get_raw());
        }
        else
        {
            static arc::str::UTF8String f_supported_formats(
                arc::str::join(supported_formats, ", "));

            logger->warning << "Font file will not be loaded as it is an "
                            << "unsupported format \"" << path << "\". "
                            << "Supported formats are: ["
                            << f_supported_formats << "]" << std::endl;
        }
    }
}

} // namespace gui
} // namespace sigma
