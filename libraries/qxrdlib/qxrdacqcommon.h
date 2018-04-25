#ifndef QXRDACQCOMMON_H
#define QXRDACQCOMMON_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepimagedata-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitionparameterpack-ptr.h"
#include "qxrddarkacquisitionparameterpack-ptr.h"
#include "qxrdacquisitionscalermodel-ptr.h"
#include "qxrdacquisitioneventlog-ptr.h"
#include "qxrdsynchronizer-ptr.h"

class QXRD_EXPORT QxrdAcqCommon : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdAcqCommon(QString name);
  ~QxrdAcqCommon();

  virtual void initialize(QcepObjectWPtr);
  virtual void shutdown() = 0;

  static QxrdAcqCommonWPtr findAcquisition(QcepObjectWPtr p);

  virtual int detectorCount() const = 0;
  virtual int activeDetectorCount() const;
  virtual QxrdDetectorSettingsPtr detector(int i) const = 0;

  virtual QxrdSynchronizerWPtr synchronizer() = 0;

  QxrdAcquisitionScalerModelPtr acquisitionScalerModel() const;

  double scalerValue(int i);

  virtual QxrdSynchronizedAcquisitionPtr synchronizedAcquisition() const = 0;

  QxrdAcquisitionEventLogWPtr   acquisitionEventLog() const;

  enum {
    StartEvent,
    StartAcquireEvent,
    StartAcquireOnceEvent,
    StartAcquireDarkEvent,
    StartAcquireIdleEvent,
    AcquireComplete,
    NIDAQStartEvent,
    NIDAQSyncEvent,
    NIDAQAnalogInputEvent,
    NIDAQAnalogPostEvent,
    DetectorFrameEvent,
    DetectorFramePostedEvent,
    AcquireSkip,
    AcquireFrame,
    AcquireDark,
    AcquirePost
  };

  void clearEventLog();
  void pauseEventLog();
  void resumeEventLog();
  void appendEvent(int eventCode,
                   int eventArg1 = -1,
                   int eventArg2 = -1,
                   QDateTime eventTime=QDateTime::currentDateTime());

  virtual void setupAcquisition() = 0;
  virtual void acquire() = 0;
  virtual void acquireOnce() = 0;
  virtual void acquireDark() = 0;
  virtual void cancel() = 0;
  virtual void trigger() = 0;

  virtual int  acquisitionStatus(double time) = 0;

  virtual void clearDropped() = 0;
  virtual void configureDetector(int i) = 0;
  virtual void openDetectorControlWindow(int i) = 0;

  QxrdAcquisitionParameterPackPtr acquisitionParameterPack();
  QxrdDarkAcquisitionParameterPackPtr darkAcquisitionParameterPack();

  virtual QxrdDetectorSettingsPtr newDetector(int detType) = 0;
  virtual void appendDetector(QxrdDetectorSettingsPtr det) = 0;
  virtual void clearDetectors() = 0;

  Q_INVOKABLE void dynamicProperties();
  void copyDynamicProperties(QObject *dest);

  int cancelling();
  void indicateDroppedFrame(int n);

  virtual void unlock();

  void fillAcquisitionProperties(QcepImageDataBasePtr img);

public slots:
  void setOutputFormat(int fmt);
  void setOutputCompression(int cmp, int lvl=0);

private slots:
  void restartDetectors();

signals:
  void acquireStarted();
  void acquireComplete();
  void acquiredFrame(QString fileName, int iphase, int nphases, int isum, int nsum, int igroup, int ngroup);
  void detectorStateChanged();
  void extraInputsChanged();

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

  Q_PROPERTY(bool   applyToAll       READ get_ApplyToAll      WRITE set_ApplyToAll)
  QCEP_BOOLEAN_PROPERTY(ApplyToAll)

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

  Q_PROPERTY(QString qxrdVersion READ get_QxrdVersion STORED false)
  QCEP_STRING_PROPERTY(QxrdVersion)

  Q_PROPERTY(QString qtVersion READ get_QtVersion STORED false)
  QCEP_STRING_PROPERTY(QtVersion)

  Q_PROPERTY(int    lastAcquired  READ get_LastAcquired WRITE set_LastAcquired STORED false)
  QCEP_INTEGER_PROPERTY(LastAcquired)

  Q_PROPERTY(int    fileIndex        READ get_FileIndex WRITE set_FileIndex)
  QCEP_INTEGER_PROPERTY(FileIndex)

  Q_PROPERTY(int    fileIndexWidth        READ get_FileIndexWidth WRITE set_FileIndexWidth)
  QCEP_INTEGER_PROPERTY(FileIndexWidth)

  Q_PROPERTY(int    filePhaseWidth        READ get_FilePhaseWidth WRITE set_FilePhaseWidth)
  QCEP_INTEGER_PROPERTY(FilePhaseWidth)

  Q_PROPERTY(int    fileOverflowWidth        READ get_FileOverflowWidth WRITE set_FileOverflowWidth)
  QCEP_INTEGER_PROPERTY(FileOverflowWidth)

  Q_PROPERTY(int    fileNumberWidth  READ get_FileNumberWidth WRITE set_FileNumberWidth)
  QCEP_INTEGER_PROPERTY(FileNumberWidth)

  Q_PROPERTY(int detectorNumberWidth READ get_DetectorNumberWidth WRITE set_DetectorNumberWidth)
  QCEP_INTEGER_PROPERTY(DetectorNumberWidth)

  Q_PROPERTY(int    fileNameFormat1   READ get_FileNameFormat1 WRITE set_FileNameFormat1)
  QCEP_INTEGER_PROPERTY(FileNameFormat1)

  Q_PROPERTY(int    fileNameFormat2   READ get_FileNameFormat2 WRITE set_FileNameFormat2)
  QCEP_INTEGER_PROPERTY(FileNameFormat2)

  Q_PROPERTY(int    fileNameFormat3   READ get_FileNameFormat3 WRITE set_FileNameFormat3)
  QCEP_INTEGER_PROPERTY(FileNameFormat3)

  Q_PROPERTY(int    fileNameFormat4   READ get_FileNameFormat4 WRITE set_FileNameFormat4)
  QCEP_INTEGER_PROPERTY(FileNameFormat4)

  Q_PROPERTY(QString fileBase        READ get_FileBase WRITE set_FileBase)
  QCEP_STRING_PROPERTY(FileBase)

  Q_PROPERTY(int     overflowLevel      READ get_OverflowLevel WRITE set_OverflowLevel)
  QCEP_INTEGER_PROPERTY(OverflowLevel)

  Q_PROPERTY(double     raw16SaveTime    READ get_Raw16SaveTime WRITE set_Raw16SaveTime)
  QCEP_DOUBLE_PROPERTY(Raw16SaveTime)

  Q_PROPERTY(double     raw32SaveTime    READ get_Raw32SaveTime WRITE set_Raw32SaveTime)
  QCEP_DOUBLE_PROPERTY(Raw32SaveTime)

  Q_PROPERTY(double     rawSaveTime    READ get_RawSaveTime WRITE set_RawSaveTime)
  QCEP_DOUBLE_PROPERTY(RawSaveTime)

  Q_PROPERTY(double     darkSaveTime    READ get_DarkSaveTime WRITE set_DarkSaveTime)
  QCEP_DOUBLE_PROPERTY(DarkSaveTime)

  Q_PROPERTY(QString userComment1 READ get_UserComment1 WRITE set_UserComment1)
  QCEP_STRING_PROPERTY(UserComment1)

  Q_PROPERTY(QString userComment2 READ get_UserComment2 WRITE set_UserComment2)
  QCEP_STRING_PROPERTY(UserComment2)

  Q_PROPERTY(QString userComment3 READ get_UserComment3 WRITE set_UserComment3)
  QCEP_STRING_PROPERTY(UserComment3)

  Q_PROPERTY(QString userComment4 READ get_UserComment4 WRITE set_UserComment4)
  QCEP_STRING_PROPERTY(UserComment4)

  Q_PROPERTY(QcepDoubleList normalization READ get_Normalization WRITE set_Normalization)
  QCEP_DOUBLE_LIST_PROPERTY(Normalization)

  Q_PROPERTY(int     droppedFrames    READ get_DroppedFrames WRITE set_DroppedFrames STORED false)
  QCEP_INTEGER_PROPERTY(DroppedFrames)

  Q_PROPERTY(bool     liveViewAtIdle   READ get_LiveViewAtIdle WRITE set_LiveViewAtIdle)
  QCEP_BOOLEAN_PROPERTY(LiveViewAtIdle)

  Q_PROPERTY(bool     acquisitionCancelsLiveView READ get_AcquisitionCancelsLiveView WRITE set_AcquisitionCancelsLiveView)
  QCEP_BOOLEAN_PROPERTY(AcquisitionCancelsLiveView)

  Q_PROPERTY(bool     retryDropped READ get_RetryDropped WRITE set_RetryDropped)
  QCEP_BOOLEAN_PROPERTY(RetryDropped)

  Q_PROPERTY(QcepDoubleVector scalerValues READ get_ScalerValues WRITE set_ScalerValues STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(ScalerValues)

private:
  QxrdAcquisitionScalerModelPtr    m_ScalerModel;
  QxrdAcquisitionEventLogPtr       m_AcquisitionEventLog;
};

#endif // QXRDACQCOMMON_H
