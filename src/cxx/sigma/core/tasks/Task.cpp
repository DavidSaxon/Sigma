#include "sigma/core/tasks/Task.hpp"

#include <algorithm>

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

Task::Task(Task* parent, const chaos::str::UTF8String& title)
    :
    m_parent(nullptr)
{
    // tasks cannot be constructed with a null parent
    if (parent == nullptr)
    {
        clean_up();
        throw chaos::ex::ValueError("Tasks cannot have a null parent");
    }

    try
    {
        // set parent
        set_parent_internal(parent);
        // set title
        set_title_internal(title);
    }
    catch(const chaos::ex::IllegalActionError& e)
    {
        clean_up();
        throw e;
    }
    catch(const chaos::ex::ValueError& e)
    {
        clean_up();
        throw e;
    }

    // assign id
    m_id = ++s_id;

    // fire callback
    s_created_callback.trigger(this);
}

Task::Task(const Task& other)
    :
    m_parent(nullptr)
{
    // check the other task is not a RootTask
    if(other.is_root())
    {
        clean_up();
        throw chaos::ex::ValueError("A RootTask cannot be copied from");
    }

    try
    {
        // set parent
        set_parent_internal(other.m_parent);
        // set title
        set_title_internal(other.m_title);
    }
    catch(const chaos::ex::IllegalActionError& e)
    {
        clean_up();
        throw e;
    }
    catch(const chaos::ex::ValueError& e)
    {
        clean_up();
        throw e;
    }

    // assign id
    m_id = ++s_id;

    // fire callback
    s_created_callback.trigger(this);
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
    // is the given parent null?
    if (parent == nullptr)
    {
        assert(m_parent != nullptr);
        m_parent->remove_child(this);
    }
    else
    {
        // set and trigger callback
        sigma::core::tasks::Task* old_parent = m_parent;
        set_parent_internal(parent);
        m_parent_changed_callback.trigger(this, old_parent, m_parent);
    }
}

std::size_t Task::get_children_count() const
{
    return m_children.size();
}

const std::vector<Task*>& Task::get_chidren() const
{
    return m_children;
}

bool Task::has_child(Task* const child) const
{
    return std::find(m_children.begin(), m_children.end(), child) !=
           m_children.end();
}

bool Task::add_child(Task* const child)
{
    // check that the task isn't already a child
    if (has_child(child))
    {
        return false;
    }

    // simply set the child's parent to be this Task
    child->set_parent(this);

    return true;
}

bool Task::remove_child(Task* const child)
{
    // if the task is not a child, do nothing and return false
    if (!has_child(child))
    {
        return false;
    }

    // just delete the child, which will trigger the clean up routine which will
    // do the rest
    delete child;

    return true;
}

void Task::clear_children()
{
    // iterate over children and remove them
    std::vector<Task*> children_copy(m_children);
    CHAOS_FOR_EACH(it, children_copy)
    {
        remove_child(*it);
    }
    m_children.clear();
}

const chaos::str::UTF8String& Task::get_title() const
{
    return m_title;
}

void Task::set_title(const chaos::str::UTF8String& title)
{
    chaos::str::UTF8String old_title(title);
    set_title_internal(title);
    // fire callback
    m_title_changed_callback.trigger(this, old_title, m_title);
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTOR
//------------------------------------------------------------------------------

Task::Task(const chaos::str::UTF8String& title)
    :
    m_parent(nullptr),
    m_title (title)
{
    // title should never be empty since the task domain should enforce this
    assert(!title.is_empty());

    // assign id
    m_id = ++s_id;

    // fire callback
    s_created_callback.trigger(this);
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Task::set_parent_internal(Task* const parent)
{
    // do nothing if the parent is the same
    if (parent == m_parent)
    {
        return;
    }

    // check if the parent is a already a descendant of this task
    if (has_descendant(parent))
    {
        throw chaos::ex::IllegalActionError(
            "A Task\'s parent cannot be set to one of it's descendants.");
    }

    // remove from the current parent
    if (m_parent != nullptr)
    {
        assert(m_parent->has_child(this));
        m_parent->m_children.erase(std::find(
                m_parent->m_children.begin(),
                m_parent->m_children.end(),
                this
        ));
    }

    // set the parent
    m_parent = parent;
    // add to the children of the parent
    m_parent->m_children.push_back(this);
}

void Task::set_title_internal(const chaos::str::UTF8String& title)
{
    // check the title is not empty
    if (title.is_empty())
    {
        throw chaos::ex::ValueError("Tasks cannot have a blank title");
    }

    m_title = title;
}

bool Task::has_descendant(Task* const descendant) const
{
    // check if any of direct children match
    CHAOS_FOR_EACH(it, m_children)
    {
        if (*it == descendant || (*it)->has_descendant(descendant))
        {
            return true;
        }
    }

    return false;
}

void Task::clean_up()
{
    // copy the list of children since deleting them will cause modifications
    // on this Task's list of children
    std::vector<Task*> children_copy(m_children);

    // iterate over children and delete them
    CHAOS_FOR_EACH(it, children_copy)
    {
        delete *it;
    }
    children_copy.clear();
    m_children.clear();

    // fire callback
    s_destroyed_callback.trigger(this);

    // clean up this task from it's parent (if it has one)
    if (m_parent != nullptr)
    {
        // find this task in the parent's list of children
        std::vector<Task*>::iterator location = std::find(
            m_parent->m_children.begin(),
            m_parent->m_children.end(),
            this
        );

        assert(location != m_parent->m_children.end());

        m_parent->m_children.erase(location);
    }
}

} // namespace tasks
} // namespace core
} // namespace sigma
