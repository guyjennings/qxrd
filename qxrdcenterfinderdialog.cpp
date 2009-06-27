/******************************************************************
*
*  $Id: qxrdcenterfinderdialog.cpp,v 1.5 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinderdialog.h"

QxrdCenterFinderDialog::QxrdCenterFinderDialog(QWidget *parent)
  : QGroupBox(parent),
    SOURCE_IDENT("$Id: qxrdcenterfinderdialog.cpp,v 1.5 2009/06/27 22:50:32 jennings Exp $")
{
  setupUi(this);
}

void QxrdCenterFinderDialog::onCenterChanged(double cx, double cy)
{
  m_CenterX -> setValue(cx);
  m_CenterY -> setValue(cy);

//  printf("QxrdCenterFinderDialog::onCenterChanged(%g,%g)\n", cx, cy);
}

/******************************************************************
*
*  $Log: qxrdcenterfinderdialog.cpp,v $
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

