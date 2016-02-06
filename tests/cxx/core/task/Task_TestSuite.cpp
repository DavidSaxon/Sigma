#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(core.tasks.task)

#include "sigma/core/Sigma.hpp"
#include "sigma/core/tasks/Task.hpp"

namespace core_tasks_task_tests
{

CHAOS_TEST_UNIT(proto)
{
    std::cout << "Test me" << std::endl;

    sigma::core::init();
}

} // namespace core_tasks_task_tests
