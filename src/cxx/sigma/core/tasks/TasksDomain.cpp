#include "sigma/core/tasks/TasksDomain.hpp"

#include "sigma/core/callback/CallbackManager.hpp"
#include "sigma/core/tasks/Task.hpp"

// TODO: REMOVE ME
#include <iostream>

namespace sigma
{
namespace core
{
namespace tasks
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void callback_test(void* v, sigma::core::tasks::Task* t, int i)
{
    std::cout << "Hello World: " << i << std::endl;
}

void init()
{
    // sigma::core::CallbackManager<int, float, int> callback_manager;

    // sigma::core::CallbackManager<sigma::core::tasks::Task*, int>
            // callback_manager;
    // chaos::uint64 id = callback_manager.register_function(NULL, callback_test);
    // callback_manager.trigger(NULL, 12);
}

} // namespace tasks
} // namespace core
} // namespace sigma
