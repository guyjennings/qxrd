/******************************************************************
*
*  $Id: qxrdintegratordialog.cpp,v 1.6 2009/10/05 21:17:46 jennings Exp $
*
*******************************************************************/

#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"

QxrdIntegratorDialog::QxrdIntegratorDialog(QxrdIntegrator *integ, QWidget *parent)
  : QWidget(parent),
    m_Integrator(integ),
    SOURCE_IDENT("$Id: qxrdintegratordialog.cpp,v 1.6 2009/10/05 21:17:46 jennings Exp $")
{
  setupUi(this);

  connect(m_IntegrateButton, SIGNAL(clicked()), m_Integrator, SLOT(integrateSaveAndDisplay()));

  m_Integrator -> prop_Oversample() -> linkTo(m_OversampleFactor);
}

/******************************************************************
*
*  $Log: qxrdintegratordialog.cpp,v $
*  Revision 1.6  2009/10/05 21:17:46  jennings
*  *** empty log message ***
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

