/*!
 * \file
 * \author David Saxon
 */
#ifndef SIGMA_CORE_TASKS_TASK_HPP_
#define SIGMA_CORE_TASKS_TASK_HPP_

#include <cstddef>

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
 * TODO: about deleting
 */
class Task
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new Task object.
     *
     * Tasks must be provided with a non-null parent task and a title.
     * Constructing a task will assign a globally unique id to the task.
     *
     * \param parent The Task this will be a child of.
     * \param title The title of the task.
     *
     * \throws chaos::ex::ValueError If the ``parent`` parameter is null, or the
     *                               ``title`` parameter is empty.
     */
    Task(Task* parent, const chaos::uni::UTF8String& title);

    /*!
     * \brief Copy constructor.
     *
     * Creates a new Task by copying values from the other given Task.
     * A RootTask cannot be copied from.
     *
     * \note This task will be assigned a new id rather than having its id
     *       copied from the given task. Likewise this task will be initialised
     *       with no children as they will not be copied from the other Task.
     *
     * \param other Task to copy from.
     *
     * \throws chaos::ex::ValueError If the ``other`` parameter is a RootTask.
     */
    Task(const Task& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Task()
    {
        clean_up();
    }

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    // tasks cannot be assigned
    Task& operator=(const Task& other) = delete;

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the globally unique identifier of this task.
     *
     * This value cannot be modified.
     */
    chaos::uint32 get_id() const;

    /*!
     * \brief Returns whether this is a RootTask or not
     */
    virtual bool is_root() const;

    /*!
     * \brief Returns the parent Task of this Task.
     *
     * All Tasks have parent Tasks apart from a RootTask.
     */
    Task* const get_parent() const;

    /*!
     * \brief Sets the parent Task of this Task.
     *
     * \throws chaos::ex::ValueError If ``parent`` is null. // TODO: REMOVE ME
     * \throws chaos::ex::IllegalActionError If the given parent is already a
     *                                       descendant of this Task.
     */
    virtual void set_parent(Task* const parent);

    /*!
     * \brief Returns the number of Tasks that have this Task as their parent.
     */
    std::size_t get_children_count() const;

    /*!
     * \brief Returns the Tasks that have this Task as their parent.
     */
    const std::vector<Task*>& get_chidren() const;

    /*!
     * \brief Returns whether this Task has the given Task as a child.
     */
    bool has_child(Task* const child) const;

    /*!
     * \brief Makes this the parent Task of the given Task.
     *
     * This will both update the parent attribute of the child Task and the
     * children attribute of this Task.
     *
     * \note If the given Task is already a child of this Task this function
     *       will do nothing.
     *
     * \warning Tasks that are above this Task in the hierarchy cannot be added
     *          as child Tasks.
     *
     * \throws chaos::ex::IllegalActionError If the Task to add as a child is
     *                                       above this Task in the hierarchy
     *                                       (i.e. it's a parent, grand-parent,
     *                                       etc).
     *
     * \return Whether this function has made any changes.
     */
    bool add_child(Task* const child);

    /*!
     * \brief Unparents the given Task from this Task.
     *
     * If the given Task is a child of this Task then it will be removed from
     * this Task's list of children. If the given Task is not a child of this
     * Task this function will have no effect.
     *
     * \warning Removing a child will result in the deletion of the child Task
     *          as it becomes unparented which qualifies for deletion.
     *
     * \return Whether this function has made any changes.
     */
    bool remove_child(Task* const child);

    /*!
     * \brief Unparents all child Tasks of this Task.
     *
     * This function will result in this Task having no children.
     *
     * \warning The child Tasks will be deleted by this function since the
     *          Tasks become unparented which qualifies for deletion.
     */
    void clear_children();

    /*!
     * \brief Returns the title string of this Task.
     */
    const chaos::uni::UTF8String& get_title() const;

    /*!
     * \brief Sets the title string of this Task.
     */
    virtual void set_title(const chaos::uni::UTF8String& title);

    //--------------------------------------------------------------------------
    //                              CALLBACK EVENTS
    //--------------------------------------------------------------------------

    //----------------------------------GLOBAL----------------------------------

    /*!
     * \brief For registering callbacks that handle when a new Task is created.
     *
     * Relevant callback functions take one argument:
     * - ``Task*`` - the Task that has just been created.
     */
    static sigma::core::CallbackInterface<Task*>* on_created()
    {
        return &s_created_callback.get_interface();
    }

    /*!
     * \brief For registering callbacks that handle when a Task is destroyed.
     *
     * Relevant callback functions take one argument:
     * - ``Task*`` - the Task that is about to be destroyed.
     *
     * \warning Exceptions should be avoided in registered callback functions
     *          since these functions will be called from within a Task's
     *          destructor.
     */
    static sigma::core::CallbackInterface<Task*>* on_destroyed()
    {
        return &s_destroyed_callback.get_interface();
    }

    //----------------------------------LOCAL-----------------------------------

    /*!
     * \brief For registering callbacks that handle when a Task has its parent
     *        attribute changed.
     *
     * Relevant callback functions take three arguments:
     * - ``Task*`` - The Task thats parent has changed.
     * - ``Task*`` - The previous parent of the Task.
     * - ``Task*`` - The new parent of the Task.
     *
     * \note If the new parent is null this Task is about to be destroyed.
     */
    sigma::core::CallbackInterface<Task*, Task*, Task*>* on_parent_changed()
    {
        return &m_parent_changed_callback.get_interface();
    }

    /*!
     * \brief For registering callbacks that handle when a Task has its title
     *        attribute changed
     *
     * Relevant callback functions take three arguments:
     * - ``Task*`` - the Task thats title has changed.
     * - ``const chaos::uni::UTF8String&`` - the previous title of the Task.
     * - ``const chaos::uni::UTF8String&`` - the new title of the Task.
     */
    sigma::core::CallbackInterface<
            Task*,
            const chaos::uni::UTF8String&,
            const chaos::uni::UTF8String&>*
    on_title_changed()
    {
        return &m_title_changed_callback.get_interface();
    }

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Root Constructor.
     *
     * Protected constructor which should only be used by RootTasks. This
     * creates a Task which has a null parent.
     *
     * \param title The title of the task.
     */
    Task(const chaos::uni::UTF8String& title);

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The title of this task.
     */
    chaos::uni::UTF8String m_title;

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE STATIC VARIABLES
    //--------------------------------------------------------------------------

    // global id counter
    static chaos::uint32 s_id;

    // global callback handlers
    static sigma::core::CallbackHandler<Task*> s_created_callback;
    static sigma::core::CallbackHandler<Task*> s_destroyed_callback;

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The globally unique identifier of this task.
     */
    chaos::uint32 m_id;

    /*!
     * \brief TODO:
     */
    Task* m_parent;
    /*!
     * \brief TODO:
     */
    std::vector<Task*> m_children;


    // TODO: brief

    // TODO: full description? with images etc???

    // TODO: comments

    // TODO: links

    // local callback handlers
    sigma::core::CallbackHandler<
            Task*,
            const chaos::uni::UTF8String&,
            const chaos::uni::UTF8String&> m_title_changed_callback;
    sigma::core::CallbackHandler<Task*, Task*, Task*> m_parent_changed_callback;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Internal function that sets this Task's parent but does not fire
     *        a callback.
     *
     * This function handles state checking and children assignment of the
     * parent task.
     */
    void set_parent_internal(Task* const parent);

    /*!
     * \brief Internal function that sets this Task's title but does not fire a
     *         callback.
     *
     * This function handles state checking.
     */
    void set_title_internal(const chaos::uni::UTF8String& title);

    /*!
     * \brief Checks whether this task has the given task as a child.
     *
     * This function will be called recursively on any child tasks.
     */
    bool has_descendant(Task* const descendant) const;

    /*!
     * \brief The deletion routine.
     *
     * Deletes all of the Task's children and removes this Task from its
     * parents list of children.
     */
    void clean_up();

};

} // namespace tasks
} // namespace core
} // namespace sigma

#endif
