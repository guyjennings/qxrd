/******************************************************************
*
*  $Id: qxrdscriptengine.h,v 1.2 2009/07/17 14:00:59 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSCRIPTENGINE_H
#define QXRDSCRIPTENGINE_H

#include "qcepmacros.h"

#include <QObject>
#include <QScriptEngine>
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
  void evaluateServerCommand(QString cmd);
  void evaluateSpecCommand(QString cmd);

signals:
  void appResultAvailable(QScriptValue res);
  void serverResultAvailable(QScriptValue cmd);
  void specResultAvailable(QScriptValue cmd);
  void printMessage(QString msg);

public:
  bool hasUncaughtException() const;
  int  uncaughtExceptionLineNumber() const;
  QString uncaughtExceptionString() const;
  void cancelCommand();

private slots:
  void              evaluate(int src, QString cmd);

private:
  static QScriptValue printFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireDarkFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue statusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue readoutModeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue exposureTimeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue summedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue darkSummedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue filesInSequenceFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue filePatternFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue outputDirectoryFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue fileIndexFunc(QScriptContext *context, QScriptEngine *engine);

private:
  mutable QMutex         m_Mutex;
  QScriptEngine         *m_ScriptEngine;
  QxrdApplication       *m_Application;
  QxrdWindow            *m_Window;
  QxrdAcquisition       *m_Acquisition;

  HEADER_IDENT("$Id: qxrdscriptengine.h,v 1.2 2009/07/17 14:00:59 jennings Exp $");
};

#endif // QXRDSCRIPTENGINE_H

/******************************************************************
*
*  $Log: qxrdscriptengine.h,v $
*  Revision 1.2  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.1  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*
*******************************************************************/
