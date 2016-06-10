/*!
 * \file
 * \brief TODO
 * \author David Saxon
 */
#ifndef CHAOSLOG_STREAM_HPP_
#define CHAOSLOG_STREAM_HPP_

#include <chaoscore/base/str/UTF8String.hpp>

namespace chlog
{

// TODO:
class Stream
{
public:

    Stream();

    void write(const chaos::str::UTF8String& message);

private:

    unsigned m_count;
};

} // namespace chlog

#endif
