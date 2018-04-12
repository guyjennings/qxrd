#ifndef QXRDCENTERINGPLOTWIDGET_H
#define QXRDCENTERINGPLOTWIDGET_H

#include "qxrdlib_global.h"
#include "qcepplotwidget.h"
#include "qxrdcenteringplotwidgetsettings-ptr.h"
#include "qcepcenterfinder-ptr.h"

class QXRD_EXPORT QxrdCenteringPlotWidget : public QcepPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdCenteringPlotWidget(QWidget *parent = 0);
  ~QxrdCenteringPlotWidget();

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdCenteringPlotWidgetSettingsWPtr settings, QcepCenterFinderWPtr cf);

private:
  QcepCenterFinderWPtr m_CenterFinder;
};

#endif // QXRDCENTERINGPLOTWIDGET_H
