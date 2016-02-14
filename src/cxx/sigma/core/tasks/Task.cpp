#include "sigma/core/tasks/Task.hpp"

namespace sigma
{
namespace core
{
namespace tasks
{

//------------------------------------------------------------------------------
//                            PRIVATE STATIC VARIABLES
//------------------------------------------------------------------------------

sigma::core::CallbackHandler<Task*> Task::s_created_callback;
sigma::core::CallbackHandler<Task*> Task::s_destroyed_callback;

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Task::Task(const chaos::uni::UTF8String& title, Task* parent)
    :
    m_title (title),
    m_parent(parent)
{
    // fire callback
    s_created_callback.trigger(this);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Task::~Task()
{
    // fire callback
    s_destroyed_callback.trigger(this);
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
    // fire callback
    m_title_changed_callback.trigger(this, title);
}

Task* const Task::get_parent() const
{
    return m_parent;
}

} // namespace tasks
} // namespace core
} // namespace sigma
