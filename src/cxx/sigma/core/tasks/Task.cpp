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

chaos::uint32 Task::s_id = 0;

sigma::core::CallbackHandler<Task*> Task::s_created_callback;
sigma::core::CallbackHandler<Task*> Task::s_destroyed_callback;

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Task::Task(Task* parent, const chaos::uni::UTF8String& title)
    :
    m_parent(parent),
    m_title (title)
{
    // check parent is not null
    if (m_parent == nullptr)
    {
        throw chaos::ex::ValueError("Tasks cannot have a null parent");
    }
    // check the title is not empty
    if (m_title.is_empty())
    {
        throw chaos::ex::ValueError("Tasks cannot have a blank title");
    }

    // assign id
    m_id = ++s_id;

    // fire callback
    s_created_callback.trigger(this);
}

Task::Task(const Task& other)
    :
    m_parent(other.m_parent),
    m_title(other.m_title + " (copy)")
{
    // assign id
    m_id = ++s_id;

    // fire callback
    s_created_callback.trigger(this);
}

//------------------------------------------------------------------------------
//                                   OPERATORS
//------------------------------------------------------------------------------

Task& Task::operator=(const Task& other)
{
    m_parent = other.m_parent;
    m_title  = other.m_title;

    return *this;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

chaos::uint32 Task::get_id() const
{
    return m_id;
}

bool Task::is_root() const
{
    return false;
}

Task* const Task::get_parent() const
{
    return m_parent;
}

void Task::set_parent(Task* const parent)
{
    // TODO:
}

std::size_t Task::get_children_count() const
{
    return m_children.size();
}

const std::set<Task*>& Task::get_chidren() const
{
    return m_children;
}

bool Task::has_child(Task* const child) const
{
    return m_children.find(child) != m_children.end();
}

bool Task::add_child(Task* const child)
{
    // TODO:

    return false;
}

bool Task::remove_child(Task* const child)
{
    // TODO:

    return false;
}

void Task::clear_children()
{
    // TODO:
}

const chaos::uni::UTF8String& Task::get_title() const
{
    return m_title;
}

void Task::set_title(const chaos::uni::UTF8String& title)
{
    chaos::uni::UTF8String old_title(title);
    m_title = title;
    // fire callback
    m_title_changed_callback.trigger(this, old_title, m_title);
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTOR
//------------------------------------------------------------------------------

Task::Task(const chaos::uni::UTF8String& title)
    :
    m_parent(nullptr),
    m_title (title)
{
    assert(!title.is_empty());

    // assign id
    m_id = ++s_id;

    // fire callback
    s_created_callback.trigger(this);
}

} // namespace tasks
} // namespace core
} // namespace sigma
