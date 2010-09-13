/******************************************************************
*
*  $Id: qxrdcenterfinderdialog.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder.h"

QxrdCenterFinderDialog::QxrdCenterFinderDialog(QxrdCenterFinderPtr cen, QWidget *parent)
  : QGroupBox(parent),
    m_CenterFinder(cen),
    SOURCE_IDENT("$Id: qxrdcenterfinderdialog.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
  setupUi(this);

  connect(m_CenterMoveUpLeft,    SIGNAL(clicked()), this, SLOT(centerMoveUpLeft()));
  connect(m_CenterMoveUp,        SIGNAL(clicked()), this, SLOT(centerMoveUp()));
  connect(m_CenterMoveUpRight,   SIGNAL(clicked()), this, SLOT(centerMoveUpRight()));
  connect(m_CenterMoveRight,     SIGNAL(clicked()), this, SLOT(centerMoveRight()));
  connect(m_CenterMoveDownRight, SIGNAL(clicked()), this, SLOT(centerMoveDownRight()));
  connect(m_CenterMoveDown,      SIGNAL(clicked()), this, SLOT(centerMoveDown()));
  connect(m_CenterMoveDownLeft,  SIGNAL(clicked()), this, SLOT(centerMoveDownLeft()));
  connect(m_CenterMoveLeft,      SIGNAL(clicked()), this, SLOT(centerMoveLeft()));

  m_CenterFinder -> prop_CenterX() -> linkTo(m_CenterX);
  m_CenterFinder -> prop_CenterY() -> linkTo(m_CenterY);
  m_CenterFinder -> prop_CenterStep() -> linkTo(m_CenterStep);
  m_CenterFinder -> prop_ImplementTilt() -> linkTo(m_ImplementTilt);
  m_CenterFinder -> prop_DetectorDistance() -> linkTo(m_DetectorDistance);
  m_CenterFinder -> prop_DetectorTilt() -> linkTo(m_DetectorTilt);
  m_CenterFinder -> prop_TiltPlaneRotation() -> linkTo(m_TiltPlaneRotation);

  connect(m_CenterFinder -> prop_ImplementTilt(), SIGNAL(changedValue(bool)), this, SLOT(onImplementTiltChanged(bool)));

  onImplementTiltChanged(m_CenterFinder -> get_ImplementTilt());
}

void QxrdCenterFinderDialog::onImplementTiltChanged(bool imp)
{
  m_DetectorDistance  -> setEnabled(true /*imp*/);
  m_DetectorTilt      -> setEnabled(imp);
  m_TiltPlaneRotation -> setEnabled(imp);
}

void QxrdCenterFinderDialog::onCenterChanged(double cx, double cy)
{
  m_CenterFinder -> set_CenterX(cx);
  m_CenterFinder -> set_CenterY(cy);

//  printf("QxrdCenterFinderDialog::onCenterChanged(%g,%g)\n", cx, cy);
}

void QxrdCenterFinderDialog::moveCenter(int dx, int dy)
{
  m_CenterFinder -> set_CenterX(m_CenterFinder -> get_CenterX() + m_CenterFinder -> get_CenterStep()*dx);
  m_CenterFinder -> set_CenterY(m_CenterFinder -> get_CenterY() + m_CenterFinder -> get_CenterStep()*dy);
}

void QxrdCenterFinderDialog::centerMoveUpLeft()
{
  moveCenter(-1,1);
}

void QxrdCenterFinderDialog::centerMoveUp()
{
  moveCenter(0,1);
}

void QxrdCenterFinderDialog::centerMoveUpRight()
{
  moveCenter(1,1);
}

void QxrdCenterFinderDialog::centerMoveRight()
{
  moveCenter(1,0);
}

void QxrdCenterFinderDialog::centerMoveDownRight()
{
  moveCenter(1,-1);
}

void QxrdCenterFinderDialog::centerMoveDown()
{
  moveCenter(0,-1);
}

void QxrdCenterFinderDialog::centerMoveDownLeft()
{
  moveCenter(-1,-1);
}

void QxrdCenterFinderDialog::centerMoveLeft()
{
  moveCenter(-1,0);
}

/******************************************************************
*
*  $Log: qxrdcenterfinderdialog.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/09/10 18:54:10  jennings
*  Partial implement tilt in center finder dialog
*
*  Revision 1.1.2.2  2010/09/08 19:40:57  jennings
*  Added tilt controls to center finder
*  Disabled fit refine command (for now)
*  Added percentile display mode
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.6.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.6  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

