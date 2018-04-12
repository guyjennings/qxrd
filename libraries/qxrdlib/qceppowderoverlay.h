#ifndef QCEPPOWDEROVERLAY_H
#define QCEPPOWDEROVERLAY_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepplotoverlay.h"
#include <qwt_plot_curve.h>
#include "qceppowderringsmodel-ptr.h"

class QCEP_EXPORT QcepPowderOverlay : public QcepPlotOverlay
{
  Q_OBJECT

public:
  QcepPowderOverlay(QString                         name,
                    QcepPlotWidget                 *plot,
                    QcepPlotWidgetSettingsWPtr set,
                    QcepPowderRingsModelWPtr        rings);

private:
  QcepPowderRingsModelWPtr m_PowderRings;
  QVector<QwtPlotCurve*>   m_PowderCurves;
};

#endif // QCEPPOWDEROVERLAY_H
