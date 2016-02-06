#include "sigma/core/tasks/Task.hpp"

namespace sigma
{
namespace core
{
namespace tasks
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Task::Task(const chaos::uni::UTF8String& title, Task* parent)
    :
    m_title (title),
    m_parent(parent)
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const chaos::uni::UTF8String& Task::get_title() const
{
    return m_title;
}

void Task::set_title(const chaos::uni::UTF8String& title)
{
    m_title = title;
    // TODO: check for other titles with the same name???
    // TODO: fire callback
}

} // namespace tasks
} // namespace core
} // namespace sigma
