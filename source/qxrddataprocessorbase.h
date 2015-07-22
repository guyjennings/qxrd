#ifndef QXRDDATAPROCESSORBASE_H
#define QXRDDATAPROCESSORBASE_H

#include "qcepmacros.h"

#include "qcepobject.h"
//#include <QReadWriteLock>
//#include <QAtomicInt>
#include <QWaitCondition>
//#include <QStack>

#include "qcepproperty.h"
#include "qcepimagequeue.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdmaskstack.h"
#include "qxrdmaskstack-ptr.h"
#include "qcepintegrateddata.h"
#include "qcepintegrateddata-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdgeneratetestimage-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdcenterfinder.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrddistortioncorrection-ptr.h"
#include "qxrddistortioncorrection.h"

class QxrdDataProcessorBase : public QcepObject
{
  Q_OBJECT

public:
  QxrdDataProcessorBase(QcepSettingsSaverWPtr saver,
                        QxrdExperimentWPtr doc,
                        QxrdAcquisitionWPtr acq,
                        QxrdFileSaverWPtr filesaver);
  virtual ~QxrdDataProcessorBase();

public:
//  Q_PROPERTY(QString outputDirectory READ get_OutputDirectory WRITE set_OutputDirectory)
//  QCEP_STRING_PROPERTY(OutputDirectory)

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName STORED false)
  QCEP_STRING_PROPERTY(FileName)

  Q_PROPERTY(QString dataPath   READ get_DataPath WRITE set_DataPath)
  QCEP_STRING_PROPERTY(DataPath)

  Q_PROPERTY(QString darkImagePath   READ get_DarkImagePath WRITE set_DarkImagePath)
  QCEP_STRING_PROPERTY(DarkImagePath)

  Q_PROPERTY(QString badPixelsPath   READ get_BadPixelsPath WRITE set_BadPixelsPath)
  QCEP_STRING_PROPERTY(BadPixelsPath)

  Q_PROPERTY(QString gainMapPath     READ get_GainMapPath WRITE set_GainMapPath)
  QCEP_STRING_PROPERTY(GainMapPath)

  Q_PROPERTY(QString maskPath     READ get_MaskPath WRITE set_MaskPath)
  QCEP_STRING_PROPERTY(MaskPath)

  Q_PROPERTY(QString scriptPath   READ get_ScriptPath WRITE set_ScriptPath)
  QCEP_STRING_PROPERTY(ScriptPath)

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

  Q_PROPERTY(double maskMinimumValue READ get_MaskMinimumValue WRITE set_MaskMinimumValue)
  QCEP_DOUBLE_PROPERTY(MaskMinimumValue)

  Q_PROPERTY(double maskMaximumValue READ get_MaskMaximumValue WRITE set_MaskMaximumValue)
  QCEP_DOUBLE_PROPERTY(MaskMaximumValue)

  Q_PROPERTY(double maskCircleRadius READ get_MaskCircleRadius WRITE set_MaskCircleRadius)
  QCEP_DOUBLE_PROPERTY(MaskCircleRadius)

  Q_PROPERTY(bool maskSetPixels READ get_MaskSetPixels WRITE set_MaskSetPixels)
  QCEP_BOOLEAN_PROPERTY(MaskSetPixels)

//  Q_PROPERTY(bool compressImages READ get_CompressImages WRITE set_CompressImages)
//  QCEP_BOOLEAN_PROPERTY(CompressImages)

  Q_PROPERTY(double     average    READ get_Average WRITE set_Average STORED false)
  QCEP_DOUBLE_PROPERTY(Average)

  Q_PROPERTY(double     averageDark    READ get_AverageDark WRITE set_AverageDark STORED false)
  QCEP_DOUBLE_PROPERTY(AverageDark)

  Q_PROPERTY(double     averageRaw    READ get_AverageRaw WRITE set_AverageRaw STORED false)
  QCEP_DOUBLE_PROPERTY(AverageRaw)

  Q_PROPERTY(int correctionQueueLength READ get_CorrectionQueueLength WRITE set_CorrectionQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(CorrectionQueueLength)

  Q_PROPERTY(int integrationQueueLength READ get_IntegrationQueueLength WRITE set_IntegrationQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(IntegrationQueueLength)

  Q_PROPERTY(int saverQueueLength READ get_SaverQueueLength WRITE set_SaverQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(SaverQueueLength)

public:
  enum {
    NoOverwrite,
    CanOverwrite
  };

signals:
  void newIntegrationAvailable(QcepIntegratedDataPtr data);

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

public slots:
  void shutdown();

  void showMaskRange();
  void hideMaskRange();
  void showMaskRange(double min, double max);
  void hideMaskRange(double min, double max);
  void showMaskAll();
  void hideMaskAll();
  void invertMask();
  void growMask();
  void shrinkMask();
  void maskCircle(QRectF pt);
  void maskPolygon(QVector<QPointF> poly);

  void measurePolygon(QVector<QPointF> poly);
  virtual void slicePolygon(QVector<QPointF> poly) = 0;
  void printMeasuredPolygon(QVector<QPointF> poly);
  void summarizeMeasuredPolygon(QVector<QPointF> poly);
  virtual void integrateSaveAndDisplay() = 0;

  QString pwd() const;
//  void cd(QString path);
  QStringList ls() const;
  QStringList ls(QString pattern) const;

  void loadData(QString name);
  void saveData(QString name, int canOverwrite=NoOverwrite);
  void saveData(QcepDataObjectPtr object, QString name, int canOverwrite=NoOverwrite);
//  void saveData(QcepDoubleImageDataPtr data, QString name, int canOverwrite=NoOverwrite);

  void loadDark(QString name);
  void saveDark(QString name, int canOverwrite=NoOverwrite);
  void loadMask(QString name);
  void saveMask(QString name, int canOverwrite=NoOverwrite);
  void loadBadPixels(QString name);
  void saveBadPixels(QString name, int canOverwrite=NoOverwrite);
  void loadGainMap(QString name);
  void saveGainMap(QString name, int canOverwrite=NoOverwrite);
  void saveCachedGeometry(QString name);
  void saveCachedIntensity(QString name);
  void clearDark();
  void clearMask();
  void clearBadPixels();
  void clearGainMap();

  void subtractDark();
  void unsubtractDark();
  void multiplyData(double scalar);
  void offsetData(double offset);

  int  maskStackSize();
  int  maskStackPosition(int pos);
  void newMaskStack();
  void pushMaskStack(QcepMaskDataPtr mask = QcepMaskDataPtr());
  void popMaskStack(int amount=1);
  void clearMaskStack();
  void clearMaskStackTop();
  void rollMaskStack(int amount);
  void exchangeMaskStack(int pos);
  void andMaskStack(int pos);
  void orMaskStack(int pos);
  void xorMaskStack(int pos);
  void andNotMaskStack(int pos);
  void orNotMaskStack(int pos);
  void xorNotMaskStack(int pos);
  void invertMaskStack(int pos);
  void growMaskStack(int pos);
  void shrinkMaskStack(int pos);
  void hideMaskAllStack(int pos);
  void showMaskAllStack(int pos);
  void hideMaskRangeStack(int pos);
  void showMaskRangeStack(int pos);

  void newImage(int ncols, int nrows);
  void exponentialTail(double cx, double cy, double width, int oversample);
  void reciprocalTail(double cx, double cy, double strength, int oversample);
  void powderRing(double cx, double cy, double radius, double width, double strength, int oversample);
  void ellipse(double cx, double cy, double a, double e, double ang, double width, double strength, int oversample);

  void updateEstimatedProcessingTime();

  void fileWriteTest(int dim, QString path);
  int status(double delay);

  void calculateROI();
  void calculateHistogram();

  void displayIntegratedData(QcepIntegratedDataPtr d);
  void writeOutputScan(QcepIntegratedDataPtr d);

  void newOutputScan(QString title);
  void appendToOutputScan(double x, double y);
  void plotOutputScan();
  void saveOutputScan(QString fileName);

  QStringList integrateRectangle(int x0, int y0, int x1, int y1);

  QString existingOutputDirectory(QString dir, QString subdir) const;
  QString filePathInExperimentDirectory(QString name) const;
  QString filePathInDataDirectory(QString name) const;
  QString filePathInDarkOutputDirectory(QString name) const;
  QString filePathInRawOutputDirectory(QString name) const;
  QString filePathInSubtractedOutputDirectory(QString name) const;
  QString filePathInIntegratedOutputDirectory(QString name) const;
  QString experimentDirectory() const;
  QString dataDirectory() const;
  QString darkOutputDirectory() const;
  QString rawOutputDirectory() const;
  QString subtractedOutputDirectory() const;
  QString integratedOutputDirectory() const;

  QcepDoubleImageDataPtr data() const;
  QcepDoubleImageDataPtr darkImage() const;
  QcepDoubleImageDataPtr gainMap() const;
  QcepDoubleImageDataPtr badPixels() const;
  QcepDoubleImageDataPtr liveData() const;
  QcepMaskDataPtr        mask() const;
  QcepMaskDataPtr        overflow() const;

public:
  void loadDefaultImages();

  QcepDoubleImageDataPtr takeNextFreeImage(int width, int height);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void setAcquisition(QxrdAcquisitionWPtr acq);
  void setWindow(QxrdWindowWPtr win);

//  QcepSettingsSaver     *saver();


  void createMaskIfNeeded();

  QxrdMaskStackPtr       maskStack();

  QxrdExperimentPtr      experiment() const;
  QxrdCenterFinderPtr    centerFinder() const;
  QxrdIntegratorPtr      integrator() const;

  QxrdGenerateTestImageWPtr generateTestImage() const;

  QxrdDistortionCorrectionPtr distortionCorrection() const;

  void newMask();

  void updateEstimatedTime(QcepDoubleProperty *prop, int msec);

public slots:
  void newData(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);

protected:
  void saveNamedImageData(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedImageData(QString name, QcepInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedImageData(QString name, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QcepInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedMaskData(QString name, QcepMaskDataPtr mask, int canOverwrite=NoOverwrite);
  void saveNamedImageDataAsText(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void writeOutputScan(QVector<double> x, QVector<double> y);

  QcepDoubleImageDataPtr processAcquiredInt16Image(QcepDoubleImageDataPtr processed, QcepInt16ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredInt32Image(QcepDoubleImageDataPtr processed, QcepInt32ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredDoubleImage(QcepDoubleImageDataPtr processed, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredDoubleImage(QcepDoubleImageDataPtr processed, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow, QcepDoubleList v);

  QcepDoubleImageDataPtr processAcquiredImage(QcepDoubleImageDataPtr processed, QcepDoubleImageDataPtr dimg, QcepDoubleImageDataPtr dark,
                                              QcepMaskDataPtr mask, QcepMaskDataPtr overflow, QcepDoubleList v=QcepDoubleList());

  void newDarkImage(QcepInt16ImageDataPtr image);
  void newDarkImage(QcepInt32ImageDataPtr image);

  double estimatedProcessingTime(double estSerTime, double estParallelTime);

protected:
  void newDarkImage(QcepDoubleImageDataPtr image);
  void newBadPixelsImage(QcepDoubleImageDataPtr image);
  void newGainMapImage(QcepDoubleImageDataPtr image);

  int newMaskWidth() const;
  int newMaskHeight() const;

protected:
  int incrementAcquiredCount();
  int decrementAcquiredCount();
  int getAcquiredCount();

  void subtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark);
  void unsubtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark);
  void correctBadPixels(QcepDoubleImageDataPtr image);
  void correctImageGains(QcepDoubleImageDataPtr image);
  void performImageCorrections(QcepDoubleImageDataPtr image);


private:
  mutable QMutex         m_Mutex;

protected:
  QxrdExperimentWPtr     m_Experiment;
  QcepSettingsSaverWPtr  m_Saver;
  QxrdWindowWPtr         m_Window;
  QxrdFileSaverWPtr      m_FileSaver;
  QxrdAcquisitionWPtr    m_Acquisition;
  QWaitCondition         m_ProcessWaiting;
  QcepInt16ImageQueue    m_AcquiredInt16Images;
  QcepInt32ImageQueue    m_AcquiredInt32Images;
  QcepDoubleImageDataPtr m_Data;
  QcepDoubleImageDataPtr m_DarkFrame;
  QcepDoubleImageDataPtr m_BadPixels;
  QcepDoubleImageDataPtr m_GainMap;
  QcepDoubleImageDataPtr m_LiveData;
//  QcepMaskDataPtr        m_Mask;
  QcepMaskDataPtr        m_Overflow;

  QxrdMaskStack          m_Masks;

  QAtomicInt             m_AcquiredCount;

  QxrdCenterFinderPtr    m_CenterFinder;
  QxrdIntegratorPtr      m_Integrator;

  QxrdGenerateTestImagePtr m_GenerateTestImage;

  QcepIntegratedDataPtr m_OutputScan;

  QxrdDistortionCorrectionPtr m_DistortionCorrection;
};

#endif
