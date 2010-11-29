#ifndef QXRDDATAPROCESSORBASE_H
#define QXRDDATAPROCESSORBASE_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>
#include <QAtomicInt>
#include <QWaitCondition>
#include <QStack>

#include "qxrdforwardtypes.h"
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddoubleimagedata.h"
#include "qxrdintegrateddata.h"
#include "qxrdringsetfitparameters.h"
#include "qxrdringsetsampleddata.h"

#include <qwt_double_rect.h>

class QxrdDataProcessorBase : public QObject
{
  Q_OBJECT;

public:
  QxrdDataProcessorBase(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent=0);
  ~QxrdDataProcessorBase();

public:
  Q_PROPERTY(QString outputDirectory READ get_OutputDirectory WRITE set_OutputDirectory);
  QCEP_STRING_PROPERTY(OutputDirectory);

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName STORED false);
  QCEP_STRING_PROPERTY(FileName);

  Q_PROPERTY(QString dataPath   READ get_DataPath WRITE set_DataPath);
  QCEP_STRING_PROPERTY(DataPath);

  Q_PROPERTY(QString darkImagePath   READ get_DarkImagePath WRITE set_DarkImagePath);
  QCEP_STRING_PROPERTY(DarkImagePath);

  Q_PROPERTY(QString badPixelsPath   READ get_BadPixelsPath WRITE set_BadPixelsPath);
  QCEP_STRING_PROPERTY(BadPixelsPath);

  Q_PROPERTY(QString gainMapPath     READ get_GainMapPath WRITE set_GainMapPath);
  QCEP_STRING_PROPERTY(GainMapPath);

  Q_PROPERTY(QString maskPath     READ get_MaskPath WRITE set_MaskPath);
  QCEP_STRING_PROPERTY(MaskPath);

  Q_PROPERTY(QString logFilePath     READ get_LogFilePath WRITE set_LogFilePath);
  QCEP_STRING_PROPERTY(LogFilePath);

  Q_PROPERTY(bool performDarkSubtraction READ get_PerformDarkSubtraction WRITE set_PerformDarkSubtraction);
  QCEP_BOOLEAN_PROPERTY(PerformDarkSubtraction);

  Q_PROPERTY(bool saveRawImages READ get_SaveRawImages WRITE set_SaveRawImages);
  QCEP_BOOLEAN_PROPERTY(SaveRawImages);

  Q_PROPERTY(bool saveDarkImages READ get_SaveDarkImages WRITE set_SaveDarkImages);
  QCEP_BOOLEAN_PROPERTY(SaveDarkImages);

  Q_PROPERTY(bool performBadPixels READ get_PerformBadPixels WRITE set_PerformBadPixels);
  QCEP_BOOLEAN_PROPERTY(PerformBadPixels);

  Q_PROPERTY(bool performGainCorrection READ get_PerformGainCorrection WRITE set_PerformGainCorrection);
  QCEP_BOOLEAN_PROPERTY(PerformGainCorrection);

  Q_PROPERTY(bool saveSubtracted READ get_SaveSubtracted WRITE set_SaveSubtracted);
  QCEP_BOOLEAN_PROPERTY(SaveSubtracted);

  Q_PROPERTY(bool saveAsText READ get_SaveAsText WRITE set_SaveAsText);
  QCEP_BOOLEAN_PROPERTY(SaveAsText);

  Q_PROPERTY(QString saveAsTextSeparator READ get_SaveAsTextSeparator WRITE set_SaveAsTextSeparator);
  QCEP_STRING_PROPERTY(SaveAsTextSeparator);

  Q_PROPERTY(int saveAsTextPerLine READ get_SaveAsTextPerLine WRITE set_SaveAsTextPerLine);
  QCEP_INTEGER_PROPERTY(SaveAsTextPerLine);

  Q_PROPERTY(bool saveOverflowFiles READ get_SaveOverflowFiles WRITE set_SaveOverflowFiles);
  QCEP_BOOLEAN_PROPERTY(SaveOverflowFiles);

  Q_PROPERTY(bool performIntegration READ get_PerformIntegration WRITE set_PerformIntegration);
  QCEP_BOOLEAN_PROPERTY(PerformIntegration);

  Q_PROPERTY(bool displayIntegratedData READ get_DisplayIntegratedData WRITE set_DisplayIntegratedData);
  QCEP_BOOLEAN_PROPERTY(DisplayIntegratedData);

  Q_PROPERTY(bool saveIntegratedData READ get_SaveIntegratedData WRITE set_SaveIntegratedData);
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedData);

  Q_PROPERTY(bool saveDarkInSubdirectory READ get_SaveDarkInSubdirectory WRITE set_SaveDarkInSubdirectory);
  QCEP_BOOLEAN_PROPERTY(SaveDarkInSubdirectory);

  Q_PROPERTY(QString saveDarkSubdirectory READ get_SaveDarkSubdirectory WRITE set_SaveDarkSubdirectory);
  QCEP_STRING_PROPERTY(SaveDarkSubdirectory);

  Q_PROPERTY(bool saveRawInSubdirectory READ get_SaveRawInSubdirectory WRITE set_SaveRawInSubdirectory);
  QCEP_BOOLEAN_PROPERTY(SaveRawInSubdirectory);

  Q_PROPERTY(QString saveRawSubdirectory READ get_SaveRawSubdirectory WRITE set_SaveRawSubdirectory);
  QCEP_STRING_PROPERTY(SaveRawSubdirectory);

  Q_PROPERTY(bool saveSubtractedInSubdirectory READ get_SaveSubtractedInSubdirectory WRITE set_SaveSubtractedInSubdirectory);
  QCEP_BOOLEAN_PROPERTY(SaveSubtractedInSubdirectory);

  Q_PROPERTY(QString saveSubtractedSubdirectory READ get_SaveSubtractedSubdirectory WRITE set_SaveSubtractedSubdirectory);
  QCEP_STRING_PROPERTY(SaveSubtractedSubdirectory);

  Q_PROPERTY(bool saveIntegratedInSeparateFiles READ get_SaveIntegratedInSeparateFiles WRITE set_SaveIntegratedInSeparateFiles);
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedInSeparateFiles);

  Q_PROPERTY(bool saveIntegratedInSubdirectory READ get_SaveIntegratedInSubdirectory WRITE set_SaveIntegratedInSubdirectory);
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedInSubdirectory);

  Q_PROPERTY(QString saveIntegratedSubdirectory READ get_SaveIntegratedSubdirectory WRITE set_SaveIntegratedSubdirectory);
  QCEP_STRING_PROPERTY(SaveIntegratedSubdirectory);

  Q_PROPERTY(double performDarkSubtractionTime READ get_PerformDarkSubtractionTime WRITE set_PerformDarkSubtractionTime);
  QCEP_DOUBLE_PROPERTY(PerformDarkSubtractionTime);

  Q_PROPERTY(double performBadPixelsTime READ get_PerformBadPixelsTime WRITE set_PerformBadPixelsTime);
  QCEP_DOUBLE_PROPERTY(PerformBadPixelsTime);

  Q_PROPERTY(double performGainCorrectionTime READ get_PerformGainCorrectionTime WRITE set_PerformGainCorrectionTime);
  QCEP_DOUBLE_PROPERTY(PerformGainCorrectionTime);

  Q_PROPERTY(double saveSubtractedTime READ get_SaveSubtractedTime WRITE set_SaveSubtractedTime);
  QCEP_DOUBLE_PROPERTY(SaveSubtractedTime);

  Q_PROPERTY(double saveAsTextTime READ get_SaveAsTextTime WRITE set_SaveAsTextTime);
  QCEP_DOUBLE_PROPERTY(SaveAsTextTime);

  Q_PROPERTY(double performIntegrationTime READ get_PerformIntegrationTime WRITE set_PerformIntegrationTime);
  QCEP_DOUBLE_PROPERTY(PerformIntegrationTime);

  Q_PROPERTY(double displayIntegratedDataTime READ get_DisplayIntegratedDataTime WRITE set_DisplayIntegratedDataTime);
  QCEP_DOUBLE_PROPERTY(DisplayIntegratedDataTime);

  Q_PROPERTY(double saveIntegratedDataTime READ get_SaveIntegratedDataTime WRITE set_SaveIntegratedDataTime);
  QCEP_DOUBLE_PROPERTY(SaveIntegratedDataTime);

  Q_PROPERTY(double estimatedProcessingTime READ get_EstimatedProcessingTime WRITE set_EstimatedProcessingTime);
  QCEP_DOUBLE_PROPERTY(EstimatedProcessingTime);

  Q_PROPERTY(double averagingRatio READ get_AveragingRatio WRITE set_AveragingRatio);
  QCEP_DOUBLE_PROPERTY(AveragingRatio);

//  Q_PROPERTY(QString fileName        READ get_FileName WRITE set_FileName);
//  QCEP_STRING_PROPERTY(FileName);

  Q_PROPERTY(double maskMinimumValue READ get_MaskMinimumValue WRITE set_MaskMinimumValue);
  QCEP_DOUBLE_PROPERTY(MaskMinimumValue);

  Q_PROPERTY(double maskMaximumValue READ get_MaskMaximumValue WRITE set_MaskMaximumValue);
  QCEP_DOUBLE_PROPERTY(MaskMaximumValue);

  Q_PROPERTY(double maskCircleRadius READ get_MaskCircleRadius WRITE set_MaskCircleRadius);
  QCEP_DOUBLE_PROPERTY(MaskCircleRadius);

  Q_PROPERTY(bool maskSetPixels READ get_MaskSetPixels WRITE set_MaskSetPixels);
  QCEP_BOOLEAN_PROPERTY(MaskSetPixels);

  Q_PROPERTY(bool compressImages READ get_CompressImages WRITE set_CompressImages);
  QCEP_BOOLEAN_PROPERTY(CompressImages);

  Q_PROPERTY(double     average    READ get_Average WRITE set_Average STORED false);
  QCEP_DOUBLE_PROPERTY(Average);

  Q_PROPERTY(double     averageDark    READ get_AverageDark WRITE set_AverageDark STORED false);
  QCEP_DOUBLE_PROPERTY(AverageDark);

  Q_PROPERTY(double     averageRaw    READ get_AverageRaw WRITE set_AverageRaw STORED false);
  QCEP_DOUBLE_PROPERTY(AverageRaw);

public:
  enum {
    NoOverwrite,
    CanOverwrite
  };

signals:
  void printMessage(QDateTime ts, QString msg) const;
  void statusMessage(QDateTime ts, QString msg) const;
  void criticalMessage(QDateTime ts, QString msg) const;

  void newIntegrationAvailable(QxrdIntegratedDataPtr data);

public slots:
  void showMaskRange(/*double min, double max*/);
  void hideMaskRange(/*double min, double max*/);
  void showMaskAll();
  void hideMaskAll();
  void invertMask();
  void maskCircle(QwtDoubleRect pt);
  void maskPolygon(QwtArray<QwtDoublePoint> poly);

  void measurePolygon(QwtArray<QwtDoublePoint> poly);
  virtual void slicePolygon(QwtArray<QwtDoublePoint> poly) = 0;
  void printMeasuredPolygon(QwtArray<QwtDoublePoint> poly);
  void summarizeMeasuredPolygon(QwtArray<QwtDoublePoint> poly);
  virtual void integrateSaveAndDisplay() = 0;

  void loadData(QString name);
  void saveData(QString name, int canOverwrite=NoOverwrite);
  void loadDark(QString name);
  void saveDark(QString name, int canOverwrite=NoOverwrite);
  void loadMask(QString name);
  void saveMask(QString name, int canOverwrite=NoOverwrite);
  void loadBadPixels(QString name);
  void saveBadPixels(QString name, int canOverwrite=NoOverwrite);
  void loadGainMap(QString name);
  void saveGainMap(QString name, int canOverwrite=NoOverwrite);
  void clearDark();
  void clearBadPixels();
  void clearGainMap();
  void clearMask();

  int  maskStackSize();
  int  maskStackPosition(int pos);
  void pushMaskStack(QxrdMaskDataPtr mask = QxrdMaskDataPtr());
  void popMaskStack(int amount=1);
  void clearMaskStack();
  void rollMaskStack(int amount);
  void exchangeMaskStack(int pos);
  void andMaskStack(int pos);
  void orMaskStack(int pos);
  void xorMaskStack(int pos);
  void andNotMaskStack(int pos);
  void orNotMaskStack(int pos);
  void xorNotMaskStack(int pos);
  void invertMaskStack(int pos);
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

  void newLogFile(QString path);
  void logMessage(QString msg);
  void closeLogFile();

  void fileWriteTest(int dim, QString path);
  int status(double delay);

  void calculateROI();
  void calculateHistogram();

  void displayIntegratedData(QxrdIntegratedDataPtr d);
  void writeOutputScan(QxrdIntegratedDataPtr d);

  virtual void processorOptionsDialog();

public:
  QString existingOutputDirectory(QString dir, QString subdir);
  QString darkOutputDirectory();
  QString rawOutputDirectory();
  QString subtractedOutputDirectory();
  QString integratedOutputDirectory();

  void loadDefaultImages();

  QxrdDoubleImageDataPtr takeNextFreeImage();

  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

  void setAcquisition(QxrdAcquisitionPtr acq);
  void setWindow(QxrdWindowPtr win);

  QxrdDoubleImageDataPtr data() const;
  QxrdDoubleImageDataPtr darkImage() const;
  QxrdMaskDataPtr        mask() const;
  QxrdMaskDataPtr        overflow() const;

  QxrdCenterFinderPtr    centerFinder() const;
  QxrdIntegratorPtr      integrator() const;
  QxrdRingSetFitParametersPtr initialRingSetFitParameters() const;
  QxrdRingSetSampledDataPtr   initialRingSetData() const;
  QxrdRingSetFitParametersPtr refinedRingSetFitParameters() const;
  QxrdRingSetSampledDataPtr   refinedRingSetData() const;

  QxrdFileSaverThreadPtr fileSaverThread() const;

  QxrdGenerateTestImagePtr generateTestImage() const;

  void newMask();

  void updateEstimatedTime(QcepDoubleProperty *prop, int msec);
  void newData(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

protected:
  void saveNamedImageData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedImageData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedImageData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void saveNamedMaskData(QString name, QxrdMaskDataPtr mask, int canOverwrite=NoOverwrite);
  void saveNamedImageDataAsText(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite=NoOverwrite);
  void writeOutputScan(QVector<double> x, QVector<double> y);

  QxrdDoubleImageDataPtr processAcquiredInt16Image(QxrdInt16ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr processAcquiredInt32Image(QxrdInt32ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr processAcquiredDoubleImage(QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr processAcquiredImage(QxrdDoubleImageDataPtr dimg, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);

  void newDarkImage(QxrdInt16ImageDataPtr image);
  void newDarkImage(QxrdInt32ImageDataPtr image);

  double estimatedProcessingTime(double estSerTime, double estParallelTime);

protected:
  void newDarkImage(QxrdDoubleImageDataPtr image);
  void newBadPixelsImage(QxrdDoubleImageDataPtr image);
  void newGainMapImage(QxrdDoubleImageDataPtr image);

private:
  int incrementAcquiredCount();
  int decrementAcquiredCount();
  int getAcquiredCount();

  void subtractDarkImage(QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark);
  void correctBadPixels(QxrdDoubleImageDataPtr image);
  void correctImageGains(QxrdDoubleImageDataPtr image);
  void performImageCorrections(QxrdDoubleImageDataPtr image);


  void openLogFile();
  void writeLogHeader();


private:
  mutable QMutex         m_Mutex;
  mutable QMutex         m_LogFileMutex;

protected:
  QxrdWindowPtr          m_Window;
  QxrdAllocatorPtr       m_Allocator;
  QxrdFileSaverThreadPtr m_FileSaverThread;
  QxrdAcquisitionPtr     m_Acquisition;
  QWaitCondition         m_ProcessWaiting;
  QxrdInt16ImageQueue    m_AcquiredInt16Images;
  QxrdInt32ImageQueue    m_AcquiredInt32Images;
  QxrdDoubleImageDataPtr m_Data;
  QxrdDoubleImageDataPtr m_DarkFrame;
  QxrdDoubleImageDataPtr m_BadPixels;
  QxrdDoubleImageDataPtr m_GainMap;
  QxrdMaskDataPtr        m_Mask;
  QxrdMaskDataPtr        m_Overflow;

  QStack<QxrdMaskDataPtr> m_Masks;

  QAtomicInt             m_AcquiredCount;

  QxrdCenterFinderPtr    m_CenterFinder;
  QxrdIntegratorPtr      m_Integrator;

  QxrdRingSetFitParametersPtr m_InitialRingSetFitParameters;
  QxrdRingSetFitParametersPtr m_RefinedRingSetFitParameters;

  QxrdRingSetSampledDataPtr m_InitialRingSetData;
  QxrdRingSetSampledDataPtr m_RefinedRingSetData;

  QxrdGenerateTestImagePtr m_GenerateTestImage;

  FILE                  *m_LogFile;
};

#endif
