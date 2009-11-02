/******************************************************************
*
*  $Id: qxrdscriptengine.cpp,v 1.15 2009/11/02 20:19:27 jennings Exp $
*
*******************************************************************/

#include "qxrdscriptengine.h"

#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdcenterfinder.h"
#include "qxrddataprocessor.h"
#include "qxrdintegrator.h"

#include <QThread>

static QxrdScriptEngine    *g_ScriptEngine;
static QxrdAcquisition     *g_Acquisition;
static QxrdApplication     *g_Application;
static QxrdDataProcessor   *g_DataProcessor;

QxrdScriptEngine::QxrdScriptEngine(QxrdApplication *app, QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Window(win),
    m_Acquisition(acq),
    SOURCE_IDENT("$Id: qxrdscriptengine.cpp,v 1.15 2009/11/02 20:19:27 jennings Exp $")
{
  g_ScriptEngine    = this;
  g_Acquisition     = acq;
  g_Application     = app;
  g_DataProcessor   = proc;
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

  m_ScriptEngine -> globalObject().setProperty("acquisition", m_ScriptEngine -> newQObject(m_Acquisition/*,QScriptEngine::QtOwnership,QScriptEngine::AutoCreateDynamicProperties*/));
  m_ScriptEngine -> globalObject().setProperty("application", m_ScriptEngine -> newQObject(m_Application));
  m_ScriptEngine -> globalObject().setProperty("window", m_ScriptEngine -> newQObject(m_Window));
  m_ScriptEngine -> globalObject().setProperty("processor", m_ScriptEngine -> newQObject(m_Application->dataProcessor()/*,QScriptEngine::QtOwnership,QScriptEngine::AutoCreateDynamicProperties*/));
  m_ScriptEngine -> globalObject().setProperty("centering", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->centerFinder()));
  m_ScriptEngine -> globalObject().setProperty("integrator", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->integrator()));
  m_ScriptEngine -> globalObject().setProperty("data", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->data()));
  m_ScriptEngine -> globalObject().setProperty("dark", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->darkImage()));
  m_ScriptEngine -> globalObject().setProperty("mask", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->mask()));

  m_ScriptEngine -> globalObject().setProperty("acquire", m_ScriptEngine -> newFunction(acquireFunc));
  m_ScriptEngine -> globalObject().setProperty("acquireDark", m_ScriptEngine -> newFunction(acquireDarkFunc));
  m_ScriptEngine -> globalObject().setProperty("status", m_ScriptEngine -> newFunction(statusFunc));
  m_ScriptEngine -> globalObject().setProperty("acquireStatus", m_ScriptEngine -> newFunction(acquireStatusFunc));
  m_ScriptEngine -> globalObject().setProperty("processStatus", m_ScriptEngine -> newFunction(processStatusFunc));
  m_ScriptEngine -> globalObject().setProperty("acquireCancel", m_ScriptEngine -> newFunction(acquireCancelFunc));
  m_ScriptEngine -> globalObject().setProperty("exposureTime", m_ScriptEngine -> newFunction(exposureTimeFunc));
  m_ScriptEngine -> globalObject().setProperty("readoutMode", m_ScriptEngine -> newFunction(readoutModeFunc));
  m_ScriptEngine -> globalObject().setProperty("summedExposures", m_ScriptEngine -> newFunction(summedExposuresFunc));
  m_ScriptEngine -> globalObject().setProperty("darkSummedExposures", m_ScriptEngine -> newFunction(darkSummedExposuresFunc));
  m_ScriptEngine -> globalObject().setProperty("preTriggerFiles", m_ScriptEngine -> newFunction(preTriggerFilesFunc));
  m_ScriptEngine -> globalObject().setProperty("postTriggerFiles", m_ScriptEngine -> newFunction(postTriggerFilesFunc));
  m_ScriptEngine -> globalObject().setProperty("trigger", m_ScriptEngine -> newFunction(triggerFunc));
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

  case 5:
    g_Acquisition -> set_PreTriggerFiles(context -> argument(4).toUInt32());

  case 4:
    g_Acquisition -> set_PostTriggerFiles(context -> argument(3).toUInt32());

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
  double time=0;
  int status=0;

  if (context->argumentCount() > 0) {
    time = context->argument(0).toNumber();
  }

  status = g_Acquisition -> acquisitionStatus(time);

  if (status == 1) {
    status = g_DataProcessor -> status(time);
  }

  return QScriptValue(engine, status);
}

QScriptValue QxrdScriptEngine::acquireStatusFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() == 0) {
    return QScriptValue(engine, g_Acquisition -> acquisitionStatus(0));
  } else {
    double time = context->argument(0).toNumber();
    return QScriptValue(engine, g_Acquisition -> acquisitionStatus(time));
  }
}

QScriptValue QxrdScriptEngine::processStatusFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() == 0) {
    return QScriptValue(engine, g_DataProcessor -> status(0));
  } else {
    double time = context->argument(0).toNumber();
    return QScriptValue(engine, g_DataProcessor -> status(time));
  }
}

QScriptValue QxrdScriptEngine::acquireCancelFunc(QScriptContext * /*context*/, QScriptEngine *engine)
{
  return QScriptValue(engine, g_Acquisition -> acquisitionCancel());
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

QScriptValue QxrdScriptEngine::preTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_PreTriggerFiles(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_PreTriggerFiles());
}

QScriptValue QxrdScriptEngine::postTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_PostTriggerFiles(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_PostTriggerFiles());
}

QScriptValue QxrdScriptEngine::triggerFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_Trigger(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_Trigger());
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
    g_DataProcessor -> set_OutputDirectory(context->argument(0).toString());
  }

  return QScriptValue(engine, g_DataProcessor -> get_OutputDirectory());
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
*  Revision 1.15  2009/11/02 20:19:27  jennings
*  Changes to make it work with VC compiler
*
*  Revision 1.14  2009/09/29 18:39:47  jennings
*  Removed references to 'QxrdDataProcessor::processedCount'
*  Fixed up the various 'status' scripting functions so that they work properly
*
*  Revision 1.13  2009/09/25 22:42:48  jennings
*  Masking changes
*
*  Revision 1.12  2009/09/23 19:00:36  jennings
*  Removed dynamic property support - it's not thread-safe and doesn't work
*  under windows
*
*  Revision 1.11  2009/09/22 13:48:17  jennings
*  Some support for dynamic properties during acquisition
*
*  Revision 1.10  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.9  2009/09/18 20:44:49  jennings
*  Add separate status functions for acquisition and processing, as well as an aggregated function
*  combining the status of the two.
*
*  Revision 1.8  2009/09/15 20:18:39  jennings
*  Added acquireCancel scripting command
*
*  Revision 1.7  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.6  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.5  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.4  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.3  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.2  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.1  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*
*******************************************************************/
