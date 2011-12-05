#ifndef QXRDDATAPROCESSORTHREADED_H
#define QXRDDATAPROCESSORTHREADED_H

#include "qcepmacros.h"

#include "qxrddataprocessorbase.h"
#include "qxrdresultserializer.h"
#include "qxrdintegrateddata.h"
#include "qxrdroidata.h"
#include "qxrdhistogramdata.h"
#include "qxrdimagedata.h"

class QxrdDataProcessorThreaded : public QxrdDataProcessorBase
{
  Q_OBJECT
public:
  QxrdDataProcessorThreaded(QxrdSettingsSaver *saver, QxrdExperiment *doc, QxrdAcquisition *acq, QxrdAllocator *allocator, QxrdFileSaverThread *filesaver, QObject *parent=0);

public:
  void beginAcquisition(int isDark);
  void idleInt16Image(QxrdInt16ImageDataPtr image);
  void acquiredInt16Image(QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow);
  void acquiredInt32Image(QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);
  void acquiredDoubleImage(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);
  void acquiredDoubleImage(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, QcepDoubleList v);

private slots:
  void onCorrectedImageAvailable();
  void onIntegratedDataAvailable();
  void onROIDataAvailable();
  void onHistogramDataAvailable();

public slots:
  void accumulateImages(QStringList names);
  void integrateData(QString name);

  void processData(QString name);
  void processDataSequence(QString path, QString filter="*.tif");
  void processDataSequence(QStringList paths);
  void processDataSequence(QString path, QStringList filter);
  void processNormalizedFile(QString path, double v1);
  void processNormalizedFile(QString path, double v1, double v2);
  void processNormalizedFile(QString path, QcepDoubleList v);

  void slicePolygon(QwtArray<QwtDoublePoint> poly);
  void integrateSaveAndDisplay();

  void fixupBadBackgroundSubtraction(QString imagePattern, int nImgExposures, QString darkPath, int nDarkExposures);

private:
  QxrdDoubleImageDataPtr correctInt16Image(QxrdDoubleImageDataPtr corrected, QxrdInt16ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr correctInt32Image(QxrdDoubleImageDataPtr corrected, QxrdInt32ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr correctDoubleImage(QxrdDoubleImageDataPtr corrected, QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr correctDoubleImage(QxrdDoubleImageDataPtr corrected, QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr overflow, QcepDoubleList v);
  QxrdIntegratedDataPtr  integrateImage(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, double cx, double cy);
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
