#ifndef QXRDDATAPROCESSORBASE_H
#define QXRDDATAPROCESSORBASE_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>
#include <QAtomicInt>
#include <QWaitCondition>

#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddoubleimagedata.h"
#include "qxrddataprocessor.h"

#include <qwt_double_rect.h>

class QxrdDataProcessorBase : public QxrdDataProcessor
{
  Q_OBJECT;

public:
  QxrdDataProcessorBase(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent=0);
  ~QxrdDataProcessorBase();

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
  void slicePolygon(QwtArray<QwtDoublePoint> poly);
  void printMeasuredPolygon(QwtArray<QwtDoublePoint> poly);
  void summarizeMeasuredPolygon(QwtArray<QwtDoublePoint> poly);

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

  void processorOptionsDialog();

public:
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

protected:
  void saveNamedImageData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite=NoOverwrite);
  void saveNamedImageData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite=NoOverwrite);
  void saveNamedImageData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite=NoOverwrite);
  void saveNamedRawImageData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite=NoOverwrite);
  void saveNamedMaskData(QString name, QxrdMaskDataPtr mask, int canOverwrite=NoOverwrite);
  void saveNamedImageDataAsText(QString name, QxrdDoubleImageDataPtr image, int canOverwrite=NoOverwrite);
  void writeOutputScan(QVector<double> x, QVector<double> y);

  QxrdDoubleImageDataPtr processAcquiredInt16Image(QxrdInt16ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr processAcquiredInt32Image(QxrdInt32ImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr processAcquiredDoubleImage(QxrdDoubleImageDataPtr image, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdDoubleImageDataPtr processAcquiredImage(QxrdDoubleImageDataPtr dimg, QxrdDoubleImageDataPtr dark, QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);

  void newDarkImage(QxrdInt16ImageDataPtr image);
  void newDarkImage(QxrdInt32ImageDataPtr image);

  void updateEstimatedTime(QcepDoubleProperty *prop, int msec);
  double estimatedProcessingTime(double estSerTime, double estParallelTime);

protected:
  void newData(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);
  void newDarkImage(QxrdDoubleImageDataPtr image);
  void newBadPixelsImage(QxrdDoubleImageDataPtr image);
  void newGainMapImage(QxrdDoubleImageDataPtr image);
  void newMask();

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
