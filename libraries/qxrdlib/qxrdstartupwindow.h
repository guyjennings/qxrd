#ifndef QXRDSTARTUPWINDOW_H
#define QXRDSTARTUPWINDOW_H

#include <QMainWindow>
#include "ui_qxrdstartupwindow.h"

class QxrdStartupWindow : public QMainWindow, public Ui::QxrdStartupWindow
{
  Q_OBJECT

public:
  explicit QxrdStartupWindow(QWidget *parent = 0);
  ~QxrdStartupWindow();

  void setApplicationIcon        (QIcon icon);
  void setApplicationDescription (QString desc);
  void appendMessage             (QString msg);
};

#endif // QXRDSTARTUPWINDOW_H
