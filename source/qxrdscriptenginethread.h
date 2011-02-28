#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include <QThread>
#include <QAtomicPointer>
#include <QDateTime>

class QxrdApplication;
class QxrdWindow;
class QxrdAcquisition;
class QxrdDataProcessor;
class QxrdScriptEngine;

class QxrdScriptEngineThread : public QThread
{
  Q_OBJECT;

public:
  QxrdScriptEngineThread(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEngine *scriptEngine() const;

protected:
  void run();

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

private:
  QAtomicPointer<QxrdScriptEngine>  m_ScriptEngine;
  QxrdApplication   *m_Application;
  QxrdWindow        *m_Window;
  QxrdAcquisition   *m_Acquisition;
  QxrdDataProcessor *m_DataProcessor;
};

#endif // QXRDSCRIPTENGINETHREAD_H
