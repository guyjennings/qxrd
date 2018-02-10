#ifndef QXRDPROCESSOR_H
#define QXRDPROCESSOR_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepimagedata-ptr.h"
#include "qcepimagequeue.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdprocessorstep-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdzingerfinder-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdpowderringsmodel-ptr.h"
#include "qxrdroivector-ptr.h"
#include "qxrdroimodel-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qcepintegrateddata-ptr.h"
#include "qxrdresultserializer.h"
#include "qxrdhistogramdata-ptr.h"
#include "qxrdpolartransform-ptr.h"
#include "qxrdpolarnormalization-ptr.h"
#include "qxrdgeneratetestimage-ptr.h"
#include "qxrdroicalculator-ptr.h"
#include <QWaitCondition>

//TODO: separate processing steps into sub-objects
//TODO: need separate settings class for experiment thread

class QXRD_EXPORT QxrdProcessor : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdProcessor(QString name);
  virtual ~QxrdProcessor();
  void initialize(QcepObjectWPtr parent);

  static QxrdProcessorWPtr findProcessor(QcepObjectWPtr p);

  void shutdown();

  QxrdExperimentWPtr   experiment() const;
  QxrdAcqCommonWPtr acquisition() const;
  QxrdDetectorSettingsWPtr detector() const;
  QxrdFileSaverWPtr    fileSaver() const;
  QxrdCenterFinderWPtr centerFinder() const;
  QxrdIntegratorPtr      integrator() const;
  QxrdPowderRingsModelWPtr powderRings() const;
  QxrdROIModelWPtr roiModel() const;
  QxrdPolarTransformPtr  polarTransform() const;
  QxrdPolarNormalizationPtr  polarNormalization() const;
  QxrdGenerateTestImageWPtr generateTestImage() const;
  QxrdROICalculatorPtr roiCalculator() const;

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  enum {
    NoOverwrite,
    CanOverwrite
  };

  enum {
    NoDisplayMode,
    ImageDisplayMode,
    IntegratedDisplayMode
  };

  // Data operations...
  void loadData(QString name);
  void saveData(QString name, int canOverwrite=NoOverwrite);
  void clearData();
  QcepImageDataBaseWPtr data() const;
  void newData(QcepImageDataBaseWPtr image);
  QString dataDirectory() const;
  void setDataDirectory(QString d);
  QString existingOutputDirectory(QString dir, QString subdir) const;
  QString filePathInDataDirectory(QString name) const;
  QString filePathInExperimentDirectory(QString name) const;
  QString filePathInDarkOutputDirectory(QString name) const;
  QString filePathInRawOutputDirectory(QString name) const;
  QString filePathInSubtractedOutputDirectory(QString name) const;
  QString filePathInIntegratedOutputDirectory(QString name) const;
  QString experimentDirectory() const;
  QString currentDirectory() const;
  QString darkOutputDirectory() const;
  QString rawOutputDirectory() const;
  QString subtractedOutputDirectory() const;
  QString integratedOutputDirectory() const;

  QString pwd() const;
  QStringList ls() const;
  QStringList ls(QString pattern) const;
  void cd(QString path);

  // dark operations...
  void loadDark(QString name);
  void saveDark(QString name, int canOverwrite=NoOverwrite);
  void clearDark();
  QcepDoubleImageDataWPtr dark() const;
  void newDark(QcepDoubleImageDataWPtr imageW);
  void newDarkInt16(QcepUInt16ImageDataWPtr imageW);
  void newDarkInt32(QcepUInt32ImageDataWPtr imageW);
  void subtractDark();
  void unsubtractDark();
  void multiplyData(double scalar);
  void offsetData(double offset);
  void onDarkImagePathChanged(QString newPath);

  // mask operations...
  void loadMask(QString name);
  void saveMask(QString name, int canOverwrite=NoOverwrite);
  void clearMask();
  QcepMaskDataWPtr        mask() const;
  void newMask(QcepMaskDataWPtr mask);
  void onMaskPathChanged(QString newPath);

  // bad pixel operations...
  void loadBadPixels(QString name);
  void saveBadPixels(QString name, int canOverwrite=NoOverwrite);
  void clearBadPixels();
  QcepDoubleImageDataWPtr badPixels() const;
  void newBadPixelsImage(QcepDoubleImageDataWPtr image);
  void onBadPixelsPathChanged(QString newPath);

  // gain map operations...
  void loadGainMap(QString name);
  void saveGainMap(QString name, int canOverwrite=NoOverwrite);
  void clearGainMap();
  QcepDoubleImageDataWPtr gainMap() const;
  void newGainMapImage(QcepDoubleImageDataWPtr image);
  void onGainMapPathChanged(QString newPath);

  // live data operations...
  QcepDoubleImageDataWPtr liveData() const;
  void newLiveData(QcepDoubleImageDataWPtr image);

  // overflow operations...
  QcepMaskDataWPtr overflow() const;
  void newOverflow(QcepMaskDataWPtr ovf);

  void loadDefaultImages();

  // masks...
  QxrdMaskStackWPtr maskStack() const;

  void newEmptyMask();
  void duplicateMask();

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

  QxrdZingerFinderWPtr zingerFinder() const;

  // acquired image correction...
  void correctBadPixels(QcepDoubleImageDataPtr image);
  void correctImageGains(QcepDoubleImageDataPtr image);
  void performImageCorrections(QcepDoubleImageDataPtr image);
  QcepDoubleImageDataPtr correctDoubleImage(QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr correctDoubleImage(QcepDoubleImageDataPtr corrected, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr overflow, QcepDoubleList v);

  int incrementAcquiredCount();
  int decrementAcquiredCount();
  int getAcquiredCount();

  // processing...
  void processData(QString name);
  void idleInt16Image(QcepUInt16ImageDataPtr image, bool liveView);
  void processDoubleImage(QcepDoubleImageDataPtr image,
                          QcepMaskDataPtr overflow);
  void processDoubleImage(QcepDoubleImageDataPtr image,
                          QcepMaskDataPtr overflow,
                          QcepDoubleList v);

  void processAcquiredImage(QcepUInt32ImageDataPtr image,
                            QcepMaskDataPtr overflow,
                            int fileIndex,
                            int phase,
                            int nPhases,
                            bool trig);

  void processDarkImage(QcepDoubleImageDataPtr image,
                        QcepMaskDataPtr overflow,
                        int fileIndex);

  void processIdleImage(QcepImageDataBasePtr image);

  void setAcquiredImageProperties(QcepImageDataBasePtr image,
                                  int fileIndex,
                                  int phase,
                                  int nPhases,
                                  bool trig);

  QcepImageDataBasePtr doDarkSubtraction    (QcepImageDataBasePtr img);
  QcepImageDataBasePtr doBadPixels          (QcepImageDataBasePtr img);
  QcepImageDataBasePtr doGainCorrection     (QcepImageDataBasePtr img);
//  QcepDoubleVector     doCalculateROICounts (QcepImageDataBasePtr img);
  void                 doSaveRawImage       (QcepImageDataBasePtr img,
                                             QcepMaskDataPtr ovf);
  void                 doSaveSubtractedImage(QcepImageDataBasePtr img,
                                             QcepMaskDataPtr ovf);
  void                 doSaveDarkImage      (QcepImageDataBasePtr img,
                                             QcepMaskDataPtr ovf);

  QcepDoubleImageDataPtr processAcquiredInt16Image(QcepDoubleImageDataPtr processed, QcepUInt16ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredInt32Image(QcepDoubleImageDataPtr processed, QcepUInt32ImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredDoubleImage(QcepDoubleImageDataPtr processed, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepDoubleImageDataPtr processAcquiredDoubleImage(QcepDoubleImageDataPtr processed, QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark, QcepMaskDataPtr mask, QcepMaskDataPtr overflow, QcepDoubleList v);
  QcepDoubleImageDataPtr processAcquiredImage(QcepDoubleImageDataPtr processed,
                                              QcepDoubleImageDataPtr dimg,
                                              QcepDoubleImageDataPtr dark,
                                              QcepMaskDataPtr mask,
                                              QcepMaskDataPtr overflow,
                                              QcepDoubleList v=QcepDoubleList());

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

  // integration...
  void integrateData(QString name);
  void integrateSaveAndDisplay();

  void clearAccumulator();
  void integrateAndAccumulate(QStringList names);
  void saveAccumulator(QString &path, QString filter);
  void displayIntegratedData(QcepIntegratedDataPtr d);

  void writeOutputScan(QVector<double> x, QVector<double> y);
  void writeOutputScan(QcepIntegratedDataPtr d);

  bool integrateParameters();
  bool polarTransformParameters();
  bool polarIntegrateParameters();

  QStringList integrateRectangle(int x0, int y0, int x1, int y1);
  QcepDataObjectPtr integrate(QcepDoubleImageDataPtr img);
  QcepDataObjectPtr polarTransform(QcepDoubleImageDataPtr img);
  QcepDataObjectPtr polarIntegrate(QcepDoubleImageDataPtr img);

  // histogram...
  QxrdHistogramDataPtr   calculateHistogram(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask);

  // save cached data...
  void saveCachedGeometry(QString name);
  void saveCachedIntensity(QString name);

  double estimatedProcessingTime(double estSerTime, double estParallelTime);
  void updateEstimatedTime(QcepDoubleProperty *prop, int msec);
  void updateEstimatedProcessingTime();

  void shiftImage(int dx, int dy);
  void sumImages(QStringList names);
  void addImages(QStringList names);
  void subtractImages(QStringList names);
  void projectImages(QStringList names, int px, int py, int pz);
  void reflectVertically();
  void reflectHorizontally();

  void measurePolygon(QVector<QPointF> poly);
  void slicePolygon(QVector<QPointF> poly);
  void printMeasuredPolygon(QVector<QPointF> poly);
  void summarizeMeasuredPolygon(QVector<QPointF> poly);

  int status(double delay);

  void fitPeakNear(double x, double y);

  void correlateImages(QStringList names);

  // ROI calculation
  QcepDoubleVector     doCalculateROICounts (QcepImageDataBasePtr img);

  // Image generation
  void newImage(int ncols, int nrows);
  void exponentialTail(double cx, double cy, double width, int oversample);
  void reciprocalTail(double cx, double cy, double strength, int oversample);
  void powderRing(double cx, double cy, double radius, double width, double strength, int oversample);
  void ellipse(double cx, double cy, double a, double e, double ang, double width, double strength, int oversample);

protected:
  void subtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark);
  void unsubtractDarkImage(QcepDoubleImageDataPtr image, QcepDoubleImageDataPtr dark);

  void saveNamedImageData(QString name, QcepImageDataBasePtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedDoubleImageData(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedUInt16ImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedUInt32ImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedMaskData(QString name, QcepMaskDataPtr mask, int canOverwrite=NoOverwrite);
  void saveNamedImageDataAsText(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite=NoOverwrite);

  void createMaskIfNeeded();
  int newMaskWidth() const;
  int newMaskHeight() const;

signals:
  void dataAvailable           (QcepDoubleImageDataPtr img);
  void darkAvailable           (QcepDoubleImageDataPtr img);
  void maskAvailable           (QcepMaskDataPtr        mask);
  void badPixelsAvailable      (QcepDoubleImageDataPtr img);
  void gainMapAvailable        (QcepDoubleImageDataPtr map);
  void liveDataAvailable       (QcepDoubleImageDataPtr img);
  void overflowAvailable       (QcepMaskDataPtr        ovf);
  void newIntegrationAvailable (QcepIntegratedDataPtr  data);

private slots:
  void onCorrectedImageAvailable();
  void onIntegratedDataAvailable();
  void onHistogramDataAvailable();

private:
  QcepIntegratedDataPtr  integrateImage(QcepDoubleImageDataPtr image, QcepMaskDataPtr mask, double, double cx);

public:
  // Properties...

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName STORED false)
  QCEP_STRING_PROPERTY(FileName)

  Q_PROPERTY(QString dataPath   READ get_DataPath WRITE set_DataPath STORED false)
  QCEP_STRING_PROPERTY(DataPath)

  //TODO: rename darkImage
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

  Q_PROPERTY(double     average    READ get_Average WRITE set_Average STORED false)
  QCEP_DOUBLE_PROPERTY(Average)

  Q_PROPERTY(double     averageDark    READ get_AverageDark WRITE set_AverageDark STORED false)
  QCEP_DOUBLE_PROPERTY(AverageDark)

  Q_PROPERTY(double     averageRaw    READ get_AverageRaw WRITE set_AverageRaw STORED false)
  QCEP_DOUBLE_PROPERTY(AverageRaw)

  Q_PROPERTY(int detectorDisplayMode READ get_DetectorDisplayMode WRITE set_DetectorDisplayMode)
  QCEP_INTEGER_PROPERTY(DetectorDisplayMode)

  Q_PROPERTY(double maskMinimumValue READ get_MaskMinimumValue WRITE set_MaskMinimumValue)
  QCEP_DOUBLE_PROPERTY(MaskMinimumValue)

  Q_PROPERTY(double maskMaximumValue READ get_MaskMaximumValue WRITE set_MaskMaximumValue)
  QCEP_DOUBLE_PROPERTY(MaskMaximumValue)

  Q_PROPERTY(double maskCircleRadius READ get_MaskCircleRadius WRITE set_MaskCircleRadius)
  QCEP_DOUBLE_PROPERTY(MaskCircleRadius)

  Q_PROPERTY(bool maskSetPixels READ get_MaskSetPixels WRITE set_MaskSetPixels)
  QCEP_BOOLEAN_PROPERTY(MaskSetPixels)

  Q_PROPERTY(bool performBadPixels READ get_PerformBadPixels WRITE set_PerformBadPixels)
  QCEP_BOOLEAN_PROPERTY(PerformBadPixels)

  Q_PROPERTY(double performBadPixelsTime READ get_PerformBadPixelsTime WRITE set_PerformBadPixelsTime)
  QCEP_DOUBLE_PROPERTY(PerformBadPixelsTime)

  Q_PROPERTY(bool performGainCorrection READ get_PerformGainCorrection WRITE set_PerformGainCorrection)
  QCEP_BOOLEAN_PROPERTY(PerformGainCorrection)

  Q_PROPERTY(double performGainCorrectionTime READ get_PerformGainCorrectionTime WRITE set_PerformGainCorrectionTime)
  QCEP_DOUBLE_PROPERTY(PerformGainCorrectionTime)

  Q_PROPERTY(bool performDarkSubtraction READ get_PerformDarkSubtraction WRITE set_PerformDarkSubtraction)
  QCEP_BOOLEAN_PROPERTY(PerformDarkSubtraction)

  Q_PROPERTY(double performDarkSubtractionTime READ get_PerformDarkSubtractionTime WRITE set_PerformDarkSubtractionTime)
  QCEP_DOUBLE_PROPERTY(PerformDarkSubtractionTime)

  Q_PROPERTY(bool saveSubtracted READ get_SaveSubtracted WRITE set_SaveSubtracted)
  QCEP_BOOLEAN_PROPERTY(SaveSubtracted)

  Q_PROPERTY(double saveSubtractedTime READ get_SaveSubtractedTime WRITE set_SaveSubtractedTime)
  QCEP_DOUBLE_PROPERTY(SaveSubtractedTime)

  Q_PROPERTY(bool saveSubtractedInSubdirectory READ get_SaveSubtractedInSubdirectory WRITE set_SaveSubtractedInSubdirectory)
  QCEP_BOOLEAN_PROPERTY(SaveSubtractedInSubdirectory)

  Q_PROPERTY(QString saveSubtractedSubdirectory READ get_SaveSubtractedSubdirectory WRITE set_SaveSubtractedSubdirectory)
  QCEP_STRING_PROPERTY(SaveSubtractedSubdirectory)

  Q_PROPERTY(bool saveRawImages READ get_SaveRawImages WRITE set_SaveRawImages)
  QCEP_BOOLEAN_PROPERTY(SaveRawImages)

  Q_PROPERTY(bool saveRawInSubdirectory READ get_SaveRawInSubdirectory WRITE set_SaveRawInSubdirectory)
  QCEP_BOOLEAN_PROPERTY(SaveRawInSubdirectory)

  Q_PROPERTY(QString saveRawSubdirectory READ get_SaveRawSubdirectory WRITE set_SaveRawSubdirectory)
  QCEP_STRING_PROPERTY(SaveRawSubdirectory)

  Q_PROPERTY(bool saveDarkImages READ get_SaveDarkImages WRITE set_SaveDarkImages)
  QCEP_BOOLEAN_PROPERTY(SaveDarkImages)

  Q_PROPERTY(bool saveDarkInSubdirectory READ get_SaveDarkInSubdirectory WRITE set_SaveDarkInSubdirectory)
  QCEP_BOOLEAN_PROPERTY(SaveDarkInSubdirectory)

  Q_PROPERTY(QString saveDarkSubdirectory READ get_SaveDarkSubdirectory WRITE set_SaveDarkSubdirectory)
  QCEP_STRING_PROPERTY(SaveDarkSubdirectory)

  Q_PROPERTY(bool saveAsText READ get_SaveAsText WRITE set_SaveAsText)
  QCEP_BOOLEAN_PROPERTY(SaveAsText)

  Q_PROPERTY(double saveAsTextTime READ get_SaveAsTextTime WRITE set_SaveAsTextTime)
  QCEP_DOUBLE_PROPERTY(SaveAsTextTime)

  Q_PROPERTY(QString saveAsTextSeparator READ get_SaveAsTextSeparator WRITE set_SaveAsTextSeparator)
  QCEP_STRING_PROPERTY(SaveAsTextSeparator)

  Q_PROPERTY(int saveAsTextPerLine READ get_SaveAsTextPerLine WRITE set_SaveAsTextPerLine)
  QCEP_INTEGER_PROPERTY(SaveAsTextPerLine)

  Q_PROPERTY(bool saveOverflowFiles READ get_SaveOverflowFiles WRITE set_SaveOverflowFiles)
  QCEP_BOOLEAN_PROPERTY(SaveOverflowFiles)

  Q_PROPERTY(int correctionQueueLength READ get_CorrectionQueueLength WRITE set_CorrectionQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(CorrectionQueueLength)

  Q_PROPERTY(bool performIntegration READ get_PerformIntegration WRITE set_PerformIntegration)
  QCEP_BOOLEAN_PROPERTY(PerformIntegration)

  Q_PROPERTY(double performIntegrationTime READ get_PerformIntegrationTime WRITE set_PerformIntegrationTime)
  QCEP_DOUBLE_PROPERTY(PerformIntegrationTime)

  Q_PROPERTY(int integrationQueueLength READ get_IntegrationQueueLength WRITE set_IntegrationQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(IntegrationQueueLength)

  Q_PROPERTY(bool displayIntegratedData READ get_DisplayIntegratedData WRITE set_DisplayIntegratedData)
  QCEP_BOOLEAN_PROPERTY(DisplayIntegratedData)

  Q_PROPERTY(double displayIntegratedDataTime READ get_DisplayIntegratedDataTime WRITE set_DisplayIntegratedDataTime)
  QCEP_DOUBLE_PROPERTY(DisplayIntegratedDataTime)

  Q_PROPERTY(bool saveIntegratedData READ get_SaveIntegratedData WRITE set_SaveIntegratedData)
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedData)

  Q_PROPERTY(double saveIntegratedDataTime READ get_SaveIntegratedDataTime WRITE set_SaveIntegratedDataTime)
  QCEP_DOUBLE_PROPERTY(SaveIntegratedDataTime)

  Q_PROPERTY(QString saveIntegratedPath READ get_SaveIntegratedPath WRITE set_SaveIntegratedPath)
  QCEP_STRING_PROPERTY(SaveIntegratedPath)

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

  Q_PROPERTY(int histogramQueueLength READ get_HistogramQueueLength WRITE set_HistogramQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(HistogramQueueLength)

  Q_PROPERTY(int saverQueueLength READ get_SaverQueueLength WRITE set_SaverQueueLength STORED false)
  QCEP_INTEGER_PROPERTY(SaverQueueLength)

  Q_PROPERTY(double estimatedProcessingTime READ get_EstimatedProcessingTime WRITE set_EstimatedProcessingTime)
  QCEP_DOUBLE_PROPERTY(EstimatedProcessingTime)

  Q_PROPERTY(double averagingRatio READ get_AveragingRatio WRITE set_AveragingRatio)
  QCEP_DOUBLE_PROPERTY(AveragingRatio)

  Q_PROPERTY(bool calculateROICounts READ get_CalculateROICounts WRITE set_CalculateROICounts)
  QCEP_BOOLEAN_PROPERTY(CalculateROICounts)

  Q_PROPERTY(bool displayROIBorders READ get_DisplayROIBorders WRITE set_DisplayROIBorders)
  QCEP_BOOLEAN_PROPERTY(DisplayROIBorders)

  Q_PROPERTY(QcepDoubleVector roiCounts READ get_RoiCounts WRITE set_RoiCounts STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(RoiCounts)

protected:
  QcepImageDataBasePtr   m_Data;
  QcepDoubleImageDataPtr m_Dark;
  QcepDoubleImageDataPtr m_BadPixels;
  QcepDoubleImageDataPtr m_GainMap;
  QcepDoubleImageDataPtr m_LiveData;
  QcepMaskDataPtr        m_Overflow;

  QxrdMaskStackPtr       m_MaskStack;
  QxrdZingerFinderPtr    m_ZingerFinder;

  QVector<QxrdProcessorStepPtr> m_ProcessorSteps;

private:
  QxrdCenterFinderPtr       m_CenterFinder;
  QxrdIntegratorPtr         m_Integrator;
  QxrdPolarTransformPtr     m_PolarTransform;
  QxrdPolarNormalizationPtr m_PolarNormalization;
  QxrdGenerateTestImagePtr  m_GenerateTestImage;
  QxrdROICalculatorPtr      m_ROICalculator;
  QxrdFileSaverWPtr         m_FileSaver;

  //TODO: store a data object, not a model
  QxrdPowderRingsModelPtr        m_PowderRings;

  QxrdROIVectorPtr       m_ROIVector;
  QxrdROIModelPtr        m_ROIModel;

  mutable QMutex         m_Mutex;
  QWaitCondition         m_ProcessWaiting;
  QcepInt16ImageQueue    m_AcquiredInt16Images;
  QcepInt32ImageQueue    m_AcquiredInt32Images;
  QAtomicInt             m_AcquiredCount;

  QxrdResultSerializer<QcepDoubleImageDataPtr>  m_CorrectedImages;
  QxrdResultSerializer<QcepIntegratedDataPtr>   m_IntegratedData;
  QxrdResultSerializer<QxrdHistogramDataPtr>    m_HistogramData;
};

#endif // QXRDPROCESSOR_H
