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
};

#endif // QXRDACQUIREDIALOG_H
