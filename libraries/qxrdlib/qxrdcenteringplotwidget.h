#ifndef QXRDCENTERINGPLOTWIDGET_H
#define QXRDCENTERINGPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdcenteringplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdCenteringPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdCenteringPlotWidget(QWidget *parent = 0);
  ~QxrdCenteringPlotWidget();

  void initialize(QxrdCenteringPlotWidgetSettingsWPtr settings);
};

#endif // QXRDCENTERINGPLOTWIDGET_H
