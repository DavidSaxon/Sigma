#ifndef META_QT_CORE_QT_HPP_
#define META_QT_CORE_QT_HPP_

#include <metaengine/Document.hpp>

#include <QtCore/Qt>

namespace meta_qt
{

//------------------------------------------------------------------------------
//                                QT::WINDOWFLAGS
//------------------------------------------------------------------------------

/*!
 * \brief MetaEngine Visitor object used to retrieve a bitwise OR of
 *       Qt::WindowFlags.
 *
 * The windows flags must be defined as a list of UTF-8 string types which match
 * the window flag types, see official Qt::WindowFlags documentation for more
 * details: http://doc.qt.io/qt-5/qt.html#WindowType-enum
 *
 * Example:
 *
 * \code
 * "window_flags": ["Popup", "Dialog"]
 * \endcode
 */
class QtWindowFlagsV : public metaengine::Visitor<Qt::WindowFlags>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static QtWindowFlagsV& instance();

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            metaengine::Document* requester,
            arc::str::UTF8String& error_message);
};

} // namespace meta_qt

#endif
