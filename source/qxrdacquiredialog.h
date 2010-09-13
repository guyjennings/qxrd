/******************************************************************
*
*  $Id: qxrdacquiredialog.h,v 1.2 2010/09/13 20:00:38 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUIREDIALOG_H
#define QXRDACQUIREDIALOG_H

#include "qcepmacros.h"
#include <QWidget>
#include "qxrdforwardtypes.h"
#include "ui_qxrdacquiredialog.h"

class QxrdAcquireDialog : public QWidget, public Ui::QxrdAcquireDialog {
  Q_OBJECT
public:
  QxrdAcquireDialog(QxrdWindowPtr win, QWidget *parent = 0);
  ~QxrdAcquireDialog();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdWindowPtr   m_Window;

  HEADER_IDENT("$Id: qxrdacquiredialog.h,v 1.2 2010/09/13 20:00:38 jennings Exp $");
};

#endif // QXRDACQUIREDIALOG_H

/******************************************************************
*
*  $Log: qxrdacquiredialog.h,v $
*  Revision 1.2  2010/09/13 20:00:38  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/09/09 19:57:31  jennings
*  Moved acquisition dialog into separate class
*  Reorganised acquisition dialog widgets to be more compact
*
*
*******************************************************************/

