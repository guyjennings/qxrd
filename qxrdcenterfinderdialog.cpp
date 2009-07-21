/******************************************************************
*
*  $Id: qxrdcenterfinderdialog.cpp,v 1.6 2009/07/21 22:55:48 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder.h"

QxrdCenterFinderDialog::QxrdCenterFinderDialog(QxrdCenterFinder *cen, QWidget *parent)
  : QGroupBox(parent),
    m_CenterFinder(cen),
    SOURCE_IDENT("$Id: qxrdcenterfinderdialog.cpp,v 1.6 2009/07/21 22:55:48 jennings Exp $")
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
*  Revision 1.6  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

