#ifndef QXRDDATAPROCESSORTHREADED_H
#define QXRDDATAPROCESSORTHREADED_H

#include "qcepmacros.h"

#include "qxrddataprocessorbase.h"
#include "qxrdresultserializer.h"
#include "qxrdintegrateddata-ptr.h"
#include "qxrdroidata.h"
#include "qxrdroidata-ptr.h"
#include "qxrdhistogramdata.h"
#include "qxrdhistogramdata-ptr.h"
#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"

class QxrdDataProcessorThreaded : public QxrdDataProcessorBase
{
  Q_OBJECT
public:
  QxrdDataProcessorThreaded(QxrdSettingsSaverWPtr saver,
                            QxrdExperimentWPtr doc,
                            QxrdAcquisitionWPtr acq,
                            QxrdAllocatorWPtr allocator,
                            QxrdFileSaverWPtr filesaver);
  virtual ~QxrdDataProcessorThreaded();

public:
  void beginAcquisition(int isDark);
  void idleInt16Image(QxrdInt16ImageDataPtr image, bool liveView);
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
  void addImages(QStringList names);
  void subtractImages(QStringList names);
  void projectImages(QStringList names, int px, int py, int pz);
  void integrateData(QString name);
  void correlateImages(QStringList names);

  void shiftImage(int dx, int dy);

  void reflectVertically();
  void reflectHorizontally();

  void processData(QString name);
  void processDataSequence(QString path, QString filter="*.tif");
  void processDataSequence(QStringList paths);
  void processDataSequence(QString path, QStringList filter);
  void processNormalizedFile(QString path, double v1);
  void processNormalizedFile(QString path, double v1, double v2);
  void processNormalizedFile(QString path, QcepDoubleList v);
  void setFileNormalization(QString path, double v1);
  void setFileNormalization(QString path, double v1, double v2);
  void setFileNormalization(QString path, QcepDoubleList v);

  void slicePolygon(QVector<QPointF> poly);
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
