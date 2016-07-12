/*!
 * \file
 * \brief Sigma specific utilities related to logging.
 * \author David Saxon
 */
#ifndef SIGMA_CORE_UTIL_LOGGING_HPP_
#define SIGMA_CORE_UTIL_LOGGING_HPP_

#include <arcanelog/Verbosity.hpp>

#include <metaengine/Document.hpp>

namespace sigma
{
namespace core
{
namespace util
{

/*!
 * \brief MetaEngine Visitor object used to retrieve arclog::Verbosity values
 *                   from a metaengine::Document.
 *
 * Verbosity values must be expressed in the document as one of the following
 * strings:
 *
 * - critical
 * - error
 * - warning
 * - notice
 * - info
 * - debug
 */
class ArcLogVerbosityV : public metaengine::Visitor<arclog::Verbosity>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static ArcLogVerbosityV& instance();

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            metaengine::Document* requester,
            arc::str::UTF8String& error_message);
};

} // namespace util
} // namespace core
} // namespace sigma

#endif
