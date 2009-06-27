/******************************************************************
*
*  $Id: qxrdcenterfinderdialog.h,v 1.4 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDERDIALOG_H
#define QXRDCENTERFINDERDIALOG_H

#include "qcepmacros.h"

#include <QGroupBox>
#include "ui_qxrdcenterfinderdialog.h"

#include <qwt_double_rect.h>

class QxrdCenterFinderDialog : public QGroupBox, public Ui::QxrdCenterFinderDialog
{
  Q_OBJECT;

public:
  QxrdCenterFinderDialog(QWidget *parent=0);

public slots:
  void onCenterChanged(double cx, double cy);

private:
  HEADER_IDENT("$Id: qxrdcenterfinderdialog.h,v 1.4 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDCENTERFINDERDIALOG_H

/******************************************************************
*
*  $Log: qxrdcenterfinderdialog.h,v $
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

