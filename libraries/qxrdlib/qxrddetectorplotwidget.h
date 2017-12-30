#ifndef QXRDDETECTORPLOTWIDGET_H
#define QXRDDETECTORPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrddetectorplotwidgetsettings-ptr.h"

class QxrdDetectorPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdDetectorPlotWidget(QWidget *parent);

  void initialize(QxrdDetectorPlotWidgetSettingsWPtr settings);
};

#endif // QXRDDETECTORPLOTWIDGET_H
