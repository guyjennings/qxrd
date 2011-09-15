#include "qxrdacquisition.h"
#include "qxrdsynchronizedacquisition.h"
//#include <QMutexLocker>
#include "qxrdmutexlocker.h"
#include "qxrdsettings.h"
#include <QMetaProperty>
#include <QStringList>
#include <QThread>
#include <QtGlobal>
#include "qxrdapplication.h"

QxrdAcquisitionParameters::QxrdAcquisitionParameters()
  : QObject(),
    m_QxrdVersion(this,"qxrdVersion",STR(QXRD_VERSION)),
    m_QtVersion(this,"qtVersion",qVersion()),
    m_DetectorType(this, "detectorType",-1),
    m_DetectorTypeName(this,"detectorTypeName",""),
    m_ExposureTime(this,"exposureTime",0.1),
    m_SkippedExposuresAtStart(this,"skippedExposuresAtStart",0),
    m_PhasesInGroup(this,"phasesInGroup",1),
    m_SummedExposures(this,"summedExposures",1),
    m_SkippedExposures(this,"skippedExposures",0),
    m_PreTriggerFiles(this,"preTriggerFiles",0),
    m_PostTriggerFiles(this,"postTriggerFiles",1),
    m_FileIndex(this,"fileIndex",0),
    m_FilePattern(this,"filePattern",""),
    m_FileIndexWidth(this, "fileIndexWidth", 5),
    m_FilePhaseWidth(this, "filePhaseWidth", 3),
    m_FileOverflowWidth(this, "fileOverflowWidth", 5),
    m_DarkSummedExposures(this,"darkSummedExposures",1),
    m_CameraGain(this,"cameraGain",0),
    m_BinningMode(this,"binningMode",0),
    m_FileBase(this,"fileBase",""),
    m_NRows(this, "nRows", 2048),
    m_NCols(this, "nCols", 2048),
    m_OverflowLevel(this, "overflowLevel", 65500),
    m_AcquireDark(this, "acquireDark", 0),
    m_Cancelling(this, "cancelling", 0),
    m_Triggered(this, "triggered", 0),
    m_TotalBufferSizeMB32(this,"totalBufferSizeMB32", 800),
    m_TotalBufferSizeMB64(this,"totalBufferSizeMB64", 2000),
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

void QxrdAcquisitionParameters::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (m_SynchronizedAcquisition) {
    m_SynchronizedAcquisition->writeSettings(settings, section+"/sync");
  }

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionParameters::readSettings(QxrdSettings &settings, QString section)
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
