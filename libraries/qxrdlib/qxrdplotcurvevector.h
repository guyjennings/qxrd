#ifndef QXRDPLOTCURVEVECTOR_H
#define QXRDPLOTCURVEVECTOR_H

#include "qwt_plot_piecewise_curve-ptr.h"
#include <QVector>
#include "qxrdlib_global.h"

class QXRD_EXPORT QxrdPlotCurveVector : public QVector<QwtPlotPiecewiseCurve*>
{
public:
  QxrdPlotCurveVector();
};

#endif // QXRDPLOTCURVEVECTOR_H
