#ifndef QWT_PLOT_SPECTROGRAMPTR_H
#define QWT_PLOT_SPECTROGRAMPTR_H

#include <QSharedPointer>

class QwtPlotSpectrogram;

typedef QSharedPointer<QwtPlotSpectrogram> QwtPlotSpectrogramPtr;
typedef QWeakPointer<QwtPlotSpectrogram>   QwtPlotSpectrogramWPtr;

#endif // QWT_PLOT_SPECTROGRAMPTR_H
