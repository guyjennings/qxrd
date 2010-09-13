/******************************************************************
*
*  $Id: qxrdcenterfinderdialog.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDERDIALOG_H
#define QXRDCENTERFINDERDIALOG_H

#include "qcepmacros.h"

#include <QGroupBox>
#include "qxrdforwardtypes.h"
#include "ui_qxrdcenterfinderdialog.h"

//#include <qwt_double_rect.h>

class QxrdCenterFinderDialog : public QGroupBox, public Ui::QxrdCenterFinderDialog
{
  Q_OBJECT;

public:
  QxrdCenterFinderDialog(QxrdCenterFinderPtr cen, QWidget *parent=0);

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
  void onImplementTiltChanged(bool imp);

private:
  void moveCenter(int dx, int dy);

private:
  QxrdCenterFinderPtr m_CenterFinder;

  HEADER_IDENT("$Id: qxrdcenterfinderdialog.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDCENTERFINDERDIALOG_H

/******************************************************************
*
*  $Log: qxrdcenterfinderdialog.h,v $
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
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.5.4.2  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.5.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.5  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

