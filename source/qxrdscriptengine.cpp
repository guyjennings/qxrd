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
//static QxrdApplicationPtr   g_Application;
static QxrdDataProcessorPtr g_DataProcessor;

QxrdScriptEngine::QxrdScriptEngine(QxrdApplicationPtr app, QxrdWindowPtr win, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_ScriptEngine(NULL),
    m_Application(app),
    m_Window(win),
    m_Acquisition(acq)
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
  m_ScriptEngine -> globalObject().setProperty("overflow", m_ScriptEngine -> newFunction(overflowFunc));
  m_ScriptEngine -> globalObject().setProperty("help", m_ScriptEngine -> newFunction(helpFunc));
  m_ScriptEngine -> globalObject().setProperty("process", m_ScriptEngine -> newFunction(processFunc));
  m_ScriptEngine -> globalObject().setProperty("typeName", m_ScriptEngine -> newFunction(typeNameFunc));

  if (g_Application->nidaqPlugin()) {
    m_ScriptEngine->globalObject().setProperty("nidaq", m_ScriptEngine->newQObject(g_Application->nidaqPlugin()));
  }
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

  emit g_ScriptEngine -> printMessage(QDateTime::currentDateTime(), msg);

  return QScriptValue(engine, 1);
}

QScriptValue QxrdScriptEngine::acquireFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_Acquisition) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();

  if (nArgs < 5) {
    g_Acquisition -> set_PreTriggerFiles(0);
  }

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
  } else {
    g_Acquisition -> set_Trigger(1);
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

QScriptValue QxrdScriptEngine::overflowFunc(QScriptContext *context, QScriptEngine *engine)
{
  return engine -> newQObject(g_DataProcessor -> overflow().data());
}

QScriptValue QxrdScriptEngine::helpFunc(QScriptContext *context, QScriptEngine *engine)
{
  return "Not yet implemented";
}

QScriptValue QxrdScriptEngine::typeNameFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    return QScriptValue(engine, QMetaType::typeName(context->argument(0).toUInt32()));
  } else {
    return QScriptValue(engine,"?");
  }
}

QScriptValue QxrdScriptEngine::processFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() >= 1) {
    QString file = context->argument(0).toString();
    QList<double> normVals;

    for (int i=1; i<context->argumentCount(); i++) {
      normVals.append(context->argument(i).toNumber());
    }

    g_DataProcessor -> processNormalizedFile(file, normVals);
  }

  return QScriptValue(engine, 1);
}
