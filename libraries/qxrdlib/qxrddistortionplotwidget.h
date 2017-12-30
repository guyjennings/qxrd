#ifndef QXRDDISTORTIONPLOTWIDGET_H
#define QXRDDISTORTIONPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrddistortionplotwidgetsettings-ptr.h"

class QxrdDistortionPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdDistortionPlotWidget(QWidget *parent = 0);

  void initialize(QxrdDistortionPlotWidgetSettingsWPtr settings);
};

#endif // QXRDDISTORTIONPLOTWIDGET_H
