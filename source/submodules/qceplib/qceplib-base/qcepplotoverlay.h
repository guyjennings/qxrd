#ifndef QCEPPLOTOVERLAY_H
#define QCEPPLOTOVERLAY_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepplotwidget-ptr.h"
#include "qcepplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepPlotOverlay : public QcepObject
{
  Q_OBJECT

public:
  QcepPlotOverlay(QString                    name,
                  QcepPlotWidget            *plot,
                  QcepPlotWidgetSettingsWPtr set);

  virtual void setPen(QPen pen);

  QcepPlotWidgetSettingsWPtr settings() const;

protected:
  QcepPlotWidget             *m_PlotWidget;
  QcepPlotWidgetSettingsWPtr  m_Settings;
};

#endif // QCEPPLOTOVERLAY_H
