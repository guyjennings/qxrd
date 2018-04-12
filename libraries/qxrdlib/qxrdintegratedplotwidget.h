#ifndef QXRDINTEGRATEDPLOTWIDGET_H
#define QXRDINTEGRATEDPLOTWIDGET_H

#include "qcepplotwidget.h"
#include "qxrdintegratedplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdIntegratedPlotWidget : public QcepPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdIntegratedPlotWidget(QWidget *parent = 0);
  ~QxrdIntegratedPlotWidget();

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdIntegratedPlotWidgetSettingsWPtr settings);
};

#endif // QXRDINTEGRATEDPLOTWIDGET_H
