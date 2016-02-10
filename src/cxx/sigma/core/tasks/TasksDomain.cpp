#include "sigma/core/tasks/TasksDomain.hpp"

#include "sigma/core/tasks/Task.hpp"

// TODO: REMOVE ME
#include <iostream>

namespace sigma
{
namespace core
{
namespace tasks
{

// TODO: domains should be an object with inheritance(or something better)

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void task_created(sigma::core::tasks::Task* task)
{
    std::cout << "new task!" << std::endl;
}

void task_destroyed(sigma::core::tasks::Task* task)
{
    std::cout << "delete task!" << std::endl;
}

void init()
{
    // TODO: this seems bad...
    sigma::core::tasks::Task::on_created()->register_function(task_created);
    sigma::core::tasks::Task::on_destroyed()->register_function(task_destroyed);

    sigma::core::tasks::Task t("my_task");
}

} // namespace tasks
} // namespace core
} // namespace sigma
