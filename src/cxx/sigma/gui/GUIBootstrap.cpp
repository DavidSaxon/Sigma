#include "sigma/gui/GUIBootstrap.hpp"

#include <algorithm>

#include <chaoscore/io/sys/FileSystemOperations.hpp>

#include <meta/Data.hpp>

#include <QtGui/QFontDatabase>
#include <QtWidgets/QApplication>

#include "sigma/gui/startup/SplashScreen.hpp"

namespace sigma
{
namespace gui
{

// TODO: MOVE THIS
meta::Data meta_res_globals(
        chaos::io::sys::Path("meta/resources/globals.json"));

int bootstrap(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // load fonts
    load_fonts();

    startup::SplashScreen* s = new startup::SplashScreen();

    s->show();

    return QApplication::exec();
}

void load_fonts()
{
    // get the directory where fonts are stored
    chaos::io::sys::Path font_directory;
    meta_res_globals.get("fonts.resource_path", font_directory);
    // iterate over each file in the fonts directory and add to the database
    std::vector<chaos::io::sys::Path> font_files =
        chaos::io::sys::list_rec(font_directory);
    CHAOS_FOR_EACH(it, font_files)
    {
        // TODO: list with skip . and ..

        // get the supported font formats
        std::vector<chaos::str::UTF8String> supported_formats;
        meta_res_globals.get("fonts.supported_formats", supported_formats);
        // check that this font is a supported format
        chaos::str::UTF8String extension(it->get_extension());
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
            // TODO: report stream of one form or another (chaos::core::)
        }
    }
}

} // namespace gui
} // namespace sigma
