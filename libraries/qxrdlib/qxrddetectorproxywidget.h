#ifndef QXRDDETECTORPROXYWIDGET_H
#define QXRDDETECTORPROXYWIDGET_H

#include "qxrdlib_global.h"
#include <QWidget>
#include "ui_qxrddetectorproxywidget.h"

class QXRD_EXPORT QxrdDetectorProxyWidget : public QWidget, public Ui::QxrdDetectorProxyWidget
{
  Q_OBJECT

public:
  explicit QxrdDetectorProxyWidget(QWidget *parent = 0);
  ~QxrdDetectorProxyWidget();

private:
};

#endif // QXRDDETECTORPROXYWIDGET_H
