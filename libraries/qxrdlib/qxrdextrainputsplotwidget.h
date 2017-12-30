#ifndef QXRDEXTRAINPUTSPLOTWIDGET_H
#define QXRDEXTRAINPUTSPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdextrainputsplotwidgetsettings-ptr.h"

class QxrdExtraInputsPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdExtraInputsPlotWidget(QWidget *parent);

  void initialize(QxrdExtraInputsPlotWidgetSettingsWPtr settings);
};

#endif // QXRDEXTRAINPUTSPLOTWIDGET_H
