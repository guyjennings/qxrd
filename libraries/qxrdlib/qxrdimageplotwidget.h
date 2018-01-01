#ifndef QXRDIMAGEPLOTWIDGET_H
#define QXRDIMAGEPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdImagePlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdImagePlotWidget(QWidget *parent = 0);
  ~QxrdImagePlotWidget();

  void initialize(QxrdImagePlotWidgetSettingsWPtr settings);
};

#endif // QXRDIMAGEPLOTWIDGET_H
