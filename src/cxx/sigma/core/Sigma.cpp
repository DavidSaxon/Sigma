#include "sigma/core/Sigma.hpp"
#include "sigma/core/tasks/TasksDomain.hpp"

// TODO: REMOVE ME
#include <chlog/Logging.hpp>

namespace sigma
{
namespace core
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// void new_task_callback(sigma::core::tasks::Task* task)
// {
//     // do something
// }

void init(APIDomains api_domains)
{
    chlog::notice("Sigma init");

    // initialise requested domains
    if ( api_domains & API_TASKS )
    {
        sigma::core::tasks::domain::init();
    }

    // CallbackManager<sigma::core::tasks::Task*,

    // planning
    // call_back_id = register_new_task_callback();
}

} // namespace core
} // namespace sigma
