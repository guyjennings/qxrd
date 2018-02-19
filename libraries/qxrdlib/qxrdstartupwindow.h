#ifndef QXRDSTARTUPWINDOW_H
#define QXRDSTARTUPWINDOW_H

#include <QMainWindow>
#include "ui_qxrdstartupwindow.h"
#include <QDateTime>

class QxrdStartupWindow : public QMainWindow, public Ui::QxrdStartupWindow
{
  Q_OBJECT

public:
  explicit QxrdStartupWindow(QWidget *parent = 0);
  ~QxrdStartupWindow();

  void setApplicationIcon        (QIcon     icon);
  void setApplicationDescription (QString   desc);

public slots:
  void appendMessage             (QString   msg,
                                  QDateTime dt=QDateTime::currentDateTime());
private:
  QDateTime m_LastDateTime;
};

#endif // QXRDSTARTUPWINDOW_H
