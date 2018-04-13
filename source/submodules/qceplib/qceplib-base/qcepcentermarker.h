#ifndef QCEPCENTERMARKER_H
#define QCEPCENTERMARKER_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepplotoverlay.h"
#include "qcepcenterfinder-ptr.h"
#include <qwt_plot_marker.h>

class QCEP_EXPORT QcepCenterMarker : public QcepPlotOverlay
{
  Q_OBJECT

public:
  QcepCenterMarker(QString                    name,
                   QcepPlotWidget            *plot,
                   QcepPlotWidgetSettingsWPtr set,
                   QcepCenterFinderWPtr cf);

  void setPen(QPen pen);

private:
  void centerChanged(QPointF c);

private:
  QcepCenterFinderWPtr m_CenterFinder;
  QwtPlotMarker       *m_CenterMarker;
};

#endif // QCEPCENTERMARKER_H
