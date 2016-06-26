/*!
 * \file
 * \brief Controls start up of the Sigma GUI.
 * \author David Saxon
 */
#ifndef SIGMA_GUI_BOOTSTRAP_HPP_
#define SIGMA_GUI_BOOTSTRAP_HPP_

namespace sigma
{
/*!
 * \brief The graphical user interface of Sigma.
 */
namespace gui
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Preforms all bootstrapping tasks and launches Sigma using using a
 *        graphical user interface.
 */
int bootstrap(int argc, char* argv[]);

/*!
 * \brief Loads the font's required by Sigma gui.
 */
void load_fonts();

} // namespace gui
} // namespace sigma

#endif
