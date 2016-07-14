#include "meta_qt/core/Geometry.hpp"

#include <json/json.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QWidget>

namespace meta_qt
{

//------------------------------------------------------------------------------
//                                  WIDGET SIZE
//------------------------------------------------------------------------------

//---------------------------PUBLIC STATIC FUNCTIONS----------------------------

WidgetSize& WidgetSize::instance(const QWidget* widget)
{
    static WidgetSize v(widget);
    v.set_widget(widget);
    return v;
}

//---------------------------PUBLIC MEMBER FUNCTIONS----------------------------

bool WidgetSize::retrieve(
        const Json::Value* data,
        const arc::str::UTF8String& key,
        metaengine::Document* requester,
        arc::str::UTF8String& error_message)
{
    // is the data an object
    if(!data->isObject())
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                      << "converted to an object type, which is required to "
                      << "retrieve Qt widget size.";
        return false;
    }

    // have the width and height members been supplied
    if(!data->isMember("width"))
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" has no member \""
                      << "width\" which is required to retrieve Qt widget "
                      << "size.";
        return false;
    }
    if(!data->isMember("height"))
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" has no member \""
                      << "height\" which is required to retrieve Qt widget "
                      << "size.";
        return false;
    }

    // get sub JSON values
    Json::Value width_member ((*data)["width"]);
    Json::Value height_member((*data)["height"]);

    // temporary value
    QSize temp;
    bool width_resolved  = false;
    bool height_resolved = false;

    // straight integers
    if(width_member.isInt())
    {
        temp.setWidth(width_member.asInt());
        width_resolved = true;
    }
    if(height_member.isInt())
    {
        temp.setHeight(height_member.asInt());
        height_resolved = true;
    }

    // resolve as strings
    arc::str::UTF8String width_s;
    arc::str::UTF8String height_s;

    if(!width_resolved)
    {
        if(!width_member.isString())
        {
            Json::FastWriter j_writer;
            error_message << "width member \"" << j_writer.write(width_member)
                          << "\" is not a valid integral or UTF-8 string "
                          << "type.";
            return false;
        }
        // retrieve the string from the member
        width_s = arc::str::UTF8String(width_member.asCString());
    }
    else
    {
        // just use the int value that was resolved earlier
        width_s << temp.width();
    }
    if(!height_resolved)
    {
        if(!height_member.isString())
        {
            Json::FastWriter j_writer;
            error_message << "height member \"" << j_writer.write(height_member)
                          << "\" is not a valid integral or UTF-8 string "
                          << "type.";
            return false;
        }
        // retrieve the string from the member
        height_s = arc::str::UTF8String(height_member.asCString());
    }
    else
    {
        // just use the int value that was resolved earlier
        height_s << temp.height();
    }

    // resolve straight pixels
    if(!width_resolved && width_s.is_int())
    {
        temp.setWidth(width_s.to_int32());
        width_resolved = true;
    }
    if(!height_resolved && height_s.is_int())
    {
        temp.setHeight(height_s.to_int32());
        height_resolved = true;
    }

    // resolve with the px extension
    if(!width_resolved && width_s.ends_with("px"))
    {
        // ensure we can get an integral
        arc::str::UTF8String integral(
            width_s.substring(0, width_s.get_length() - 2));
        if(!integral.is_int())
        {
            error_message << "Cannot convert width member pixel value \""
                          << integral << "\" to an integral type.";
            return false;
        }


        temp.setWidth(integral.to_int32());
        width_resolved = true;
    }
    if(!height_resolved && height_s.ends_with("px"))
    {
        // ensure we can get an integral
        arc::str::UTF8String integral(
            height_s.substring(0, height_s.get_length() - 2));
        if(!integral.is_int())
        {
            error_message << "Cannot convert height member pixel value \""
                          << integral << "\" to an integral type.";
            return false;
        }


        temp.setHeight(integral.to_int32());
        height_resolved = true;
    }

    // resolve relative to parent's size
    if(!width_resolved && width_s.ends_with("%"))
    {
        error_message << "Cannot resolve width member relative to parent\'s "
                      << "size as this has not yet been implemented.";
        return false;
    }
    if(!height_resolved && height_s.ends_with("%"))
    {
        error_message << "Cannot resolve height member relative to parent\'s "
                      << "size as this has not yet been implemented.";
        return false;
    }

    // resolve based on the desktop size
    if(!width_resolved && width_s.ends_with("#"))
    {
        // ensure we can get an integral
        arc::str::UTF8String integral(
            width_s.substring(0, width_s.get_length() - 1));
        if(!integral.is_int())
        {
            error_message << "Cannot convert width member relative desktop "
                          << "value \"" << integral << "\" to an integral "
                          << "type.";
            return false;
        }
        // get as percentage
        float percent = static_cast<float>(integral.to_int32()) / 100.0F;

        // get the width of the desktop
        arc::int32 desktop_width =
            QApplication::desktop()->screenGeometry().width();

        // apply
        temp.setWidth(desktop_width * percent);
        width_resolved = true;
    }
    if(!height_resolved && height_s.ends_with("#"))
    {
        // ensure we can get an integral
        arc::str::UTF8String integral(
            height_s.substring(0, height_s.get_length() - 1));
        if(!integral.is_int())
        {
            error_message << "Cannot convert height member relative desktop "
                          << "value \"" << integral << "\" to an integral "
                          << "type.";
            return false;
        }
        // get as percentage
        float percent = static_cast<float>(integral.to_int32()) / 100.0F;

        // get the height of the desktop
        arc::int32 desktop_height =
            QApplication::desktop()->screenGeometry().height();

        // apply
        temp.setHeight(desktop_height * percent);
        height_resolved = true;
    }

    // resolve based on ratio
    if(!width_resolved && width_s.find_first(":") != arc::str::npos)
    {
        // split into the ratio parts
        std::vector<arc::str::UTF8String> ratio_comps =
            width_s.split(":");
        // must be exactly two parts
        if(ratio_comps.size() == 2)
        {
            // check both parts are integral
            if(!ratio_comps[0].is_uint() || !ratio_comps[1].is_uint())
            {
                error_message << "Cannot convert width member to ratio as "
                              << "value \"" << width_s << "\" is not a valid "
                              << "ratio of two unsigned integrals.";
                return false;
            }
            // get ratio values
            arc::uint32 antecedent = ratio_comps[0].to_uint32();
            arc::uint32 consequent = ratio_comps[1].to_uint32();

            // ensure the consequent isn't zero
            if(consequent == 0)
            {
                error_message << "Cannot convert width member to ratio \""
                              << width_s << "\" as the ratio consequent is 0.";
                return false;
            }

            // convert the ration to a percent
            float percent =
                static_cast<float>(antecedent) / static_cast<float>(consequent);

            // has height been resolved?
            if(!height_resolved)
            {
                error_message << "Cannot use ratio for width member \""
                              << width_s << "\" as the height member \""
                              << height_s << "\" is either invalid or a ratio "
                              << "also";
                return false;
            }

            // apply the ratio
            temp.setWidth(temp.height() * percent);
            width_resolved = true;
        }
    }
    if(!height_resolved && height_s.find_first(":") != arc::str::npos)
    {
        // split into the ratio parts
        std::vector<arc::str::UTF8String> ratio_comps =
            height_s.split(":");
        // must be exactly two parts
        if(ratio_comps.size() == 2)
        {
            // check both parts are integral
            if(!ratio_comps[0].is_uint() || !ratio_comps[1].is_uint())
            {
                error_message << "Cannot convert height member to ratio as "
                              << "value \"" << height_s << "\" is not a valid "
                              << "ratio of two unsigned integrals.";
                return false;
            }
            // get ratio values
            arc::uint32 antecedent = ratio_comps[0].to_uint32();
            arc::uint32 consequent = ratio_comps[1].to_uint32();

            // ensure the consequent isn't zero
            if(consequent == 0)
            {
                error_message << "Cannot convert height member to ratio \""
                              << height_s << "\" as the ratio consequent is 0.";
                return false;
            }

            // convert the ration to a percent
            float percent =
                static_cast<float>(antecedent) / static_cast<float>(consequent);

            // has width been resolved?
            if(!width_resolved)
            {
                error_message << "Cannot use ratio for height member \""
                              << height_s << "\" as the width member \""
                              << width_s << "\" is either invalid or a ratio "
                              << "also";
                return false;
            }

            // apply the ratio
            temp.setHeight(temp.width() * percent);
            height_resolved = true;
        }
    }

    // report if members failed to resolve
    if(!width_resolved)
    {
        error_message << "Cannot resolve width member \"" << width_s << "\" "
                      << "since it has an unrecognised syntax.";
        return false;
    }
    if(!height_resolved)
    {
        error_message << "Cannot resolve height member \"" << height_s << "\" "
                      << "since it has an unrecognised syntax.";
        return false;
    }

    // assign and return
    m_value = temp;
    return true;
}

void WidgetSize::set_widget(const QWidget* widget)
{
    m_widget = widget;
}

const QWidget* WidgetSize::get_widget() const
{
    return m_widget;
}

//------------------------------------------------------------------------------
//                                WIDGET POSITION
//------------------------------------------------------------------------------

//---------------------------PUBLIC STATIC FUNCTIONS----------------------------

WidgetPosition& WidgetPosition::instance(const QWidget* widget)
{
    static WidgetPosition v(nullptr);
    v.set_widget(widget);
    return v;
}

//---------------------------PUBLIC MEMBER FUNCTIONS----------------------------

bool WidgetPosition::retrieve(
        const Json::Value* data,
        const arc::str::UTF8String& key,
        metaengine::Document* requester,
        arc::str::UTF8String& error_message)
{
    // is the data an object
    if(!data->isObject())
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                      << "converted to an object type, which is required to "
                      << "retrieve Qt widget position.";
        return false;
    }

    // have the width and height members been supplied
    if(!data->isMember("x"))
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" has no member \""
                      << "x\" which is required to retrieve Qt widget "
                      << "position.";
        return false;
    }
    if(!data->isMember("y"))
    {
        Json::FastWriter j_writer;
        error_message << "\"" << j_writer.write(*data) << "\" has no member \""
                      << "y\" which is required to retrieve Qt widget "
                      << "position.";
        return false;
    }

    // get sub JSON values
    Json::Value x_member((*data)["x"]);
    Json::Value y_member((*data)["y"]);

    // temporary value
    QPoint temp;
    bool x_resolved = false;
    bool y_resolved = false;

    // straight integers
    if(x_member.isInt())
    {
        temp.setX(x_member.asInt());
        x_resolved = true;
    }
    if(y_member.isInt())
    {
        temp.setY(y_member.asInt());
        y_resolved = true;
    }

    // resolve as strings
    arc::str::UTF8String x_s;
    arc::str::UTF8String y_s;

    if(!x_resolved)
    {
        if(!x_member.isString())
        {
            Json::FastWriter j_writer;
            error_message << "x member \"" << j_writer.write(x_member)
                          << "\" is not a valid integral or UTF-8 string "
                          << "type.";
            return false;
        }
        // retrieve the string from the member
        x_s = arc::str::UTF8String(x_member.asCString());
    }
    // TODO: need?
    else
    {
        // just use the int value that was resolved earlier
        x_s << temp.x();
    }
    if(!y_resolved)
    {
        if(!y_member.isString())
        {
            Json::FastWriter j_writer;
            error_message << "y member \"" << j_writer.write(y_member)
                          << "\" is not a valid integral or UTF-8 string "
                          << "type.";
            return false;
        }
        // retrieve the string from the member
        y_s = arc::str::UTF8String(y_member.asCString());
    }
    // TODO: need?
    else
    {
        // just use the int value that was resolved earlier
        y_s << temp.y();
    }

    // resolve straight pixels
    if(!x_resolved && x_s.is_int())
    {
        temp.setX(x_s.to_int32());
        x_resolved = true;
    }
    if(!y_resolved && y_s.is_int())
    {
        temp.setY(y_s.to_int32());
        y_resolved = true;
    }

    // resolve with the px extension
    if(!x_resolved && x_s.ends_with("px"))
    {
        // ensure we can get an integral
        arc::str::UTF8String integral(
            x_s.substring(0, x_s.get_length() - 2));
        if(!integral.is_int())
        {
            error_message << "Cannot convert x member pixel value \""
                          << integral << "\" to an integral type.";
            return false;
        }


        temp.setX(integral.to_int32());
        x_resolved = true;
    }
    if(!y_resolved && y_s.ends_with("px"))
    {
        // ensure we can get an integral
        arc::str::UTF8String integral(
            y_s.substring(0, y_s.get_length() - 2));
        if(!integral.is_int())
        {
            error_message << "Cannot convert y member pixel value \""
                          << integral << "\" to an integral type.";
            return false;
        }


        temp.setY(integral.to_int32());
        y_resolved = true;
    }

    // resolve relative to parent's position
    if(!x_resolved && x_s.ends_with("%"))
    {
        error_message << "Cannot resolve x member relative to parent\'s "
                      << "position as this has not yet been implemented.";
        return false;
    }
    if(!y_resolved && y_s.ends_with("%"))
    {
        error_message << "Cannot resolve y member relative to parent\'s "
                      << "position as this has not yet been implemented.";
        return false;
    }

    // resolve based on the desktop size
    if(!x_resolved && x_s.ends_with("#"))
    {
        // ensure we can get an integral
        arc::str::UTF8String integral(
            x_s.substring(0, x_s.get_length() - 1));
        if(!integral.is_int())
        {
            error_message << "Cannot convert x member relative desktop "
                          << "value \"" << integral << "\" to an integral "
                          << "type.";
            return false;
        }
        // get as percentage
        float percent = static_cast<float>(integral.to_int32()) / 100.0F;

        // get the width of the desktop
        arc::int32 desktop_width =
            QApplication::desktop()->screenGeometry().width();

        // apply
        temp.setX(desktop_width * percent);
        x_resolved = true;
    }
    if(!y_resolved && y_s.ends_with("#"))
    {
        // ensure we can get an integral
        arc::str::UTF8String integral(
            y_s.substring(0, y_s.get_length() - 1));
        if(!integral.is_int())
        {
            error_message << "Cannot convert y member relative desktop "
                          << "value \"" << integral << "\" to an integral "
                          << "type.";
            return false;
        }
        // get as percentage
        float percent = static_cast<float>(integral.to_int32()) / 100.0F;

        // get the height of the desktop
        arc::int32 desktop_height =
            QApplication::desktop()->screenGeometry().height();

        // apply
        temp.setY(desktop_height * percent);
        y_resolved = true;
    }

    // resolve based on ratio
    if(!x_resolved && x_s.find_first(":") != arc::str::npos)
    {
        // split into the ratio parts
        std::vector<arc::str::UTF8String> ratio_comps = x_s.split(":");
        // must be exactly two parts
        if(ratio_comps.size() == 2)
        {
            // check both parts are integral
            if(!ratio_comps[0].is_uint() || !ratio_comps[1].is_uint())
            {
                error_message << "Cannot convert x member to ratio as "
                              << "value \"" << x_s << "\" is not a valid "
                              << "ratio of two unsigned integrals.";
                return false;
            }
            // get ratio values
            arc::uint32 antecedent = ratio_comps[0].to_uint32();
            arc::uint32 consequent = ratio_comps[1].to_uint32();

            // ensure the consequent isn't zero
            if(consequent == 0)
            {
                error_message << "Cannot convert x member to ratio \""
                              << x_s << "\" as the ratio consequent is 0.";
                return false;
            }

            // convert the ration to a percent
            float percent =
                static_cast<float>(antecedent) / static_cast<float>(consequent);

            // has height been resolved?
            if(!y_resolved)
            {
                error_message << "Cannot use ratio for x member \""
                              << x_s << "\" as the y member \""
                              << y_s << "\" is either invalid or a ratio "
                              << "also";
                return false;
            }

            // apply the ratio
            temp.setX(temp.y() * percent);
            x_resolved = true;
        }
    }
    if(!y_resolved && y_s.find_first(":") != arc::str::npos)
    {
        // split into the ratio parts
        std::vector<arc::str::UTF8String> ratio_comps = y_s.split(":");
        // must be exactly two parts
        if(ratio_comps.size() == 2)
        {
            // check both parts are integral
            if(!ratio_comps[0].is_uint() || !ratio_comps[1].is_uint())
            {
                error_message << "Cannot convert y member to ratio as "
                              << "value \"" << y_s << "\" is not a valid "
                              << "ratio of two unsigned integrals.";
                return false;
            }
            // get ratio values
            arc::uint32 antecedent = ratio_comps[0].to_uint32();
            arc::uint32 consequent = ratio_comps[1].to_uint32();

            // ensure the consequent isn't zero
            if(consequent == 0)
            {
                error_message << "Cannot convert y member to ratio \""
                              << y_s << "\" as the ratio consequent is 0.";
                return false;
            }

            // convert the ration to a percent
            float percent =
                static_cast<float>(antecedent) / static_cast<float>(consequent);

            // has width been resolved?
            if(!x_resolved)
            {
                error_message << "Cannot use ratio for y member \""
                              << y_s << "\" as the x member \""
                              << x_s << "\" is either invalid or a ratio "
                              << "also";
                return false;
            }

            // apply the ratio
            temp.setY(temp.y() * percent);
            y_resolved = true;
        }
    }

    // report if members failed to resolve
    if(!x_resolved)
    {
        error_message << "Cannot resolve x member \"" << x_s << "\" "
                      << "since it has an unrecognised syntax.";
        return false;
    }
    if(!y_resolved)
    {
        error_message << "Cannot resolve y member \"" << y_s << "\" "
                      << "since it has an unrecognised syntax.";
        return false;
    }

    // is there origin_x to resolve?
    arc::int32 origin_x = 0;
    if(data->isMember("origin_x"))
    {
        // get sub JSON value
        Json::Value o_x_member((*data)["origin_x"]);

        bool o_x_resolved = false;
        // straight integer
        if(o_x_member.isInt())
        {
            origin_x = o_x_member.asInt();
            o_x_resolved = true;
        }

        // resolve as a string
        arc::str::UTF8String o_x_s;

        if(!o_x_resolved)
        {
            if(!o_x_member.isString())
            {
                Json::FastWriter j_writer;
                error_message << "origin_x member \""
                              << j_writer.write(o_x_member)
                              << "\" is not a valid integral or UTF-8 string "
                              << "type.";
                return false;
            }
            // retrieve the string from the member
            o_x_s = arc::str::UTF8String(o_x_member.asCString());
        }
        else
        {
            // just use the int value that was resolved earlier
            o_x_s << origin_x;
        }

        // resolve straight pixels
        if(!o_x_resolved && o_x_s.is_int())
        {
            origin_x = o_x_s.to_int32();
            o_x_resolved = true;
        }

        // resolve with the px extension
        if(!o_x_resolved && o_x_s.ends_with("px"))
        {
            // ensure we can get an integral
            arc::str::UTF8String integral(
                o_x_s.substring(0, o_x_s.get_length() - 2));
            if(!integral.is_int())
            {
                error_message << "Cannot convert origin_x member pixel value \""
                              << integral << "\" to an integral type.";
                return false;
            }

            origin_x = integral.to_int32();
            o_x_resolved = true;
        }

        // resolve relative percentage
        if(!o_x_resolved && o_x_s.ends_with("%"))
        {
            // ensure we can get an integral
            arc::str::UTF8String integral(
                o_x_s.substring(0, o_x_s.get_length() - 1));
            if(!integral.is_int())
            {
                error_message << "Cannot convert origin_x member percentage \""
                              << integral << "\" to an integral type.";
                return false;
            }

            // get the percentage
            float percent = static_cast<float>(integral.to_int32()) / 100.0F;

            // apply
            origin_x = m_widget->geometry().width() * percent;
            o_x_resolved = true;
        }

        // report if  it couldn't be resolved
        if(!o_x_resolved)
        {
            error_message << "Cannot resolve origin_x member \"" << o_x_s
                          << "\" since it has an unrecognised syntax.";
            return false;
        }
    }

    // is there origin_y to resolve?
    arc::int32 origin_y = 0;
    if(data->isMember("origin_y"))
    {
        // get sub JSON value
        Json::Value o_y_member((*data)["origin_y"]);

        bool o_y_resolved = false;
        // straight integer
        if(o_y_member.isInt())
        {
            origin_y = o_y_member.asInt();
            o_y_resolved = true;
        }

        // resolve as a string
        arc::str::UTF8String o_y_s;

        if(!o_y_resolved)
        {
            if(!o_y_member.isString())
            {
                Json::FastWriter j_writer;
                error_message << "origin_y member \""
                              << j_writer.write(o_y_member)
                              << "\" is not a valid integral or UTF-8 string "
                              << "type.";
                return false;
            }
            // retrieve the string from the member
            o_y_s = arc::str::UTF8String(o_y_member.asCString());
        }
        else
        {
            // just use the int value that was resolved earlier
            o_y_s << origin_y;
        }

        // resolve straight pixels
        if(!o_y_resolved && o_y_s.is_int())
        {
            origin_y = o_y_s.to_int32();
            o_y_resolved = true;
        }

        // resolve with the px extension
        if(!o_y_resolved && o_y_s.ends_with("px"))
        {
            // ensure we can get an integral
            arc::str::UTF8String integral(
                o_y_s.substring(0, o_y_s.get_length() - 2));
            if(!integral.is_int())
            {
                error_message << "Cannot convert origin_y member pixel value \""
                              << integral << "\" to an integral type.";
                return false;
            }

            origin_y = integral.to_int32();
            o_y_resolved = true;
        }

        // resolve relative percentage
        if(!o_y_resolved && o_y_s.ends_with("%"))
        {
            // ensure we can get an integral
            arc::str::UTF8String integral(
                o_y_s.substring(0, o_y_s.get_length() - 1));
            if(!integral.is_int())
            {
                error_message << "Cannot convert origin_y member percentage \""
                              << integral << "\" to an integral type.";
                return false;
            }

            // get the percentage
            float percent = static_cast<float>(integral.to_int32()) / 100.0F;

            // apply
            origin_y = m_widget->geometry().height() * percent;
            o_y_resolved = true;
        }

        // report if  it couldn't be resolved
        if(!o_y_resolved)
        {
            error_message << "Cannot resolve origin_y member \"" << o_y_s
                          << "\" since it has an unrecognised syntax.";
            return false;
        }
    }

    // apply origin
    temp.setX(temp.x() - origin_x);
    temp.setY(temp.y() - origin_y);

    // assign and return
    m_value = temp;
    return true;
}

void WidgetPosition::set_widget(const QWidget* widget)
{
    m_widget = widget;
}

const QWidget* WidgetPosition::get_widget() const
{
    return m_widget;
}

} // namespace meta_qt
