#include "sigma/core/tasks/RootTask.hpp"

namespace sigma
{
namespace core
{
namespace tasks
{

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool RootTask::is_root() const
{
    return true;
}

void RootTask::set_parent(Task* const parent)
{
    throw arc::ex::IllegalActionError("");
}

void RootTask::set_title(const arc::str::UTF8String& title)
{
    // ensure this is a unique title using the resolver
    arc::str::UTF8String resolved;
    // only use the resolver if the title has changed
    if(title == m_title)
    {
        resolved = title;
    }
    else
    {
        m_title_resolver(title, resolved);
    }

    // super call
    Task::set_title(resolved);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

RootTask::RootTask(
        const arc::str::UTF8String& title,
        TitleResolver_t title_resolver)
    :
    Task            (title),
    m_title_resolver(title_resolver)
{
}

} // namespace tasks
} // namespace core
} // namespace sigma
