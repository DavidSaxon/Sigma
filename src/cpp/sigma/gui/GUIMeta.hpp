/*!
 * \file
 * \brief MetaEngine data for Sigma's GUI.
 * \author David Saxon
 */
#ifndef SIGMA_GUI_META_HPP_
#define SIGMA_GUI_META_HPP_

#include <memory>

#include <metaengine/Document.hpp>

namespace sigma
{
namespace gui
{
/*!
 * \brief MetaEngine Data objects relating to the GUI.
 */
namespace meta
{

//------------------------------------------------------------------------------
//                              TYPEDEF DEFINITIONS
//------------------------------------------------------------------------------

typedef std::unique_ptr<metaengine::Document> DocumentPtr;

//------------------------------------------------------------------------------
//                                      DATA
//------------------------------------------------------------------------------

/*!
 * \brief MetaEngine data for logging the GUI runtime.
 *
 * This data is loaded before other data since logging needs to be connected
 * first.
 */
extern DocumentPtr logging;

/*!
 * \brief MetaEngine data for directories where to find resources.
 */
extern DocumentPtr resource_locations;
/*!
 * \brief MetaEngine data about fonts.
 */
extern DocumentPtr fonts;

/*!
 * \brief MetaEngine data about startup widgets.
 */
extern DocumentPtr widgets_startup;


//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Initialises the MetaEngine Document pointers.
 */
void init();


} // namespace meta
} // namespace gui
} // namespace sigma

#endif
