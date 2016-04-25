#ifndef QXRDACQUISITIONINTERFACE_H
#define QXRDACQUISITIONINTERFACE_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include <QWaitCondition>
//#include <QFutureWatcher>
#include <QSemaphore>

//#include <QEnableSharedFromThis>
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdacquisitionexecutionthread-ptr.h"
#include "qxrdacquisitionparameterpack-ptr.h"
#include "qxrddarkacquisitionparameterpack-ptr.h"

class QxrdAcquisitionInterface : public QcepObject
{
  Q_OBJECT

public:
  QxrdAcquisitionInterface(QcepSettingsSaverWPtr saver,
                           QxrdExperimentWPtr doc,
                           QxrdDataProcessorWPtr proc,
                           QcepAllocatorWPtr allocator);
  ~QxrdAcquisitionInterface();

  virtual void initialize();

signals:
  void acquiredFrame(QString fileName, int iphase, int nphases, int isum, int nsum, int igroup, int ngroup);
  void acquireStarted();
  void acquireComplete();

public slots:
  void acquire();
  void acquireDark();
  void cancel();
  void trigger();

  int acquisitionStatus(double time);

  virtual void doAcquire    () = 0;
  virtual void doAcquireDark() = 0;

  virtual bool sanityCheckAcquire()     = 0;
  virtual bool sanityCheckAcquireDark() = 0;

public:
  QxrdAcquisitionParameterPackPtr acquisitionParameterPack();
  QxrdDarkAcquisitionParameterPackPtr darkAcquisitionParameterPack();

protected:
  QcepSettingsSaverWPtr  m_Saver;
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

private:
  QxrdAcquisitionExecutionThreadPtr m_ExecutionThread;

public:
  Q_PROPERTY(int     cancelling      READ get_Cancelling WRITE set_Cancelling STORED false)
  QCEP_INTEGER_PROPERTY(Cancelling)

  Q_PROPERTY(int     triggered      READ get_Triggered WRITE set_Triggered STORED false)
  QCEP_INTEGER_PROPERTY(Triggered)

  Q_PROPERTY(double exposureTime     READ get_ExposureTime WRITE set_ExposureTime)
  QCEP_DOUBLE_PROPERTY(ExposureTime)

  Q_PROPERTY(int    skippedExposuresAtStart  READ get_SkippedExposuresAtStart WRITE set_SkippedExposuresAtStart)
  QCEP_INTEGER_PROPERTY(SkippedExposuresAtStart)

  Q_PROPERTY(int    phasesInGroup  READ get_PhasesInGroup WRITE set_PhasesInGroup)
  QCEP_INTEGER_PROPERTY(PhasesInGroup)

  Q_PROPERTY(int    currentPhase READ get_CurrentPhase WRITE set_CurrentPhase STORED false)
  QCEP_INTEGER_PROPERTY(CurrentPhase)

  Q_PROPERTY(int    summedExposures  READ get_SummedExposures WRITE set_SummedExposures)
  QCEP_INTEGER_PROPERTY(SummedExposures)

  Q_PROPERTY(int    darkSummedExposures READ get_DarkSummedExposures WRITE set_DarkSummedExposures)
  QCEP_INTEGER_PROPERTY(DarkSummedExposures)

  Q_PROPERTY(int    currentSummation READ get_CurrentSummation WRITE set_CurrentSummation STORED false)
  QCEP_INTEGER_PROPERTY(CurrentSummation)

  Q_PROPERTY(int    skippedExposures  READ get_SkippedExposures WRITE set_SkippedExposures)
  QCEP_INTEGER_PROPERTY(SkippedExposures)

  Q_PROPERTY(int    preTriggerFiles  READ get_PreTriggerFiles WRITE set_PreTriggerFiles)
  QCEP_INTEGER_PROPERTY(PreTriggerFiles)

  Q_PROPERTY(int    postTriggerFiles  READ get_PostTriggerFiles WRITE set_PostTriggerFiles)
  QCEP_INTEGER_PROPERTY(PostTriggerFiles)

  Q_PROPERTY(int    currentFile  READ get_CurrentFile WRITE set_CurrentFile STORED false)
  QCEP_INTEGER_PROPERTY(CurrentFile)

  Q_PROPERTY(QString filePattern     READ get_FilePattern WRITE set_FilePattern)
  QCEP_STRING_PROPERTY(FilePattern)

};

#endif // QXRDACQUISITIONINTERFACE_H
