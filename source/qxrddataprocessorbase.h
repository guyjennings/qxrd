/******************************************************************
*
*  $Id: qxrddataprocessorbase.h,v 1.4 2010/10/22 21:44:26 jennings Exp $
*
*******************************************************************/

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
  void printMessage(QString msg) const;
  void statusMessage(QString msg) const;
  void criticalMessage(QString msg) const;

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

  QAtomicInt             m_AcquiredCount;

  QxrdCenterFinderPtr    m_CenterFinder;
  QxrdIntegratorPtr      m_Integrator;

  QxrdRingSetFitParametersPtr m_InitialRingSetFitParameters;
  QxrdRingSetFitParametersPtr m_RefinedRingSetFitParameters;

  QxrdRingSetSampledDataPtr m_InitialRingSetData;
  QxrdRingSetSampledDataPtr m_RefinedRingSetData;

  QxrdGenerateTestImagePtr m_GenerateTestImage;

  FILE                  *m_LogFile;

  HEADER_IDENT("$Id: qxrddataprocessorbase.h,v 1.4 2010/10/22 21:44:26 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrddataprocessorbase.h,v $
*  Revision 1.4  2010/10/22 21:44:26  jennings
*  *** empty log message ***
*
*  Revision 1.3  2010/10/21 19:44:03  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.6  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
*
*  Revision 1.1.2.5  2010/08/03 19:24:19  jennings
*  Added data types to hold sampled powder ring coordinates
*
*  Revision 1.1.2.4  2010/08/02 21:10:31  jennings
*  Added "rings" object to data processor
*
*  Revision 1.1.2.3  2010/07/28 19:29:45  jennings
*  Modified script output for void function results - no output in window
*  Added 'ellipse' command to processor
*  Implemented QxrdGenerateTestImage.generateImage
*
*  Revision 1.1.2.2  2010/07/27 21:53:03  jennings
*  Added double list property type
*  Added 'testImage' script object, interface to QxrdGenerateTestImage
*  Added QxrdDetectorGeometry class
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.14  2010/07/21 19:39:08  jennings
*  Improved calculation of estimated processing time
*
*  Revision 1.1.2.13  2010/06/23 21:45:53  jennings
*  Eliminated some test code, and lots of unneeded locks
*
*  Revision 1.1.2.12  2010/06/17 18:38:03  jennings
*  Implemented multi-threaded processing, with serialization of saves/display operation
*
*  Revision 1.1.2.11  2010/06/17 16:02:35  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.1.2.10  2010/06/15 20:18:54  jennings
*  First steps to adding CUDA support
*
*  Revision 1.1.2.9  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.1.2.8  2010/06/11 21:22:54  jennings
*  When calling integrator, pass data, dark and mask images explicitly.
*
*  Revision 1.1.2.7  2010/06/09 19:21:03  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*  Moved file saving code to QxrdFileSaver, accessed through QxrdFileSaverThread
*
*  Revision 1.1.2.6  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.5  2010/05/20 20:16:58  jennings
*  Added simple threaded processor implementation using QtConcurrent::run
*
*  Revision 1.1.2.4  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.1.2.3  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.1.2.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.1  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.47.2.5  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.47.2.4  2010/04/22 21:42:07  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.47.2.3  2010/04/22 20:56:26  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.47.2.2  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.47.2.1  2010/04/21 21:45:03  jennings
*  Changed mask values to shorts instead of bools.  Added ROI and histogram calculation tests.
*
*  Revision 1.47  2010/03/05 23:06:27  jennings
*  Modified text saving
*
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

