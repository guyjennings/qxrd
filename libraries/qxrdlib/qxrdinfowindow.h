#ifndef QXRDINFOWINDOW_H
#define QXRDINFOWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdinfowindow.h"

class QXRD_EXPORT QxrdInfoWindow : public QxrdMainWindow, public Ui::QxrdInfoWindow
{
  Q_OBJECT

public:
  explicit QxrdInfoWindow(QWidget *parent = 0);
  ~QxrdInfoWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDINFOWINDOW_H
