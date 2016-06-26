/*!
 * \file
 * \brief MetaEngine data for Sigma's GUI for written into the source code so it
 *        can be loaded from memory.
 * \author David Saxon
 */
#ifndef SIGMA_GUI_METACOMPILED_HPP_
#define SIGMA_GUI_METACOMPILED_HPP_

#include <arcanecore/base/str/UTF8String.hpp>

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
extern const arc::str::UTF8String logging;

/*!
 * \brief MetaEngine JSON data for directories where to find resources.
 */
extern const arc::str::UTF8String resource_locations;
/*!
 * \brief MetaEngine JSON data in relation to fonts.
 */
extern const arc::str::UTF8String fonts;

} // namespace meta_comp
} // namespace gui
} // namespace sigma

#endif
