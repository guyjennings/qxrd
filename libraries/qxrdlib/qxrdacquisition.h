#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QAtomicInt>
#include <QTimer>
#include <QWaitCondition>
#include <QElapsedTimer>

#include "qcepobject.h"
//#include "qxrdrasterdata.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrddetectorsettings-ptr.h"
//#include "qxrddetectorthread-ptr.h"
//#include "qxrddetectorproxy-ptr.h"
#include "qxrddataprocessor-ptr.h"
//#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdprocessargs-ptr.h"
#include "qxrdacquisitionscalermodel-ptr.h"
#include "qxrdacquisitionexecutionthread-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdacquisitionparameterpack-ptr.h"
#include "qxrddarkacquisitionparameterpack-ptr.h"

class QXRD_EXPORT QxrdAcquisition : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdAcquisition(QString name);
  ~QxrdAcquisition();

  virtual void initialize();

  static QxrdAcquisitionPtr newAcquisition();

  virtual void addChildPtr(QcepObjectPtr child);
  virtual void removeChildPtr(QcepObjectPtr child);

  void setWindow(QxrdWindowWPtr win);

  QxrdAcquisitionWPtr myself();

public slots:
  void acquire();
  void acquireOnce();
  void acquireDark();
  void cancel();
  void trigger();

  int acquisitionStatus(double time);

  void propertyList();
  void Message(QString cmd);

  void dynamicProperties();
  void updateSaveTimes();

  void clearDropped();

  void shutdown();

  void doAcquire    ();
  void doAcquireOnce();
  void doAcquireDark();

  bool sanityCheckCommon();
  bool sanityCheckAcquire();
  bool sanityCheckAcquireDark();

  virtual void onExposureTimeChanged(double expos);

  void appendDetector(int detType);
  void appendDetector(QxrdDetectorSettingsPtr proxy);
  void clearDetectors();

  void configureDetector(int i);
  void openDetectorControlWindow(int i);

  QxrdDetectorSettingsWPtr      detector(int n);
  QxrdDetectorSettings         *det(int n);

  QString currentFileBase(int detNum, QString extension="");

  double scalerValue(int i);

public:
  void openWindows();
  void closeWindows();

  virtual void setupExposureMenu(QDoubleSpinBox *cb);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void copyDynamicProperties(QObject *dest);

  void indicateDroppedFrame(int n);
//  virtual QxrdAcquisitionDialogPtr controlPanel(QxrdWindowWPtr win);

  QxrdSynchronizedAcquisitionPtr synchronizedAcquisition() const;
  QxrdAcquisitionExtraInputsPtr acquisitionExtraInputs() const;

  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfacePtr nidaqPlugin() const;

  void setAcquisitionScalerModel(QxrdAcquisitionScalerModelPtr model);
  QxrdAcquisitionScalerModelPtr acquisitionScalerModel() const;

protected:
  void getFileBaseAndName(QString filePattern, QString extent, int detNum, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName);

  QxrdApplicationWPtr application() const;
  QxrdExperimentWPtr experiment() const;
  QxrdDataProcessorWPtr dataProcessor() const;

protected slots:
  void onIdleTimeout();

signals:
  void detectorStateChanged();
  void extraInputsChanged();
  void acquiredFrame(QString fileName, int iphase, int nphases, int isum, int nsum, int igroup, int ngroup);
  void acquireStarted();
  void acquireComplete();

private:
  void executeAcquisition(QxrdAcquisitionParameterPackPtr parmsp);
  virtual void stopIdling();
  virtual void startIdling();

  void accumulateAcquiredImage(QcepUInt16ImageDataPtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepUInt32ImageDataPtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepImageDataBasePtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow);

  void processImage(const QxrdProcessArgs &args);
  void processImage        (QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  void processAcquiredImage(QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  void processDarkImage    (QString filePattern, QString extent, int fileIndex,                                    QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);

  int cancelling();

protected:
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

public:
  QxrdAcquisitionParameterPackPtr acquisitionParameterPack();
  QxrdDarkAcquisitionParameterPackPtr darkAcquisitionParameterPack();

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

  Q_PROPERTY(QString qxrdVersion READ get_QxrdVersion STORED false)
  QCEP_STRING_PROPERTY(QxrdVersion)

  Q_PROPERTY(QString qtVersion READ get_QtVersion STORED false)
  QCEP_STRING_PROPERTY(QtVersion)

  Q_PROPERTY(int detectorCount READ get_DetectorCount WRITE set_DetectorCount STORED false)
  QCEP_INTEGER_PROPERTY(DetectorCount)

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

  Q_PROPERTY(int detectorNumberWidth READ get_DetectorNumberWidth WRITE set_DetectorNumberWidth)
  QCEP_INTEGER_PROPERTY(DetectorNumberWidth)

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
  mutable QMutex                   m_Mutex;

  QxrdSynchronizedAcquisitionPtr   m_SynchronizedAcquisition;
  QxrdAcquisitionExtraInputsPtr    m_AcquisitionExtraInputs;

  QxrdWindowWPtr                   m_Window;

//  QxrdAcquisitionDialogPtr         m_ControlPanel;

  QTimer                           m_IdleTimer;
  QAtomicInt                       m_Idling;

  QVector<QxrdDetectorSettingsPtr> m_Detectors;

  QxrdAcquisitionScalerModelPtr    m_ScalerModel;
};

Q_DECLARE_METATYPE(QxrdAcquisition*)

#endif
