/******************************************************************
*
*  $Id: qxrdintegratordialog.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"

QxrdIntegratorDialog::QxrdIntegratorDialog(QxrdIntegratorPtr integ, QWidget *parent)
  : QWidget(parent),
    m_Integrator(integ),
    SOURCE_IDENT("$Id: qxrdintegratordialog.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
{
  setupUi(this);

  connect(m_IntegrateButton, SIGNAL(clicked()), m_Integrator, SLOT(integrateSaveAndDisplay()));

  m_Integrator -> prop_Oversample()      -> linkTo(m_OversampleFactor);
  m_Integrator -> prop_IntegrationStep() -> linkTo(m_IntegratorStepSize);
}

/******************************************************************
*
*  $Log: qxrdintegratordialog.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.6.4.4  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.6.4.3  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.6.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.6.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.6  2009/10/05 21:17:46  jennings
*  Integrate button now causes integrated curve to be saved and plotted, as well
*
*  Revision 1.5  2009/08/04 22:03:31  jennings
*  Moved integration code into QxrdIntegrator, added oversampling option
*  Add each integration result to the az-avg plot panel
*
*  Revision 1.4  2009/08/04 20:42:53  jennings
*  Simple, initial, implementation of integration
*
*  Revision 1.3  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

