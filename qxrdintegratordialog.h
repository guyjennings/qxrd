/******************************************************************
*
*  $Id: qxrdintegratordialog.h,v 1.3 2009/07/21 22:55:48 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATORDIALOG_H
#define QXRDINTEGRATORDIALOG_H

#include "qcepmacros.h"

class QxrdIntegrator;

#include <QGroupBox>
#include "ui_qxrdintegratordialog.h"

class QxrdIntegratorDialog : public QWidget, public Ui::QxrdIntegratorDialog
{
  Q_OBJECT;

public:
  QxrdIntegratorDialog(QxrdIntegrator *integ, QWidget *parent=0);

private:
  QxrdIntegrator   *m_Integrator;

  HEADER_IDENT("$Id: qxrdintegratordialog.h,v 1.3 2009/07/21 22:55:48 jennings Exp $");
};

#endif // QXRDINTEGRATORDIALOG_H

/******************************************************************
*
*  $Log: qxrdintegratordialog.h,v $
*  Revision 1.3  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

