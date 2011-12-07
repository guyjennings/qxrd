#include "qxrdacquisition.h"
#include "qxrdsynchronizedacquisition.h"
//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
#include <QMetaProperty>
#include <QStringList>
#include <QThread>
#include <QtGlobal>
#include "qxrdapplication.h"

QxrdAcquisitionParameters::QxrdAcquisitionParameters(QxrdSettingsSaver *saver)
  : QObject(),
    m_QxrdVersion(saver, this,"qxrdVersion",STR(QXRD_VERSION)),
    m_QtVersion(saver, this,"qtVersion",qVersion()),
    m_DetectorType(saver, this, "detectorType",-1),
    m_DetectorTypeName(saver, this,"detectorTypeName",""),
    m_ExposureTime(saver, this,"exposureTime",0.1),
    m_SkippedExposuresAtStart(saver, this,"skippedExposuresAtStart",0),
    m_LastAcquired(saver, this, "LastAcquired", 0),
    m_PhasesInGroup(saver, this,"phasesInGroup",1),
    m_SummedExposures(saver, this,"summedExposures",1),
    m_SkippedExposures(saver, this,"skippedExposures",0),
    m_PreTriggerFiles(saver, this,"preTriggerFiles",0),
    m_PostTriggerFiles(saver, this,"postTriggerFiles",1),
    m_FileIndex(saver, this,"fileIndex",0),
    m_FilePattern(saver, this,"filePattern",""),
    m_FileIndexWidth(saver, this, "fileIndexWidth", 5),
    m_FilePhaseWidth(saver, this, "filePhaseWidth", 3),
    m_FileOverflowWidth(saver, this, "fileOverflowWidth", 5),
    m_DarkSummedExposures(saver, this,"darkSummedExposures",1),
    m_CameraGain(saver, this,"cameraGain",0),
    m_BinningMode(saver, this,"binningMode",0),
    m_FileBase(saver, this,"fileBase",""),
    m_NRows(saver, this, "nRows", 2048),
    m_NCols(saver, this, "nCols", 2048),
    m_OverflowLevel(saver, this, "overflowLevel", 65500),
    m_AcquireDark(saver, this, "acquireDark", 0),
    m_Cancelling(saver, this, "cancelling", 0),
    m_Triggered(saver, this, "triggered", 0),
    m_TotalBufferSizeMB32(saver, this,"totalBufferSizeMB32", 800),
    m_TotalBufferSizeMB64(saver, this,"totalBufferSizeMB64", 2000),
    m_Raw16SaveTime(saver, this,"raw16SaveTime", 0.1),
    m_Raw32SaveTime(saver, this,"raw32SaveTime", 0.2),
    m_RawSaveTime(saver, this,"rawSaveTime", 0.2),
    m_DarkSaveTime(saver, this,"darkSaveTime", 0.2),
    m_UserComment1(saver, this,"userComment1",""),
    m_UserComment2(saver, this,"userComment2",""),
    m_UserComment3(saver, this,"userComment3",""),
    m_UserComment4(saver, this,"userComment4",""),
    m_DroppedFrames(saver, this,"droppedFrames",0),
    m_Test(saver, this,"test",0),
    m_Mutex(QMutex::Recursive),
    m_SynchronizedAcquisition(NULL)
{
  connect(prop_Raw16SaveTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateSaveTimes()));
  connect(prop_Raw32SaveTime(), SIGNAL(valueChanged(double,int)), this, SLOT(updateSaveTimes()));
  connect(prop_SummedExposures(), SIGNAL(valueChanged(int,int)), this, SLOT(updateSaveTimes()));
  connect(prop_DarkSummedExposures(), SIGNAL(valueChanged(int,int)), this, SLOT(updateSaveTimes()));

//  m_FileIndex.setDebug(1);
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
    g_Application->printMessage(tr("acquisition.%1\n").arg(name.data()));
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

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionParameters::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->readSettings(settings, section+"/sync");
  }

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  if (get_TotalBufferSizeMB32() > 100000000) {
    set_TotalBufferSizeMB32(get_TotalBufferSizeMB32()/MegaBytes);
  }

  if (get_TotalBufferSizeMB64() > 100000000) {
    set_TotalBufferSizeMB64(get_TotalBufferSizeMB64()/MegaBytes);
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
