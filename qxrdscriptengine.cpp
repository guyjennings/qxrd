/******************************************************************
*
*  $Id: qxrdscriptengine.cpp,v 1.2 2009/06/28 11:21:58 jennings Exp $
*
*******************************************************************/

#include "qxrdscriptengine.h"

#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include <QThread>

static QxrdScriptEngine    *g_ScriptEngine;
static QxrdAcquisition     *g_Acquisition;
static QxrdApplication     *g_Application;

QxrdScriptEngine::QxrdScriptEngine(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Window(win),
    m_Acquisition(acq),
    SOURCE_IDENT("$Id: qxrdscriptengine.cpp,v 1.2 2009/06/28 11:21:58 jennings Exp $")
{
  g_ScriptEngine    = this;
  g_Acquisition     = acq;
  g_Application     = app;
}

void QxrdScriptEngine::initialize()
{
  m_ScriptEngine = new QScriptEngine(this);

  qScriptRegisterSequenceMetaType< QList<int> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QList<bool> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QList<double> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QList<QString> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<int> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<bool> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<double> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<QString> >(m_ScriptEngine);

  m_ScriptEngine -> globalObject().setProperty("acquisition", m_ScriptEngine -> newQObject(m_Acquisition));
  m_ScriptEngine -> globalObject().setProperty("application", m_ScriptEngine -> newQObject(m_Application));
  m_ScriptEngine -> globalObject().setProperty("window", m_ScriptEngine -> newQObject(m_Window));
  m_ScriptEngine -> globalObject().setProperty("acquire", m_ScriptEngine -> newFunction(acquireFunc));
  m_ScriptEngine -> globalObject().setProperty("acquireDark", m_ScriptEngine -> newFunction(acquireDarkFunc));
  m_ScriptEngine -> globalObject().setProperty("status", m_ScriptEngine -> newFunction(statusFunc));
  m_ScriptEngine -> globalObject().setProperty("exposureTime", m_ScriptEngine -> newFunction(exposureTimeFunc));
  m_ScriptEngine -> globalObject().setProperty("readoutMode", m_ScriptEngine -> newFunction(readoutModeFunc));
  m_ScriptEngine -> globalObject().setProperty("summedExposures", m_ScriptEngine -> newFunction(summedExposuresFunc));
  m_ScriptEngine -> globalObject().setProperty("darkSummedExposures", m_ScriptEngine -> newFunction(darkSummedExposuresFunc));
  m_ScriptEngine -> globalObject().setProperty("filesInSequence", m_ScriptEngine -> newFunction(filesInSequenceFunc));
  m_ScriptEngine -> globalObject().setProperty("filePattern", m_ScriptEngine -> newFunction(filePatternFunc));
  m_ScriptEngine -> globalObject().setProperty("outputDirectory", m_ScriptEngine -> newFunction(outputDirectoryFunc));
  m_ScriptEngine -> globalObject().setProperty("fileIndex", m_ScriptEngine -> newFunction(fileIndexFunc));
  m_ScriptEngine -> globalObject().setProperty("print", m_ScriptEngine -> newFunction(printFunc));

}

void QxrdScriptEngine::evaluateAppCommand(QString expr)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdScriptingEngine::evaluateAppCommand(%s)\n", qPrintable(expr));

  QMetaObject::invokeMethod(this, "evaluate", Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(QString, expr));
}

void QxrdScriptEngine::evaluateServerCommand(QString expr)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdScriptingEngine::evaluateServerCommand(%s)\n", qPrintable(expr));

  QMetaObject::invokeMethod(this, "evaluate", Qt::QueuedConnection, Q_ARG(int, 1), Q_ARG(QString, expr));
}

void QxrdScriptEngine::evaluateSpecCommand(QString expr)
{
  QMutexLocker lock(&m_Mutex);

//  printf("QxrdScriptingEngine::evaluateSpecCommand(%s)\n", qPrintable(expr));

  QMetaObject::invokeMethod(this, "evaluate", Qt::QueuedConnection, Q_ARG(int, 2), Q_ARG(QString, expr));
}

void QxrdScriptEngine::evaluate(int src, QString expr)
{
  THREAD_CHECK;

//  printf("QxrdScriptingEngine::evaluate(%s)\n", qPrintable(expr));

  QScriptValue result = m_ScriptEngine -> evaluate(expr);

  switch (src) {
  case 0:
    emit appResultAvailable(result);
    break;
  case 1:
    emit serverResultAvailable(result);
    break;
  case 2:
    emit specResultAvailable(result);
    break;
  }
}

void QxrdScriptEngine::cancelCommand()
{
  m_ScriptEngine -> abortEvaluation();
}

bool QxrdScriptEngine::hasUncaughtException() const
{
  QMutexLocker lock(&m_Mutex);

  return m_ScriptEngine -> hasUncaughtException();
}

int  QxrdScriptEngine::uncaughtExceptionLineNumber() const
{
  QMutexLocker lock(&m_Mutex);

  return m_ScriptEngine -> uncaughtExceptionLineNumber();
}

QString QxrdScriptEngine::uncaughtExceptionString() const
{
  QMutexLocker lock(&m_Mutex);

  return m_ScriptEngine -> uncaughtException().toString();
}

QScriptValue QxrdScriptEngine::printFunc(QScriptContext *context, QScriptEngine *engine)
{
  int nArgs = context->argumentCount();
  QString msg;

  for (int i=0; i<nArgs; i++) {
    if (i != 0) {
      msg += " ";
    }

    msg += context -> argument(i).toString();
  }

  emit g_ScriptEngine -> printMessage(msg);

  return QScriptValue(engine, 1);
}

QScriptValue QxrdScriptEngine::acquireFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_Acquisition) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();

  switch (nArgs) {
  default:
  case 4:
    g_Acquisition -> set_FilesInSequence(context -> argument(3).toUInt32());

  case 3:
    g_Acquisition -> set_SummedExposures(context -> argument(2).toUInt32());

  case 2:
    g_Acquisition -> set_ExposureTime(context -> argument(1).toNumber());

  case 1:
    g_Acquisition -> set_FilePattern(context -> argument(0).toString());

  case 0:
    QMetaObject::invokeMethod(g_Acquisition, "acquire");
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdScriptEngine::acquireDarkFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_Acquisition) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();

  switch (nArgs) {
  default:
  case 3:
    g_Acquisition -> set_DarkSummedExposures(context -> argument(2).toUInt32());

  case 2:
    g_Acquisition -> set_ExposureTime(context -> argument(1).toNumber());

  case 1:
    g_Acquisition -> set_FilePattern(context -> argument(0).toString());

  case 0:
    QMetaObject::invokeMethod(g_Acquisition, "acquireDark");
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdScriptEngine::statusFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() == 0) {
    return QScriptValue(engine, g_Acquisition -> acquisitionStatus(0));
  } else {
    double time = context->argument(0).toNumber();
    return QScriptValue(engine, g_Acquisition -> acquisitionStatus(time));
  }
}

QScriptValue QxrdScriptEngine::readoutModeFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_ReadoutMode(context->argument(0).toInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_ReadoutMode());
}

QScriptValue QxrdScriptEngine::exposureTimeFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_ExposureTime(context->argument(0).toNumber());
  }

  return QScriptValue(engine, g_Acquisition -> get_ExposureTime());
}

QScriptValue QxrdScriptEngine::summedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_SummedExposures(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_SummedExposures());
}

QScriptValue QxrdScriptEngine::darkSummedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_DarkSummedExposures(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_DarkSummedExposures());
}

QScriptValue QxrdScriptEngine::filesInSequenceFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_FilesInSequence(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_FilesInSequence());
}

QScriptValue QxrdScriptEngine::filePatternFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_FilePattern(context->argument(0).toString());
  }

  return QScriptValue(engine, g_Acquisition -> get_FilePattern());
}

QScriptValue QxrdScriptEngine::outputDirectoryFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_OutputDirectory(context->argument(0).toString());
  }

  return QScriptValue(engine, g_Acquisition -> get_OutputDirectory());
}

QScriptValue QxrdScriptEngine::fileIndexFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_FileIndex(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_FileIndex());
}

/******************************************************************
*
*  $Log: qxrdscriptengine.cpp,v $
*  Revision 1.2  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.1  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*
*******************************************************************/
