#include <arcanecore/test/ArcTest.hpp>

ARC_TEST_MODULE(core.tasks.TaskDomain)

#include "sigma/core/tasks/RootTask.hpp"
#include "sigma/core/tasks/TasksDomain.hpp"

namespace
{

/*!
 * This test suite also covers tests for the RootTask object.
 */

//------------------------------------------------------------------------------
//                                  BASE FIXTURE
//------------------------------------------------------------------------------

class TaskDomainBaseFixture : public arc::test::Fixture
{
public:

    //--------------------------------FUNCTIONS---------------------------------

    void setup()
    {
        sigma::core::tasks::domain::init();
    }

    virtual void teardown()
    {
        sigma::core::tasks::domain::clean_up();
    }

    bool has_board(
            const std::set<std::unique_ptr<
                sigma::core::tasks::RootTask>>& boards,
            sigma::core::tasks::RootTask* board)
    {
        ARC_FOR_EACH(it, boards)
        {
            if (it->get() == board)
            {
                return true;
            }
        }
        return false;
    }

    bool has_board_with_title(
            const std::set<std::unique_ptr<
                sigma::core::tasks::RootTask>>& boards,
            const arc::str::UTF8String& title)
    {
        ARC_FOR_EACH(it, boards)
        {
            if ((*it)->get_title() == title)
            {
                return true;
            }
        }
        return false;
    }
};

//------------------------------------------------------------------------------
//                                     BOARDS
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(boards, TaskDomainBaseFixture)
{
    // get a reference to the boards
    const std::set<std::unique_ptr<sigma::core::tasks::RootTask>>& boards(
        sigma::core::tasks::domain::get_boards());

    ARC_TEST_MESSAGE("Checking number of boards is 0");
    ARC_CHECK_EQUAL(boards.size(), 0);

    // add a board
    sigma::core::tasks::RootTask* board_1 =
        sigma::core::tasks::domain::new_board("First");

    ARC_TEST_MESSAGE("Checking that returned boards are root tasks");
    ARC_CHECK_TRUE(board_1->is_root());

    ARC_TEST_MESSAGE("Checking number of boards is 1");
    ARC_CHECK_EQUAL(boards.size(), 1);
    ARC_CHECK_TRUE((fixture->has_board(boards, board_1)));

    // add another board
    sigma::core::tasks::RootTask* board_2 =
        sigma::core::tasks::domain::new_board("Second");

    ARC_TEST_MESSAGE(
        "Checking that RootTasks cannot have their parent's set");
    ARC_CHECK_THROW(
        board_1->set_parent(nullptr),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        board_1->set_parent(board_2),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        board_1->set_parent(board_1),
        arc::ex::IllegalActionError
    );

    ARC_TEST_MESSAGE("Checking number of boards is 2");
    ARC_CHECK_EQUAL(boards.size(), 2);
    ARC_CHECK_TRUE((fixture->has_board(boards, board_2)));

    ARC_TEST_MESSAGE("Checking ids");
    ARC_CHECK_EQUAL(board_1->get_id(), 1);
    ARC_CHECK_EQUAL(board_2->get_id(), 2);

    ARC_TEST_MESSAGE("Checking error on board with empty title");
    ARC_CHECK_THROW(
        sigma::core::tasks::domain::new_board(""),
        arc::ex::ValueError
    );
    ARC_TEST_MESSAGE("Checking id hasn't been incremented");
    sigma::core::tasks::Task* id_checker_1 =
        new sigma::core::tasks::Task(board_1, "id_checker_1");
    ARC_CHECK_EQUAL(id_checker_1->get_id(), 3);

    ARC_TEST_MESSAGE("Checking title mangling on new board");
    sigma::core::tasks::RootTask* board_3 =
        sigma::core::tasks::domain::new_board("First");
    ARC_CHECK_EQUAL(board_3->get_title(), "First (1)");
    sigma::core::tasks::RootTask* board_4 =
        sigma::core::tasks::domain::new_board("First");
    ARC_CHECK_EQUAL(board_4->get_title(), "First (2)");
    sigma::core::tasks::RootTask* board_5 =
        sigma::core::tasks::domain::new_board("First (1)");
    ARC_CHECK_EQUAL(board_5->get_title(), "First (1) (1)");

    ARC_TEST_MESSAGE("Checking title mangling on title change");
    board_3->set_title("Second");
    ARC_CHECK_EQUAL(board_3->get_title(), "Second (1)");
    board_5->set_title("First");
    ARC_CHECK_EQUAL(board_5->get_title(), "First (1)");
    board_2->set_title("First");
    ARC_CHECK_EQUAL(board_2->get_title(), "First (3)");
    board_1->set_title("First");
    ARC_CHECK_EQUAL(board_1->get_title(), "First");
    board_1->set_title("Second");
    ARC_CHECK_EQUAL(board_1->get_title(), "Second");

    ARC_TEST_MESSAGE("Checking deleting boards");
    ARC_CHECK_TRUE(sigma::core::tasks::domain::delete_board(board_1));
    ARC_CHECK_EQUAL(boards.size(), 4);
    ARC_CHECK_FALSE(fixture->has_board_with_title(boards, "Second"));
    ARC_CHECK_TRUE(sigma::core::tasks::domain::delete_board(board_5));
    ARC_CHECK_EQUAL(boards.size(), 3);
    ARC_CHECK_FALSE(fixture->has_board_with_title(boards, "First (1)"));
    ARC_CHECK_TRUE(sigma::core::tasks::domain::delete_board(board_3));
    ARC_CHECK_EQUAL(boards.size(), 2);
    ARC_CHECK_FALSE(fixture->has_board_with_title(boards, "Second (1)"));
    ARC_CHECK_FALSE(sigma::core::tasks::domain::delete_board(board_1));
    ARC_CHECK_EQUAL(boards.size(), 2);
}

} // namespace anonymous
