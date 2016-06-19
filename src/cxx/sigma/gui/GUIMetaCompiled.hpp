/*!
 * \file
 * \brief MetaEngine data for Sigma's GUI for written into the source code so it
 *        can be loaded from memory.
 * \author David Saxon
 */
#ifndef SIGMA_GUI_METACOMPILED_HPP_
#define SIGMA_GUI_METACOMPILED_HPP_

#include <chaoscore/base/str/UTF8String.hpp>

namespace sigma
{
namespace gui
{
/*!
 * \brief Compiled MetaEngine data files, to be used as schema's and loading
 *        from memory.
 */
namespace meta_comp
{

/*!
 * \brief MetaEngine JSON data related to logging the GUI runtime.
 */
extern const chaos::str::UTF8String logging;

} // namespace meta_comp
} // namespace gui
} // namespace sigma

#endif
