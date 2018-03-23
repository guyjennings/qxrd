#ifndef QXRDNIDAQSYNCHRONIZER_H
#define QXRDNIDAQSYNCHRONIZER_H

#include "qcepobject.h"
#include <QDateTime>
#include <QMutex>
#include <QStringList>
#include <QVector>

#include "qxrdsynchronizer.h"
#include "NIDAQmx.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdsynchronizeddetectorchannel-ptr.h"
#include "qxrdsynchronizedoutputchannel-ptr.h"
#include "qxrdsynchronizedinputchannel-ptr.h"

class QxrdNIDAQSynchronizer : public QxrdSynchronizer
{
  Q_OBJECT

private:
  typedef QxrdSynchronizer inherited;

public:
  QxrdNIDAQSynchronizer(QString name);
  virtual ~QxrdNIDAQSynchronizer();

  void initialize(QcepObjectWPtr parent);
  QString name() const;

  void startSynchronizer();
  void stopSynchronizer();

public slots:
  void   changeExposureTime(double t, int n);

  void   clearSync();
  void   clearDetectorSync();
  void   clearOutputChannels();
  void   clearInputChannels();

  void   addDetectorSync(QxrdSynchronizedDetectorChannelWPtr p);
  void   addOutputChannel(QxrdSynchronizedOutputChannelWPtr p);
  void   addInputChannel(QxrdSynchronizedInputChannelWPtr p);

  void   startSync();
  void   startDetectorSync();
  void   startOutputChannels();
  void   startInputChannels();

  QStringList deviceNames();
  int         deviceIsSimulated(QString device);
  QStringList deviceAIChannels(QString device);
  QStringList deviceAOChannels(QString device);
  QStringList deviceCOChannels(QString device);

  int detectorDeviceCount();
  QString detectorDeviceName(int n);

  int outputDeviceCount();
  QString outputDeviceName(int n);

  int inputDeviceCount();
  QString inputDeviceName(int n);

  void prepareForIdling        (QxrdSynchronizedAcquisitionWPtr      s,
                                QxrdAcquisitionParameterPackWPtr     p);
  void updateSyncWaveforms     (QxrdSynchronizedAcquisitionWPtr      s,
                                QxrdAcquisitionParameterPackWPtr     p);
  void prepareForAcquisition   (QxrdSynchronizedAcquisitionWPtr      s,
                                QxrdAcquisitionParameterPackWPtr     p);
  void prepareForDarkAcquistion(QxrdSynchronizedAcquisitionWPtr      s,
                                QxrdDarkAcquisitionParameterPackWPtr p);

public:
  int32 syncCallback(TaskHandle task, int32 status);
  int32 aiCallback(TaskHandle task, int32 eventType, int32 nSamples);
//  int32 aoCallback(TaskHandle task, int32 status);

private:
  void closeTaskHandles();
  void errorCheck(const char* file, int line, int err);
  void dumpTask(TaskHandle t);

private:
  QxrdAcqCommonWPtr               m_Acquisition;
  QxrdSynchronizedAcquisitionWPtr m_SynchronizedAcquisition;

  TaskHandle          m_SyncTask;
  QVector<TaskHandle> m_SyncDetTasks;
  TaskHandle          m_SyncAOTask;
  TaskHandle          m_SyncAITask;

  QString             m_PrimaryCounterName;
  QString             m_PrimaryTriggerName;
  double              m_ExposureTime;
  int                 m_SyncCounter;
  double              m_SyncLongTime;
  int                 m_SyncNPhases;

  int                 m_OutputNSamples;
  int                 m_InputNSamples;

  QVector<QxrdSynchronizedDetectorChannelWPtr> m_DetectorChannels;
  QVector<QxrdSynchronizedOutputChannelWPtr>   m_OutputChannels;
  QVector<QxrdSynchronizedInputChannelWPtr>    m_InputChannels;

  int                 m_DeviceCount;
  QStringList         m_DeviceNames;

  int                 m_DetectorDeviceCount;
  QStringList         m_DetectorDeviceNames;

  int                 m_OutputDeviceCount;
  QStringList         m_OutputDeviceNames;

  int                 m_InputDeviceCount;
  QStringList         m_InputDeviceNames;
};

Q_DECLARE_METATYPE(QxrdNIDAQSynchronizer*)

#endif // QXRDNIDAQSYNCHRONIZER_H
