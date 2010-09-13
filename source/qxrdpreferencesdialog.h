/******************************************************************
*
*  $Id: qxrdpreferencesdialog.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPREFERENCESDIALOG_H
#define QXRDPREFERENCESDIALOG_H

#include "qcepmacros.h"

#include <QDialog>

namespace Ui {
  class QxrdPreferencesDialog;
}

class QxrdPreferencesDialog : public QDialog {
  Q_OBJECT;
public:
  QxrdPreferencesDialog(QWidget *parent = 0);
  ~QxrdPreferencesDialog();

public slots:
  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QxrdPreferencesDialog *ui;
  HEADER_IDENT("$Id: qxrdpreferencesdialog.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDPREFERENCESDIALOG_H

/******************************************************************
*
*  $Log: qxrdpreferencesdialog.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.4  2010/05/21 18:30:32  jennings
*  Implemented preferences dialog allowing choise of detector type, processor type and debug level
*
*  Revision 1.1.2.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.2  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.1  2010/04/21 19:48:41  jennings
*  Added QxrdAcquisitionAreaDetector, QxrdAcquisitionPilatus, QxrdDataProcessorCuda, QxrdDataProcessorThreaded
*  and QxrdPreferencesDialog - they don't do much yet, though.
*
*
*******************************************************************/
