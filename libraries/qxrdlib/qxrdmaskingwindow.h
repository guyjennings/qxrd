#ifndef QXRDMASKINGWINDOW_H
#define QXRDMASKINGWINDOW_H

#include "qcepdebug.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdmaskingwindow.h"

class QXRD_EXPORT QxrdMaskingWindow : public QxrdMainWindow, public Ui::QxrdMaskingWindow
{
  Q_OBJECT

public:
  explicit QxrdMaskingWindow(QWidget *parent = 0);
  ~QxrdMaskingWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDMASKINGWINDOW_H
