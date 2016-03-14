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

/*!
 * \brief Preforms all bootstrapping tasks and launches Sigma using using a
 *        graphical user interface.
 */
int bootstrap(int argc, char* argv[]);

} // namespace gui
} // namespace sigma

#endif
