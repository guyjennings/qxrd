#ifndef QXRDROIOVERLAY_H
#define QXRDROIOVERLAY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrdplotoverlay.h"
#include "qxrdroicoordinateslistmodel-ptr.h"
#include <qwt_plot_curve.h>

class QXRD_EXPORT QxrdROIOverlay : public QxrdPlotOverlay
{
  Q_OBJECT

public:
  QxrdROIOverlay(QString                         name,
                 QxrdPlotWidget                 *plot,
                 QxrdPlotWidgetSettingsWPtr      set,
                 QxrdROICoordinatesListModelWPtr roic);

private:
  QxrdROICoordinatesListModelWPtr m_ROICoordsModel;
  QVector<QwtPlotCurve*>          m_ROICurves;
};

#endif // QXRDROIOVERLAY_H
