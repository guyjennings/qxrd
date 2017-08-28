#ifndef QCEPMAINWINDOW_H
#define QCEPMAINWINDOW_H

#include "qceplib_global.h"
#include <QMainWindow>
#include <QDateTime>

class QCEP_EXPORT QcepMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit QcepMainWindow(QWidget *parent = 0);
  virtual ~QcepMainWindow();

  void possiblyClose();
  bool wantToClose();

signals:

public slots:
  void shrinkPanels(int fontSize, int spacing);
  void setFontSize(int fontSize);
  void setSpacing(int spacing);
  virtual void printLine(QString line) = 0;
  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;

protected:
  void shrinkDockWidget(QDockWidget *dockWidget, int fontSize, int spacing);
  void shrinkObject(QObject *obj, int fontSize, int spacing);
  void setObjectSpacing(QObject *obj, int spacing);
};

#endif // QCEPMAINWINDOW_H
