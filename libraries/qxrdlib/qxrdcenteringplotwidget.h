#ifndef QXRDCENTERINGPLOTWIDGET_H
#define QXRDCENTERINGPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdcenteringplotwidgetsettings-ptr.h"
#include "qxrdcenterfinder-ptr.h"

class QXRD_EXPORT QxrdCenteringPlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdCenteringPlotWidget(QWidget *parent = 0);
  ~QxrdCenteringPlotWidget();

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdCenteringPlotWidgetSettingsWPtr settings, QxrdCenterFinderWPtr cf);

private:
  QxrdCenterFinderWPtr m_CenterFinder;
};

#endif // QXRDCENTERINGPLOTWIDGET_H
