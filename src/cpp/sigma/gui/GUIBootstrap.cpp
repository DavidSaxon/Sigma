#include "sigma/gui/GUIBootstrap.hpp"

#include <algorithm>

#include <arcanecore/base/str/StringOperations.hpp>
#include <arcanecore/io/sys/FileSystemOperations.hpp>

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
    arc::io::sys::Path font_directory;
    meta::resource_locations->get("fonts_resource_path", font_directory);
    // iterate over each file in the fonts directory and add to the database
    std::vector<arc::io::sys::Path> font_files =
        arc::io::sys::list_rec(font_directory);
    ARC_FOR_EACH(it, font_files)
    {
        // TODO: list with skip . and ..

        // get the supported font formats
        std::vector<arc::str::UTF8String> supported_formats;
        meta::fonts->get("supported_formats", supported_formats);
        // check that this font is a supported format
        arc::str::UTF8String extension(it->get_extension());
        // currently only TrueTypeFont and OpenTypeFont formats are supported by
        // QT
        if (std::find(
                supported_formats.begin(),
                supported_formats.end(),
                extension
            ) != supported_formats.end())
        {
            QFontDatabase::addApplicationFont(it->to_native().get_raw());
        }
        else
        {
            static arc::str::UTF8String f_supported_formats(
                arc::str::join(supported_formats, ", "));

            logger->warning << "Font file will not be loaded as it is an "
                            << "unsupported format \"" << *it << "\". "
                            << "Supported formats are: ["
                            << f_supported_formats << "]" << std::endl;
        }
    }
}

} // namespace gui
} // namespace sigma
