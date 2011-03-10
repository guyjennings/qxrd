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
//  void   setAnalogChannel();
  void   setAnalogWaveform(int chan, double rate, double wfm[], int size);
  void   triggerAnalogWaveform();

  void   aoSet(QString chan, double val);
  double aiGet(QString chan);
  void   aoWave(QString chan, int type, double freq, double amplitude, double offset);
  void   aoSet(double val1, double val2);
  double aiGet(int chan);

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

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
