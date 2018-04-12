#ifndef QXRDDISTORTIONPLOTWIDGET_H
#define QXRDDISTORTIONPLOTWIDGET_H

#include "qcepplotwidget.h"
#include "qxrddistortionplotwidgetsettings-ptr.h"

class QxrdDistortionPlotWidget : public QcepPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdDistortionPlotWidget(QWidget *parent = 0);

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdDistortionPlotWidgetSettingsWPtr settings);
};

#endif // QXRDDISTORTIONPLOTWIDGET_H
