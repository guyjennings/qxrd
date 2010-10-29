#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>
#include <QAtomicInt>
#include <QWaitCondition>

#include "qxrdforwardtypes.h"
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddoubleimagedata.h"
#include "qxrdacquisition.h"
#include "qxrdintegrateddata.h"
#include "qxrdroidata.h"
#include "qxrdhistogramdata.h"
#include "qxrdringsetfitparameters.h"
#include "qxrdringsetsampleddata.h"

#include <qwt_double_rect.h>

class QxrdDataProcessor : public QObject
{
  Q_OBJECT;

public:
  QxrdDataProcessor(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent=0);
  ~QxrdDataProcessor();

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

  Q_PROPERTY(bool performIntegration READ get_PerformIntegration WRITE set_PerformIntegration);
  QCEP_BOOLEAN_PROPERTY(PerformIntegration);

  Q_PROPERTY(bool displayIntegratedData READ get_DisplayIntegratedData WRITE set_DisplayIntegratedData);
  QCEP_BOOLEAN_PROPERTY(DisplayIntegratedData);

  Q_PROPERTY(bool saveIntegratedData READ get_SaveIntegratedData WRITE set_SaveIntegratedData);
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedData);

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
  virtual void beginAcquisition(int isDark) = 0;
  virtual void idleInt16Image(QxrdInt16ImageDataPtr image) = 0;
  virtual void acquiredInt16Image(QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow) = 0;
  virtual void acquiredInt32Image(QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow) = 0;
  virtual void updateEstimatedTime(QcepDoubleProperty *prop, int msec) = 0;

public:
  enum {
    NoOverwrite,
    CanOverwrite
  };

  virtual void loadData(QString name) = 0;
  virtual void saveData(QString name, int canOverwrite=NoOverwrite) = 0;
  virtual void loadDark(QString name) = 0;
  virtual void saveDark(QString name, int canOverwrite=NoOverwrite) = 0;
  virtual void loadMask(QString name) = 0;
  virtual void saveMask(QString name, int canOverwrite=NoOverwrite) = 0;
////  void loadDarkImage(QString name);
  virtual void loadBadPixels(QString name) = 0;
  virtual void saveBadPixels(QString name, int canOverwrite=NoOverwrite) = 0;
  virtual void loadGainMap(QString name) = 0;
  virtual void saveGainMap(QString name, int canOverwrite=NoOverwrite) = 0;

  virtual void newData(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow) = 0;
  virtual void newDarkImage(QxrdDoubleImageDataPtr image) = 0;
  virtual void newBadPixelsImage(QxrdDoubleImageDataPtr image) = 0;
  virtual void newGainMapImage(QxrdDoubleImageDataPtr image) = 0;

  virtual void newLogFile(QString path) = 0;
  virtual void logMessage(QString msg) = 0;
  virtual void closeLogFile() = 0;

  virtual int status(double delay) = 0;
  virtual void loadDefaultImages() = 0;
  virtual void readSettings(QxrdSettings &settings, QString section) = 0;
  virtual void writeSettings(QxrdSettings &settings, QString section) = 0;

  virtual void setAcquisition(QxrdAcquisitionPtr acq) = 0;
  virtual void setWindow(QxrdWindowPtr win) = 0;

  virtual void writeOutputScan(QxrdIntegratedDataPtr data) = 0;
  virtual void displayIntegratedData(QxrdIntegratedDataPtr data) = 0;

  virtual double estimatedProcessingTime(double estSerTime, double estParallelTime) = 0;

  virtual QxrdDoubleImageDataPtr data() const = 0;
  virtual QxrdDoubleImageDataPtr darkImage() const = 0;
  virtual QxrdMaskDataPtr        mask() const = 0;

  virtual QxrdCenterFinderPtr    centerFinder() const = 0;
  virtual QxrdIntegratorPtr      integrator() const = 0;
  virtual QxrdGenerateTestImagePtr generateTestImage() const = 0;

  virtual QxrdRingSetFitParametersPtr initialRingSetFitParameters() const = 0;
  virtual QxrdRingSetSampledDataPtr   initialRingSetData() const = 0;
  virtual QxrdRingSetFitParametersPtr refinedRingSetFitParameters() const = 0;
  virtual QxrdRingSetSampledDataPtr   refinedRingSetData() const = 0;
};

#endif
