#ifndef QXRDSCRIPTENGINE_H
#define QXRDSCRIPTENGINE_H

#include "qcepmacros.h"

#include <QObject>
#include <QScriptEngine>
#include <QMutex>

#include "qxrdforwardtypes.h"
#include "qxrddataprocessor.h"

class QxrdScriptEngine : public QObject
{
  Q_OBJECT;

public:
  QxrdScriptEngine(QxrdApplicationPtr app, QxrdWindowPtr win, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc);
  void initialize();

public slots:
  void evaluateAppCommand(QString cmd);
  void evaluateSimpleServerCommand(QString cmd);
  void evaluateSpecCommand(QString cmd);

signals:
  void appResultAvailable(QScriptValue res);
  void simpleServerResultAvailable(QScriptValue cmd);
  void specResultAvailable(QScriptValue cmd);

  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

public:
  bool hasUncaughtException() const;
  int  uncaughtExceptionLineNumber() const;
  QString uncaughtExceptionString() const;
  void cancelCommand();

  QScriptEngine* scriptEngine() const;

private slots:
  void              evaluate(int src, QString cmd);

private:
  static QScriptValue printFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireDarkFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue statusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireStatusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue processStatusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireCancelFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue exposureTimeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue summedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue skippedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue darkSummedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue preTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue postTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue triggerFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue filePatternFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue outputDirectoryFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue fileIndexFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue dataFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue darkFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue maskFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue overflowFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue helpFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue processFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue typeNameFunc(QScriptContext *context, QScriptEngine *engine);

private:
  mutable QMutex     m_Mutex;
  QScriptEnginePtr   m_ScriptEngine;
  QxrdApplicationPtr m_Application;
  QxrdWindowPtr      m_Window;
  QxrdAcquisitionPtr m_Acquisition;
};

#endif // QXRDSCRIPTENGINE_H
