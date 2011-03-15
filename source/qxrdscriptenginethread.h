#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qxrdthread.h"
#include <QAtomicPointer>
#include <QDateTime>

class QxrdApplication;
class QxrdWindow;
class QxrdAcquisition;
class QxrdDataProcessor;
class QxrdScriptEngine;

class QxrdScriptEngineThread : public QxrdThread
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
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

private:
  QAtomicPointer<QxrdScriptEngine>  m_ScriptEngine;
  QxrdApplication   *m_Application;
  QxrdWindow        *m_Window;
  QxrdAcquisition   *m_Acquisition;
  QxrdDataProcessor *m_DataProcessor;
};

#endif // QXRDSCRIPTENGINETHREAD_H
