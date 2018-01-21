#ifndef QXRDPLOTOVERLAY_H
#define QXRDPLOTOVERLAY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qxrdplotwidget-ptr.h"
#include "qxrdplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdPlotOverlay : public QcepObject
{
  Q_OBJECT

public:
  QxrdPlotOverlay(QString                    name,
                  QxrdPlotWidgetWPtr         plot,
                  QxrdPlotWidgetSettingsWPtr set);

  virtual void setPen(const QPen &pen);

protected:
  QxrdPlotWidgetWPtr         m_PlotWidget;
  QxrdPlotWidgetSettingsWPtr m_Settings;
};

#endif // QXRDPLOTOVERLAY_H