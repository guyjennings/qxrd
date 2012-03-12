#ifndef QXRDNIDAQPLUGIN_H
#define QXRDNIDAQPLUGIN_H

#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QStringList>
#include <QVector>

#include "qxrdnidaqplugininterface.h"
#include "NIDAQmx.h"

class QxrdNIDAQPlugin : public QObject, public QxrdNIDAQPluginInterface
{
  Q_OBJECT
  Q_INTERFACES(QxrdNIDAQPluginInterface)

public:
  QxrdNIDAQPlugin();
  virtual ~QxrdNIDAQPlugin();
  virtual void setErrorOutput(QObject *errors);

  QString name() const;

public slots:
  void   setAnalogWaveform(int chan, double rate, double wfm[], int size);
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

  void   continuousAnalogInput(QStringList chans, double sampleRate, int bufferSize);
  void   haltContinuousInput();
  QVector<double> readContinuousInput();

private:
  void initTaskHandles();
  void closeTaskHandles();
  void errorCheck(const char* file, int line, int err);

private:
  QObject   *m_ErrorOutput;
  QMutex     m_Mutex;
  TaskHandle m_AOTaskHandle;
  TaskHandle m_AITaskHandle;
  TaskHandle m_TrigAOTask;
  TaskHandle m_PulseTask;
  TaskHandle m_CountersTask;
  int        m_NCounters;
  QVector<double> m_Counts;

  TaskHandle m_ContinuousInputTask;
  int        m_NContinuousInputs;
  int        m_NContinuousSamples;
};

#endif // QXRDNIDAQPLUGIN_H
