#ifndef QXRDNIDAQPLUGIN_H
#define QXRDNIDAQPLUGIN_H

#include <QObject>
#include <QDateTime>
#include <QMutex>

#include "qxrdnidaqplugininterface.h"
#include "NIDAQmx.h"

class QxrdNIDAQPlugin : public QObject, public QxrdNIDAQPluginInterface
{
  Q_OBJECT;
  Q_INTERFACES(QxrdNIDAQPluginInterface);

public:
  QxrdNIDAQPlugin();
  virtual ~QxrdNIDAQPlugin();

  QString name() const;

public slots:
  void   setAnalogWaveform(int chan, double rate, double wfm[], int size);
  void   setAnalogOutput(int chan, double val);
  double getAnalogInput(int chan);
  void   triggerAnalogWaveform();

private:
  void initTaskHandles();
  void closeTaskHandles();
  void errorCheck(const char* file, int line, int err);

private:
  QMutex     m_Mutex;
  TaskHandle m_AOTaskHandle;
  TaskHandle m_AITaskHandle;
  TaskHandle m_TrigAOTask;
};

#endif // QXRDNIDAQPLUGIN_H
