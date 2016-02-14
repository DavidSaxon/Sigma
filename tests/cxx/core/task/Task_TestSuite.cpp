#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(core.tasks.task)

#include "sigma/core/Sigma.hpp"
#include "sigma/core/tasks/Task.hpp"

namespace core_tasks_task_tests
{

//------------------------------------------------------------------------------
//                               TITLE CONSTRUCTOR
//------------------------------------------------------------------------------

class TitleConstructorFixture : public chaos::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::ScopedCallback task_created_cb_id;
    bool callback_called;
    sigma::core::tasks::Task* callback_task;

    //--------------------------------FUNCTIONS---------------------------------

    void on_task_created(sigma::core::tasks::Task* task)
    {
        callback_called = true;
        callback_task = task;
    }

    virtual void setup()
    {
        // set state
        callback_called = false;
        callback_task = nullptr;

        // connect callback
        task_created_cb_id = sigma::core::tasks::Task::on_created()->
                register_member_function<
                        TitleConstructorFixture,
                        &TitleConstructorFixture::on_task_created
                >(this);
    }
};

CHAOS_TEST_UNIT_FIXTURE(title_constructor, TitleConstructorFixture)
{
    CHAOS_TEST_MESSAGE("Checking callback is uncalled");
    CHAOS_CHECK_FALSE(fixture->callback_called);
    CHAOS_CHECK_EQUAL(fixture->callback_task, nullptr);

    CHAOS_TEST_MESSAGE("Checking callback after construction");
    chaos::uni::UTF8String task_title("test_task");
    sigma::core::tasks::Task task(task_title);
    CHAOS_CHECK_TRUE(fixture->callback_called);
    CHAOS_CHECK_EQUAL(fixture->callback_task, &task);

    // create another child task
    chaos::uni::UTF8String child_task_title("child_task");
    sigma::core::tasks::Task child_task(child_task_title, &task);

    CHAOS_TEST_MESSAGE("Checking title");
    CHAOS_CHECK_EQUAL(task.get_title(), task_title);
    CHAOS_CHECK_EQUAL(child_task.get_title(), child_task_title);

    CHAOS_TEST_MESSAGE("Checking parent");
    CHAOS_CHECK_EQUAL(task.get_parent(), nullptr);
    CHAOS_CHECK_EQUAL(child_task.get_parent(), &task);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

class DestructorFixture : public chaos::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::ScopedCallback task_destroyed_cb_id;
    bool callback_called;
    sigma::core::tasks::Task* callback_task;

    //--------------------------------FUNCTIONS---------------------------------

    void reset()
    {
        callback_called = false;
        callback_task = nullptr;
    }

    void on_task_destroyed(sigma::core::tasks::Task* task)
    {
        callback_called = true;
        callback_task = task;
    }

    virtual void setup()
    {
        // set state
        reset();

        // connect callback
        task_destroyed_cb_id = sigma::core::tasks::Task::on_destroyed()->
                register_member_function<
                        DestructorFixture,
                        &DestructorFixture::on_task_destroyed
                >(this);
    }
};

CHAOS_TEST_UNIT_FIXTURE(destructor, DestructorFixture)
{
    sigma::core::tasks::Task* task_ptr = new sigma::core::tasks::Task("t_1");
    CHAOS_TEST_MESSAGE("Checking callback is uncalled");
    CHAOS_CHECK_FALSE(fixture->callback_called);
    CHAOS_CHECK_EQUAL(fixture->callback_task, nullptr);

    CHAOS_TEST_MESSAGE("Checking deleting task");
    delete task_ptr;
    CHAOS_CHECK_TRUE(fixture->callback_called);
    CHAOS_CHECK_EQUAL(fixture->callback_task, task_ptr);

    {
        fixture->reset();
        sigma::core::tasks::Task task("t_2");
        task_ptr = &task;
        CHAOS_TEST_MESSAGE("Checking callback is uncalled");
        CHAOS_CHECK_FALSE(fixture->callback_called);
        CHAOS_CHECK_EQUAL(fixture->callback_task, nullptr);
    }
    CHAOS_TEST_MESSAGE("Checking task going out of scope");
    CHAOS_CHECK_TRUE(fixture->callback_called);
    CHAOS_CHECK_EQUAL(fixture->callback_task, task_ptr);
}

//------------------------------------------------------------------------------
//                                   SET TITLE
//------------------------------------------------------------------------------

class SetTitleFixture : public chaos::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------


    sigma::core::tasks::Task* task;
    sigma::core::ScopedCallback title_changed_cb_id;
    bool callback_called;
    sigma::core::tasks::Task* callback_task;
    chaos::uni::UTF8String callback_title;

    //-------------------------------CONSTRUCTOR--------------------------------

    SetTitleFixture()
        :
        Fixture()
        // task   (new sigma::core::tasks::Task(""))
    {
        std::cout << "construct" << std::endl;
    }

    //--------------------------------FUNCTIONS---------------------------------

    void on_title_changed(
            sigma::core::tasks::Task* task,
            const chaos::uni::UTF8String& title)
    {
        callback_called = true;
        callback_task = task;
        callback_title = title;
    }

    virtual void setup()
    {
        task = new sigma::core::tasks::Task("");

        // set state
        callback_called = false;
        callback_task = nullptr;

        //

        // connect callback
        // title_changed_cb_id = task.on_title_changed()->
        //         register_member_function<
        //                 SetTitleFixture,
        //                 &SetTitleFixture::on_title_changed
        //         >(this);
    }
};

CHAOS_TEST_UNIT_FIXTURE(set_title, SetTitleFixture)
{
}

} // namespace core_tasks_task_tests
