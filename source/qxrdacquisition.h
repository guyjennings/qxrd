#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qcepmacros.h"

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
#include "qxrdimagequeue.h"
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
#include "qxrdobjectnamer.h"

class QxrdAcquisitionParameterPack
{
public:
  QxrdAcquisitionParameterPack(QString fileBase, double exposure, int nsummed, int preTrigger, int postTrigger, int nphases, int skipBefore, int skipBetween)
    : m_FileBase(fileBase), m_Exposure(exposure), m_NSummed(nsummed), m_PreTrigger(preTrigger), m_PostTrigger(postTrigger),
      m_NPhases(nphases), m_SkipBefore(skipBefore), m_SkipBetween(skipBetween) {}

  QString fileBase() { return m_FileBase; }
  double  exposure() { return m_Exposure; }
  int     nsummed()  { return m_NSummed; }
  int     preTrigger()   { return m_PreTrigger; }
  int     postTrigger()   { return m_PostTrigger; }
  int     nphases()  { return m_NPhases; }
  int     skipBefore() { return m_SkipBefore; }
  int     skipBetween() { return m_SkipBetween; }

private:
  QString m_FileBase;
  double  m_Exposure;
  int     m_NSummed;
  int     m_PreTrigger;
  int     m_PostTrigger;
  int     m_NPhases;
  int     m_SkipBefore;
  int     m_SkipBetween;
};

class QxrdDarkAcquisitionParameterPack
{
public:
  QxrdDarkAcquisitionParameterPack(QString fileBase, double exposure, int nsummed, int skipBefore)
    : m_FileBase(fileBase), m_Exposure(exposure), m_NSummed(nsummed), m_SkipBefore(skipBefore) {}

  QString fileBase() { return m_FileBase; }
  double  exposure() { return m_Exposure; }
  int     nsummed()  { return m_NSummed; }
  int     skipBefore() { return m_SkipBefore; }

private:
  QString m_FileBase;
  double  m_Exposure;
  int     m_NSummed;
  int     m_SkipBefore;
};

class QxrdProcessArgs {
public:
  QxrdProcessArgs(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow)
    : m_FilePattern(filePattern),
      m_FileIndex(fileIndex),
      m_Phase(phase),
      m_NPhases(nPhases),
      m_Trig(trig),
      m_Image(image),
      m_Overflow(overflow) {}

public:
  QString               m_FilePattern;
  int                   m_FileIndex;
  int                   m_Phase;
  int                   m_NPhases;
  bool                  m_Trig;
  QxrdInt32ImageDataPtr m_Image;
  QxrdMaskDataPtr       m_Overflow;
};

class QxrdAcquisition : public QObject
{
  Q_OBJECT

public:
  QxrdAcquisition(QxrdSettingsSaverWPtr saver,
                  QxrdExperimentWPtr doc,
                  QxrdDataProcessorWPtr proc,
                  QxrdAllocatorWPtr allocator);
  ~QxrdAcquisition();
  void initialize(QxrdAcquisitionWPtr acq);

  void setWindow(QxrdWindowWPtr win);
  void setDetector(QxrdDetectorWPtr det);

public slots:
  void propertyList();
  void Message(QString cmd);

  void dynamicProperties();
  void updateSaveTimes();

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  void acquire();
  void acquireDark();
  void cancel();
  //  void cancelDark();
  void trigger();
  void clearDropped();

  void shutdown();


  int acquisitionStatus(double time);

  void onExposureTimeChanged();
  void onBinningModeChanged();
  void onCameraGainChanged();

  void onBufferSizeChanged(int newMB);

  void doAcquire    (QxrdAcquisitionParameterPack parms);
  void doAcquireDark(QxrdDarkAcquisitionParameterPack parms);

public:
  void enqueueAcquiredFrame(QxrdInt16ImageDataPtr img);

signals:
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireStarted();
  void acquireComplete();

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void copyDynamicProperties(QObject *dest);

  int currentPhase(int frameNumber);

  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);

  void indicateDroppedFrame(int n);
  virtual QxrdAcquisitionDialogPtr controlPanel(QxrdWindowWPtr win);
  QxrdAllocatorWPtr allocator() const;

  QxrdSynchronizedAcquisitionPtr synchronizedAcquisition() const;
  QxrdAcquisitionExtraInputsPtr acquisitionExtraInputs() const;

  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfacePtr nidaqPlugin() const;

  QxrdAcquisitionParameterPack acquisitionParameterPack();
  QxrdDarkAcquisitionParameterPack darkAcquisitionParameterPack();

protected:
  void acquisition(int isDark);
  void copyParameters(int isDark);

  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

  void getFileBaseAndName(QString filePattern, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName);

protected slots:
  void onAcquireComplete();
  void onIdleTimeout();

private:
  enum { MegaBytes = 0x100000 };

  QxrdInt16ImageDataPtr acquireFrame(double exposure);
  QxrdInt16ImageDataPtr acquireFrameIfAvailable(double exposure);
  void flushImageQueue();

  virtual void stopIdling();
  virtual void startIdling();

  template <typename T>
  void accumulateAcquiredImage(QSharedPointer< QxrdImageData<T> > image, QxrdInt32ImageDataPtr accum, QxrdMaskDataPtr overflow);

  void processImage(const QxrdProcessArgs &args);
  void processImage        (QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);
  void processAcquiredImage(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);
  void processDarkImage    (QString filePattern, int fileIndex,                                    QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);

  int cancelling();

private:
  QxrdObjectNamer               m_ObjectNamer;
  QxrdSettingsSaverWPtr         m_Saver;

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

private:
  mutable QMutex                m_Mutex;

  QxrdAcquisitionWPtr             m_Acquisition;
  QxrdSynchronizedAcquisitionPtr  m_SynchronizedAcquisition;
  QxrdAcquisitionExtraInputsPtr   m_AcquisitionExtraInputs;

  QxrdExperimentWPtr     m_Experiment;
  QxrdWindowWPtr         m_Window;
  QxrdAllocatorWPtr      m_Allocator;
  QxrdDataProcessorWPtr  m_DataProcessor;
  QxrdDetectorWPtr       m_Detector;

  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

  QSemaphore             m_NAcquiredImages;
  QxrdInt16ImageQueue    m_AcquiredImages;

  QxrdAcquisitionDialogPtr m_ControlPanel;

  QFutureWatcher<void>   m_Watcher;

  QTimer                 m_IdleTimer;
  QAtomicInt             m_Idling;
};

typedef QSharedPointer<QxrdAcquisition> QxrdAcquisitionPtr;
typedef QWeakPointer<QxrdAcquisition> QxrdAcquisitionWPtr;

#endif
