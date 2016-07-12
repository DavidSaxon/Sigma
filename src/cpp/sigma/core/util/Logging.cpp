#include "sigma/core/util/Logging.hpp"

#include <json/json.h>

namespace sigma
{
namespace core
{
namespace util
{

//------------------------------------------------------------------------------
//                             LOG VERBOSITY VISITOR
//------------------------------------------------------------------------------

ArcLogVerbosityV& ArcLogVerbosityV::instance()
{
    static ArcLogVerbosityV v;
    return v;
}

bool ArcLogVerbosityV::retrieve(
        const Json::Value* data,
        const arc::str::UTF8String& key,
        metaengine::Document* requester,
        arc::str::UTF8String& error_message)
{
    // check type
    if(data->isString())
    {
        arc::str::UTF8String str(data->asCString());
        if(str == "critical")
        {
            m_value = arclog::VERBOSITY_CRITICAL;
        }
        else if(str == "error")
        {
            m_value = arclog::VERBOSITY_ERROR;
        }
        else if(str == "warning")
        {
            m_value = arclog::VERBOSITY_WARNING;
        }
        else if(str == "notice")
        {
            m_value = arclog::VERBOSITY_NOTICE;
        }
        else if(str == "info")
        {
            m_value = arclog::VERBOSITY_INFO;
        }
        else if(str == "debug")
        {
            m_value = arclog::VERBOSITY_DEBUG;
        }
        else
        {
            error_message << "\"" << str << "\" cannot be converted to "
                          << "arclog::Verbosity since it is not a recognised "
                          << "verbosity level.";
            return false;
        }
    }
    else
    {
        error_message << "\"" << data->toStyledString() << "\" cannot be "
                      << "converted to arclog::Verbosity since it is not of "
                      << "string type.";
        return false;
    }

    return true;
}

} // namespace util
} // namespace core
} // namespace sigma
