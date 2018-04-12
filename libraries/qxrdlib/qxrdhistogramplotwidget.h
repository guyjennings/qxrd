#ifndef QXRDHISTOGRAMPLOTWIDGET_H
#define QXRDHISTOGRAMPLOTWIDGET_H

#include "qcepplotwidget.h"
#include "qxrdhistogramplotwidgetsettings-ptr.h"

class QxrdHistogramPlotWidget : public QcepPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdHistogramPlotWidget(QWidget *parent);

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdHistogramPlotWidgetSettingsWPtr settings);
};

#endif // QXRDHISTOGRAMPLOTWIDGET_H
