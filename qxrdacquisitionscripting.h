/******************************************************************
*
*  $Id: qxrdacquisitionscripting.h,v 1.4 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONSCRIPTING_H
#define QXRDACQUISITIONSCRIPTING_H

#include "qcepmacros.h"

#include "qxrdacquisitionparameters.h"

#include <QScriptEngine>
class QxrdAcquisitionThread;

class QxrdAcquisitionScripting : public QxrdAcquisitionParameters
{
  Q_OBJECT;
public:
  QxrdAcquisitionScripting(QxrdAcquisitionThread *thread);

  QxrdAcquisitionThread *acquisitionThread() const;

public slots:
  double sleep(double time);
  void propertyList();
  QVariant evaluate(QString cmd);
  void message(QString cmd);\

protected:
  virtual void acquire() = 0;
  virtual void acquireDark() = 0;
  virtual int acquisitionStatus(double time) = 0;

private:
  mutable QMutex         m_Mutex;
  QxrdAcquisitionThread *m_AcquisitionThread;
  QScriptEngine          m_ScriptEngine;

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

  static QxrdAcquisitionScripting *g_Acquisition;

//  static QScriptValue setCameraModeFunc(QScriptContext *context, QScriptEngine *engine);
//  static QScriptValue setFrameSyncModeFunc(QScriptContext *context, QScriptEngine *engine);
//  static QScriptValue setTimerSyncFunc(QScriptContext *context, QScriptEngine *engine);
//  static QScriptValue setCameraGainFunc(QScriptContext *context, QScriptEngine *engine);
  HEADER_IDENT("$Id: qxrdacquisitionscripting.h,v 1.4 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDACQUISITIONSCRIPTING_H

/******************************************************************
*
*  $Log: qxrdacquisitionscripting.h,v $
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

