#ifndef QXRDNIDAQPLUGIN_H
#define QXRDNIDAQPLUGIN_H

#include <QObject>
#include <QDateTime>
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
  void   setAnalogChannel(int chan);
  void   setAnalogWaveform(double rate, double wfm[], int size);
  void   triggerAnalogWaveform();

  void   aoSet(QString chan, double val);
  double aiGet(QString chan);
  void   aoWave(QString chan, int type, double freq, double amplitude, double offset);
  void   aoSet(double val1, double val2);
  double aiGet(int chan);

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

private:
  void initTaskHandles();
  void closeTaskHandles();
  void errorCheck(const char* file, int line, int err);

private:
  TaskHandle m_AOTaskHandle;
  TaskHandle m_AITaskHandle;
  TaskHandle m_TrigAOTask;
};

#endif // QXRDNIDAQPLUGIN_H
