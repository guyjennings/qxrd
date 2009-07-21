/******************************************************************
*
*  $Id: qxrdcenterfinderdialog.h,v 1.5 2009/07/21 22:55:48 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDERDIALOG_H
#define QXRDCENTERFINDERDIALOG_H

#include "qcepmacros.h"

class QxrdCenterFinder;

#include <QGroupBox>
#include "ui_qxrdcenterfinderdialog.h"

//#include <qwt_double_rect.h>

class QxrdCenterFinderDialog : public QGroupBox, public Ui::QxrdCenterFinderDialog
{
  Q_OBJECT;

public:
  QxrdCenterFinderDialog(QxrdCenterFinder *cen, QWidget *parent=0);

public slots:
  void centerMoveUpLeft();
  void centerMoveUp();
  void centerMoveUpRight();
  void centerMoveRight();
  void centerMoveDownRight();
  void centerMoveDown();
  void centerMoveDownLeft();
  void centerMoveLeft();

  void onCenterChanged(double cx, double cy);

private:
  void moveCenter(int dx, int dy);

private:
  QxrdCenterFinder   *m_CenterFinder;

  HEADER_IDENT("$Id: qxrdcenterfinderdialog.h,v 1.5 2009/07/21 22:55:48 jennings Exp $");
};

#endif // QXRDCENTERFINDERDIALOG_H

/******************************************************************
*
*  $Log: qxrdcenterfinderdialog.h,v $
*  Revision 1.5  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

