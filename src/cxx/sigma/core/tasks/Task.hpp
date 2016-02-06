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
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    const chaos::uni::UTF8String& get_title() const;

    void set_title(const chaos::uni::UTF8String& title);

    // TODO: needs callbacks implemented

    // Task* get_parent() const;

    // std::size_t get_children_size() const;

    // Task* get_child(std::size_t index) const;

private:

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


};

} // namespace tasks
} // namespace core
} // namespace sigma

#endif
