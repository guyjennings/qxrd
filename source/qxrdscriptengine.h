#ifndef QXRDSCRIPTENGINE_H
#define QXRDSCRIPTENGINE_H

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QDateTime>
#include <QMutex>

class QxrdApplication;

#include "qxrddocument.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"

class QxrdScriptEngine : public QObject
{
  Q_OBJECT

public:
  QxrdScriptEngine(QxrdApplication *app/*, QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc*/);
  void initialize();

  void documentOpened(QxrdDocument *doc);
  void documentClosed(QxrdDocument *doc);

  void windowOpened(QxrdWindow *win);
  void windowClosed(QxrdWindow *win);

public slots:
  void evaluateAppCommand(QString cmd);
  void evaluateSimpleServerCommand(QString cmd);
  void evaluateSpecCommand(QString cmd);
  void loadScript(QString path);

signals:
  void appResultAvailable(QScriptValue res);
  void simpleServerResultAvailable(QScriptValue cmd);
  void specResultAvailable(QScriptValue cmd);

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
  static QScriptValue phasesInGroupFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue preTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue postTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine);
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
  static QScriptValue matchFilesFunc(QScriptContext *context, QScriptEngine *engine);

private:
  mutable QMutex     m_Mutex;
  QScriptEngine     *m_ScriptEngine;
  QxrdApplication   *m_Application;
//  QxrdWindow        *m_Window;
//  QxrdAcquisition   *m_Acquisition;

  QList<QxrdDocument*> m_Documents;
  QList<QxrdWindow*>   m_Windows;
};

#endif // QXRDSCRIPTENGINE_H
