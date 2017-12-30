#ifndef QXRDEXTRAOUTPUTSPLOTWIDGET_H
#define QXRDEXTRAOUTPUTSPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdextraoutputsplotwidgetsettings-ptr.h"

class QxrdExtraOutputsPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdExtraOutputsPlotWidget(QWidget *parent);

  void initialize(QxrdExtraOutputsPlotWidgetSettingsWPtr settings);
};

#endif // QXRDEXTRAOUTPUTSPLOTWIDGET_H
