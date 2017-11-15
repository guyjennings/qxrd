#ifndef QXRDINTEGRATEDPLOTWIDGET_H
#define QXRDINTEGRATEDPLOTWIDGET_H

#include "qxrdplotwidget.h"

class QXRD_EXPORT QxrdIntegratedPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdIntegratedPlotWidget(QWidget *parent = 0);
  ~QxrdIntegratedPlotWidget();
};

#endif // QXRDINTEGRATEDPLOTWIDGET_H
