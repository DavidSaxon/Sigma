#include <chaoscore/test/ChaosTest.hpp>

CHAOS_TEST_MODULE(core.tasks.TaskDomain)

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

class TaskDomainBaseFixture : public chaos::test::Fixture
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
        CHAOS_FOR_EACH(it, boards)
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
            const chaos::uni::UTF8String& title)
    {
        CHAOS_FOR_EACH(it, boards)
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

CHAOS_TEST_UNIT_FIXTURE(boards, TaskDomainBaseFixture)
{
    // get a reference to the boards
    const std::set<std::unique_ptr<sigma::core::tasks::RootTask>>& boards(
        sigma::core::tasks::domain::get_boards());

    CHAOS_TEST_MESSAGE("Checking number of boards is 0");
    CHAOS_CHECK_EQUAL(boards.size(), 0);

    // add a board
    sigma::core::tasks::RootTask* board_1 =
        sigma::core::tasks::domain::new_board("First");

    CHAOS_TEST_MESSAGE("Checking that returned boards are root tasks");
    CHAOS_CHECK_TRUE(board_1->is_root());

    CHAOS_TEST_MESSAGE("Checking number of boards is 1");
    CHAOS_CHECK_EQUAL(boards.size(), 1);
    CHAOS_CHECK_TRUE((fixture->has_board(boards, board_1)));

    // add another board
    sigma::core::tasks::RootTask* board_2 =
        sigma::core::tasks::domain::new_board("Second");

    CHAOS_TEST_MESSAGE(
        "Checking that RootTasks cannot have their parent's set");
    CHAOS_CHECK_THROW(
        board_1->set_parent(nullptr),
        chaos::ex::IllegalActionError
    );
    CHAOS_CHECK_THROW(
        board_1->set_parent(board_2),
        chaos::ex::IllegalActionError
    );
    CHAOS_CHECK_THROW(
        board_1->set_parent(board_1),
        chaos::ex::IllegalActionError
    );

    CHAOS_TEST_MESSAGE("Checking number of boards is 2");
    CHAOS_CHECK_EQUAL(boards.size(), 2);
    CHAOS_CHECK_TRUE((fixture->has_board(boards, board_2)));

    CHAOS_TEST_MESSAGE("Checking title mangling on new board");
    sigma::core::tasks::RootTask* board_3 =
        sigma::core::tasks::domain::new_board("First");
    CHAOS_CHECK_EQUAL(board_3->get_title(), "First (1)");
    sigma::core::tasks::RootTask* board_4 =
        sigma::core::tasks::domain::new_board("First");
    CHAOS_CHECK_EQUAL(board_4->get_title(), "First (2)");
    sigma::core::tasks::RootTask* board_5 =
        sigma::core::tasks::domain::new_board("First (1)");
    CHAOS_CHECK_EQUAL(board_5->get_title(), "First (1) (1)");

    CHAOS_TEST_MESSAGE("Checking title mangling on title change");
    board_3->set_title("Second");
    CHAOS_CHECK_EQUAL(board_3->get_title(), "Second (1)");
    board_5->set_title("First");
    CHAOS_CHECK_EQUAL(board_5->get_title(), "First (1)");
    board_2->set_title("First");
    CHAOS_CHECK_EQUAL(board_2->get_title(), "First (3)");
    board_1->set_title("First");
    CHAOS_CHECK_EQUAL(board_1->get_title(), "First");
    board_1->set_title("Second");
    CHAOS_CHECK_EQUAL(board_1->get_title(), "Second");

    CHAOS_TEST_MESSAGE("Checking deleting boards");
    CHAOS_CHECK_TRUE(sigma::core::tasks::domain::delete_board(board_1));
    CHAOS_CHECK_EQUAL(boards.size(), 4);
    CHAOS_CHECK_FALSE(fixture->has_board_with_title(boards, "Second"));
    CHAOS_CHECK_TRUE(sigma::core::tasks::domain::delete_board(board_5));
    CHAOS_CHECK_EQUAL(boards.size(), 3);
    CHAOS_CHECK_FALSE(fixture->has_board_with_title(boards, "First (1)"));
    CHAOS_CHECK_TRUE(sigma::core::tasks::domain::delete_board(board_3));
    CHAOS_CHECK_EQUAL(boards.size(), 2);
    CHAOS_CHECK_FALSE(fixture->has_board_with_title(boards, "Second (1)"));
    CHAOS_CHECK_FALSE(sigma::core::tasks::domain::delete_board(board_1));
    CHAOS_CHECK_EQUAL(boards.size(), 2);
}

} // namespace anonymous
