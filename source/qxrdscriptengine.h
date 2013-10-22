#ifndef QXRDSCRIPTENGINE_H
#define QXRDSCRIPTENGINE_H

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QDateTime>
#include <QMutex>
#include <QRegExp>

#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdwindow-ptr.h"

class QxrdScriptEngine : public QScriptEngine
{
  Q_OBJECT

public:
  QxrdScriptEngine(QxrdApplicationWPtr app, QxrdExperimentWPtr exp);
  virtual ~QxrdScriptEngine();
  void initialize();

  void lock();
  void unlock();

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

  QxrdApplicationWPtr application() const;
  QxrdExperimentWPtr experiment() const;
  QxrdAcquisitionWPtr acquisition() const;
  QxrdDataProcessorWPtr dataProcessor() const;
  QxrdWindowWPtr window() const;

  void setWindow(QxrdWindowWPtr win);

  QString documentationLink(QString base, QString subItem);
  QByteArray helpText(QString item);

public slots:
  QString documentationText(QString item);
  QString documentationText(QRegExp pattern);

  static QString convertToString(QScriptValue result);

  void dumpLocks();

//  QScriptValue newQxrdObject(QObject *object, QString objectName);
//  QScriptValue newQxrdFunction(FunctionSignature fun, QString objectName);

private slots:
  void              evaluateScript(int src, QString cmd);

private:
  static QScriptValue printFunc(QScriptContext *context, QScriptEngine *engine, void *u);
  static QScriptValue acquireFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireDarkFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue statusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireStatusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue processStatusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireCancelFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue triggerFunc(QScriptContext *context, QScriptEngine *engine);
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
  static QScriptValue liveDataFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue helpFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue processFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setFileNormalizationFunc(QScriptContext *context, QScriptEngine *engine);
//  static QScriptValue typeNameFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue matchFilesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue extraChannelFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue mapUserFunctionFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue timeStampFunc(QScriptContext *context, QScriptEngine *engine);

  QString tableHeader();
  QString tableFooter();

private:
  mutable QMutex         m_Mutex;
  QxrdApplicationWPtr    m_Application;
  QxrdExperimentWPtr     m_Experiment;
  QxrdAcquisitionWPtr    m_Acquisition;
  QxrdDataProcessorWPtr  m_DataProcessor;
  QxrdWindowWPtr         m_Window;
};

#endif // QXRDSCRIPTENGINE_H
