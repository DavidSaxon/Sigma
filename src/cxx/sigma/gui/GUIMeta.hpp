/*!
 * \file
 * \brief MetaEngine data for Sigma's GUI.
 * \author David Saxon
 */
#ifndef SIGMA_GUI_META_HPP_
#define SIGMA_GUI_META_HPP_

#include <memory>

#include <metaeng/Data.hpp>

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

typedef std::unique_ptr<metaeng::Data> DataPtr;

//------------------------------------------------------------------------------
//                                      DATA
//------------------------------------------------------------------------------

/*!
 * \brief Meta data for logging the GUI runtime.
 *
 * This metadata is loaded before other data since logging needs to be connected
 * first.
 */
extern DataPtr logging;

// TODO: REMOVE ME?
/*!
 * \brief Meta data for global resource information.
 */
extern DataPtr resource_globals;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Initialises the MetaEngine Data pointers.
 */
void init();


} // namespace meta
} // namespace gui
} // namespace sigma

#endif
