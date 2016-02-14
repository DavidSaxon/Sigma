/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_TASKS_TASK_HPP_
#define SIGMA_CORE_TASKS_TASK_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include <chaoscore/base/uni/UTF8String.hpp>

#include "sigma/core/Callback.hpp"

namespace sigma
{
namespace core
{
namespace tasks
{

/*!
 * \brief TODO
 *
 * TODO
 */
class Task
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     *
     * TODO
     */
    Task(const chaos::uni::UTF8String& title, Task* parent=nullptr);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Task();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    const chaos::uni::UTF8String& get_title() const;

    void set_title(const chaos::uni::UTF8String& title);

    Task* const get_parent() const;

    // void set_parent(Task* parent);

    // std::size_t get_children_size() const;

    // Task* get_child(std::size_t index) const;

    //--------------------------------------------------------------------------
    //                              CALLBACK EVENTS
    //--------------------------------------------------------------------------

    //----------------------------------GLOBAL----------------------------------

    /*!
     * \brief TODO
     */
    static sigma::core::CallbackInterface<Task*>* on_created()
    {
        return &s_created_callback.get_interface();
    }

    /*!
     * \brief TODO
     */
    static sigma::core::CallbackInterface<Task*>* on_destroyed()
    {
        return &s_destroyed_callback.get_interface();
    }

    //----------------------------------LOCAL-----------------------------------

    /*!
     * \brief TODO
     */
    sigma::core::CallbackInterface<Task*, const chaos::uni::UTF8String&>*
    on_title_changed()
    {
        return &m_title_changed_callback.get_interface();
    }

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE STATIC VARIABLES
    //--------------------------------------------------------------------------

    // global callback handlers
    static sigma::core::CallbackHandler<Task*> s_created_callback;
    static sigma::core::CallbackHandler<Task*> s_destroyed_callback;

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The title of this task.
     */
    chaos::uni::UTF8String m_title;

    // TODO: brief

    // TODO: full description? with images etc???

    // TODO: comments

    /*!
     * \brief TODO:
     */
    Task* m_parent;
    /*!
     * \brief TODO:
     */
    std::vector<std::unique_ptr<Task>> m_children;


    // TODO: links


    // local callback handlers
    sigma::core::CallbackHandler<Task*, const chaos::uni::UTF8String&>
            m_title_changed_callback;
};

} // namespace tasks
} // namespace core
} // namespace sigma

#endif
