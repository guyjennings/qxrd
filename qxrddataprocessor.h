/******************************************************************
*
*  $Id: qxrddataprocessor.h,v 1.46 2010/03/05 22:32:03 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

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

#include <qwt_double_rect.h>

class QxrdAcquisition;
class QxrdCenterFinder;
class QxrdIntegrator;
class QxrdWindow;

class QxrdDataProcessor : public QObject
{
  Q_OBJECT;

public:
  QxrdDataProcessor(QxrdWindow *win, QxrdAcquisition *acq, QObject *parent=0);
  ~QxrdDataProcessor();

public:
  Q_PROPERTY(QString outputDirectory READ get_OutputDirectory WRITE set_OutputDirectory);
  QCEP_STRING_PROPERTY(OutputDirectory);

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

  Q_PROPERTY(bool performBadPixels READ get_PerformBadPixels WRITE set_PerformBadPixels);
  QCEP_BOOLEAN_PROPERTY(PerformBadPixels);

  Q_PROPERTY(bool performGainCorrection READ get_PerformGainCorrection WRITE set_PerformGainCorrection);
  QCEP_BOOLEAN_PROPERTY(PerformGainCorrection);

  Q_PROPERTY(bool saveSubtracted READ get_SaveSubtracted WRITE set_SaveSubtracted);
  QCEP_BOOLEAN_PROPERTY(SaveSubtracted);

  Q_PROPERTY(bool saveAsText READ get_SaveAsText WRITE set_SaveAsText);
  QCEP_BOOLEAN_PROPERTY(SaveAsText);

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

  Q_PROPERTY(QString fileName        READ get_FileName WRITE set_FileName);
  QCEP_STRING_PROPERTY(FileName);

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

signals:
  void printMessage(QString msg) const;
  void statusMessage(QString msg) const;
  void criticalMessage(QString msg) const;

public:
  enum {
    NoOverwrite,
    CanOverwrite
  };

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

  void processData(QString name);
  void processDataSequence(QString path, QString filter="*.tif");
  void processDataSequence(QStringList paths);
  void processDataSequence(QString path, QStringList filter);

  void loadData(QString name);
  void saveData(QString name, int canOverwrite=NoOverwrite);
  void loadDark(QString name);
  void saveDark(QString name, int canOverwrite=NoOverwrite);
  void loadMask(QString name);
  void saveMask(QString name, int canOverwrite=NoOverwrite);
//  void loadDarkImage(QString name);
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

  QString uniqueFileName(QString name);

  void updateEstimatedProcessingTime();

  void newLogFile(QString path);
  void logMessage(QString msg);
  void closeLogFile();

  void fileWriteTest(int dim, QString path);
  int status(double delay);

public:
  void loadDefaultImages();

  QxrdDoubleImageData *takeNextFreeImage();

  void returnImageToPool(QxrdDoubleImageData *img);
  void returnInt16ImageToPool(QxrdInt16ImageData *img);
  void returnInt32ImageToPool(QxrdInt32ImageData *img);

  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

  void setAcquisition(QxrdAcquisition *acq);
  void setWindow(QxrdWindow *win);

  bool saveNamedImageData(QString name, QxrdDoubleImageData *image, int canOverwrite=NoOverwrite);
  bool saveNamedImageData(QString name, QxrdInt16ImageData *image, int canOverwrite=NoOverwrite);
  bool saveNamedImageData(QString name, QxrdInt32ImageData *image, int canOverwrite=NoOverwrite);
  bool saveNamedRawImageData(QString name, QxrdInt16ImageData *image, int canOverwrite=NoOverwrite);
  bool saveNamedRawImageData(QString name, QxrdInt32ImageData *image, int canOverwrite=NoOverwrite);
  bool saveNamedMaskData(QString name, QxrdMaskData *mask, int canOverwrite=NoOverwrite);
  bool saveNamedImageDataAsText(QString name, QxrdDoubleImageData *image, int canOverwrite=NoOverwrite);

  void writeOutputScan(QVector<double> x, QVector<double> y);

  QxrdDoubleImageData *data() const;
  QxrdDoubleImageData *darkImage() const;
  QxrdMaskData  *mask() const;

  int incrementAcquiredCount();
  int decrementAcquiredCount();
  int getAcquiredCount();

  QxrdCenterFinder  *centerFinder() const;
  QxrdIntegrator    *integrator() const;

private slots:
  void onAcquiredInt16ImageAvailable(QxrdInt16ImageData *image);
  void onAcquiredInt32ImageAvailable(QxrdInt32ImageData *image);

private:
  void processAcquiredInt16Image(QxrdInt16ImageData *image);
  void processAcquiredInt32Image(QxrdInt32ImageData *image);
  void processAcquiredImage(QxrdDoubleImageData *dimg);

  void subtractDarkImage(QxrdDoubleImageData *image, QxrdDoubleImageData *dark);
  void correctBadPixels(QxrdDoubleImageData *image);
  void correctImageGains(QxrdDoubleImageData *image);
  void performImageCorrections(QxrdDoubleImageData *image);

  void newData(QxrdDoubleImageData *image);
  void newDarkImage(QxrdDoubleImageData *image);
  void newDarkImage(QxrdInt16ImageData *image);
  void newDarkImage(QxrdInt32ImageData *image);
  void newBadPixelsImage(QxrdDoubleImageData *image);
  void newGainMapImage(QxrdDoubleImageData *image);
  void newMask();

  void openLogFile();
  void writeLogHeader();

  void updateEstimatedTime(QcepDoubleProperty *prop, int msec);

private:
  mutable QMutex            m_Mutex;
  mutable QMutex            m_LogFileMutex;

  QxrdWindow               *m_Window;
  QxrdAcquisition          *m_Acquisition;
  QReadWriteLock            m_DarkUsage;
//  QReadWriteLock            m_Processing;
  QWaitCondition            m_ProcessWaiting;
  QxrdDoubleImageQueue      m_FreeImages;
  QxrdDoubleImageData      *m_Data;
  QxrdDoubleImageData      *m_DarkFrame;
  QxrdDoubleImageData      *m_BadPixels;
  QxrdDoubleImageData      *m_GainMap;
  QxrdMaskData             *m_Mask;

  QAtomicInt                m_AcquiredCount;

  QxrdCenterFinder         *m_CenterFinder;
  QxrdIntegrator           *m_Integrator;

  FILE                     *m_LogFile;

  HEADER_IDENT("$Id: qxrddataprocessor.h,v 1.46 2010/03/05 22:32:03 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrddataprocessor.h,v $
*  Revision 1.46  2010/03/05 22:32:03  jennings
*  Version 0.3.9 adds text file output and conversion
*
*  Revision 1.45  2009/12/03 21:35:37  jennings
*  Corrected locking problem with dark image
*
*  Revision 1.44  2009/10/16 21:54:17  jennings
*  Implemented various processDataSequence commands
*
*  Revision 1.43  2009/10/02 20:11:02  jennings
*  Added support for (optionally) saving and/or displaying integrated data
*
*  Revision 1.42  2009/10/01 21:44:05  jennings
*  Delete QxrdDataProcessor object at program exit
*  Removed some commented out dead wood
*
*  Revision 1.41  2009/09/29 18:39:47  jennings
*  Removed references to 'QxrdDataProcessor::processedCount'
*  Fixed up the various 'status' scripting functions so that they work properly
*
*  Revision 1.40  2009/09/26 04:55:46  jennings
*  Removed some commented-out sections
*  Removed QxrdDataProcessor::copyImage(src,dst), replaced with templatized image member fn
*
*  Revision 1.39  2009/09/25 22:42:48  jennings
*  Masking changes
*
*  Revision 1.38  2009/09/23 21:16:26  jennings
*  Removed acquiredInt{16,32}Image queues from data processor
*
*  Revision 1.37  2009/09/21 19:40:46  jennings
*  Added version number to window title, added more measurement output
*
*  Revision 1.36  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.35  2009/09/18 20:44:49  jennings
*  Add separate status functions for acquisition and processing, as well as an aggregated function
*  combining the status of the two.
*
*  Revision 1.34  2009/09/14 19:08:57  jennings
*  Added more checks for appropriate data type / exposure etc. before subtracting
*  backgrounds
*
*  Revision 1.33  2009/09/13 13:59:48  jennings
*  Added 'canOverwrite' argument to data saving routines and arrange
*  that saves via file dialogs can overwrite, programmatic saves use
*  unique file names
*
*  Revision 1.32  2009/09/12 14:44:20  jennings
*  Moved lock to base class, made into mutex
*
*  Revision 1.31  2009/09/12 13:44:37  jennings
*  Renamed convertImage to copyImage, added double->double version of copyImage,
*  added timer to copyImage
*
*  Revision 1.30  2009/09/07 22:10:14  jennings
*  Allow NULL mask
*
*  Revision 1.29  2009/09/04 21:11:41  jennings
*  Support for file write timing tests
*
*  Revision 1.28  2009/09/04 15:15:43  jennings
*  Added log file routines
*  Removed newlines from any printMessage calls.
*
*  Revision 1.27  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.26  2009/08/27 21:55:43  jennings
*  Added code to make sure file saving routines will not overwrite data
*
*  Revision 1.25  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.24  2009/08/27 17:06:52  jennings
*  Added code to load/save dark and mask data
*
*  Revision 1.23  2009/08/26 20:57:13  jennings
*  Starting to implement separate Double Int16 and Int32 processing chains
*
*  Revision 1.22  2009/08/25 20:07:00  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.21  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.20  2009/08/09 15:40:32  jennings
*  Added measurer tool to all graphs
*
*  Revision 1.19  2009/08/08 20:14:55  jennings
*  Added powder ring calculation operations
*
*  Revision 1.18  2009/08/07 22:21:56  jennings
*  Added a number of sample data creation routines to QxrdDataProcessor
*  Added a parallelized integration routine to QxrdIntegrator
*
*  Revision 1.17  2009/08/04 20:42:53  jennings
*  Simple, initial, implementation of integration
*
*  Revision 1.16  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.15  2009/08/03 13:26:25  jennings
*  Added option to set/clear mask pixels
*
*  Revision 1.14  2009/08/02 21:14:16  jennings
*  Added masking dummy routines
*
*  Revision 1.13  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.12  2009/07/20 00:31:31  jennings
*  Removed image queues for acquired and dark images - use 'connect' args instead
*
*  Revision 1.11  2009/07/17 20:41:20  jennings
*  Modifications related to mask display
*
*  Revision 1.10  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.9  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.8  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.7  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.6  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

