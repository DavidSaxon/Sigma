#include "sigma/gui/startup/SplashScreen.hpp"

#include <QtCore/QSize>
#include <QtCore/Qt>
#include <QtGui/QFont>
#include <QtGui/QPixmap>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QVBoxLayout>

namespace
{

//------------------------------------------------------------------------------
//                                   CONSTANTS
//------------------------------------------------------------------------------

// TODO: explore meta-programming or QML
static const QSize SPLASH_SIZE(1000, 500);

} // namespace anonymous

namespace sigma
{
namespace gui
{
namespace startup
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

SplashScreen::SplashScreen()
    :
    QWidget()
{
    setWindowFlags(Qt::SplashScreen);
    resize(SPLASH_SIZE);

    // TODO: explore on disk style sheets or QML
    setStyleSheet(
        "background: qlineargradient( "
        "x1:0 y1:0, x2:1 y2:0, stop:0 #4699BB, stop:1 #7046BB);"
    );

    // set up layouts
    QVBoxLayout* main_layout = new QVBoxLayout();
    // main_layout->setSpacing(0);
    // main_layout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* title_layout = new QHBoxLayout();
    title_layout->setSpacing(30);
    title_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addLayout(title_layout);

    QLabel* logo = new QLabel();
    QPixmap logo_pix("res/gui/logo.png");
    logo->setPixmap(logo_pix.scaledToWidth(350));
    logo->setStyleSheet("background: rgba(0, 0, 0, 0);");
    logo->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    title_layout->addWidget(logo);

    QLabel* title = new QLabel("SIGMA");
    title->setStyleSheet(
        "background: rgba(0, 0, 0, 0); "
        "color: white; font-size: 190px;"
    );
    title->setFont(QFont("Roboto Th"));
    title_layout->addWidget(title);

    QLabel* state_info = new QLabel("Bootstrapping...");
    state_info->setStyleSheet(
        "background: rgba(0, 0, 0, 0); "
        "font: Courier; color: white; font-size: 16px;"
    );
    state_info->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    main_layout->addWidget(state_info);

    setLayout(main_layout);
}

} // namespace startup
} // namespace gui
} // namespace sigma
