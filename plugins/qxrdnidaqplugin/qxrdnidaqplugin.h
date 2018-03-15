#ifndef QXRDNIDAQPLUGIN_H
#define QXRDNIDAQPLUGIN_H

#include "qcepobject.h"
#include <QDateTime>
#include <QMutex>
#include <QStringList>
#include <QVector>

#include "qxrdnidaq.h"
#include "NIDAQmx.h"
#include "qxrdnidaqsyncdetectoroutput-ptr.h"
#include "qxrdnidaqsyncwaveformoutput-ptr.h"
#include "qxrdnidaqsyncanaloginput-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdsynchronizeddetectorchannel-ptr.h"
#include "qxrdsynchronizedoutputchannel-ptr.h"
#include "qxrdsynchronizedinputchannel-ptr.h"

class QxrdNIDAQPlugin :
    public QxrdNIDAQ
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID NIDAQInterface_iid FILE "nidaq.json")
  Q_INTERFACES(QxrdNIDAQ)

private:
  typedef QxrdNIDAQ inherited;

public:
  QxrdNIDAQPlugin();
  virtual ~QxrdNIDAQPlugin();
  virtual void setErrorOutput(QObject *errors);

  void registerMetaTypes();

  void initialize(QcepObjectWPtr parent);
  QString name() const;

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

//  void   addSyncDetectorOutput(int n, double d0, double d1);
//  void   addSyncWaveformOutput(int style, double v0, double v1);
//  void   addSyncAnalogInput();

//  void   clearSyncDetectorOutputs();
//  void   clearSyncWaveformOutputs();
//  void   clearSyncAnalogInputs();

//  void   syncOutput(double period, int nphases);
//  void   syncStop();

//  void   syncTest();
//  void   syncClear();

//  void   syncReadAnalogInputs();

//  QVector<double> syncAnalogInputs();

  void   setAnalogWaveform(QString chan, double rate, double wfm[], int size);
  void   setAnalogOutput(int chan, double val);
  double getAnalogInput(int chan);
  void   triggerAnalogWaveform();

  void   pulseOutput();

  double count(int chan, double time);

  int configCounters(QStringList chans);
  QVector<double> readCounters();

  QStringList deviceNames();
  QString     deviceType(QString device);
  int         deviceIsSimulated(QString device);
  QStringList deviceAIChannels(QString device);
  QStringList deviceAOChannels(QString device);
  QStringList deviceDIPorts(QString device);
  QStringList deviceDILines(QString port);
  QStringList deviceDOPorts(QString device);
  QStringList deviceDOLines(QString port);
  QStringList deviceCIChannels(QString device);
  QStringList deviceCOChannels(QString device);

  double getAnalogInput(QString channelName);
  void   setAnalogOutput(QString channelName, double value);
  void   setAnalogOutput(double value);

  int prepareContinuousInput(double sampleRate,
                                     double acquireDelay,
                                     double exposureTime,
                                     QStringList chans,
                                     QVector<double> minVals,
                                     QVector<double> maxVals);
  int countContinuousInput();
  int readContinuousInput();
  QVector<double> readContinuousInputChannel(int ch);
  void finishContinuousInput();

  int detectorDeviceCount();
  QString detectorDeviceName(int n);

  int outputDeviceCount();
  QString outputDeviceName(int n);

  int inputDeviceCount();
  QString inputDeviceName(int n);

  void updateSyncWaveforms     (QxrdSynchronizedAcquisitionWPtr      s,
                                QxrdAcquisitionParameterPackWPtr     p);
  void prepareForAcquisition   (QxrdSynchronizedAcquisitionWPtr      s,
                                QxrdAcquisitionParameterPackWPtr     p);
  void prepareForDarkAcquistion(QxrdSynchronizedAcquisitionWPtr      s,
                                QxrdDarkAcquisitionParameterPackWPtr p);

public:
  int32 syncCallback(TaskHandle task, int32 status);
//  int32 aiCallback(TaskHandle task, int32 status);
//  int32 aoCallback(TaskHandle task, int32 status);

private:
  void initTaskHandles();
  void closeTaskHandles();
  void errorCheck(const char* file, int line, int err);

private:
  QObject            *m_ErrorOutput;
  QMutex              m_Mutex;
  TaskHandle          m_AOTaskHandle;
  TaskHandle          m_AITaskHandle;
  TaskHandle          m_TrigAOTask;
  TaskHandle          m_PulseTask;
  TaskHandle          m_CountersTask;
  int                 m_NCounters;
  QVector<double>     m_Counts;

  double              m_SampleRate;
  double              m_ExposureTime;
  double              m_AcquireDelay;

  int                 m_NContinuousInputs;
  int                 m_NContinuousSamples;

  int                 m_NAIChannels;
  int                 m_NCIChannels;
  TaskHandle          m_ContinuousAITask;
  QVector<TaskHandle> m_ContinuousCITasks;
  QVector<int>        m_ContinuousFlags;
  QVector<int>        m_ContinuousChans;

  QVector< QVector<double> > m_ContinuousInputData;

  TaskHandle          m_SyncTask;
  QVector<TaskHandle> m_SyncDetTasks;
  QVector<TaskHandle> m_SyncAOTasks;
  QVector<TaskHandle> m_SyncAITasks;

  int                 m_SyncCounter;
  double              m_SyncPeriod;
  double              m_SyncLongTime;
  int                 m_SyncNPhases;
  int                 m_SyncBufferSize;

  QVector<double>     m_SyncAnalogInputs;

  QVector<QxrdNIDAQSyncDetectorOutputPtr>  m_SyncDetectors;
  QVector<QxrdNIDAQSyncWaveformOutputPtr>  m_SyncWaveforms;
  QVector<QxrdNIDAQSyncAnalogInputPtr>     m_SyncInputs;

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

Q_DECLARE_METATYPE(QxrdNIDAQPlugin*)

#endif // QXRDNIDAQPLUGIN_H
