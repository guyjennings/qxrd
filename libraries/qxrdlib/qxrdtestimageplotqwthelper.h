#ifndef QXRDTESTIMAGEPLOTQWTHELPER_H
#define QXRDTESTIMAGEPLOTQWTHELPER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepimagedata-ptr.h"
#include "qcepimagedata.h"
#include <qwt_plot_spectrogram.h>

typedef QSharedPointer<QwtPlotSpectrogram> QwtPlotSpectrogramPtr;

class QXRD_EXPORT QxrdTestImagePlotQwtHelper : public QcepObject
{
  Q_OBJECT

public:
  QxrdTestImagePlotQwtHelper();

public slots:
  virtual void onNewImageAvailable(QcepDoubleImageDataPtr data);

signals:
  void newPlotSpectrogramAvailable(QcepDoubleImageDataPtr data);
};

Q_DECLARE_METATYPE(QwtPlotSpectrogramPtr)

#endif // QXRDTESTIMAGEPLOTQWTHELPER_H
