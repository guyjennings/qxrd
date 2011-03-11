#ifndef QXRDSCRIPTENGINE_H
#define QXRDSCRIPTENGINE_H

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QDateTime>
#include <QMutex>

class QxrdApplication;
class QxrdWindow;
class QxrdAcquisition;
class QxrdDataProcessor;

class QxrdScriptEngine : public QObject
{
  Q_OBJECT;

public:
  QxrdScriptEngine(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc);
  void initialize();

public slots:
  void evaluateAppCommand(QString cmd);
  void evaluateSimpleServerCommand(QString cmd);
  void evaluateSpecCommand(QString cmd);

signals:
  void appResultAvailable(QScriptValue res);
  void simpleServerResultAvailable(QScriptValue cmd);
  void specResultAvailable(QScriptValue cmd);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

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
  static QScriptValue filesInGroupFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue groupsInSequenceFunc(QScriptContext *context, QScriptEngine *engine);
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
  QScriptEngine     *m_ScriptEngine;
  QxrdApplication   *m_Application;
  QxrdWindow        *m_Window;
  QxrdAcquisition   *m_Acquisition;
};

#endif // QXRDSCRIPTENGINE_H
