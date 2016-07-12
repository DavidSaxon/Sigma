#include "meta_qt/core/Qt.hpp"

#include <json/json.h>

namespace meta_qt
{

//------------------------------------------------------------------------------
//                                QT::WINDOWFLAGS
//------------------------------------------------------------------------------

QtWindowFlagsV& QtWindowFlagsV::instance()
{
    static QtWindowFlagsV v;
    return v;
}

bool QtWindowFlagsV::retrieve(
        const Json::Value* data,
        const arc::str::UTF8String& key,
        metaengine::Document* requester,
        arc::str::UTF8String& error_message)
{
    // is the data a list
    if(!data->isArray())
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                      << "converted to array type, which is required to "
                      << "define Qt::WindowFlags";
        return false;
    }

    Qt::WindowFlags temp = 0;

    // iterate over the elements of the list
    Json::Value::const_iterator child;
    for(child = data->begin(); child != data->end(); ++child)
    {
        // check if the data can be converted
        if(!child->isString())
        {
            Json::FastWriter j_writer;
            error_message << "Qt::WindowFlags element \""
                          << j_writer.write(*child) << "\" cannot be converted "
                          << "to UTF-8 string type.";
            return false;
        }

        // get as a string
        arc::str::UTF8String element(child->asCString());

        // evaluate valid types
        if(element == "Widget")
        {
            temp |= Qt::Widget;
        }
        else if(element == "Window")
        {
            temp |= Qt::Window;
        }
        else if(element == "Dialog")
        {
            temp |= Qt::Dialog;
        }
        else if(element == "Sheet")
        {
            temp |= Qt::Sheet;
        }
        else if(element == "Drawer")
        {
            temp |= Qt::Drawer;
        }
        else if(element == "Popup")
        {
            temp |= Qt::Popup;
        }
        else if(element == "Tool")
        {
            temp |= Qt::Tool;
        }
        else if(element == "ToolTip")
        {
            temp |= Qt::ToolTip;
        }
        else if(element == "SplashScreen")
        {
            temp |= Qt::SplashScreen;
        }
        else if(element == "Desktop")
        {
            temp |= Qt::Desktop;
        }
        else if(element == "SubWindow")
        {
            temp |= Qt::SubWindow;
        }
        else if(element == "ForeignWindow")
        {
            temp |= Qt::ForeignWindow;
        }
        else if(element == "CoverWindow")
        {
            temp |= Qt::CoverWindow;
        }
        else if(element == "MSWindowsFixedSizeDialogHint")
        {
            temp |= Qt::MSWindowsFixedSizeDialogHint;
        }
        else if(element == "MSWindowsOwnDC")
        {
            temp |= Qt::MSWindowsOwnDC;
        }
        else if(element == "BypassWindowManagerHint")
        {
            temp |= Qt::BypassWindowManagerHint;
        }
        else if(element == "X11BypassWindowManagerHint")
        {
            temp |= Qt::X11BypassWindowManagerHint;
        }
        else if(element == "FramelessWindowHint")
        {
            temp |= Qt::FramelessWindowHint;
        }
        else if(element == "NoDropShadowWindowHint")
        {
            temp |= Qt::NoDropShadowWindowHint;
        }
        else if(element == "X11BypassWindowManagerHint")
        {
            temp |= Qt::X11BypassWindowManagerHint;
        }
        else if(element == "FramelessWindowHint")
        {
            temp |= Qt::FramelessWindowHint;
        }
        else if(element == "NoDropShadowWindowHint")
        {
            temp |= Qt::NoDropShadowWindowHint;
        }
        else if(element == "CustomizeWindowHint")
        {
            temp |= Qt::CustomizeWindowHint;
        }
        else if(element == "WindowTitleHint")
        {
            temp |= Qt::WindowTitleHint;
        }
        else if(element == "WindowSystemMenuHint")
        {
            temp |= Qt::WindowSystemMenuHint;
        }
        else if(element == "WindowMinimizeButtonHint")
        {
            temp |= Qt::WindowMinimizeButtonHint;
        }
        else if(element == "WindowMaximizeButtonHint")
        {
            temp |= Qt::WindowMaximizeButtonHint;
        }
        else if(element == "WindowMinMaxButtonsHint")
        {
            temp |= Qt::WindowMinMaxButtonsHint;
        }
        else if(element == "WindowCloseButtonHint")
        {
            temp |= Qt::WindowCloseButtonHint;
        }
        else if(element == "WindowContextHelpButtonHint")
        {
            temp |= Qt::WindowContextHelpButtonHint;
        }
        else if(element == "MacWindowToolBarButtonHint")
        {
            temp |= Qt::MacWindowToolBarButtonHint;
        }
        else if(element == "WindowFullscreenButtonHint")
        {
            temp |= Qt::WindowFullscreenButtonHint;
        }
        else if(element == "BypassGraphicsProxyWidget")
        {
            temp |= Qt::BypassGraphicsProxyWidget;
        }
        else if(element == "WindowShadeButtonHint")
        {
            temp |= Qt::WindowShadeButtonHint;
        }
        else if(element == "WindowStaysOnTopHint")
        {
            temp |= Qt::WindowStaysOnTopHint;
        }
        else if(element == "WindowStaysOnBottomHint")
        {
            temp |= Qt::WindowStaysOnBottomHint;
        }
        else if(element == "WindowOkButtonHint")
        {
            temp |= Qt::WindowOkButtonHint;
        }
        else if(element == "WindowCancelButtonHint")
        {
            temp |= Qt::WindowCancelButtonHint;
        }
        else if(element == "WindowTransparentForInput")
        {
            temp |= Qt::WindowTransparentForInput;
        }
        else if(element == "WindowOverridesSystemGestures")
        {
            temp |= Qt::WindowOverridesSystemGestures;
        }
        else if(element == "WindowDoesNotAcceptFocus")
        {
            temp |= Qt::WindowDoesNotAcceptFocus;
        }
        else
        {
            error_message << "\"" << element << "\" cannot be converted to "
                          << "Qt::WindowFlag since it is not a recognised "
                          << "window flag.";
            return false;
        }
    }

    // successful
    m_value = temp;
    return true;
}

} // namespace meta_qt
