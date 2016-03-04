/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_TASKS_ROOTTASK_HPP_
#define SIGMA_CORE_TASKS_ROOTTASK_HPP_

#include "sigma/core/tasks/Task.hpp"
#include "sigma/core/tasks/TasksDomain.hpp"

namespace sigma
{
namespace core
{
namespace tasks
{

typedef void (*TitleResolver_t)(
        const chaos::uni::UTF8String&,
        chaos::uni::UTF8String&);

/*!
 * \brief TODO
 *
 * TODO:
 */
class RootTask : public Task
{

    friend RootTask* domain::new_board(const chaos::uni::UTF8String&);

public:

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual bool is_root() const;

    /*!
     * \brief Throws an chaos::ex::IllegalActionError since a RootTask cannot
     *        have a parent.
     */
    virtual void set_parent(Task* const parent);

    virtual void set_title(const chaos::uni::UTF8String& title);

private:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     *\brief Private constructor.
     *
     * Creates a new RootTask with a null parent.
     * This constructor is private as RootTasks should only be constructed by
     * the API.
     *
     * \param title The title of this task.
     * \param title_resolver Function pointer to use for resolving unique
     *                       titles.
     */
    RootTask(
            const chaos::uni::UTF8String& title,
            TitleResolver_t title_resolver);

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The function to use for resolving board titles.
     */
    TitleResolver_t m_title_resolver;
};

} // namespace tasks
} // namespace core
} // namespace sigma

#endif
