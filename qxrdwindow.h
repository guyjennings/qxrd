#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include <QMainWindow>
#include "ui_qxrdwindow.h"

class QxrdApplication;
class QxrdAcquisitionThread;

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

 public:
  QxrdWindow(QxrdApplication *app, QWidget *parent=0);

 public slots:
  void setAcquisitionThread(QxrdAcquisitionThread *acq);

 private:
  QxrdApplication        *m_Application;
  QxrdAcquisitionThread  *m_AcquisitionThread;
};

#endif
