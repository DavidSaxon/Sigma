#include "sigma/core/tasks/TasksDomain.hpp"

#include "sigma/core/tasks/RootTask.hpp"

namespace sigma
{
namespace core
{
namespace tasks
{
namespace domain
{

//------------------------------------------------------------------------------
//                                   VARIABLES
//------------------------------------------------------------------------------

namespace
{

 /*!
  * \brief The existing Task boards stored by their root nodes.
  */
std::set<std::unique_ptr<RootTask>> m_boards;

} // namespace anonymous

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \brief Resolves the given board title so that it is unique, by appending
 *        a number to the end of the title.
 *
 * \param original the board title to resolve.
 * \param resolved Returns title once it has been resolved, if there is no board
 *                 with the original title then this will return the original
 *                 title.
 */
void resolve_unique_board_title(
        const arc::str::UTF8String& original,
        arc::str::UTF8String& resolved);

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void init()
{
}

void clean_up()
{
    // delete all the current boards
    m_boards.clear();
}

const std::set<std::unique_ptr<RootTask>>& get_boards()
{
    return m_boards;
}

RootTask* new_board(const arc::str::UTF8String& title)
{
    // check the title is not empty
    if(title.is_empty())
    {
        throw arc::ex::ValueError("Task Boards cannot have a blank title");
    }

    // ensure we have a unique title
    arc::str::UTF8String resolved_title;
    resolve_unique_board_title(title, resolved_title);
    // create the Root Task
    std::unique_ptr<RootTask> root(
        new RootTask(resolved_title, resolve_unique_board_title));
    RootTask* r = root.get();
    // store
    m_boards.insert(std::move(root));
    // return pointer
    return r;
}

void resolve_unique_board_title(
        const arc::str::UTF8String& original,
        arc::str::UTF8String& resolved)
{
    resolved = original;

    // search over the current titles to find titles that match
    bool hard_match = false;
    std::set<arc::uint32> reserved_numbers;
    ARC_CONST_FOR_EACH(board_it, m_boards)
    {
        // check for an exact match
        if((*board_it)->get_title() == original)
        {
            hard_match = true;
        }
        // check for a match with a suffix
        else if((*board_it)->get_title().starts_with(original))
        {
            arc::str::UTF8String conflicting((*board_it)->get_title());

            // read the suffix
            std::size_t par_end_i   = conflicting.find_last(")");
            std::size_t par_begin_i = conflicting.find_last("(");
            if(par_end_i   != arc::str::npos &&
                par_begin_i != arc::str::npos &&
                par_begin_i < par_end_i         &&
                par_begin_i - 1 == original.get_length() )
            {
                arc::str::UTF8String par_contents(conflicting.substring(
                    par_begin_i + 1,
                    par_end_i - (par_begin_i + 1)
                ));
                // is this a number?
                if(par_contents.is_uint())
                {
                    reserved_numbers.insert(par_contents.to_uint32());
                }
            }
        }
    }

    // has there are conflict?
    if(hard_match)
    {
        // find a unique suffix
        arc::uint32 s = 1;
        for(; reserved_numbers.find(s) != reserved_numbers.end(); ++s);
        // append the unique suffix
        resolved << " (" << s << ")";
    }
    // // search over current boards and check their titles
    // ARC_CONST_FOR_EACH(board_it, m_boards)
    // {
    //     if((*board_it)->get_title() == original)
    //     {
    //         arc::str::UTF8String conflicting((*board_it)->get_title());

    //         // the number that will be used to make this title unique
    //         arc::uint32 num = 1;

    //         // match found, check if this title already has a unique number
    //         std::size_t par_end_i = conflicting.find_last(")");
    //         if(par_end_i != arc::str::npos)
    //         {
    //             std::size_t par_begin_i = conflicting.find_last("(");
    //             if( par_begin_i != arc::str::npos)
    //             {
    //                 // get the contents of the parentheses
    //                 arc::str::UTF8String par_contents(conflicting.substring(
    //                     par_begin_i + 1,
    //                     par_end_i - (par_begin_i + 1)
    //                 ));
    //                 // is this a number
    //                 if(par_contents.is_uint())
    //                 {
    //                     num = par_contents.to_uint32() + 1;
    //                     // FUCK
    //                     // strip the parentheses from the title
    //                     // resolved = original.substring(0, par_begin_i - 1);
    //                     // std::cout << "stripped: " << resolved << std::endl;
    //                 }
    //             }
    //         }

    //         arc::str::UTF8String modified(resolved);
    //         modified << " (" << num << ")";
    //         // check the new title
    //         resolve_unique_board_title(modified, resolved);
    //         return;
    //     }
    // }
}

bool delete_board(RootTask* board_root)
{
    std::set<std::unique_ptr<RootTask>>::iterator it = m_boards.begin();
    for (; it != m_boards.end();)
    {
        if(it->get() == board_root)
        {
            it = m_boards.erase(it);
            return true;
        }
        else
        {
            ++it;
        }
    }
    return false;
}

} // namespace domain
} // namespace tasks
} // namespace core
} // namespace sigma
