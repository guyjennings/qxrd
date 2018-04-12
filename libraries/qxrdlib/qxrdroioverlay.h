#ifndef QXRDROIOVERLAY_H
#define QXRDROIOVERLAY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepplotoverlay.h"
#include "qxrdroimodel-ptr.h"
#include <qwt_plot_curve.h>

class QXRD_EXPORT QxrdROIOverlay : public QcepPlotOverlay
{
  Q_OBJECT

public:
  QxrdROIOverlay(QString                         name,
                 QcepPlotWidget                 *plot,
                 QcepPlotWidgetSettingsWPtr set,
                 QxrdROIModelWPtr roic);

private:
  QxrdROIModelWPtr       m_ROIModel;
  QVector<QwtPlotCurve*> m_ROICurves;
};

#endif // QXRDROIOVERLAY_H
