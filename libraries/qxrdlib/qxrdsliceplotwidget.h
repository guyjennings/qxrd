#ifndef QXRDSLICEPLOTWIDGET_H
#define QXRDSLICEPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdsliceplotwidgetsettings-ptr.h"

class QxrdSlicePlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdSlicePlotWidget(QWidget *parent);

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdSlicePlotWidgetSettingsWPtr settings);
};

#endif // QXRDSLICEPLOTWIDGET_H
