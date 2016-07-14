#ifndef META_QT_CORE_GEOMETRY_HPP_
#define META_QT_CORE_GEOMETRY_HPP_

#include <metaengine/Document.hpp>

#include <QtCore/QPoint>
#include <QtCore/QSize>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class QWidget;

namespace meta_qt
{

//------------------------------------------------------------------------------
//                                 QT WIDGET SIZE
//------------------------------------------------------------------------------

/*!
 * \brief MetaEngine Visitor that is used to retrieve the absolute size of a
 *        QWidget as a QSize from a JSON object type in a metaengine::Doument.
 *
 * The JSON data must be an object that has two sub members named ```width```
 * and ```height``` which may be either integral or string type.
 *
 * Example:
 *
 * \code
 * "size":
 *  {
 *      "width": 200,
 *      "height": "1:2"
 *  }
 * \endcode
 *
 * The ```width``` and ```height``` members allow for a number of different
 * syntax:
 *
 * - <b>integral, string of integral type, or ```px``` suffix:</b> The absolute
 *   pixel value of the size. Examples: ```1000```, ```"200"```, ```"50px"```.
 *
 * - <b>string with ```%``` suffix:</b> The size is a relative percent of the
 *   widget's parent's size. Example: ```"50%"``` - the width/height will be
 *   half of the parent's width/height.
 *
 * - <b>string with ```#``` suffix:</b> The size is a percent of the desktop's
 *   default screen's size. Example: ```"22#"``` - the width/height will be 22%
 *   of the desktop's width/height.
 *
 * - <b>string ratio: </b> The size is a percentage of the other size member.
 *   This means both width and height cannot both be ratios.
 *   Example: ```"2:1"``` - if this was set for width, the width would be twice
 *   the size of the height.
 */
class WidgetSize : public metaengine::Visitor<QSize>
{
public:

    //-------------------------------CONSTRUCTOR--------------------------------

    /*!
     * \brief Constructs a new WidgetSize MetaEngine Visitor.
     *
     * \param widget Pointer to the widget that this is retrieve the size for.
     */
    WidgetSize(const QWidget* widget)
        :
        metaengine::Visitor<QSize>(),
        m_widget                  (widget)
    {
    }

    //-------------------------PUBLIC STATIC FUNCTIONS--------------------------

    /*!
     * \brief Provides an existing static instance of this object.
     *
     * \param widget The QWidget that the returned instance will be retrieving
     *               the size for.
     */
    static WidgetSize& instance(const QWidget* widget = nullptr);

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            metaengine::Document* requester,
            arc::str::UTF8String& error_message);

    /*!
     * \brief Sets the QWidget this will be retrieving the size for.
     */
    void set_widget(const QWidget* widget);

    /*!
     * \brief Returns the QWidget this is retrieving the size for.
     */
    const QWidget* get_widget() const;

private:

    //----------------------------PRIVATE ATTRIBUTES----------------------------

    /*!
     * \brief Pointer to the QWidget this is retrieving the size for.
     */
    const QWidget* m_widget;
};

//------------------------------------------------------------------------------
//                                WIDGET POSITION
//------------------------------------------------------------------------------

/*!
 * \brief MetaEngine Visitor that is used to retrieve the absolute position of a
 *        QWidget as a QPoint from a JSON object type in a metaengine::Doument.
 *
 * The JSON data must be an object that has sub members named ```x```
 * and ```y```, and optional members named ```origin_x``` and ```origin_y```,
 * which may be either integral or string type. The origin x and y values define
 * what will be considered the origin of the widget to move, they default
 * to ```0,0``` which is the top left of the widget.
 *
 * Example:
 *
 * \code
 "position":
 * {
 *     "x": "50#",
 *     "y": "50#",
 *     "origin_x": "50%",
 *     "origin_y": "50%"
 * }
 * \endcode
 *
 * The x and y values support the following syntax:
 *
 * - <b>integral, string of integral type, or ```px``` suffix:</b> The absolute
 *   pixel value of the position.
 *   Examples: ```75```, ```"200"```, ```"500px"```.
 *
 * - <b>string with ```%``` suffix:</b> The position is a relative percent of
 *   the widget's parent's position. Example: ```"50%"``` - the x/y position
 *   will be half of the parent's x/y position.
 *
 * - <b>string with ```#``` suffix:</b> The position a relative percent of the
 *   desktop's default screen's size. Example: ```"40#"``` - the x/y position
 *   will be 40% of the desktop's width/height.
 *
 * - <b>string ratio: </b> The x/y value is a percentage of the other x/y
 *   member. This means both x and y cannot both be ratios.
 *   Example: ```"1:3"``` - if this was set for x, the x position would be a
 *   third of the y position.
 *
 * The origin_x and origin_y values support the following syntax:
 *
 * - <b>integral, string of integral type, or ```px``` suffix:</b> The absolute
 *   pixel value of the widget's origin from 0, 0 (top left corner).
 *   Examples: ```20```, ```"44"```, ```"0px"```.
 *
 * - <b>string with ```%``` suffix:</b> The widget's origin from it's top left
 *   corner as a percentage of the widget's size. Example: ```"75%"```.
 */
class WidgetPosition : public metaengine::Visitor<QPoint>
{
public:

    //-------------------------------CONSTRUCTOR--------------------------------

    /*!
     * \brief Constructs a new WidgetPosition MetaEngine Visitor.
     *
     * \param widget Pointer to the widget that this is retrieve the position
     *               for.
     */
    WidgetPosition(const QWidget* widget)
        :
        metaengine::Visitor<QPoint>(),
        m_widget                   (widget)
    {
    }

    //-------------------------PUBLIC STATIC FUNCTIONS--------------------------

    /*!
     * \brief Provides an existing static instance of this object.
     *
     * \param widget The QWidget that the returned instance will be retrieving
     *               the position for.
     */
    static WidgetPosition& instance(const QWidget* widget = nullptr);

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            metaengine::Document* requester,
            arc::str::UTF8String& error_message);

    /*!
     * \brief Sets the QWidget this will be retrieving the position for.
     */
    void set_widget(const QWidget* widget);

    /*!
     * \brief Returns the QWidget this is retrieving the position for.
     */
    const QWidget* get_widget() const;

private:

    //----------------------------PRIVATE ATTRIBUTES----------------------------

    /*!
     * \brief Pointer to the QWidget this is retrieving the position for.
     */
    const QWidget* m_widget;
};

} // namespace meta_qt

#endif
