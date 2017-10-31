#ifndef QXRDSCRIPTINGWINDOW_H
#define QXRDSCRIPTINGWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdscriptingwindow.h"

class QXRD_EXPORT QxrdScriptingWindow : public QxrdMainWindow, public Ui::QxrdScriptingWindow
{
  Q_OBJECT

public:
  explicit QxrdScriptingWindow(QWidget *parent = 0);
  ~QxrdScriptingWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDSCRIPTINGWINDOW_H
