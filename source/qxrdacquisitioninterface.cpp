#include "qxrdacquisitioninterface.h"
#include <QThread>
#include <QtConcurrentRun>
#include "qxrdacquisition.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrddarkacquisitionparameterpack.h"
#include "qcepmutexlocker.h"
#include "qxrdacquisitionexecutionthread.h"

QxrdAcquisitionInterface::QxrdAcquisitionInterface(QcepSettingsSaverWPtr saver,
                                                   QxrdExperimentWPtr    doc,
                                                   QxrdDataProcessorWPtr proc,
                                                   QcepAllocatorWPtr     allocator)
  : QcepObject("acquistion", NULL),
    m_Saver(saver),
    m_ExecutionThread(),
    m_Cancelling(QcepSettingsSaverWPtr(), this, "cancelling", 0, "Cancel Acquisition?"),
    m_Triggered(QcepSettingsSaverWPtr(), this, "triggered", 0, "Trigger Acquisition"),
    m_ExposureTime(saver, this,"exposureTime",0.1, "Exposure Time (in sec)"),
    m_SkippedExposuresAtStart(saver, this,"skippedExposuresAtStart",0, "Exposures to Skip at Start"),
    m_PhasesInGroup(saver, this,"phasesInGroup",1, "Number of Image Phases"),
    m_SummedExposures(saver, this,"summedExposures",1, "Summed Exposures per Image"),
    m_DarkSummedExposures(saver, this,"darkSummedExposures",1, "Summed Exposures in Dark Image"),
    m_SkippedExposures(saver, this,"skippedExposures",0, "Skipped Exposures between Images"),
    m_PreTriggerFiles(saver, this,"preTriggerFiles",0, "Number of pre-Trigger Images"),
    m_PostTriggerFiles(saver, this,"postTriggerFiles",1, "Number of post-Trigger Images"),
    m_FilePattern(saver, this,"filePattern","", "File Name Pattern")
{
}

void QxrdAcquisitionInterface::initialize()
{
  m_ExecutionThread =
      QxrdAcquisitionExecutionThreadPtr(new QxrdAcquisitionExecutionThread(sharedFromThis()));

  m_ExecutionThread->start();
}

QxrdAcquisitionInterface::~QxrdAcquisitionInterface()
{
}

void QxrdAcquisitionInterface::acquire()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "acquire", Qt::BlockingQueuedConnection));
  } else if (sanityCheckAcquire()) {
    if (m_Acquiring.tryLock()) {
      set_Cancelling(false);
      set_Triggered(false);

      statusMessage("Starting acquisition");
      emit acquireStarted();

      //      QtConcurrent::run(this, &QxrdAcquisitionInterface::doAcquire, acquisitionParameterPack());

      if (m_ExecutionThread) {
        m_ExecutionThread->doAcquire();
      }
    } else {
      statusMessage("Acquisition is already in progress");
    }
  }
}

void QxrdAcquisitionInterface::acquireDark()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "acquireDark", Qt::BlockingQueuedConnection));
  } else if (sanityCheckAcquireDark()) {
    if (m_Acquiring.tryLock()) {
      set_Cancelling(false);
      set_Triggered(true);

      statusMessage("Starting dark acquisition");
      emit acquireStarted();

      //      QtConcurrent::run(this, &QxrdAcquisitionInterface::doAcquireDark, darkAcquisitionParameterPack());

      if (m_ExecutionThread) {
        m_ExecutionThread->doAcquireDark();
      }
    } else {
      statusMessage("Acquisition is already in progress");
    }
  }
}

void QxrdAcquisitionInterface::trigger()
{
  set_Triggered(true);
}

void QxrdAcquisitionInterface::cancel()
{
  set_Cancelling(true);

  m_NAcquiredImages.release(1);
}

//void QxrdAcquisitionInterface::onAcquireComplete()
//{
//  m_Acquiring.unlock();

//  emit acquireComplete();
//}

int  QxrdAcquisitionInterface::acquisitionStatus(double time)
{
  if (m_Acquiring.tryLock()) {
    m_Acquiring.unlock();

    //    printf("m_Acquiring.tryLock() succeeded\n");

    return 1;
  }

  QMutex mutex;
  QcepMutexLocker lock(__FILE__, __LINE__, &mutex);

  if (m_StatusWaiting.wait(&mutex, (int)(time*1000))) {
    //    printf("m_StatusWaiting.wait succeeded\n");
    return 1;
  } else {
    //    printf("m_StatusWaiting.wait failed\n");
    return 0;
  }
}

QxrdAcquisitionParameterPackPtr QxrdAcquisitionInterface::acquisitionParameterPack()
{
  return QxrdAcquisitionParameterPackPtr(
        new QxrdAcquisitionParameterPack (get_FilePattern(),
                                          get_ExposureTime(),
                                          get_SummedExposures(),
                                          get_PreTriggerFiles(),
                                          get_PostTriggerFiles(),
                                          get_PhasesInGroup(),
                                          get_SkippedExposuresAtStart(),
                                          get_SkippedExposures()));
}

QxrdDarkAcquisitionParameterPackPtr QxrdAcquisitionInterface::darkAcquisitionParameterPack()
{
  return QxrdDarkAcquisitionParameterPackPtr(
        new QxrdDarkAcquisitionParameterPack(get_FilePattern(),
                                          get_ExposureTime(),
                                          get_DarkSummedExposures(),
                                          get_SkippedExposuresAtStart()));

}


