#ifndef QXRDCENTERMARKER_H
#define QXRDCENTERMARKER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrdplotoverlay.h"
#include "qxrdcenterfinder-ptr.h"
#include <qwt_plot_marker.h>

class QXRD_EXPORT QxrdCenterMarker : public QxrdPlotOverlay
{
  Q_OBJECT

public:
  QxrdCenterMarker(QString                    name,
                   QxrdPlotWidget            *plot,
                   QxrdPlotWidgetSettingsWPtr set,
                   QxrdCenterFinderWPtr       cf);

  void setPen(QPen pen);

private:
  void centerChanged(QPointF c);

private:
  QxrdCenterFinderWPtr m_CenterFinder;
  QwtPlotMarker       *m_CenterMarker;
};

#endif // QXRDCENTERMARKER_H
