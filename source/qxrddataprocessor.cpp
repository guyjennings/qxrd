/******************************************************************
*
*  $Id: qxrddataprocessor.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrddataprocessor.h"
#include <QtConcurrentRun>
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdimagedata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qxrdmutexlocker.h"

#include "tiffio.h"

#include <QTime>
#include <QPainter>
#include <QDirIterator>
#include <math.h>

QxrdDataProcessor::QxrdDataProcessor
    (QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent)
  : QObject(parent),
    m_ProcessorType(this,"processorType",0),
    m_ProcessorTypeName(this,"processorTypeName","processorType"),
    m_OutputDirectory(this,"outputDirectory", ""),
    m_DataPath(this,"dataPath", ""),
    m_DarkImagePath(this, "darkImagePath", ""),
    m_BadPixelsPath(this, "badPixelsPath", ""),
    m_GainMapPath(this, "gainMapPath", ""),
    m_MaskPath(this, "maskPath", ""),
    m_LogFilePath(this, "logFilePath", "qxrd.log"),
    m_PerformDarkSubtraction(this, "performDarkSubtraction", true),
    m_SaveRawImages(this, "saveRawImages", true),
    m_SaveDarkImages(this, "saveDarkImages", true),
    m_PerformBadPixels(this, "performBadPixels", true),
    m_PerformGainCorrection(this, "performGainCorrection", true),
    m_SaveSubtracted(this, "saveSubtracted", true),
    m_SaveAsText(this, "saveAsText", false),
    m_SaveAsTextSeparator(this, "saveAsTextSeparator", " "),
    m_SaveAsTextPerLine(this,"saveAsTextPerLine",16),
    m_PerformIntegration(this, "performIntegration", true),
    m_DisplayIntegratedData(this, "displayIntegratedData", true),
    m_SaveIntegratedData(this, "saveIntegratedData", true),
    m_PerformDarkSubtractionTime(this, "performDarkSubtractionTime", 0.01),
    m_PerformBadPixelsTime(this, "performBadPixelsTime", 0.01),
    m_PerformGainCorrectionTime(this, "performGainCorrectionTime", 0.01),
    m_SaveSubtractedTime(this, "saveSubtractedTime", 0.1),
    m_SaveAsTextTime(this, "saveAsTextTime", 0.1),
    m_PerformIntegrationTime(this, "performIntegrationTime", 0.05),
    m_DisplayIntegratedDataTime(this, "displayIntegratedDataTime", 0.2),
    m_SaveIntegratedDataTime(this, "saveIntegratedDataTime", 0.01),
    m_EstimatedProcessingTime(this, "estimatedProcessingTime", 0.1),
    m_AveragingRatio(this, "averagingRatio", 0.1),
//    m_FileName(this,"fileName",""),
    m_MaskMinimumValue(this, "maskMinimumValue", 0),
    m_MaskMaximumValue(this, "maskMaximumValue", 20000),
    m_MaskCircleRadius(this, "maskCircleRadius", 10),
    m_MaskSetPixels(this, "maskSetPixels", true),
    m_CompressImages(this, "compressImages", false),
    m_Average(this,"average",0.0),
    m_AverageDark(this,"averageDark",0.0),
    m_AverageRaw(this,"averageRaw",0.0),
    SOURCE_IDENT("$Id: qxrddataprocessor.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
}

QxrdDataProcessor::~QxrdDataProcessor()
{
}

/******************************************************************
*
*  $Log: qxrddataprocessor.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.71.2.21  2010/07/21 19:39:08  jennings
*  Improved calculation of estimated processing time
*
*  Revision 1.71.2.20  2010/06/23 21:46:36  jennings
*  Removed the fileName property
*
*  Revision 1.71.2.19  2010/06/17 19:19:59  jennings
*  Added 'save dark' and 'skip at start' options
*
*  Revision 1.71.2.18  2010/06/11 21:22:10  jennings
*  Removed duplicate center finder and integrator objects
*
*  Revision 1.71.2.17  2010/06/09 19:19:23  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.71.2.16  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.71.2.15  2010/05/21 20:04:42  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.71.2.14  2010/05/20 00:16:14  jennings
*  *** empty log message ***
*
*  Revision 1.71.2.13  2010/05/19 21:15:35  jennings
*  PE acquisition debugging
*
*  Revision 1.71.2.12  2010/05/18 22:47:01  jennings
*  More perkin elmer readout fixes
*
*  Revision 1.71.2.11  2010/04/27 19:36:34  jennings
*  Deleted commented remnants of split of QxrdDataProcessor into QxrdDataProcessor & QxrdDataProcessorBase
*
*  Revision 1.71.2.10  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.71.2.9  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.71.2.8  2010/04/25 03:38:39  jennings
*  Added scrolling area to acquire dialog.  Instrument image queues.
*
*  Revision 1.71.2.7  2010/04/23 21:26:32  jennings
*  Rearranged message output
*
*  Revision 1.71.2.6  2010/04/22 21:42:07  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.71.2.5  2010/04/22 20:56:26  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.71.2.4  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.71.2.3  2010/04/21 21:45:03  jennings
*  Changed mask values to shorts instead of bools.  Added ROI and histogram calculation tests.
*
*  Revision 1.71.2.2  2010/04/19 21:48:27  jennings
*  More rearrangement
*
*  Revision 1.71.2.1  2010/04/15 19:32:34  jennings
*  Fixed typo in saveSubtractedTime property
*
*  Revision 1.71  2010/03/05 23:45:00  jennings
*  Fixed problems with text output separators
*
*  Revision 1.70  2010/03/05 23:06:27  jennings
*  Modified text saving
*
*  Revision 1.69  2010/03/05 22:32:03  jennings
*  Version 0.3.9 adds text file output and conversion
*
*  Revision 1.68  2009/12/11 17:49:04  jennings
*  Added 'ImageSaved' property to image data and used this to avoid double-saving raw data when
*  processing data off-line
*
*  Revision 1.67  2009/12/03 21:35:37  jennings
*  Corrected locking problem with dark image
*
*  Revision 1.66  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.65  2009/11/02 20:16:22  jennings
*  Changes to make it work with VC compiler
*
*  Revision 1.64  2009/10/26 02:40:24  jennings
*  Fixed initial mask display state
*
*  Revision 1.63  2009/10/24 05:23:55  jennings
*  Fixed problem with integrated curves coming in wrong order
*
*  Revision 1.62  2009/10/16 21:54:17  jennings
*  Implemented various processDataSequence commands
*
*  Revision 1.61  2009/10/02 21:03:11  jennings
*  Call fflush at strategic places during log file output
*
*  Revision 1.60  2009/10/02 20:18:35  jennings
*  Added support for (optionally) saving and/or displaying integrated data
*
*  Revision 1.59  2009/10/02 20:11:02  jennings
*  Added support for (optionally) saving and/or displaying integrated data
*
*  Revision 1.58  2009/10/01 21:44:05  jennings
*  Delete QxrdDataProcessor object at program exit
*  Removed some commented out dead wood
*
*  Revision 1.57  2009/09/29 18:39:46  jennings
*  Removed references to 'QxrdDataProcessor::processedCount'
*  Fixed up the various 'status' scripting functions so that they work properly
*
*  Revision 1.56  2009/09/28 22:06:55  jennings
*  Added circular integration to processing steps
*
*  Revision 1.55  2009/09/26 04:55:46  jennings
*  Removed some commented-out sections
*  Removed QxrdDataProcessor::copyImage(src,dst), replaced with templatized image member fn
*
*  Revision 1.54  2009/09/25 22:42:48  jennings
*  Masking changes
*
*  Revision 1.53  2009/09/25 14:22:16  jennings
*  Simplified double-buffering for plotted data - there is now a separate copy of data and mask
*  in QxrdWindow
*
*  Revision 1.52  2009/09/22 18:19:00  jennings
*  Added slicing routines
*  Set title for traces in avg data graph
*
*  Revision 1.51  2009/09/21 19:40:46  jennings
*  Added version number to window title, added more measurement output
*
*  Revision 1.50  2009/09/21 16:27:58  jennings
*  Added user interface to log file path
*
*  Revision 1.49  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.48  2009/09/18 20:44:49  jennings
*  Add separate status functions for acquisition and processing, as well as an aggregated function
*  combining the status of the two.
*
*  Revision 1.47  2009/09/14 19:08:57  jennings
*  Added more checks for appropriate data type / exposure etc. before subtracting
*  backgrounds
*
*  Revision 1.46  2009/09/13 13:59:47  jennings
*  Added 'canOverwrite' argument to data saving routines and arrange
*  that saves via file dialogs can overwrite, programmatic saves use
*  unique file names
*
*  Revision 1.45  2009/09/12 14:44:20  jennings
*  Moved lock to base class, made into mutex
*
*  Revision 1.44  2009/09/12 13:44:37  jennings
*  Renamed convertImage to copyImage, added double->double version of copyImage,
*  added timer to copyImage
*
*  Revision 1.43  2009/09/12 13:36:19  jennings
*  Added more locks to QxrdDataProcessor
*
*  Revision 1.42  2009/09/10 21:33:30  jennings
*  Added TIFF error handling
*
*  Revision 1.41  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.40  2009/09/08 21:41:59  jennings
*  Update estimated processing time accumulators
*
*  Revision 1.39  2009/09/07 22:10:14  jennings
*  Allow NULL mask
*
*  Revision 1.38  2009/09/04 21:11:41  jennings
*  Support for file write timing tests
*
*  Revision 1.37  2009/09/04 20:04:31  jennings
*  Debugging pre-trigger acquisition
*
*  Revision 1.36  2009/09/04 15:15:42  jennings
*  Added log file routines
*  Removed newlines from any printMessage calls.
*
*  Revision 1.35  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.34  2009/08/27 21:55:43  jennings
*  Added code to make sure file saving routines will not overwrite data
*
*  Revision 1.33  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.32  2009/08/27 17:06:52  jennings
*  Added code to load/save dark and mask data
*
*  Revision 1.31  2009/08/26 20:57:13  jennings
*  Starting to implement separate Double Int16 and Int32 processing chains
*
*  Revision 1.30  2009/08/25 18:49:19  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.29  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.28  2009/08/09 15:40:32  jennings
*  Added measurer tool to all graphs
*
*  Revision 1.27  2009/08/08 20:14:55  jennings
*  Added powder ring calculation operations
*
*  Revision 1.26  2009/08/07 22:21:56  jennings
*  Added a number of sample data creation routines to QxrdDataProcessor
*  Added a parallelized integration routine to QxrdIntegrator
*
*  Revision 1.25  2009/08/04 20:42:53  jennings
*  Simple, initial, implementation of integration
*
*  Revision 1.24  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.23  2009/08/03 20:58:59  jennings
*  Minor fixups
*
*  Revision 1.22  2009/08/03 13:26:25  jennings
*  Added option to set/clear mask pixels
*
*  Revision 1.21  2009/08/02 21:14:16  jennings
*  Added masking dummy routines
*
*  Revision 1.20  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.19  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.18  2009/07/20 00:34:49  jennings
*  Send data between acquisition and data processor via signal/slot args, rather
*  than image queues
*
*  Revision 1.17  2009/07/17 21:10:39  jennings
*  Modifications related to mask display
*
*  Revision 1.16  2009/07/17 20:41:20  jennings
*  Modifications related to mask display
*
*  Revision 1.15  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.14  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.13  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.12  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.11  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.10  2009/06/30 21:35:05  jennings
*  Debugging meta data static init problems
*
*  Revision 1.9  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.8  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

