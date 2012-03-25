#include "qxrdacquisition.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdacquisitiontriggerthread.h"
#include "qxrdacquisitiontrigger.h"
#include "qxrdacquisitionextrainputs.h"
//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
#include <QMetaProperty>
#include <QStringList>
#include <QThread>
#include <QtGlobal>
#include "qxrdapplication.h"

QxrdAcquisitionParameters::QxrdAcquisitionParameters(QxrdSettingsSaverWPtr saver)
  : QObject(),
    m_Saver(saver),
    m_QxrdVersion(QxrdSettingsSaverPtr(), this,"qxrdVersion",STR(QXRD_VERSION), "QXRD Version Number"),
    m_QtVersion(QxrdSettingsSaverPtr(), this,"qtVersion",qVersion(), "QT Version Number"),
//    m_DetectorType(QxrdSettingsSaverPtr(), this, "detectorType", 0, "Detector Type (0 = simulated, 1 = PE, 2 = Pilatus, 3 = EPICS, 4 = Files)"),
    m_ExposureTime(saver, this,"exposureTime",0.1, "Exposure Time (in sec)"),
    m_SkippedExposuresAtStart(saver, this,"skippedExposuresAtStart",0, "Exposures to Skip at Start"),
    m_LastAcquired(QxrdSettingsSaverPtr(), this, "lastAcquired", 0, "Internal Acquisition Flag"),
    m_PhasesInGroup(saver, this,"phasesInGroup",1, "Number of Image Phases"),
    m_SummedExposures(saver, this,"summedExposures",1, "Summed Exposures per Image"),
    m_SkippedExposures(saver, this,"skippedExposures",0, "Skipped Exposures between Images"),
    m_PreTriggerFiles(saver, this,"preTriggerFiles",0, "Number of pre-Trigger Images"),
    m_PostTriggerFiles(saver, this,"postTriggerFiles",1, "Number of post-Trigger Images"),
    m_FileIndex(saver, this,"fileIndex",0, "File Index"),
    m_FilePattern(saver, this,"filePattern","", "File Name Pattern"),
    m_FileIndexWidth(saver, this, "fileIndexWidth", 5, "Digits in File Index Field"),
    m_FilePhaseWidth(saver, this, "filePhaseWidth", 3, "Digits in Phase Number Field"),
    m_FileOverflowWidth(saver, this, "fileOverflowWidth", 5, "Digits in Overflow Index Field"),
    m_DarkSummedExposures(saver, this,"darkSummedExposures",1, "Summed Exposures in Dark Image"),
    m_CameraGain(saver, this,"cameraGain",0, "Detector Gain"),
    m_BinningMode(saver, this,"binningMode",0, "Binning Mode"),
    m_FileBase(saver, this,"fileBase","", "File Base"),
    m_NRows(saver, this, "nRows", 2048, "Number of Rows"),
    m_NCols(saver, this, "nCols", 2048, "Number of Cols"),
    m_OverflowLevel(saver, this, "overflowLevel", 65500, "Overflow level (per exposure)"),
    m_AcquireDark(QxrdSettingsSaverPtr(), this, "acquireDark", 0, "Acquire Dark Image?"),
    m_Cancelling(QxrdSettingsSaverPtr(), this, "cancelling", 0, "Cancel Acquisition?"),
    m_Triggered(QxrdSettingsSaverPtr(), this, "triggered", 0, "Trigger Acquisition"),
    m_Raw16SaveTime(saver, this,"raw16SaveTime", 0.1, "Time to save 16 bit images"),
    m_Raw32SaveTime(saver, this,"raw32SaveTime", 0.2, "Time to save 32 bit images"),
    m_RawSaveTime(saver, this,"rawSaveTime", 0.2, "Time to save raw images"),
    m_DarkSaveTime(saver, this,"darkSaveTime", 0.2, "Time to save dark images"),
    m_UserComment1(saver, this,"userComment1","", "User Comment 1"),
    m_UserComment2(saver, this,"userComment2","", "User Comment 2"),
    m_UserComment3(saver, this,"userComment3","", "User Comment 3"),
    m_UserComment4(saver, this,"userComment4","", "User Comment 4"),
    m_DroppedFrames(QxrdSettingsSaverPtr(), this,"droppedFrames",0, "Number of Dropped Frames"),
    m_Mutex(QMutex::Recursive),
    m_SynchronizedAcquisition(NULL),
    m_AcquisitionTriggerThread(NULL),
    m_AcquisitionTrigger(NULL),
    m_AcquisitionExtraInputs(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionParameters::QxrdAcquisitionParameters(%p)\n", this);
  }

  connect(prop_Raw16SaveTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateSaveTimes()));
  connect(prop_Raw32SaveTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateSaveTimes()));
  connect(prop_SummedExposures(), SIGNAL(valueChanged(int,int)), this, SLOT(updateSaveTimes()));
  connect(prop_DarkSummedExposures(), SIGNAL(valueChanged(int,int)), this, SLOT(updateSaveTimes()));

//  m_FileIndex.setDebug(1);

  setObjectName("acquisition");
}

QxrdAcquisitionParameters::~QxrdAcquisitionParameters()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionParameters::~QxrdAcquisitionParameters(%p)\n", this);
  }
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

  if (g_Application) {
    foreach(name, dynamicPropertyNames()) {
      g_Application->printMessage(tr("acquisition.%1\n").arg(name.data()));
    }
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

void QxrdAcquisitionParameters::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->writeSettings(settings, section+"/sync");
  }

  if (m_AcquisitionTrigger) {
    m_AcquisitionTrigger->writeSettings(settings, section+"/trigger");
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->writeSettings(settings, section+"/extrainputs");
  }

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionParameters::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->readSettings(settings, section+"/sync");
  }

  if (m_AcquisitionTrigger) {
    m_AcquisitionTrigger->readSettings(settings, section+"/trigger");
  }

  if (m_AcquisitionExtraInputs) {
    m_AcquisitionExtraInputs->readSettings(settings, section+"/extrainputs");
  }

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
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
