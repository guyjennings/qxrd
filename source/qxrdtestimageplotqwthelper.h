#ifndef QXRDTESTIMAGEPLOTQWTHELPER_H
#define QXRDTESTIMAGEPLOTQWTHELPER_H

#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"
#include "qcepimagedata.h"
#include <qwt_plot_spectrogram.h>

typedef QSharedPointer<QwtPlotSpectrogram> QwtPlotSpectrogramPtr;

class QxrdTestImagePlotQwtHelper : public QcepSerializableObject
{
  Q_OBJECT

public:
  QxrdTestImagePlotQwtHelper();

public slots:
  virtual void onNewImageAvailable(QcepDoubleImageDataPtr data);

signals:
  void newPlotSpectrogramAvailable(QwtPlotSpectrogramPtr);
};

Q_DECLARE_METATYPE(QwtPlotSpectrogramPtr)

#endif // QXRDTESTIMAGEPLOTQWTHELPER_H
