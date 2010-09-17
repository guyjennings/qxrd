/******************************************************************
*
*  $Id: qxrdscriptengine.cpp,v 1.3 2010/09/17 16:22:35 jennings Exp $
*
*******************************************************************/

#include "qxrdscriptengine.h"

#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdcenterfinder.h"
#include "qxrddataprocessor.h"
#include "qxrdintegrator.h"
#include "qxrdmutexlocker.h"
#include "qxrdgeneratetestimage.h"
#include "qxrdringfitparameters.h"
#include "qxrdringsetfitparameters.h"
#include "qxrdringsampleddata.h"
#include "qxrdringsetsampleddata.h"

#include <QThread>

static QxrdScriptEnginePtr  g_ScriptEngine;
static QxrdAcquisitionPtr   g_Acquisition;
static QxrdApplicationPtr   g_Application;
static QxrdDataProcessorPtr g_DataProcessor;

QxrdScriptEngine::QxrdScriptEngine(QxrdApplicationPtr app, QxrdWindowPtr win, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Window(win),
    m_Acquisition(acq),
    SOURCE_IDENT("$Id: qxrdscriptengine.cpp,v 1.3 2010/09/17 16:22:35 jennings Exp $")
{
  g_ScriptEngine    = QxrdScriptEnginePtr(this);
  g_Acquisition     = acq;
  g_Application     = app;
  g_DataProcessor   = proc;
}

void QxrdScriptEngine::initialize()
{
  m_ScriptEngine = QScriptEnginePtr(new QScriptEngine(this));

  qScriptRegisterMetaType(m_ScriptEngine, ::QxrdRingFitToScriptValue, ::QxrdRingFitFromScriptValue);
  qScriptRegisterMetaType(m_ScriptEngine, ::QxrdRingSampledDataToScriptValue, ::QxrdRingSampledDataFromScriptValue);
//  qScriptRegisterMetaType(m_ScriptEngine, ::QxrdRingSampledDataPtrToScriptValue, ::QxrdRingSampledDataPtrFromScriptValue);

  qScriptRegisterSequenceMetaType< QList<int> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QList<bool> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QList<double> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QList<QString> >(m_ScriptEngine);
//  qScriptRegisterSequenceMetaType< QList<QxrdRingFitParameters*> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<int> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<bool> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<double> >(m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<QString> >(m_ScriptEngine);
//  qScriptRegisterSequenceMetaType< QVector<QxrdRingFitParameters*> >(m_ScriptEngine);

  m_ScriptEngine -> globalObject().setProperty("acquisition", m_ScriptEngine -> newQObject(m_Acquisition));
  m_ScriptEngine -> globalObject().setProperty("application", m_ScriptEngine -> newQObject(m_Application));
  m_ScriptEngine -> globalObject().setProperty("window", m_ScriptEngine -> newQObject(m_Window));
  m_ScriptEngine -> globalObject().setProperty("processor", m_ScriptEngine -> newQObject(m_Application->dataProcessor()/*,QScriptEngine::QtOwnership,QScriptEngine::AutoCreateDynamicProperties*/));
  m_ScriptEngine -> globalObject().setProperty("centering", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->centerFinder()));
  m_ScriptEngine -> globalObject().setProperty("integrator", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->integrator()));
  m_ScriptEngine -> globalObject().setProperty("initialFit", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->initialRingSetFitParameters()));
  m_ScriptEngine -> globalObject().setProperty("refinedFit", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->refinedRingSetFitParameters()));
  m_ScriptEngine -> globalObject().setProperty("initialData", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->initialRingSetData()));
  m_ScriptEngine -> globalObject().setProperty("refinedData", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->refinedRingSetData()));
//  m_ScriptEngine -> globalObject().setProperty("data", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->data().data()));
//  m_ScriptEngine -> globalObject().setProperty("dark", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->darkImage().data()));
//  m_ScriptEngine -> globalObject().setProperty("mask", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->mask().data()));
  m_ScriptEngine -> globalObject().setProperty("testImage", m_ScriptEngine -> newQObject(m_Application->dataProcessor()->generateTestImage()));
  m_ScriptEngine -> globalObject().setProperty("global", m_ScriptEngine -> newQObject(m_ScriptEngine -> globalObject().toQObject()));
  m_ScriptEngine -> globalObject().setProperty("imageGraph", m_ScriptEngine -> newQObject(m_Application->window()->m_Plot));
  m_ScriptEngine -> globalObject().setProperty("centeringGraph", m_ScriptEngine -> newQObject(m_Application->window()->m_CenterFinderPlot));
  m_ScriptEngine -> globalObject().setProperty("integratorGraph", m_ScriptEngine -> newQObject(m_Application->window()->m_IntegratorPlot));

  m_ScriptEngine -> globalObject().setProperty("acquire", m_ScriptEngine -> newFunction(acquireFunc));
  m_ScriptEngine -> globalObject().setProperty("acquireDark", m_ScriptEngine -> newFunction(acquireDarkFunc));
  m_ScriptEngine -> globalObject().setProperty("status", m_ScriptEngine -> newFunction(statusFunc));
  m_ScriptEngine -> globalObject().setProperty("acquireStatus", m_ScriptEngine -> newFunction(acquireStatusFunc));
  m_ScriptEngine -> globalObject().setProperty("processStatus", m_ScriptEngine -> newFunction(processStatusFunc));
  m_ScriptEngine -> globalObject().setProperty("acquireCancel", m_ScriptEngine -> newFunction(acquireCancelFunc));
  m_ScriptEngine -> globalObject().setProperty("exposureTime", m_ScriptEngine -> newFunction(exposureTimeFunc));
  m_ScriptEngine -> globalObject().setProperty("summedExposures", m_ScriptEngine -> newFunction(summedExposuresFunc));
  m_ScriptEngine -> globalObject().setProperty("skippedExposures", m_ScriptEngine -> newFunction(skippedExposuresFunc));
  m_ScriptEngine -> globalObject().setProperty("darkSummedExposures", m_ScriptEngine -> newFunction(darkSummedExposuresFunc));
  m_ScriptEngine -> globalObject().setProperty("preTriggerFiles", m_ScriptEngine -> newFunction(preTriggerFilesFunc));
  m_ScriptEngine -> globalObject().setProperty("postTriggerFiles", m_ScriptEngine -> newFunction(postTriggerFilesFunc));
  m_ScriptEngine -> globalObject().setProperty("trigger", m_ScriptEngine -> newFunction(triggerFunc));
  m_ScriptEngine -> globalObject().setProperty("filePattern", m_ScriptEngine -> newFunction(filePatternFunc));
  m_ScriptEngine -> globalObject().setProperty("outputDirectory", m_ScriptEngine -> newFunction(outputDirectoryFunc));
  m_ScriptEngine -> globalObject().setProperty("fileIndex", m_ScriptEngine -> newFunction(fileIndexFunc));
  m_ScriptEngine -> globalObject().setProperty("print", m_ScriptEngine -> newFunction(printFunc));
  m_ScriptEngine -> globalObject().setProperty("printMessage", m_ScriptEngine -> newFunction(printFunc));
  m_ScriptEngine -> globalObject().setProperty("data", m_ScriptEngine -> newFunction(dataFunc));
  m_ScriptEngine -> globalObject().setProperty("dark", m_ScriptEngine -> newFunction(darkFunc));
  m_ScriptEngine -> globalObject().setProperty("mask", m_ScriptEngine -> newFunction(maskFunc));
  m_ScriptEngine -> globalObject().setProperty("help", m_ScriptEngine -> newFunction(helpFunc));
}

QScriptEngine* QxrdScriptEngine::scriptEngine() const
{
  return m_ScriptEngine;
}

void QxrdScriptEngine::evaluateAppCommand(QString expr)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//  printf("QxrdScriptingEngine::evaluateAppCommand(%s)\n", qPrintable(expr));

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "evaluate", Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(QString, expr)));
}

void QxrdScriptEngine::evaluateSimpleServerCommand(QString expr)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//  printf("QxrdScriptingEngine::evaluateServerCommand(%s)\n", qPrintable(expr));

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "evaluate", Qt::QueuedConnection, Q_ARG(int, 1), Q_ARG(QString, expr)));
}

void QxrdScriptEngine::evaluateSpecCommand(QString expr)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//  printf("QxrdScriptingEngine::evaluateSpecCommand(%s)\n", qPrintable(expr));

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "evaluate", Qt::QueuedConnection, Q_ARG(int, 2), Q_ARG(QString, expr)));
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
    emit simpleServerResultAvailable(result);
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
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_ScriptEngine -> hasUncaughtException();
}

int  QxrdScriptEngine::uncaughtExceptionLineNumber() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_ScriptEngine -> uncaughtExceptionLineNumber();
}

QString QxrdScriptEngine::uncaughtExceptionString() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

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
    INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "acquire"));
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
    INVOKE_CHECK(QMetaObject::invokeMethod(g_Acquisition, "acquireDark"));
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
  g_Acquisition -> cancel();

  return QScriptValue(engine, 1);
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

QScriptValue QxrdScriptEngine::skippedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_SkippedExposures(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_SkippedExposures());
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

QScriptValue QxrdScriptEngine::dataFunc(QScriptContext *context, QScriptEngine *engine)
{
  return engine -> newQObject(g_DataProcessor -> data().data());
}

QScriptValue QxrdScriptEngine::darkFunc(QScriptContext *context, QScriptEngine *engine)
{
  return engine -> newQObject(g_DataProcessor -> darkImage().data());
}

QScriptValue QxrdScriptEngine::maskFunc(QScriptContext *context, QScriptEngine *engine)
{
  return engine -> newQObject(g_DataProcessor -> mask().data());
}

QScriptValue QxrdScriptEngine::helpFunc(QScriptContext *context, QScriptEngine *engine)
{
  return "Not yet implemented";
}

/******************************************************************
*
*  $Log: qxrdscriptengine.cpp,v $
*  Revision 1.3  2010/09/17 16:22:35  jennings
*  Added graph objects to the scripting engine
*
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.9  2010/08/17 19:20:50  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.8  2010/08/16 21:45:28  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
*
*  Revision 1.1.2.7  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*  Revision 1.1.2.6  2010/08/03 19:24:19  jennings
*  Added data types to hold sampled powder ring coordinates
*
*  Revision 1.1.2.5  2010/08/02 21:51:03  jennings
*  Better integration of powder ring parameter objects to script system
*
*  Revision 1.1.2.4  2010/08/02 21:11:02  jennings
*  Added "rings" object to script engine
*
*  Revision 1.1.2.3  2010/07/28 20:50:15  jennings
*  Implementing routines to access image data from scripts
*  Implement min max and average routines for data
*  Changed 'data', 'dark' and 'mask' script functions to
*  dynamically return correct objects
*
*  Revision 1.1.2.2  2010/07/27 21:53:03  jennings
*  Added double list property type
*  Added 'testImage' script object, interface to QxrdGenerateTestImage
*  Added QxrdDetectorGeometry class
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.16.2.10  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.16.2.9  2010/05/02 08:12:07  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.16.2.8  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.16.2.7  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.16.2.6  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.16.2.5  2010/04/21 20:06:21  jennings
*  Corrected the skippedExposures scripting function
*
*  Revision 1.16.2.4  2010/04/20 21:20:26  jennings
*  Added script support for skippedExposures parameter
*
*  Revision 1.16.2.3  2010/04/19 21:48:27  jennings
*  More rearrangement
*
*  Revision 1.16.2.2  2010/04/18 17:20:48  jennings
*  Further updates
*
*  Revision 1.16.2.1  2010/04/15 19:33:25  jennings
*  Eliminated acquisitionCancel(), replace by cancel()
*
*  Revision 1.16  2009/11/17 20:43:00  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
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
