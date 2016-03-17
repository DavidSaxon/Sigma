/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_GUI_STARTUP_SPLASH_HPP_
#define SIGMA_GUI_STARTUP_SPLASH_HPP_

#include <QtWidgets/QWidget>

namespace sigma
{
namespace gui
{
namespace startup
{

/*!
 * \brief Base widget for the Sigma startup splash screen.
 */
class SplashScreen : public QWidget
{
    Q_OBJECT

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    SplashScreen();
};

} // namespace startup
} // namespace gui
} // namespace sigma

#endif
