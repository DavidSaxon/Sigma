#include "sigma/gui/GUIBootstrap.hpp"

#include <chaoscore/io/sys/FileSystemOperations.hpp>

#include <QtGui/QFontDatabase>
#include <QtWidgets/QApplication>

#include "sigma/gui/startup/SplashScreen.hpp"

// TODO: REMOVE ME
#include <iostream>

namespace sigma
{
namespace gui
{

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
    chaos::io::sys::Path font_directory;
    // TODO: META
    font_directory << "res" << "gui" << "fonts";
    // iterate over each file in the fonts directory and add to the database
    std::vector<chaos::io::sys::Path> font_files =
        chaos::io::sys::list_rec(font_directory);
    CHAOS_FOR_EACH(it, font_files)
    {

        chaos::uni::UTF8String extension(it->get_extension());
        // TODO: META
        // currently only TrueTypeFont and OpenTypeFont formats are supported by
        // QT
        if (extension == "ttf" || extension == "otf")
        {
            QFontDatabase::addApplicationFont(it->to_native().get_raw());
        }
    }
}

} // namespace gui
} // namespace sigma
