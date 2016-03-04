#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(core.tasks.task)

#include "sigma/core/tasks/RootTask.hpp"
#include "sigma/core/tasks/TasksDomain.hpp"

namespace
{

//------------------------------------------------------------------------------
//                                  BASE FIXTURE
//------------------------------------------------------------------------------

class TaskBaseFixture : public chaos::test::Fixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::tasks::RootTask* board;

    //--------------------------------FUNCTIONS---------------------------------

    void setup()
    {
        sigma::core::tasks::domain::init();

        board = sigma::core::tasks::domain::new_board("root");
    }

    virtual void teardown()
    {
        sigma::core::tasks::domain::clean_up();
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

class ConstructorFixture : public TaskBaseFixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::ScopedCallback task_created_cb_id;
    sigma::core::tasks::Task* callback_task;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        // super call
        TaskBaseFixture::setup();

        // set state
        callback_task = nullptr;

        // connect callback
        task_created_cb_id = sigma::core::tasks::Task::on_created()->
                register_member_function<
                        ConstructorFixture,
                        &ConstructorFixture::on_task_created
                >(this);
    }

    void on_task_created(sigma::core::tasks::Task* task)
    {
        callback_task = task;
    }
};

CHAOS_TEST_UNIT_FIXTURE(constructor, ConstructorFixture)
{
    CHAOS_TEST_MESSAGE("Checking callback is uncalled");
    CHAOS_CHECK_EQUAL(fixture->callback_task, nullptr);

    CHAOS_TEST_MESSAGE("Checking callback after construction");
    chaos::uni::UTF8String task_1_title("task_1");
    sigma::core::tasks::Task* task_1 =
        new sigma::core::tasks::Task(fixture->board, task_1_title);
    CHAOS_CHECK_EQUAL(fixture->callback_task, task_1);

    chaos::uni::UTF8String task_2_title("task_2");
    sigma::core::tasks::Task* task_2 =
        new sigma::core::tasks::Task(task_1, task_2_title);
    CHAOS_CHECK_EQUAL(fixture->callback_task, task_2);

    CHAOS_TEST_MESSAGE("Checking Ids");
    CHAOS_CHECK_EQUAL(task_1->get_id(), 2);
    CHAOS_CHECK_EQUAL(task_2->get_id(), 3);

    CHAOS_TEST_MESSAGE("Checking parents");
    CHAOS_CHECK_EQUAL(task_1->get_parent(), fixture->board);
    CHAOS_CHECK_EQUAL(task_2->get_parent(), task_1);

    CHAOS_TEST_MESSAGE("Checking titles");
    CHAOS_CHECK_EQUAL(task_1->get_title(), task_1_title);
    CHAOS_CHECK_EQUAL(task_2->get_title(), task_2_title);

    CHAOS_TEST_MESSAGE("Checking error on null parent");
    CHAOS_CHECK_THROW(
        new sigma::core::tasks::Task(nullptr, "null_parent_task"),
        chaos::ex::ValueError
    );
    CHAOS_TEST_MESSAGE("Checking id hasn't been incremented");
    sigma::core::tasks::Task* id_checker_1 =
        new sigma::core::tasks::Task(fixture->board, "id_checker_1");
    CHAOS_CHECK_EQUAL(id_checker_1->get_id(), 4);

    CHAOS_TEST_MESSAGE("Checking error on empty title");
    CHAOS_CHECK_THROW(
        new sigma::core::tasks::Task(fixture->board, ""),
        chaos::ex::ValueError
    );
    CHAOS_TEST_MESSAGE("Checking id hasn't been incremented");
    sigma::core::tasks::Task* id_checker_2 =
        new sigma::core::tasks::Task(fixture->board, "id_checker_2");
    CHAOS_CHECK_EQUAL(id_checker_2->get_id(), 5);
}

} // namespace anonymous

namespace core_tasks_task_tests
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

// CHAOS_TEST_UNIT(construtor)
// {

// }

// //------------------------------------------------------------------------------
// //                               TITLE CONSTRUCTOR
// //------------------------------------------------------------------------------

// class TitleConstructorFixture : public chaos::test::Fixture
// {
// public:

//     //--------------------------------ATTRIBUTES--------------------------------

//     sigma::core::ScopedCallback task_created_cb_id;
//     bool callback_called;
//     sigma::core::tasks::Task* callback_task;

//     //--------------------------------FUNCTIONS---------------------------------

//     void on_task_created(sigma::core::tasks::Task* task)
//     {
//         callback_called = true;
//         callback_task = task;
//     }

//     virtual void setup()
//     {
//         // set state
//         callback_called = false;
//         callback_task = nullptr;

//         // connect callback
//         task_created_cb_id = sigma::core::tasks::Task::on_created()->
//                 register_member_function<
//                         TitleConstructorFixture,
//                         &TitleConstructorFixture::on_task_created
//                 >(this);
//     }
// };

// CHAOS_TEST_UNIT_FIXTURE(title_constructor, TitleConstructorFixture)
// {
//     CHAOS_TEST_MESSAGE("Checking callback is uncalled");
//     CHAOS_CHECK_FALSE(fixture->callback_called);
//     CHAOS_CHECK_EQUAL(fixture->callback_task, nullptr);

//     CHAOS_TEST_MESSAGE("Checking callback after construction");
//     chaos::uni::UTF8String task_title("test_task");
//     sigma::core::tasks::Task task(task_title);
//     CHAOS_CHECK_TRUE(fixture->callback_called);
//     CHAOS_CHECK_EQUAL(fixture->callback_task, &task);

//     // create another child task
//     chaos::uni::UTF8String child_task_title("child_task");
//     sigma::core::tasks::Task child_task(child_task_title, &task);

//     CHAOS_TEST_MESSAGE("Checking title");
//     CHAOS_CHECK_EQUAL(task.get_title(), task_title);
//     CHAOS_CHECK_EQUAL(child_task.get_title(), child_task_title);

//     CHAOS_TEST_MESSAGE("Checking parent");
//     CHAOS_CHECK_EQUAL(task.get_parent(), nullptr);
//     CHAOS_CHECK_EQUAL(child_task.get_parent(), &task);

//     // check the task can't be defined with a null parent
//     // TODO:


// }

// //------------------------------------------------------------------------------
// //                                   DESTRUCTOR
// //------------------------------------------------------------------------------

// class DestructorFixture : public chaos::test::Fixture
// {
// public:

//     //--------------------------------ATTRIBUTES--------------------------------

//     sigma::core::ScopedCallback task_destroyed_cb_id;
//     bool callback_called;
//     sigma::core::tasks::Task* callback_task;

//     //--------------------------------FUNCTIONS---------------------------------

//     void reset()
//     {
//         callback_called = false;
//         callback_task = nullptr;
//     }

//     void on_task_destroyed(sigma::core::tasks::Task* task)
//     {
//         callback_called = true;
//         callback_task = task;
//     }

//     virtual void setup()
//     {
//         // set state
//         reset();

//         // connect callback
//         task_destroyed_cb_id = sigma::core::tasks::Task::on_destroyed()->
//                 register_member_function<
//                         DestructorFixture,
//                         &DestructorFixture::on_task_destroyed
//                 >(this);
//     }
// };

// CHAOS_TEST_UNIT_FIXTURE(destructor, DestructorFixture)
// {
//     sigma::core::tasks::Task* task_ptr = new sigma::core::tasks::Task("t_1");
//     CHAOS_TEST_MESSAGE("Checking callback is uncalled");
//     CHAOS_CHECK_FALSE(fixture->callback_called);
//     CHAOS_CHECK_EQUAL(fixture->callback_task, nullptr);

//     CHAOS_TEST_MESSAGE("Checking deleting task");
//     delete task_ptr;
//     CHAOS_CHECK_TRUE(fixture->callback_called);
//     CHAOS_CHECK_EQUAL(fixture->callback_task, task_ptr);

//     {
//         fixture->reset();
//         sigma::core::tasks::Task task("t_2");
//         task_ptr = &task;
//         CHAOS_TEST_MESSAGE("Checking callback is uncalled");
//         CHAOS_CHECK_FALSE(fixture->callback_called);
//         CHAOS_CHECK_EQUAL(fixture->callback_task, nullptr);
//     }
//     CHAOS_TEST_MESSAGE("Checking task going out of scope");
//     CHAOS_CHECK_TRUE(fixture->callback_called);
//     CHAOS_CHECK_EQUAL(fixture->callback_task, task_ptr);
// }

// //------------------------------------------------------------------------------
// //                                   SET TITLE
// //------------------------------------------------------------------------------

// class SetTitleFixture : public chaos::test::Fixture
// {
// public:

//     //--------------------------------ATTRIBUTES--------------------------------


//     chaos::uni::UTF8String initial_title;
//     sigma::core::tasks::Task task;
//     sigma::core::ScopedCallback title_changed_cb_id;
//     bool callback_called;
//     sigma::core::tasks::Task* callback_task;
//     chaos::uni::UTF8String callback_title;

//     //-------------------------------CONSTRUCTOR--------------------------------

//     SetTitleFixture()
//         :
//         Fixture      (),
//         initial_title("initial_title"),
//         task         (initial_title)
//     {
//     }

//     //--------------------------------FUNCTIONS---------------------------------

//     void on_title_changed(
//             sigma::core::tasks::Task* task,
//             const chaos::uni::UTF8String& title)
//     {
//         callback_called = true;
//         callback_task = task;
//         callback_title = title;
//     }

//     virtual void setup()
//     {
//         // set state
//         callback_called = false;
//         callback_task = nullptr;

//         // connect callback
//         title_changed_cb_id = task.on_title_changed()->
//                 register_member_function<
//                         SetTitleFixture,
//                         &SetTitleFixture::on_title_changed
//                 >(this);
//     }
// };

// CHAOS_TEST_UNIT_FIXTURE(set_title, SetTitleFixture)
// {
//     CHAOS_TEST_MESSAGE("Checking initial title");
//     CHAOS_CHECK_EQUAL(fixture->task.get_title(), fixture->initial_title);

//     CHAOS_TEST_MESSAGE("Checking callback is uncalled");
//     CHAOS_CHECK_FALSE(fixture->callback_called);

//     CHAOS_TEST_MESSAGE("Checking title after set");
//     chaos::uni::UTF8String new_title("new_title");
//     fixture->task.set_title("new_title");
//     CHAOS_CHECK_EQUAL(fixture->task.get_title(), new_title);

//     CHAOS_TEST_MESSAGE("Checking callback is called");
//     CHAOS_CHECK_TRUE(fixture->callback_called);
//     CHAOS_CHECK_EQUAL(fixture->callback_task, &fixture->task);
//     CHAOS_CHECK_EQUAL(fixture->callback_title, new_title);
// }

// //------------------------------------------------------------------------------
// //                                   SET PARENT
// //------------------------------------------------------------------------------

// class SetParentFixture : public chaos::test::Fixture
// {
// public:

//     //--------------------------------ATTRIBUTES--------------------------------

//     bool callback_called;
//     sigma::core::tasks::Task* this_task;
//     sigma::core::tasks::Task* new_parent;
//     sigma::core::tasks::Task* old_parent;

//     //--------------------------------FUNCTIONS---------------------------------

//     void reset()
//     {
//         callback_called = false;
//         this_task  = nullptr;
//         new_parent = nullptr;
//         old_parent = nullptr;
//     }

//     void on_parent_changed(
//             sigma::core::tasks::Task* a_this_task,
//             sigma::core::tasks::Task* a_new_parent,
//             sigma::core::tasks::Task* a_old_parent )
//     {
//         callback_called = false;
//         this_task  = a_this_task;
//         new_parent = a_new_parent;
//         old_parent = a_old_parent;
//     }

//     virtual void setup()
//     {
//         reset();

//         // // set state
//         // callback_called = false;
//         // callback_task = nullptr;

//         // // connect callback
//         // title_changed_cb_id = task.on_title_changed()->
//         //         register_member_function<
//         //                 SetParentFixture,
//         //                 &SetParentFixture::on_title_changed
//         //         >(this);
//     }
// };

// CHAOS_TEST_UNIT_FIXTURE(set_parent, SetParentFixture)
// {
//     // // create a parent and child
//     // {
//     // sigma::core::tasks::Task* parent_task =
//     //         new sigma::core::tasks::Task("parent_task");
//     // sigma::core::tasks::Task* child_task =
//     //         new sigma::core::tasks::Task("child_task");

//     // // hook up callback
//     // child_task->on_parent_changed()->register_member_function<
//     //         SetParentFixture,
//     //         &SetParentFixture::on_parent_changed
//     // >(fixture);

//     // CHAOS_TEST_MESSAGE("Checking callback uncalled");
//     // CHAOS_CHECK_FALSE(fixture->callback_called);
//     // CHAOS_CHECK_EQUAL(fixture->this_task, nullptr);
//     // CHAOS_CHECK_EQUAL(fixture->new_parent, nullptr);
//     // CHAOS_CHECK_EQUAL(fixture->old_parent, nullptr);

//     // // set parent
//     // child_task->set_parent(parent_task);

//     // CHAOS_TEST_MESSAGE("Checking parent");
//     // CHAOS_CHECK_EQUAL(child_task->get_parent(), parent_task);

//     // CHAOS_TEST_MESSAGE("Checking callback called");
//     // CHAOS_CHECK_TRUE(fixture->callback_called);
//     // CHAOS_CHECK_EQUAL(fixture->this_task, child_task);
//     // CHAOS_CHECK_EQUAL(fixture->new_parent, nullptr);
//     // CHAOS_CHECK_EQUAL(fixture->old_parent, parent_task);

//     // // clean up
//     // delete parent_task;
//     // delete child_task;
//     // }
// }

} // namespace core_tasks_task_tests
