#ifndef QXRDCENTERINGPLOTWIDGET_H
#define QXRDCENTERINGPLOTWIDGET_H

#include "qxrdplotwidget.h"

class QXRD_EXPORT QxrdCenteringPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdCenteringPlotWidget(QWidget *parent = 0);
  ~QxrdCenteringPlotWidget();
};

#endif // QXRDCENTERINGPLOTWIDGET_H
