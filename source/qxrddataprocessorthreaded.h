#ifndef QXRDDATAPROCESSORTHREADED_H
#define QXRDDATAPROCESSORTHREADED_H

#include "qcepmacros.h"

#include "qxrddataprocessorbase.h"
#include "qxrdresultserializer.h"
#include "qcepintegrateddata-ptr.h"
#include "qxrdroidata.h"
#include "qxrdroidata-ptr.h"
#include "qxrdhistogramdata.h"
#include "qxrdhistogramdata-ptr.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"

class QxrdDataProcessorThreaded : public QxrdDataProcessorBase
{
  Q_OBJECT
public:
  QxrdDataProcessorThreaded(QString name);
  virtual ~QxrdDataProcessorThreaded();

public:
  void beginAcquisition(int isDark);
  void idleInt16Image(QcepUInt16ImageDataPtr image, bool liveView);
  void acquiredInt16Image(QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow);
  void acquiredInt32Image(QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  void acquiredDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);
  void acquiredDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, QcepDoubleList v);

private slots:
  void onCorrectedImageAvailable();
  void onIntegratedDataAvailable();
  void onROIDataAvailable();
  void onHistogramDataAvailable();

public slots:
  void sumImages(QStringList names);
  void addImages(QStringList names);
  void subtractImages(QStringList names);
  void projectImages(QStringList names, int px, int py, int pz);
  void integrateData(QString name);
  void correlateImages(QStringList names);

  void clearAccumulator();
  void integrateAndAccumulate(QStringList names);
  void saveAccumulator(QString &path, QString filter);

  void shiftImage(int dx, int dy);

  void reflectVertically();
  void reflectHorizontally();

  void fitPeakNear(double x, double y);

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
  QcepDoubleImageDataPtr correctInt16Image(QcepDoubleImageDataPtr corrected, QcepUInt16ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr correctInt32Image(QcepDoubleImageDataPtr corrected, QcepUInt32ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr correctDoubleImage(QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr correctDoubleImage(QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr overflow, QcepDoubleList v);
  QcepIntegratedDataPtr  integrateImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask, double, double cx);
  QxrdROIDataPtr         calculateROI(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask);
  QxrdHistogramDataPtr   calculateHistogram(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask);

private:
  QxrdResultSerializer<QcepDoubleImageDataPtr>  m_CorrectedImages;
  QxrdResultSerializer<QcepIntegratedDataPtr>   m_IntegratedData;
  QxrdResultSerializer<QxrdROIDataPtr>          m_ROIData;
  QxrdResultSerializer<QxrdHistogramDataPtr>    m_HistogramData;

protected:
  double estimatedProcessingTime(double estSerTime, double estParallelTime);
};

#endif // QXRDDATAPROCESSORTHREADED_H
