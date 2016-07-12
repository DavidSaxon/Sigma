#ifndef META_QT_CORE_GEOMETRY_HPP_
#define META_QT_CORE_GEOMETRY_HPP_

#include <metaengine/Document.hpp>

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
 * \brief TODO:
 *
 * TODO:
 */
class QtWidgetSize : public metaengine::Visitor<QSize>
{
public:

    //-------------------------------CONSTRUCTOR--------------------------------

    QtWidgetSize(const QWidget* widget)
        :
        metaengine::Visitor<QSize>(),
        m_widget(widget)
    {
    }

    //-------------------------PUBLIC STATIC FUNCTIONS--------------------------

    /*!
     * \brief Provides an existing static instance of this object.
     *
     * \param widget The QWidget that will used by the returned instance.
     */
    static QtWidgetSize& instance(const QWidget* widget = nullptr);

    //-------------------------PUBLIC MEMBER FUNCTIONS--------------------------

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            metaengine::Document* requester,
            arc::str::UTF8String& error_message);

    /*!
     * \brief Sets the QWidget this sizing is intended for.
     */
    void set_widget(const QWidget* widget);

    /*!
     * \brief Returns the QWidget this sizing is intended for.
     */
    const QWidget* get_widget() const;

private:

    //----------------------------PRIVATE ATTRIBUTES----------------------------

    /*!
     * \brief TODO:
     */
    const QWidget* m_widget;
};

} // namespace meta_qt

#endif
