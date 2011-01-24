#ifndef QXRDNIDAQPLUGIN_H
#define QXRDNIDAQPLUGIN_H

#include <QObject>
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
  void   aoSet(QString chan, double val);
  double aiGet(QString chan);
  void   aoWave(QString chan, int type, double freq, double amplitude, double offset);
  void   aoSet(double val1, double val2);
  double aiGet(int chan);

private:
  void initTaskHandles();
  void closeTaskHandles();
  static void errorCheck(const char* file, int line, int err);

private:
  TaskHandle m_AOTaskHandle;
  TaskHandle m_AITaskHandle;
};

#endif // QXRDNIDAQPLUGIN_H
