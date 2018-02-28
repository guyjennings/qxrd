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

  void initialize(QcepObjectWPtr parent);
  QString name() const;

public slots:
  void   addSyncDetectorOutput(int n, double d0, double d1);
  void   addSyncWaveformOutput();
  void   addSyncAnalogInput();
  void   syncOutput(double period, int nphases);

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

  virtual int prepareContinuousInput(double sampleRate,
                                     double acquireDelay,
                                     double exposureTime,
                                     QStringList chans,
                                     QVector<double> minVals,
                                     QVector<double> maxVals);
  virtual int countContinuousInput();
  virtual int readContinuousInput();
  virtual QVector<double> readContinuousInputChannel(int ch);
  virtual void finishContinuousInput();

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
  TaskHandle          m_SyncAOTask;
  TaskHandle          m_SyncAITask;

  QVector<QxrdNIDAQSyncDetectorOutputPtr>  m_SyncDetectors;
  QVector<QxrdNIDAQSyncWaveformOutputPtr>  m_SyncWaveforms;
  QVector<QxrdNIDAQSyncAnalogInputPtr>     m_SyncInputs;
};

#endif // QXRDNIDAQPLUGIN_H
