#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qcepobject.h"
//#include <QReadWriteLock>
//#include <QAtomicInt>
#include <QWaitCondition>
//#include <QStack>

#include "qcepproperty.h"
#include "qxrdprocessor.h"
#include "qcepimagequeue.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdmaskstack.h"
#include "qxrdmaskstack-ptr.h"
#include "qcepintegrateddata.h"
#include "qcepintegrateddata-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdgeneratetestimage-ptr.h"
#include "qxrdpolartransform-ptr.h"
#include "qxrdpolartransform.h"
#include "qxrdpolarnormalization-ptr.h"
#include "qxrdpolarnormalization.h"
#include "qxrdintegrator-ptr.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdcenterfinder.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrdroidata-ptr.h"
#include "qxrdhistogramdata-ptr.h"
#include "qxrdresultserializer.h"
#include "qxrddataprocessor-ptr.h"

//TODO: eliminate
class QXRD_EXPORT QxrdDataProcessor : public QxrdProcessor
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdDataProcessor(QString name);
  virtual ~QxrdDataProcessor();
  static QxrdDataProcessorPtr newDataProcessor();

  virtual void addChildPtr(QcepObjectPtr child);
  virtual void removeChildPtr(QcepObjectPtr child);

//  QxrdExperimentWPtr  experiment() const;
  QxrdAcquisitionWPtr acquisition() const;
//  QxrdFileSaverWPtr   fileSaver() const;

public:
//  Q_PROPERTY(QString outputDirectory READ get_OutputDirectory WRITE set_OutputDirectory)
//  QCEP_STRING_PROPERTY(OutputDirectory)

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName STORED false)
  QCEP_STRING_PROPERTY(FileName)

//  Q_PROPERTY(QString dataPath   READ get_DataPath WRITE set_DataPath)
//  QCEP_STRING_PROPERTY(DataPath)

//  Q_PROPERTY(QString darkImagePath   READ get_DarkImagePath WRITE set_DarkImagePath)
//  QCEP_STRING_PROPERTY(DarkImagePath)

//  Q_PROPERTY(QString badPixelsPath   READ get_BadPixelsPath WRITE set_BadPixelsPath)
//  QCEP_STRING_PROPERTY(BadPixelsPath)

//  Q_PROPERTY(QString gainMapPath     READ get_GainMapPath WRITE set_GainMapPath)
//  QCEP_STRING_PROPERTY(GainMapPath)

//  Q_PROPERTY(QString maskPath     READ get_MaskPath WRITE set_MaskPath)
//  QCEP_STRING_PROPERTY(MaskPath)

  Q_PROPERTY(bool performDarkSubtraction READ get_PerformDarkSubtraction WRITE set_PerformDarkSubtraction)
  QCEP_BOOLEAN_PROPERTY(PerformDarkSubtraction)

  Q_PROPERTY(bool saveRawImages READ get_SaveRawImages WRITE set_SaveRawImages)
  QCEP_BOOLEAN_PROPERTY(SaveRawImages)

  Q_PROPERTY(bool saveDarkImages READ get_SaveDarkImages WRITE set_SaveDarkImages)
  QCEP_BOOLEAN_PROPERTY(SaveDarkImages)

  Q_PROPERTY(bool performBadPixels READ get_PerformBadPixels WRITE set_PerformBadPixels)
  QCEP_BOOLEAN_PROPERTY(PerformBadPixels)

  Q_PROPERTY(bool performGainCorrection READ get_PerformGainCorrection WRITE set_PerformGainCorrection)
  QCEP_BOOLEAN_PROPERTY(PerformGainCorrection)

  Q_PROPERTY(bool saveSubtracted READ get_SaveSubtracted WRITE set_SaveSubtracted)
  QCEP_BOOLEAN_PROPERTY(SaveSubtracted)

  Q_PROPERTY(bool saveAsText READ get_SaveAsText WRITE set_SaveAsText)
  QCEP_BOOLEAN_PROPERTY(SaveAsText)

  Q_PROPERTY(QString saveAsTextSeparator READ get_SaveAsTextSeparator WRITE set_SaveAsTextSeparator)
  QCEP_STRING_PROPERTY(SaveAsTextSeparator)

  Q_PROPERTY(int saveAsTextPerLine READ get_SaveAsTextPerLine WRITE set_SaveAsTextPerLine)
  QCEP_INTEGER_PROPERTY(SaveAsTextPerLine)

  Q_PROPERTY(bool saveOverflowFiles READ get_SaveOverflowFiles WRITE set_SaveOverflowFiles)
  QCEP_BOOLEAN_PROPERTY(SaveOverflowFiles)

  Q_PROPERTY(bool performIntegration READ get_PerformIntegration WRITE set_PerformIntegration)
  QCEP_BOOLEAN_PROPERTY(PerformIntegration)

  Q_PROPERTY(bool displayIntegratedData READ get_DisplayIntegratedData WRITE set_DisplayIntegratedData)
  QCEP_BOOLEAN_PROPERTY(DisplayIntegratedData)

  Q_PROPERTY(bool saveIntegratedData READ get_SaveIntegratedData WRITE set_SaveIntegratedData)
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedData)

  Q_PROPERTY(QString saveIntegratedPath READ get_SaveIntegratedPath WRITE set_SaveIntegratedPath)
  QCEP_STRING_PROPERTY(SaveIntegratedPath)

  Q_PROPERTY(bool saveDarkInSubdirectory READ get_SaveDarkInSubdirectory WRITE set_SaveDarkInSubdirectory)
  QCEP_BOOLEAN_PROPERTY(SaveDarkInSubdirectory)

  Q_PROPERTY(QString saveDarkSubdirectory READ get_SaveDarkSubdirectory WRITE set_SaveDarkSubdirectory)
  QCEP_STRING_PROPERTY(SaveDarkSubdirectory)

  Q_PROPERTY(bool saveRawInSubdirectory READ get_SaveRawInSubdirectory WRITE set_SaveRawInSubdirectory)
  QCEP_BOOLEAN_PROPERTY(SaveRawInSubdirectory)

  Q_PROPERTY(QString saveRawSubdirectory READ get_SaveRawSubdirectory WRITE set_SaveRawSubdirectory)
  QCEP_STRING_PROPERTY(SaveRawSubdirectory)

  Q_PROPERTY(bool saveSubtractedInSubdirectory READ get_SaveSubtractedInSubdirectory WRITE set_SaveSubtractedInSubdirectory)
  QCEP_BOOLEAN_PROPERTY(SaveSubtractedInSubdirectory)

  Q_PROPERTY(QString saveSubtractedSubdirectory READ get_SaveSubtractedSubdirectory WRITE set_SaveSubtractedSubdirectory)
  QCEP_STRING_PROPERTY(SaveSubtractedSubdirectory)

  Q_PROPERTY(bool saveIntegratedInSeparateFiles READ get_SaveIntegratedInSeparateFiles WRITE set_SaveIntegratedInSeparateFiles)
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedInSeparateFiles)

  Q_PROPERTY(bool saveIntegratedInSubdirectory READ get_SaveIntegratedInSubdirectory WRITE set_SaveIntegratedInSubdirectory)
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedInSubdirectory)

  Q_PROPERTY(QString saveIntegratedSubdirectory READ get_SaveIntegratedSubdirectory WRITE set_SaveIntegratedSubdirectory)
  QCEP_STRING_PROPERTY(SaveIntegratedSubdirectory)

  Q_PROPERTY(bool accumulateIntegrated2D READ get_AccumulateIntegrated2D WRITE set_AccumulateIntegrated2D)
  QCEP_BOOLEAN_PROPERTY(AccumulateIntegrated2D)

  Q_PROPERTY(QString accumulateIntegratedName READ get_AccumulateIntegratedName WRITE set_AccumulateIntegratedName)
  QCEP_STRING_PROPERTY(AccumulateIntegratedName)

  Q_PROPERTY(QString accumulateIntegratedDirectory READ get_AccumulateIntegratedDirectory WRITE set_AccumulateIntegratedDirectory)
  QCEP_STRING_PROPERTY(AccumulateIntegratedDirectory)

  Q_PROPERTY(QString accumulateIntegratedFileName READ get_AccumulateIntegratedFileName WRITE set_AccumulateIntegratedFileName)
  QCEP_STRING_PROPERTY(AccumulateIntegratedFileName)

  Q_PROPERTY(QString accumulateIntegratedFormat READ get_AccumulateIntegratedFormat WRITE set_AccumulateIntegratedFormat)
  QCEP_STRING_PROPERTY(AccumulateIntegratedFormat)

  Q_PROPERTY(double performDarkSubtractionTime READ get_PerformDarkSubtractionTime WRITE set_PerformDarkSubtractionTime)
  QCEP_DOUBLE_PROPERTY(PerformDarkSubtractionTime)

  Q_PROPERTY(double performBadPixelsTime READ get_PerformBadPixelsTime WRITE set_PerformBadPixelsTime)
  QCEP_DOUBLE_PROPERTY(PerformBadPixelsTime)

  Q_PROPERTY(double performGainCorrectionTime READ get_PerformGainCorrectionTime WRITE set_PerformGainCorrectionTime)
  QCEP_DOUBLE_PROPERTY(PerformGainCorrectionTime)

  Q_PROPERTY(double saveSubtractedTime READ get_SaveSubtractedTime WRITE set_SaveSubtractedTime)
  QCEP_DOUBLE_PROPERTY(SaveSubtractedTime)

  Q_PROPERTY(double saveAsTextTime READ get_SaveAsTextTime WRITE set_SaveAsTextTime)
  QCEP_DOUBLE_PROPERTY(SaveAsTextTime)

  Q_PROPERTY(double performIntegrationTime READ get_PerformIntegrationTime WRITE set_PerformIntegrationTime)
  QCEP_DOUBLE_PROPERTY(PerformIntegrationTime)

  Q_PROPERTY(double displayIntegratedDataTime READ get_DisplayIntegratedDataTime WRITE set_DisplayIntegratedDataTime)
  QCEP_DOUBLE_PROPERTY(DisplayIntegratedDataTime)

  Q_PROPERTY(double saveIntegratedDataTime READ get_SaveIntegratedDataTime WRITE set_SaveIntegratedDataTime)
  QCEP_DOUBLE_PROPERTY(SaveIntegratedDataTime)

  Q_PROPERTY(double estimatedProcessingTime READ get_EstimatedProcessingTime WRITE set_EstimatedProcessingTime)
  QCEP_DOUBLE_PROPERTY(EstimatedProcessingTime)

  Q_PROPERTY(double averagingRatio READ get_AveragingRatio WRITE set_AveragingRatio)
  QCEP_DOUBLE_PROPERTY(AveragingRatio)

//  Q_PROPERTY(QString fileName        READ get_FileName WRITE set_FileName)
//  QCEP_STRING_PROPERTY(FileName)

//  Q_PROPERTY(double maskMinimumValue READ get_MaskMinimumValue WRITE set_MaskMinimumValue)
//  QCEP_DOUBLE_PROPERTY(MaskMinimumValue)

//  Q_PROPERTY(double maskMaximumValue READ get_MaskMaximumValue WRITE set_MaskMaximumValue)
//  QCEP_DOUBLE_PROPERTY(MaskMaximumValue)

//  Q_PROPERTY(double maskCircleRadius READ get_MaskCircleRadius WRITE set_MaskCircleRadius)
//  QCEP_DOUBLE_PROPERTY(MaskCircleRadius)

//  Q_PROPERTY(bool maskSetPixels READ get_MaskSetPixels WRITE set_MaskSetPixels)
//  QCEP_BOOLEAN_PROPERTY(MaskSetPixels)

//  Q_PROPERTY(bool compressImages READ get_CompressImages WRITE set_CompressImages)
//  QCEP_BOOLEAN_PROPERTY(CompressImages)

//  Q_PROPERTY(double     average    READ get_Average WRITE set_Average STORED false)
//  QCEP_DOUBLE_PROPERTY(Average)

//  Q_PROPERTY(double     averageDark    READ get_AverageDark WRITE set_AverageDark STORED false)
//  QCEP_DOUBLE_PROPERTY(AverageDark)

//  Q_PROPERTY(double     averageRaw    READ get_AverageRaw WRITE set_AverageRaw STORED false)
//  QCEP_DOUBLE_PROPERTY(AverageRaw)

  Q_PROPERTY(int correctionQueueLength READ get_CorrectionQueueLength WRITE set_CorrectionQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(CorrectionQueueLength)

  Q_PROPERTY(int integrationQueueLength READ get_IntegrationQueueLength WRITE set_IntegrationQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(IntegrationQueueLength)

  Q_PROPERTY(int saverQueueLength READ get_SaverQueueLength WRITE set_SaverQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(SaverQueueLength)

//  Q_PROPERTY(int zingerAlgorithm READ get_ZingerAlgorithm WRITE set_ZingerAlgorithm)
//  QCEP_INTEGER_PROPERTY(ZingerAlgorithm)

//  Q_PROPERTY(int zingerMask READ get_ZingerMask WRITE set_ZingerMask)
//  QCEP_INTEGER_PROPERTY(ZingerMask)

//  Q_PROPERTY(int zingerDestination READ get_ZingerDestination WRITE set_ZingerDestination)
//  QCEP_INTEGER_PROPERTY(ZingerDestination)

//  Q_PROPERTY(int zingerSize1 READ get_ZingerSize1 WRITE set_ZingerSize1)
//  QCEP_INTEGER_PROPERTY(ZingerSize1)

//  Q_PROPERTY(int zingerSize2 READ get_ZingerSize2 WRITE set_ZingerSize2)
//  QCEP_INTEGER_PROPERTY(ZingerSize2)

//  Q_PROPERTY(double zingerThreshold READ get_ZingerThreshold WRITE set_ZingerThreshold)
//  QCEP_DOUBLE_PROPERTY(ZingerThreshold)

public:
//  enum {
//    NoOverwrite,
//    CanOverwrite
//  };

signals:
  void newIntegrationAvailable(QcepIntegratedDataPtr data);

public slots:
  void shutdown();

//  void showMaskRange();
//  void hideMaskRange();
//  void showMaskRange(double min, double max);
//  void hideMaskRange(double min, double max);
//  void showMaskAll();
//  void hideMaskAll();
//  void invertMask();
//  void growMask();
//  void shrinkMask();
//  void maskCircle(QRectF pt);
//  void maskPolygon(QVector<QPointF> poly);

  void measurePolygon(QVector<QPointF> poly);
  void slicePolygon(QVector<QPointF> poly);
  void printMeasuredPolygon(QVector<QPointF> poly);
  void summarizeMeasuredPolygon(QVector<QPointF> poly);
  void integrateSaveAndDisplay();

  QString pwd() const;
//  void cd(QString path);
  QStringList ls() const;
  QStringList ls(QString pattern) const;
//  void saveData(QcepDataObjectPtr object, QString name, int canOverwrite=NoOverwrite);
//  void saveData(QcepDoubleImageDataPtr data, QString name, int canOverwrite=NoOverwrite);
  void saveCachedGeometry(QString name);
  void saveCachedIntensity(QString name);

  void subtractDark();
  void unsubtractDark();
  void multiplyData(double scalar);
  void offsetData(double offset);

//  int  maskStackSize();
//  int  maskStackPosition(int pos);
//  void newEmptyMask();
//  void duplicateMask();
//  void popMaskStack(int amount=1);
//  void clearMaskStack();
//  void clearMaskStackTop();
//  void rollMaskStack(int amount);
//  void exchangeMaskStack(int pos);
//  void andMaskStack(int pos);
//  void orMaskStack(int pos);
//  void xorMaskStack(int pos);
//  void andNotMaskStack(int pos);
//  void orNotMaskStack(int pos);
//  void xorNotMaskStack(int pos);
//  void invertMaskStack(int pos);
//  void growMaskStack(int pos);
//  void shrinkMaskStack(int pos);
//  void hideMaskAllStack(int pos);
//  void showMaskAllStack(int pos);
//  void hideMaskRangeStack(int pos);
//  void showMaskRangeStack(int pos);
//  void findZingers();

  void newImage(int ncols, int nrows);
  void exponentialTail(double cx, double cy, double width, int oversample);
  void reciprocalTail(double cx, double cy, double strength, int oversample);
  void powderRing(double cx, double cy, double radius, double width, double strength, int oversample);
  void ellipse(double cx, double cy, double a, double e, double ang, double width, double strength, int oversample);

  void updateEstimatedProcessingTime();

  int status(double delay);

  void displayIntegratedData(QcepIntegratedDataPtr d);
  void writeOutputScan(QcepIntegratedDataPtr d);

  void newOutputScan(QString title);
  void appendToOutputScan(double x, double y);
  void plotOutputScan();
  void saveOutputScan(QString fileName);

  QStringList integrateRectangle(int x0, int y0, int x1, int y1);

  QString existingOutputDirectory(QString dir, QString subdir) const;
  QString filePathInExperimentDirectory(QString name) const;
  QString filePathInDarkOutputDirectory(QString name) const;
  QString filePathInRawOutputDirectory(QString name) const;
  QString filePathInSubtractedOutputDirectory(QString name) const;
  QString filePathInIntegratedOutputDirectory(QString name) const;
  QString experimentDirectory() const;
  QString darkOutputDirectory() const;
  QString rawOutputDirectory() const;
  QString subtractedOutputDirectory() const;
  QString integratedOutputDirectory() const;

//  QcepDoubleImageDataPtr liveData() const;
  QcepMaskDataPtr        overflow() const;

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

  void fixupBadBackgroundSubtraction(QString imagePattern, int nImgExposures, QString darkPath, int nDarkExposures);

public:
//  enum {
//    ZingerNoMask = -2,
//    ZingerDataMask = -1,
//    ZingerTopStack = -1
//  } ZingerConstants;

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void setAcquisition(QxrdAcquisitionWPtr acq);
//  void setWindow(QxrdWindowWPtr win);

//  QcepSettingsSaver     *saver();


//  void createMaskIfNeeded();

//  QxrdMaskStackPtr       maskStack();
//  QxrdCenterFinderPtr    centerFinder() const;
  QxrdIntegratorPtr      integrator() const;
  QxrdPolarTransformPtr  polarTransform() const;
  QxrdPolarNormalizationPtr  polarNormalization() const;

  QxrdGenerateTestImageWPtr generateTestImage() const;

  void updateEstimatedTime(QcepDoubleProperty *prop, int msec);

public slots:

  QcepDataObjectPtr integrate(QcepDoubleImageDataPtr img);
  QcepDataObjectPtr polarTransform(QcepDoubleImageDataPtr img);
  QcepDataObjectPtr polarIntegrate(QcepDoubleImageDataPtr img);

  bool integrateParameters();
  bool polarTransformParameters();
  bool polarIntegrateParameters();

protected:
//  void saveNamedImageData(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
//  void saveNamedImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
//  void saveNamedImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
//  void saveNamedRawImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
//  void saveNamedRawImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
//  void saveNamedMaskData(QString name, QcepMaskDataPtr mask, int canOverwrite=NoOverwrite);
//  void saveNamedImageDataAsText(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void writeOutputScan(QVector<double> x, QVector<double> y);

  QcepDoubleImageDataPtr processAcquiredInt16Image(QcepDoubleImageDataPtr processed, QcepUInt16ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredInt32Image(QcepDoubleImageDataPtr processed, QcepUInt32ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredDoubleImage(QcepDoubleImageDataPtr processed, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredDoubleImage(QcepDoubleImageDataPtr processed, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow, QcepDoubleList v);

  QcepDoubleImageDataPtr processAcquiredImage(QcepDoubleImageDataPtr processed, QcepDoubleImageDataPtr dimg, QcepDoubleImageDataPtr dark,
                                              QcepMaskDataPtr mask, QcepMaskDataPtr overflow, QcepDoubleList v=QcepDoubleList());

//  void newDarkImage(QcepUInt16ImageDataPtr image);
//  void newDarkImage(QcepUInt32ImageDataPtr image);

  double estimatedProcessingTime(double estSerTime, double estParallelTime);

protected:
//  void newDarkImage(QcepDoubleImageDataPtr image);
//  void newBadPixelsImage(QcepDoubleImageDataPtr image);
//  void newGainMapImage(QcepDoubleImageDataPtr image);

//  int newMaskWidth() const;
//  int newMaskHeight() const;

protected:
  int incrementAcquiredCount();
  int decrementAcquiredCount();
  int getAcquiredCount();

//  void subtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark);
//  void unsubtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark);
  void correctBadPixels(QcepDoubleImageDataPtr image);
  void correctImageGains(QcepDoubleImageDataPtr image);
  void performImageCorrections(QcepDoubleImageDataPtr image);

public:
  void idleInt16Image(QcepUInt16ImageDataPtr image, bool liveView);
//  void acquiredInt16Image(QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow);
//  void processInt32Image(QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  void processDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);
  void processDoubleImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, QcepDoubleList v);

private slots:
  void onCorrectedImageAvailable();
  void onIntegratedDataAvailable();
  void onROIDataAvailable();
  void onHistogramDataAvailable();

private:
//  QcepDoubleImageDataPtr correctInt16Image(QcepDoubleImageDataPtr corrected, QcepUInt16ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
//  QcepDoubleImageDataPtr correctInt32Image(QcepDoubleImageDataPtr corrected, QcepUInt32ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr correctDoubleImage(QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr correctDoubleImage(QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr overflow, QcepDoubleList v);
  QcepIntegratedDataPtr  integrateImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask, double, double cx);
  QxrdROIDataPtr         calculateROI(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask);
  QxrdHistogramDataPtr   calculateHistogram(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask);

private:
  mutable QMutex         m_Mutex;

protected:
//  QxrdWindowWPtr         m_Window;
  QWaitCondition         m_ProcessWaiting;
  QcepInt16ImageQueue    m_AcquiredInt16Images;
  QcepInt32ImageQueue    m_AcquiredInt32Images;
//  QcepDoubleImageDataPtr m_Data;
//  QcepDoubleImageDataPtr m_DarkFrame;
//  QcepDoubleImageDataPtr m_BadPixels;
//  QcepDoubleImageDataPtr m_GainMap;
//  QcepDoubleImageDataPtr m_LiveData;
//  QcepMaskDataPtr        m_Mask;
//  QcepMaskDataPtr        m_Overflow;

//  QxrdMaskStack          m_Masks;

  QAtomicInt             m_AcquiredCount;

//  QxrdCenterFinderPtr       m_CenterFinder;
  QxrdIntegratorPtr         m_Integrator;
  QxrdPolarTransformPtr     m_PolarTransform;
  QxrdPolarNormalizationPtr m_PolarNormalization;

  QxrdGenerateTestImagePtr m_GenerateTestImage;

  QcepIntegratedDataPtr m_OutputScan;

private:
  QxrdResultSerializer<QcepDoubleImageDataPtr>  m_CorrectedImages;
  QxrdResultSerializer<QcepIntegratedDataPtr>   m_IntegratedData;
  QxrdResultSerializer<QxrdROIDataPtr>          m_ROIData;
  QxrdResultSerializer<QxrdHistogramDataPtr>    m_HistogramData;
};

Q_DECLARE_METATYPE(QxrdDataProcessor*)

#endif
