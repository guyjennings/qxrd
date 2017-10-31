#ifndef QXRDMAINWINDOW_H
#define QXRDMAINWINDOW_H

#include "qxrdlib_global.h"
#include "qcepmainwindow.h"

class QXRD_EXPORT QxrdMainWindow : public QcepMainWindow
{
  Q_OBJECT
public:
  explicit QxrdMainWindow(QWidget *parent = 0);

  virtual void setupMenus();

public slots:
  void populateWindowsMenu();

private:
  QMenu *m_MainWindowsMenu;
};

#endif // QXRDMAINWINDOW_H
