#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(core.tasks.task)

#include "sigma/core/Sigma.hpp"
#include "sigma/core/tasks/Task.hpp"

namespace core_tasks_task_tests
{

//------------------------------------------------------------------------------
//                                   PROTO TEST
//------------------------------------------------------------------------------


class TitleConstructorFixture : public chaos::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::ScopedCallback task_created_cb_id;
    bool callback_called;

    //--------------------------------FUNCTIONS---------------------------------

    void on_task_created(sigma::core::tasks::Task* task)
    {

    }

    virtual void setup()
    {
        // set state
        callback_called = false;

        // connect callback
        // sigma::core::ScopedCallback member_callback(
        //         handler.get_interface().register_member_function<
        //                 NoParametersFixture,
        //                 &NoParametersFixture::member_func
        //         >(fixture)
        // );
    }
};

CHAOS_TEST_UNIT_FIXTURE(title_constructor, TitleConstructorFixture)
{
}

} // namespace core_tasks_task_tests
