#ifndef QXRDDATAPROCESSORTHREADED_H
#define QXRDDATAPROCESSORTHREADED_H

#include "qcepmacros.h"

#include "qxrddataprocessorbase.h"
#include "qxrdresultserializer.h"

class QxrdDataProcessorThreaded : public QxrdDataProcessorBase
{
  Q_OBJECT;
public:
  QxrdDataProcessorThreaded(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent=0);

public:
  void beginAcquisition(int isDark);
  void idleInt16Image(QxrdInt16ImageDataPtr image);
  void acquiredInt16Image(QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow);
  void acquiredInt32Image(QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);

private slots:
  void onCorrectedImageAvailable();
  void onIntegratedDataAvailable();
  void onROIDataAvailable();
  void onHistogramDataAvailable();

private:
  QxrdDoubleImageDataPtr correctInt16Image(QxrdInt16ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr correctInt32Image(QxrdInt32ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdIntegratedDataPtr  integrateImage(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, double cx, double cy);
  QxrdROIDataPtr         calculateROI(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask);
  QxrdHistogramDataPtr   calculateHistogram(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask);

private:
  QxrdResultSerializer<QxrdDoubleImageDataPtr>  m_CorrectedImages;
  QxrdResultSerializer<QxrdIntegratedDataPtr>   m_IntegratedData;
  QxrdResultSerializer<QxrdROIDataPtr>          m_ROIData;
  QxrdResultSerializer<QxrdHistogramDataPtr>    m_HistogramData;

protected:
  double estimatedProcessingTime(double estSerTime, double estParallelTime);
};

#endif // QXRDDATAPROCESSORTHREADED_H
