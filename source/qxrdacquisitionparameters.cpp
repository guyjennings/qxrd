/******************************************************************
*
*  $Id: qxrdacquisitionparameters.cpp,v 1.3 2010/10/21 19:44:02 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionparameters.h"

//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
#include "qxrdsettings.h"
#include <QMetaProperty>
#include <QStringList>
#include <QThread>

QxrdAcquisitionParameters::QxrdAcquisitionParameters(/*QxrdDataProcessor *proc*/)
  : QObject(),
    m_DetectorType(this, "detectorType",-1),
    m_DetectorTypeName(this,"detectorTypeName",""),
    m_ExposureTime(this,"exposureTime",0),
    m_SkippedExposuresAtStart(this,"skippedExposuresAtStart",0),
    m_SummedExposures(this,"summedExposures",1),
    m_SkippedExposures(this,"skippedExposures",0),
    m_PreTriggerFiles(this,"preTriggerFiles", 0),
    m_PostTriggerFiles(this,"postTriggerFiles",1),
    m_Trigger(this,"triggered",0),
    m_FileIndex(this,"fileIndex",0),
    m_FilePattern(this,"filePattern",""),
    m_DarkSummedExposures(this,"darkSummedExposures",1),
    m_CameraGain(this,"cameraGain",0),
    m_BinningMode(this,"binningMode",0),
    m_FileBase(this,"fileBase",""),
    m_NRows(this, "nRows", 2048),
    m_NCols(this, "nCols", 2048),
    m_OverflowLevel(this, "overflowLevel", 65500),
    m_AcquireDark(this, "acquireDark", 0),
    m_Cancelling(this, "cancelling", 0),
    m_ExposuresToSum(this, "exposuresToSum", 1),
    m_ExposuresToSkip(this, "exposuresToSkip", 1),
    m_FilesInAcquiredSequence(this, "filesInAcquiredSequence", 1),
    m_TotalBufferSizeMB(this,"totalBufferSizeMB", 500),
    m_Raw16SaveTime(this,"raw16SaveTime", 0.1),
    m_Raw32SaveTime(this,"raw32SaveTime", 0.2),
    m_RawSaveTime(this,"rawSaveTime", 0.2),
    m_DarkSaveTime(this,"darkSaveTime", 0.2),
    m_UserComment1(this,"userComment1",""),
    m_UserComment2(this,"userComment2",""),
    m_UserComment3(this,"userComment3",""),
    m_UserComment4(this,"userComment4",""),
    m_DroppedFrames(this,"droppedFrames",0),
    m_Mutex(QMutex::Recursive),
    SOURCE_IDENT("$Id: qxrdacquisitionparameters.cpp,v 1.3 2010/10/21 19:44:02 jennings Exp $")
{
  connect(prop_Raw16SaveTime(), SIGNAL(changedValue(double)), this, SLOT(updateSaveTimes()));
  connect(prop_Raw32SaveTime(), SIGNAL(changedValue(double)), this, SLOT(updateSaveTimes()));
  connect(prop_SummedExposures(), SIGNAL(changedValue(int)), this, SLOT(updateSaveTimes()));
  connect(prop_DarkSummedExposures(), SIGNAL(changedValue(int)), this, SLOT(updateSaveTimes()));
}

void QxrdAcquisitionParameters::updateSaveTimes()
{
  if (get_SummedExposures() <= 1) {
    set_RawSaveTime(get_Raw16SaveTime());
  } else {
    set_RawSaveTime(get_Raw32SaveTime());
  }

  if (get_DarkSummedExposures() <= 1) {
    set_DarkSaveTime(get_Raw16SaveTime());
  } else {
    set_DarkSaveTime(get_Raw32SaveTime());
  }
}

void QxrdAcquisitionParameters::dynamicProperties()
{
  QByteArray name;

  foreach(name, dynamicPropertyNames()) {
    emit printMessage(tr("acquisition.%1\n").arg(name.data()));
  }
}

void QxrdAcquisitionParameters::copyDynamicProperties(QObject *dest)
{
  if (dest) {
    QByteArray name;

    foreach(name, dynamicPropertyNames()) {
      dest -> setProperty(name.data(), property(name.data()));
    }
  }
}

void QxrdAcquisitionParameters::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionParameters::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  if (get_TotalBufferSizeMB() > 100000000) {
    set_TotalBufferSizeMB(get_TotalBufferSizeMB()/MegaBytes);
  }
}

//void QxrdAcquisitionParameters::setDynamicProperty(QString name, QVariant value)
//{
//  if (QThread::currentThread() == thread()) {
//    setProperty(qPrintable(name), value);
//  } else {
//    QTimer::singleShot
//    INVOKE_CHECK(QMetaObject::invokeMethod(this, "setDynamicProperty", Qt::QueuedConnection, Q_ARG(QString,name), Q_ARG(QVariant,value)));
//  }
//}
//
//QVariant QxrdAcquisitionParameters::dynamicProperty(QString name)
//{
//  return property(qPrintable(name));
//}
//
/******************************************************************
*
*  $Log: qxrdacquisitionparameters.cpp,v $
*  Revision 1.3  2010/10/21 19:44:02  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.2  2010/09/13 20:00:38  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/17 19:20:50  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.1  2010/07/22 18:39:36  jennings
*  Moving files into source subdirectory
*
*  Revision 1.18.2.15  2010/07/21 20:13:15  jennings
*  Improved calculation and display of estimated save times
*
*  Revision 1.18.2.14  2010/06/17 19:19:59  jennings
*  Added 'save dark' and 'skip at start' options
*
*  Revision 1.18.2.13  2010/06/17 18:37:13  jennings
*  Added dropped frames counter to display window
*
*  Revision 1.18.2.12  2010/05/28 21:44:57  jennings
*  *** empty log message ***
*
*  Revision 1.18.2.11  2010/05/27 21:47:51  jennings
*  Renamed totalBufferSize parameter to totalBufferSizeMB to avoid conflicts with qxrd 0.3.x which interprets
*  totalBufferSize in bytes, rather than MB
*
*  Revision 1.18.2.10  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.18.2.9  2010/05/21 20:04:41  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.18.2.8  2010/05/19 21:15:34  jennings
*  PE acquisition debugging
*
*  Revision 1.18.2.7  2010/05/17 15:59:52  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.18.2.6  2010/04/26 00:37:09  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.18.2.5  2010/04/22 20:56:26  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.18.2.4  2010/04/21 20:51:21  jennings
*  Added detectorType and detectorTypeName properties to each detector type
*
*  Revision 1.18.2.3  2010/04/20 21:18:58  jennings
*  Added skipped exposures parameter
*
*  Revision 1.18.2.2  2010/04/19 21:48:26  jennings
*  More rearrangement
*
*  Revision 1.18.2.1  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.18  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.17  2009/11/09 16:44:38  jennings
*  Reduced initial buffer size to 500M, fixed problem loading 32-bit his files
*
*  Revision 1.16  2009/09/23 19:00:36  jennings
*  Removed dynamic property support - it's not thread-safe and doesn't work
*  under windows
*
*  Revision 1.15  2009/09/22 20:37:23  jennings
*  Dynamic properties attached to the acquisition object are propagated through to saved images
*
*  Revision 1.14  2009/09/22 13:48:17  jennings
*  Some support for dynamic properties during acquisition
*
*  Revision 1.13  2009/09/21 18:12:54  jennings
*  Added 'triggered', 'usercomment{1-4}' properties to data
*
*  Revision 1.12  2009/09/04 12:45:38  jennings
*  Added binning mode parameter
*
*  Revision 1.11  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.10  2009/08/26 16:58:13  jennings
*  Added TotalBufferSize property to specify the amount of memory to use for acquisition buffers
*
*  Revision 1.9  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.8  2009/07/14 20:07:00  jennings
*  Implemented simple simulated acquisition
*
*  Revision 1.7  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.6  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.5  2009/07/07 22:05:25  jennings
*  *** empty log message ***
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

