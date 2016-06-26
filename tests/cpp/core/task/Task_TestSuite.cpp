#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(core.tasks.Task)

#include "sigma/core/tasks/RootTask.hpp"
#include "sigma/core/tasks/TasksDomain.hpp"

namespace
{

//------------------------------------------------------------------------------
//                                  BASE FIXTURE
//------------------------------------------------------------------------------

class TaskBaseFixture : public arc::test::Fixture
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

ARC_TEST_UNIT_FIXTURE(constructor, ConstructorFixture)
{
    ARC_TEST_MESSAGE("Checking callback is uncalled");
    ARC_CHECK_EQUAL(fixture->callback_task, nullptr);

    ARC_TEST_MESSAGE("Checking callback after construction");
    arc::str::UTF8String task_1_title("task_1");
    sigma::core::tasks::Task* task_1 =
        new sigma::core::tasks::Task(fixture->board, task_1_title);
    ARC_CHECK_EQUAL(fixture->callback_task, task_1);

    arc::str::UTF8String task_2_title("task_2");
    sigma::core::tasks::Task* task_2 =
        new sigma::core::tasks::Task(task_1, task_2_title);
    ARC_CHECK_EQUAL(fixture->callback_task, task_2);

    ARC_TEST_MESSAGE("Checking Ids");
    ARC_CHECK_EQUAL(task_1->get_id(), 2);
    ARC_CHECK_EQUAL(task_2->get_id(), 3);

    ARC_TEST_MESSAGE("Checking parents");
    ARC_CHECK_EQUAL(task_1->get_parent(), fixture->board);
    ARC_CHECK_EQUAL(task_2->get_parent(), task_1);

    ARC_TEST_MESSAGE("Checking children");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->board->has_child(task_1));
    ARC_CHECK_EQUAL(task_1->get_children_count(), 1);
    ARC_CHECK_TRUE(task_1->has_child(task_2));
    ARC_CHECK_EQUAL(task_2->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking titles");
    ARC_CHECK_EQUAL(task_1->get_title(), task_1_title);
    ARC_CHECK_EQUAL(task_2->get_title(), task_2_title);

    ARC_TEST_MESSAGE("Checking is not root task");
    ARC_CHECK_FALSE(task_1->is_root());
    ARC_CHECK_FALSE(task_2->is_root());

    ARC_TEST_MESSAGE("Checking error on null parent");
    ARC_CHECK_THROW(
        new sigma::core::tasks::Task(nullptr, "null_parent_task"),
        arc::ex::ValueError
    );
    ARC_TEST_MESSAGE("Checking id hasn't been incremented");
    sigma::core::tasks::Task* id_checker_1 =
        new sigma::core::tasks::Task(fixture->board, "id_checker_1");
    ARC_CHECK_EQUAL(id_checker_1->get_id(), 4);

    ARC_TEST_MESSAGE("Checking error on empty title");
    ARC_CHECK_THROW(
        new sigma::core::tasks::Task(fixture->board, ""),
        arc::ex::ValueError
    );
    ARC_TEST_MESSAGE("Checking id hasn't been incremented");
    sigma::core::tasks::Task* id_checker_2 =
        new sigma::core::tasks::Task(fixture->board, "id_checker_2");
    ARC_CHECK_EQUAL(id_checker_2->get_id(), 5);
}

//------------------------------------------------------------------------------
//                                COPY CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(copy_constructor, ConstructorFixture)
{
    ARC_TEST_MESSAGE("Checking callback is uncalled");
    ARC_CHECK_EQUAL(fixture->callback_task, nullptr);

    // create a tasks to copy from
    arc::str::UTF8String task_1_title("task_1");
    sigma::core::tasks::Task* task_1 =
        new sigma::core::tasks::Task(fixture->board, task_1_title);
    arc::str::UTF8String task_2_title("task_2");
    sigma::core::tasks::Task* task_2 =
        new sigma::core::tasks::Task(task_1, task_2_title);

    ARC_TEST_MESSAGE("Checking callback is after copy construction");
    sigma::core::tasks::Task* task_3 =
        new sigma::core::tasks::Task(*task_1);
    ARC_CHECK_EQUAL(fixture->callback_task, task_3);
    sigma::core::tasks::Task* task_4 =
        new sigma::core::tasks::Task(*task_2);
    ARC_CHECK_EQUAL(fixture->callback_task, task_4);

    ARC_TEST_MESSAGE("Checking Ids");
    ARC_CHECK_EQUAL(task_3->get_id(), 4);
    ARC_CHECK_EQUAL(task_4->get_id(), 5);

    ARC_TEST_MESSAGE("Checking parents");
    ARC_CHECK_EQUAL(task_3->get_parent(), fixture->board);
    ARC_CHECK_EQUAL(task_4->get_parent(), task_1);

    ARC_TEST_MESSAGE("Checking children");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->board->has_child(task_1));
    ARC_CHECK_TRUE(fixture->board->has_child(task_3));
    ARC_CHECK_EQUAL(task_1->get_children_count(), 2);
    ARC_CHECK_TRUE(task_1->has_child(task_2));
    ARC_CHECK_TRUE(task_1->has_child(task_4));
    ARC_CHECK_EQUAL(task_2->get_children_count(), 0);
    ARC_CHECK_EQUAL(task_3->get_children_count(), 0);
    ARC_CHECK_EQUAL(task_4->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking titles");
    ARC_CHECK_EQUAL(task_3->get_title(), task_1_title);
    ARC_CHECK_EQUAL(task_4->get_title(), task_2_title);

    ARC_TEST_MESSAGE("Checking is not root task");
    ARC_CHECK_FALSE(task_3->is_root());
    ARC_CHECK_FALSE(task_4->is_root());

    ARC_TEST_MESSAGE("Checking error on copy from root");
    ARC_CHECK_THROW(
        new sigma::core::tasks::Task(*fixture->board),
        arc::ex::ValueError
    );
    ARC_TEST_MESSAGE("Checking id hasn't been incremented");
    sigma::core::tasks::Task* id_checker_1 =
        new sigma::core::tasks::Task(fixture->board, "id_checker_1");
    ARC_CHECK_EQUAL(id_checker_1->get_id(), 6);
}

//------------------------------------------------------------------------------
//                                   SET PARENT
//------------------------------------------------------------------------------

class SetParentFixture : public TaskBaseFixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::tasks::Task* callback_task;
    sigma::core::tasks::Task* callback_old;
    sigma::core::tasks::Task* callback_new;

    sigma::core::tasks::Task* destroyed_task;
    sigma::core::tasks::Task* prev_destroyed_task;

    sigma::core::tasks::Task* task_1;
    sigma::core::tasks::Task* task_2;
    sigma::core::tasks::Task* task_3;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        // super call
        TaskBaseFixture::setup();

        // set state
        callback_task = nullptr;
        callback_old  = nullptr;
        callback_new  = nullptr;
        destroyed_task = nullptr;
        prev_destroyed_task = nullptr;

        task_1 = new sigma::core::tasks::Task(board, "task_1");
        task_2 = new sigma::core::tasks::Task(board, "task_2");
        task_3 = new sigma::core::tasks::Task(task_1, "task_3");

        // connect callbacks
        task_1->on_parent_changed()->
                register_member_function<
                        SetParentFixture,
                        &SetParentFixture::on_parent_changed
                >(this);
        task_2->on_parent_changed()->
                register_member_function<
                        SetParentFixture,
                        &SetParentFixture::on_parent_changed
                >(this);
        task_3->on_parent_changed()->
                register_member_function<
                        SetParentFixture,
                        &SetParentFixture::on_parent_changed
                >(this);

        sigma::core::tasks::Task::on_destroyed()->
                register_member_function<
                        SetParentFixture,
                        &SetParentFixture::on_task_destroyed
                >(this);
    }

    void on_parent_changed(
            sigma::core::tasks::Task* task,
            sigma::core::tasks::Task* old_parent,
            sigma::core::tasks::Task* new_parent)
    {
        callback_task = task;
        callback_old = old_parent;
        callback_new = new_parent;
    }

    void on_task_destroyed(sigma::core::tasks::Task* task)
    {
        prev_destroyed_task = destroyed_task;
        destroyed_task = task;
    }
};

ARC_TEST_UNIT_FIXTURE(set_parent, SetParentFixture)
{
    ARC_TEST_MESSAGE("Checking initial states");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking callbacks are uncalled");
    ARC_CHECK_EQUAL(fixture->callback_task, nullptr);
    ARC_CHECK_EQUAL(fixture->destroyed_task, nullptr);

    ARC_TEST_MESSAGE("Checking case 1");
    fixture->task_2->set_parent(fixture->task_1);
    ARC_TEST_MESSAGE("Checking callback");
    ARC_CHECK_EQUAL(fixture->callback_task, fixture->task_2);
    ARC_CHECK_EQUAL(fixture->callback_old, fixture->board);
    ARC_CHECK_EQUAL(fixture->callback_new, fixture->task_1);
    ARC_TEST_MESSAGE("Checking hierarchy");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_FALSE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking case 2");
    fixture->task_3->set_parent(fixture->task_2);
    ARC_TEST_MESSAGE("Checking callback");
    ARC_CHECK_EQUAL(fixture->callback_task, fixture->task_3);
    ARC_CHECK_EQUAL(fixture->callback_old, fixture->task_1);
    ARC_CHECK_EQUAL(fixture->callback_new, fixture->task_2);
    ARC_TEST_MESSAGE("Checking hierarchy");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 1);
    ARC_CHECK_FALSE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->task_2->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking tasks can't be parented to descendants");
    ARC_CHECK_THROW(
        fixture->task_2->set_parent(fixture->task_3),
        arc::ex::IllegalActionError
    )
    ARC_CHECK_EQUAL(fixture->task_2->get_parent(), fixture->task_1);
    ARC_CHECK_THROW(
        fixture->task_1->set_parent(fixture->task_3),
        arc::ex::IllegalActionError
    )
    ARC_CHECK_EQUAL(fixture->task_1->get_parent(), fixture->board);

    ARC_TEST_MESSAGE("Checking setting parent to null deletes the task");
    fixture->task_3->set_parent(nullptr);
    ARC_CHECK_EQUAL(fixture->destroyed_task, fixture->task_3);
    ARC_CHECK_FALSE(fixture->task_2->has_child(fixture->task_3));

    fixture->task_1->set_parent(nullptr);
    ARC_CHECK_EQUAL(fixture->destroyed_task, fixture->task_1);
    ARC_CHECK_EQUAL(fixture->prev_destroyed_task, fixture->task_2);
    ARC_CHECK_FALSE(fixture->board->has_child(fixture->task_1));
}

//------------------------------------------------------------------------------
//                                   ADD CHILD
//------------------------------------------------------------------------------

class AddChildFixture : public TaskBaseFixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::tasks::Task* callback_task;
    sigma::core::tasks::Task* callback_old;
    sigma::core::tasks::Task* callback_new;

    sigma::core::tasks::Task* task_1;
    sigma::core::tasks::Task* task_2;
    sigma::core::tasks::Task* task_3;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        // super call
        TaskBaseFixture::setup();

        // set state
        callback_task = nullptr;
        callback_old  = nullptr;
        callback_new  = nullptr;

        task_1 = new sigma::core::tasks::Task(board, "task_1");
        task_2 = new sigma::core::tasks::Task(board, "task_2");
        task_3 = new sigma::core::tasks::Task(task_1, "task_3");

        // connect callbacks
        task_1->on_parent_changed()->
                register_member_function<
                        AddChildFixture,
                        &AddChildFixture::on_parent_changed
                >(this);
        task_2->on_parent_changed()->
                register_member_function<
                        AddChildFixture,
                        &AddChildFixture::on_parent_changed
                >(this);
        task_3->on_parent_changed()->
                register_member_function<
                        AddChildFixture,
                        &AddChildFixture::on_parent_changed
                >(this);
    }

    void on_parent_changed(
            sigma::core::tasks::Task* task,
            sigma::core::tasks::Task* old_parent,
            sigma::core::tasks::Task* new_parent)
    {
        callback_task = task;
        callback_old = old_parent;
        callback_new = new_parent;
    }
};

ARC_TEST_UNIT_FIXTURE(add_child, AddChildFixture)
{
    ARC_TEST_MESSAGE("Checking initial states");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking callback is uncalled");
    ARC_CHECK_EQUAL(fixture->callback_task, nullptr);

    ARC_TEST_MESSAGE("Checking case 1");
    ARC_CHECK_TRUE(fixture->task_1->add_child(fixture->task_2));
    ARC_TEST_MESSAGE("Checking callback");
    ARC_CHECK_EQUAL(fixture->callback_task, fixture->task_2);
    ARC_CHECK_EQUAL(fixture->callback_old, fixture->board);
    ARC_CHECK_EQUAL(fixture->callback_new, fixture->task_1);
    ARC_TEST_MESSAGE("Checking hierarchy");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_FALSE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking case 2");
    ARC_CHECK_TRUE(fixture->task_2->add_child(fixture->task_3));
    ARC_TEST_MESSAGE("Checking callback");
    ARC_CHECK_EQUAL(fixture->callback_task, fixture->task_3);
    ARC_CHECK_EQUAL(fixture->callback_old, fixture->task_1);
    ARC_CHECK_EQUAL(fixture->callback_new, fixture->task_2);
    ARC_TEST_MESSAGE("Checking hierarchy");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 1);
    ARC_CHECK_FALSE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->task_2->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking tasks can't be parented to descendants");
    ARC_CHECK_THROW(
        fixture->task_3->add_child(fixture->task_2),
        arc::ex::IllegalActionError
    )
    ARC_CHECK_EQUAL(fixture->task_2->get_parent(), fixture->task_1);
    ARC_CHECK_THROW(
        fixture->task_3->add_child(fixture->task_1),
        arc::ex::IllegalActionError
    )
    ARC_CHECK_EQUAL(fixture->task_1->get_parent(), fixture->board);

    // Checking that an existing child cannot be added
    ARC_CHECK_FALSE(fixture->board->add_child(fixture->task_1));
    ARC_CHECK_FALSE(fixture->task_1->add_child(fixture->task_2));
    ARC_CHECK_FALSE(fixture->task_2->add_child(fixture->task_3));
}

//------------------------------------------------------------------------------
//                                  REMOVE CHILD
//------------------------------------------------------------------------------

class RemoveChildFixture : public TaskBaseFixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::tasks::Task* destroyed_task;
    sigma::core::tasks::Task* prev_destroyed_task_1;
    sigma::core::tasks::Task* prev_destroyed_task_2;

    sigma::core::tasks::Task* task_1;
    sigma::core::tasks::Task* task_2;
    sigma::core::tasks::Task* task_3;
    sigma::core::tasks::Task* task_4;
    sigma::core::tasks::Task* task_5;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        // super call
        TaskBaseFixture::setup();

        // set state
        destroyed_task = nullptr;
        prev_destroyed_task_1 = nullptr;
        prev_destroyed_task_2 = nullptr;

        task_1 = new sigma::core::tasks::Task(board, "task_1");
        task_2 = new sigma::core::tasks::Task(board, "task_2");
        task_3 = new sigma::core::tasks::Task(task_1, "task_3");
        task_4 = new sigma::core::tasks::Task(task_3, "task_4");
        task_5 = new sigma::core::tasks::Task(task_3, "task_5");

        // connect callbacks
        sigma::core::tasks::Task::on_destroyed()->
                register_member_function<
                        RemoveChildFixture,
                        &RemoveChildFixture::on_task_destroyed
                >(this);
    }

    void on_task_destroyed(sigma::core::tasks::Task* task)
    {
        prev_destroyed_task_2 = prev_destroyed_task_1;
        prev_destroyed_task_1 = destroyed_task;
        destroyed_task = task;
    }
};

ARC_TEST_UNIT_FIXTURE(remove_child, RemoveChildFixture)
{
    ARC_TEST_MESSAGE("Checking initial states");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->task_3->has_child(fixture->task_4));
    ARC_CHECK_TRUE(fixture->task_3->has_child(fixture->task_5));
    ARC_CHECK_EQUAL(fixture->task_4->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_5->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking callback is uncalled");
    ARC_CHECK_EQUAL(fixture->destroyed_task, nullptr);

    ARC_TEST_MESSAGE("Checking removing Tasks that's aren't children");
    ARC_CHECK_FALSE(fixture->board->remove_child(fixture->task_3));
    ARC_CHECK_FALSE(fixture->task_1->remove_child(fixture->board));
    ARC_CHECK_FALSE(fixture->task_1->remove_child(fixture->task_2));
    ARC_CHECK_FALSE(fixture->task_1->remove_child(fixture->task_4));
    ARC_CHECK_FALSE(fixture->task_2->remove_child(fixture->task_3));
    ARC_CHECK_FALSE(fixture->task_2->remove_child(fixture->task_5));

    ARC_TEST_MESSAGE("Checking hierarchy has remained the same");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->task_3->has_child(fixture->task_4));
    ARC_CHECK_TRUE(fixture->task_3->has_child(fixture->task_5));
    ARC_CHECK_EQUAL(fixture->task_4->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_5->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking removing task_2 from task_1");
    ARC_CHECK_TRUE(fixture->board->remove_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->destroyed_task, fixture->task_2);
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_FALSE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->task_3->has_child(fixture->task_4));
    ARC_CHECK_TRUE(fixture->task_3->has_child(fixture->task_5));

    ARC_TEST_MESSAGE("Checking removing task_3 from task_1");
    ARC_CHECK_TRUE(fixture->task_1->remove_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->destroyed_task, fixture->task_3);
    ARC_CHECK_EQUAL(fixture->prev_destroyed_task_1, fixture->task_5);
    ARC_CHECK_EQUAL(fixture->prev_destroyed_task_2, fixture->task_4);
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 0);
    ARC_CHECK_FALSE(fixture->task_1->has_child(fixture->task_3));
}

class ClearChildrenFixture : public TaskBaseFixture
{
public:

    //--------------------------------ATTRIBUTES--------------------------------

    sigma::core::tasks::Task* destroyed_task;
    sigma::core::tasks::Task* prev_destroyed_task_1;
    sigma::core::tasks::Task* prev_destroyed_task_2;

    sigma::core::tasks::Task* task_1;
    sigma::core::tasks::Task* task_2;
    sigma::core::tasks::Task* task_3;
    sigma::core::tasks::Task* task_4;
    sigma::core::tasks::Task* task_5;

    //--------------------------------FUNCTIONS---------------------------------

    virtual void setup()
    {
        // super call
        TaskBaseFixture::setup();

        // set state
        destroyed_task = nullptr;
        prev_destroyed_task_1 = nullptr;
        prev_destroyed_task_2 = nullptr;

        task_1 = new sigma::core::tasks::Task(board, "task_1");
        task_2 = new sigma::core::tasks::Task(board, "task_2");
        task_3 = new sigma::core::tasks::Task(task_1, "task_3");
        task_4 = new sigma::core::tasks::Task(task_3, "task_4");
        task_5 = new sigma::core::tasks::Task(task_3, "task_5");

        // connect callbacks
        sigma::core::tasks::Task::on_destroyed()->
                register_member_function<
                        ClearChildrenFixture,
                        &ClearChildrenFixture::on_task_destroyed
                >(this);
    }

    void on_task_destroyed(sigma::core::tasks::Task* task)
    {
        prev_destroyed_task_2 = prev_destroyed_task_1;
        prev_destroyed_task_1 = destroyed_task;
        destroyed_task = task;
    }
};

ARC_TEST_UNIT_FIXTURE(clear_children, ClearChildrenFixture)
{
    ARC_TEST_MESSAGE("Checking initial states");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 1);
    ARC_CHECK_TRUE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_3->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->task_3->has_child(fixture->task_4));
    ARC_CHECK_TRUE(fixture->task_3->has_child(fixture->task_5));
    ARC_CHECK_EQUAL(fixture->task_4->get_children_count(), 0);
    ARC_CHECK_EQUAL(fixture->task_5->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking callback is uncalled");
    ARC_CHECK_EQUAL(fixture->destroyed_task, nullptr);

    ARC_TEST_MESSAGE("Checking case 1");
    fixture->task_1->clear_children();
    ARC_TEST_MESSAGE("Checking callback");
    ARC_CHECK_EQUAL(fixture->prev_destroyed_task_2, fixture->task_4);
    ARC_CHECK_EQUAL(fixture->prev_destroyed_task_1, fixture->task_5);
    ARC_CHECK_EQUAL(fixture->destroyed_task, fixture->task_3);
    ARC_TEST_MESSAGE("Checking hierarchy");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 2);
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_TRUE(fixture->board->has_child(fixture->task_2));
    ARC_CHECK_EQUAL(fixture->task_1->get_children_count(), 0);
    ARC_CHECK_FALSE(fixture->task_1->has_child(fixture->task_3));
    ARC_CHECK_EQUAL(fixture->task_2->get_children_count(), 0);

    ARC_TEST_MESSAGE("Checking case 2");
    fixture->board->clear_children();
    ARC_TEST_MESSAGE("Checking callback");
    ARC_CHECK_EQUAL(fixture->prev_destroyed_task_1, fixture->task_1);
    ARC_CHECK_EQUAL(fixture->destroyed_task, fixture->task_2);
    ARC_TEST_MESSAGE("Checking hierarchy");
    ARC_CHECK_EQUAL(fixture->board->get_children_count(), 0);
    ARC_CHECK_FALSE(fixture->board->has_child(fixture->task_1));
    ARC_CHECK_FALSE(fixture->board->has_child(fixture->task_2));
}

} // namespace anonymous
