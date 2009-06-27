/******************************************************************
*
*  $Id: qxrdintegratordialog.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATORDIALOG_H
#define QXRDINTEGRATORDIALOG_H

#include "qcepmacros.h"

#include <QGroupBox>
#include "ui_qxrdintegratordialog.h"

class QxrdIntegratorDialog : public QGroupBox, public Ui::QxrdIntegratorDialog
{
  Q_OBJECT;

public:
  QxrdIntegratorDialog(QWidget *parent=0);

private:
  HEADER_IDENT("$Id: qxrdintegratordialog.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDINTEGRATORDIALOG_H

/******************************************************************
*
*  $Log: qxrdintegratordialog.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

