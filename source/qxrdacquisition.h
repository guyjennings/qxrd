#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qcepmacros.h"
#include "qxrdacquisitioninterface.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QAtomicInt>
#include <QTimer>

#if QT_VERSION >= 0x040700
#include <QElapsedTimer>
#endif

#include "qxrdrasterdata.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitiondialog-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrddetectorthread-ptr.h"
#include "qxrddetectorproxy-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qcepallocator-ptr.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdprocessargs-ptr.h"
#include "qxrdacquisitionscalermodel-ptr.h"

class QxrdAcquisition : public QxrdAcquisitionInterface
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

  void clearDropped();

  void shutdown();

  void onBufferSizeChanged(int newMB);

  void doAcquire    ();
  void doAcquireDark();

  bool sanityCheckCommon();
  bool sanityCheckAcquire();
  bool sanityCheckAcquireDark();

  virtual void onExposureTimeChanged();

  void appendDetector(int detType);
  void appendDetectorProxy(QxrdDetectorProxyPtr proxy);
  void clearDetectors();

  void configureDetector(int i);
  void openDetectorControlWindow(int i);

  QxrdDetectorThreadPtr detectorThread(int n);
  QxrdDetectorPtr       detector(int n);

  QString currentFileBase(int detNum, QString extension="");

  double scalerValue(int i);

public:
  virtual void setupExposureMenu(QDoubleSpinBox *cb);

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

  QxrdAcquisitionScalerModelPtr acquisitionScalerModel() const;

protected:
  void acquisition(int isDark);
  void copyParameters(int isDark);

  void getFileBaseAndName(QString filePattern, QString extent, int detNum, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName);

  QxrdExperimentWPtr experiment();
  QxrdDataProcessorWPtr dataProcessor();

  QxrdAcquisitionPtr myself();

protected slots:
  void onIdleTimeout();

signals:
  void detectorStateChanged();
  void extraInputsChanged();

private:
  virtual void stopIdling();
  virtual void startIdling();

  void accumulateAcquiredImage(QcepInt16ImageDataPtr image, QcepInt32ImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepInt32ImageDataPtr image, QcepInt32ImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepImageDataBasePtr image, QcepInt32ImageDataPtr accum, QcepMaskDataPtr overflow);

  void processImage(const QxrdProcessArgs &args);
  void processImage        (QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  void processAcquiredImage(QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  void processDarkImage    (QString filePattern, QString extent, int fileIndex,                                    QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow);

  int cancelling();

public:
  Q_PROPERTY(QString qxrdVersion READ get_QxrdVersion STORED false)
  QCEP_STRING_PROPERTY(QxrdVersion)

  Q_PROPERTY(QString qtVersion READ get_QtVersion STORED false)
  QCEP_STRING_PROPERTY(QtVersion)

  Q_PROPERTY(int detectorCount READ get_DetectorCount WRITE set_DetectorCount)
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
  mutable QMutex                m_Mutex;

  QxrdSynchronizedAcquisitionPtr  m_SynchronizedAcquisition;
  QxrdAcquisitionExtraInputsPtr   m_AcquisitionExtraInputs;

  QxrdExperimentWPtr     m_Experiment;
  QxrdWindowWPtr         m_Window;
  QcepAllocatorWPtr      m_Allocator;
  QxrdDataProcessorWPtr  m_DataProcessor;

  QxrdAcquisitionDialogPtr m_ControlPanel;

  QTimer                 m_IdleTimer;
  QAtomicInt             m_Idling;

  QVector<QxrdDetectorThreadPtr>    m_DetectorThreads;
  QVector<QxrdDetectorPtr>          m_Detectors;

  QxrdAcquisitionScalerModelPtr m_ScalerModel;
};

#endif
