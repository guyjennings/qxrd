#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qcepmacros.h"
#include "qcepobject.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QAtomicInt>
#include <QFutureWatcher>
#include <QTimer>
#include <QSemaphore>

#if QT_VERSION >= 0x040700
#include <QElapsedTimer>
#endif

#include "qxrdrasterdata.h"
#include "qcepimagequeue.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitiondialog-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qcepallocator-ptr.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdacquisitionparameterpack-ptr.h"
#include "qxrddarkacquisitionparameterpack-ptr.h"
#include "qxrdprocessargs-ptr.h"

class QxrdAcquisition : public QcepObject, public QEnableSharedFromThis<QxrdAcquisition>
{
  Q_OBJECT

public:
  QxrdAcquisition(QcepSettingsSaverWPtr saver,
                  QxrdExperimentWPtr doc,
                  QxrdDataProcessorWPtr proc,
                  QcepAllocatorWPtr allocator);
  ~QxrdAcquisition();
  void initialize();

  void setWindow(QxrdWindowWPtr win);

public slots:
  void propertyList();
  void Message(QString cmd);

  void dynamicProperties();
  void updateSaveTimes();

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

  void acquire();
  void acquireDark();
  void cancel();
  //  void cancelDark();
  void trigger();
  void clearDropped();

  void shutdown();


  int acquisitionStatus(double time);

  void onBufferSizeChanged(int newMB);

  void doAcquire    (QxrdAcquisitionParameterPackWPtr parms);
  void doAcquireDark(QxrdDarkAcquisitionParameterPackWPtr parms);

  virtual void onExposureTimeChanged() = 0;
  virtual void onBinningModeChanged() = 0;
  virtual void onCameraGainChanged() = 0;

public:
  void enqueueAcquiredFrame(QcepInt16ImageDataPtr img);

signals:
  void acquiredFrame(QString fileName, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireStarted();
  void acquireComplete();

public:
  virtual void beginAcquisition() = 0;
  virtual void endAcquisition() = 0;
  virtual void shutdownAcquisition() = 0;

  virtual void setupExposureMenu(QDoubleSpinBox *cb) = 0;
  virtual void setupCameraGainMenu(QComboBox *cb) = 0;
  virtual void setupCameraBinningModeMenu(QComboBox *cb) = 0;

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void copyDynamicProperties(QObject *dest);

  int currentPhase(int frameNumber);

  void indicateDroppedFrame(int n);
  virtual QxrdAcquisitionDialogPtr controlPanel(QxrdWindowWPtr win);
  QcepAllocatorWPtr allocator() const;

  QxrdSynchronizedAcquisitionPtr synchronizedAcquisition() const;
  QxrdAcquisitionExtraInputsPtr acquisitionExtraInputs() const;

  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfacePtr nidaqPlugin() const;

  QxrdAcquisitionParameterPackPtr acquisitionParameterPack();
  QxrdDarkAcquisitionParameterPackPtr darkAcquisitionParameterPack();

protected:
  void acquisition(int isDark);
  void copyParameters(int isDark);

  void getFileBaseAndName(QString filePattern, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName);

  QxrdExperimentWPtr experiment();
  QxrdDataProcessorWPtr dataProcessor();

protected slots:
  void onAcquireComplete();
  void onIdleTimeout();

private:
  enum { MegaBytes = 0x100000 };

  QcepInt16ImageDataPtr acquireFrame(double exposure);
  QcepInt16ImageDataPtr acquireFrameIfAvailable(double exposure);
  void flushImageQueue();

  virtual void stopIdling();
  virtual void startIdling();

  void accumulateAcquiredImage(QcepInt16ImageDataPtr image, QcepInt32ImageDataPtr accum, QcepMaskDataPtr overflow);

  void processImage(const QxrdProcessArgs &args);
  void processImage        (QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  void processAcquiredImage(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  void processDarkImage    (QString filePattern, int fileIndex,                                    QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow);

  int cancelling();

protected:
  QcepSettingsSaverWPtr         m_Saver;

public:
  Q_PROPERTY(QString qxrdVersion READ get_QxrdVersion STORED false)
  QCEP_STRING_PROPERTY(QxrdVersion)

  Q_PROPERTY(QString qtVersion READ get_QtVersion STORED false)
  QCEP_STRING_PROPERTY(QtVersion)

  Q_PROPERTY(double exposureTime     READ get_ExposureTime WRITE set_ExposureTime)
  QCEP_DOUBLE_PROPERTY(ExposureTime)

  Q_PROPERTY(int    skippedExposuresAtStart  READ get_SkippedExposuresAtStart WRITE set_SkippedExposuresAtStart)
  QCEP_INTEGER_PROPERTY(SkippedExposuresAtStart)

  Q_PROPERTY(int    lastAcquired  READ get_LastAcquired WRITE set_LastAcquired STORED false)
  QCEP_INTEGER_PROPERTY(LastAcquired)

  Q_PROPERTY(int    phasesInGroup  READ get_PhasesInGroup WRITE set_PhasesInGroup)
  QCEP_INTEGER_PROPERTY(PhasesInGroup)

  Q_PROPERTY(int    summedExposures  READ get_SummedExposures WRITE set_SummedExposures)
  QCEP_INTEGER_PROPERTY(SummedExposures)

  Q_PROPERTY(int    skippedExposures  READ get_SkippedExposures WRITE set_SkippedExposures)
  QCEP_INTEGER_PROPERTY(SkippedExposures)

  Q_PROPERTY(int    preTriggerFiles  READ get_PreTriggerFiles WRITE set_PreTriggerFiles)
  QCEP_INTEGER_PROPERTY(PreTriggerFiles)

  Q_PROPERTY(int    postTriggerFiles  READ get_PostTriggerFiles WRITE set_PostTriggerFiles)
  QCEP_INTEGER_PROPERTY(PostTriggerFiles)

  Q_PROPERTY(int    fileIndex        READ get_FileIndex WRITE set_FileIndex)
  QCEP_INTEGER_PROPERTY(FileIndex)

  Q_PROPERTY(QString filePattern     READ get_FilePattern WRITE set_FilePattern)
  QCEP_STRING_PROPERTY(FilePattern)

  Q_PROPERTY(int    fileIndexWidth        READ get_FileIndexWidth WRITE set_FileIndexWidth)
  QCEP_INTEGER_PROPERTY(FileIndexWidth)

  Q_PROPERTY(int    filePhaseWidth        READ get_FilePhaseWidth WRITE set_FilePhaseWidth)
  QCEP_INTEGER_PROPERTY(FilePhaseWidth)

  Q_PROPERTY(int    fileOverflowWidth        READ get_FileOverflowWidth WRITE set_FileOverflowWidth)
  QCEP_INTEGER_PROPERTY(FileOverflowWidth)

  Q_PROPERTY(int     darkSummedExposures READ get_DarkSummedExposures WRITE set_DarkSummedExposures)
  QCEP_INTEGER_PROPERTY(DarkSummedExposures)

  Q_PROPERTY(int     cameraGain      READ get_CameraGain WRITE set_CameraGain)
  QCEP_INTEGER_PROPERTY(CameraGain)

  Q_PROPERTY(int     binningMode      READ get_BinningMode WRITE set_BinningMode)
  QCEP_INTEGER_PROPERTY(BinningMode)

  Q_PROPERTY(QString fileBase        READ get_FileBase WRITE set_FileBase)
  QCEP_STRING_PROPERTY(FileBase)

  Q_PROPERTY(int     nRows      READ get_NRows WRITE set_NRows)
  QCEP_INTEGER_PROPERTY(NRows)

  Q_PROPERTY(int     nCols      READ get_NCols WRITE set_NCols)
  QCEP_INTEGER_PROPERTY(NCols)

  Q_PROPERTY(int     overflowLevel      READ get_OverflowLevel WRITE set_OverflowLevel)
  QCEP_INTEGER_PROPERTY(OverflowLevel)

  Q_PROPERTY(int     acquireDark      READ get_AcquireDark WRITE set_AcquireDark STORED false)
  QCEP_INTEGER_PROPERTY(AcquireDark)

  Q_PROPERTY(int     cancelling      READ get_Cancelling WRITE set_Cancelling STORED false)
  QCEP_INTEGER_PROPERTY(Cancelling)

  Q_PROPERTY(int     triggered      READ get_Triggered WRITE set_Triggered STORED false)
  QCEP_INTEGER_PROPERTY(Triggered)

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

private:
  mutable QMutex                m_Mutex;

  QxrdSynchronizedAcquisitionPtr  m_SynchronizedAcquisition;
  QxrdAcquisitionExtraInputsPtr   m_AcquisitionExtraInputs;

  QxrdExperimentWPtr     m_Experiment;
  QxrdWindowWPtr         m_Window;
  QcepAllocatorWPtr      m_Allocator;
  QxrdDataProcessorWPtr  m_DataProcessor;

  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

  QSemaphore             m_NAcquiredImages;
  QcepInt16ImageQueue    m_AcquiredImages;

  QxrdAcquisitionDialogPtr m_ControlPanel;

  QFutureWatcher<void>   m_Watcher;

  QTimer                 m_IdleTimer;
  QAtomicInt             m_Idling;
};

typedef QSharedPointer<QxrdAcquisition> QxrdAcquisitionPtr;
typedef QWeakPointer<QxrdAcquisition> QxrdAcquisitionWPtr;

#endif
