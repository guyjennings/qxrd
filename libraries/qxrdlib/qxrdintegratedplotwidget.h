#ifndef QXRDINTEGRATEDPLOTWIDGET_H
#define QXRDINTEGRATEDPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdintegratedplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdIntegratedPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdIntegratedPlotWidget(QWidget *parent = 0);
  ~QxrdIntegratedPlotWidget();

  //TODO: change to QObjectWPtr
  void initialize(QxrdIntegratedPlotWidgetSettingsWPtr settings);
};

#endif // QXRDINTEGRATEDPLOTWIDGET_H
