#ifndef QXRDSCRIPTENGINETHREAD_H
#define QXRDSCRIPTENGINETHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include <QSharedPointer>

#include "qxrdforwardtypes.h"
#include "qxrddataprocessor.h"

class QxrdScriptEngineThread : public QThread
{
  Q_OBJECT;

public:
  QxrdScriptEngineThread(QxrdApplicationPtr app, QxrdWindowPtr win, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc);
  ~QxrdScriptEngineThread();

  void shutdown();
  QxrdScriptEnginePtr scriptEngine() const;

protected:
  void run();

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

private:
  QAtomicPointer<QxrdScriptEngine>  m_ScriptEngine;
  QxrdApplicationPtr   m_Application;
  QxrdWindowPtr        m_Window;
  QxrdAcquisitionPtr   m_Acquisition;
  QxrdDataProcessorPtr m_DataProcessor;
};

#endif // QXRDSCRIPTENGINETHREAD_H
