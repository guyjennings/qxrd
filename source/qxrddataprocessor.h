/******************************************************************
*
*  $Id: qxrddataprocessor.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

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
  Q_PROPERTY(int    processorType     READ get_ProcessorType WRITE set_ProcessorType STORED false);
  QCEP_INTEGER_PROPERTY(ProcessorType);

  Q_PROPERTY(QString processorTypeName READ get_ProcessorTypeName WRITE set_ProcessorTypeName STORED false);
  QCEP_STRING_PROPERTY(ProcessorTypeName);

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
  virtual void acquiredInt16Image(QxrdInt16ImageDataPtr image) = 0;
  virtual void acquiredInt32Image(QxrdInt32ImageDataPtr image) = 0;
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

  virtual void newData(QxrdDoubleImageDataPtr image) = 0;
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

private:

  HEADER_IDENT("$Id: qxrddataprocessor.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrddataprocessor.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.5  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
*
*  Revision 1.1.2.4  2010/08/03 19:24:19  jennings
*  Added data types to hold sampled powder ring coordinates
*
*  Revision 1.1.2.3  2010/08/02 21:10:31  jennings
*  Added "rings" object to data processor
*
*  Revision 1.1.2.2  2010/07/27 21:53:03  jennings
*  Added double list property type
*  Added 'testImage' script object, interface to QxrdGenerateTestImage
*  Added QxrdDetectorGeometry class
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.47.2.19  2010/07/21 19:39:08  jennings
*  Improved calculation of estimated processing time
*
*  Revision 1.47.2.18  2010/06/23 21:46:36  jennings
*  Removed the fileName property
*
*  Revision 1.47.2.17  2010/06/17 19:19:59  jennings
*  Added 'save dark' and 'skip at start' options
*
*  Revision 1.47.2.16  2010/06/17 16:02:35  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.47.2.15  2010/06/14 20:34:11  jennings
*  Removed QxrdIntegratedDataPtr from qxrdforwardtypes.h
*
*  Revision 1.47.2.14  2010/06/11 21:22:10  jennings
*  Removed duplicate center finder and integrator objects
*
*  Revision 1.47.2.13  2010/06/09 19:19:23  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.47.2.12  2010/05/25 22:05:17  jennings
*  Fixed problem of detector type name and processor type name labels not being displayed correctly
*
*  Revision 1.47.2.11  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.47.2.10  2010/05/21 20:04:42  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.47.2.9  2010/05/19 21:15:35  jennings
*  PE acquisition debugging
*
*  Revision 1.47.2.8  2010/04/27 19:36:34  jennings
*  Deleted commented remnants of split of QxrdDataProcessor into QxrdDataProcessor & QxrdDataProcessorBase
*
*  Revision 1.47.2.7  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.47.2.6  2010/04/26 20:53:25  jennings
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

