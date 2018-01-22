#ifndef QXRDPOWDEROVERLAY_H
#define QXRDPOWDEROVERLAY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrdplotoverlay.h"
#include <qwt_plot_curve.h>
#include "qxrdpowderringsmodel-ptr.h"

class QXRD_EXPORT QxrdPowderOverlay : public QxrdPlotOverlay
{
  Q_OBJECT

public:
  QxrdPowderOverlay(QString                         name,
                    QxrdPlotWidget                 *plot,
                    QxrdPlotWidgetSettingsWPtr      set,
                    QxrdPowderRingsModelWPtr        rings);

private:
  QxrdPowderRingsModelWPtr m_PowderRings;
  QVector<QwtPlotCurve*>   m_PowderCurves;
};

#endif // QXRDPOWDEROVERLAY_H
