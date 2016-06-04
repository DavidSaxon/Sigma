/*!
 * \file
 * \author David Saxon
 * \brief Provides the outwards facing entry point for the Task Management
 *        API domain.
 */
#ifndef SIGMA_CORE_TASKS_TASKSDOMAIN_HPP_
#define SIGMA_CORE_TASKS_TASKSDOMAIN_HPP_

#include <memory>
#include <set>

#include <chaoscore/base/str/UTF8String.hpp>

namespace sigma
{
namespace core
{
/*!
 * \brief The task management API module of Sigma.
 *
 * TODO:
 */
namespace tasks
{

//------------------------------------------------------------------------------
//                                TYPE DEFINITIONS
//------------------------------------------------------------------------------

class RootTask;

/*!
 * \brief The domain for interacting with the task management module.
 */
namespace domain
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: doxygen hide

/*!
 * \brief Initialises the task management API component.
 *
 * TODO: can we hide this symbol?
 */
void init();

/*!
 * \brief Uninitialises the task management API component.
 *
 * TODO:
 */
void clean_up();

// TODO: doxygen hide end??

/*!
 * \brief TODO:
 */
const std::set<std::unique_ptr<RootTask>>& get_boards();

/*!
 * \brief TODO:
 */
RootTask* new_board(const chaos::str::UTF8String& title);

/*!
 * \brief TODO
 *
 * TODO
 */
bool delete_board(RootTask* board_root);

} // namespace domain
} // namespace tasks
} // namespace core
} // namespace sigma

#endif
