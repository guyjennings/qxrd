/******************************************************************
*
*  $Id: qxrdscriptengine.h,v 1.6 2009/09/20 21:18:53 jennings Exp $
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
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

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
  static QScriptValue acquireStatusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue processStatusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireCancelFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue readoutModeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue exposureTimeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue summedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue darkSummedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue preTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue postTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue triggerFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue filePatternFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue outputDirectoryFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue fileIndexFunc(QScriptContext *context, QScriptEngine *engine);

private:
  mutable QMutex         m_Mutex;
  QScriptEngine         *m_ScriptEngine;
  QxrdApplication       *m_Application;
  QxrdWindow            *m_Window;
  QxrdAcquisition       *m_Acquisition;

  HEADER_IDENT("$Id: qxrdscriptengine.h,v 1.6 2009/09/20 21:18:53 jennings Exp $");
};

#endif // QXRDSCRIPTENGINE_H

/******************************************************************
*
*  $Log: qxrdscriptengine.h,v $
*  Revision 1.6  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.5  2009/09/18 20:44:49  jennings
*  Add separate status functions for acquisition and processing, as well as an aggregated function
*  combining the status of the two.
*
*  Revision 1.4  2009/09/15 20:18:39  jennings
*  Added acquireCancel scripting command
*
*  Revision 1.3  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.2  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.1  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*
*******************************************************************/
