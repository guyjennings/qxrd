#ifndef QCEPROIOVERLAY_H
#define QCEPROIOVERLAY_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepplotoverlay.h"
#include "qceproimodel-ptr.h"
#include <qwt_plot_curve.h>

class QCEP_EXPORT QcepROIOverlay : public QcepPlotOverlay
{
  Q_OBJECT

public:
  QcepROIOverlay(QString                         name,
                 QcepPlotWidget                 *plot,
                 QcepPlotWidgetSettingsWPtr set,
                 QcepROIModelWPtr roic);

private:
  QcepROIModelWPtr       m_ROIModel;
  QVector<QwtPlotCurve*> m_ROICurves;
};

#endif // QCEPROIOVERLAY_H
