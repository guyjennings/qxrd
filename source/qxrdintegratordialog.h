/******************************************************************
*
*  $Id: qxrdintegratordialog.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATORDIALOG_H
#define QXRDINTEGRATORDIALOG_H

#include "qcepmacros.h"

#include <QGroupBox>
#include "qxrdforwardtypes.h"
#include "ui_qxrdintegratordialog.h"

class QxrdIntegratorDialog : public QWidget, public Ui::QxrdIntegratorDialog
{
  Q_OBJECT;

public:
  QxrdIntegratorDialog(QxrdIntegratorPtr integ, QWidget *parent=0);

private:
  QxrdIntegratorPtr m_Integrator;

  HEADER_IDENT("$Id: qxrdintegratordialog.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDINTEGRATORDIALOG_H

/******************************************************************
*
*  $Log: qxrdintegratordialog.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.3.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.3.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
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

