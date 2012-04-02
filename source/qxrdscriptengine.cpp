#include "qxrdscriptengine.h"

#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdexperiment.h"
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
#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdacquisitiontrigger.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdallocator.h"
#include "qxrdserver.h"
#include "qxrdsimpleserver.h"
#include "qxrddocumentationdictionary.h"
#include "qxrddocumentationforobject.h"
#include "qxrddocumentationforfunction.h"

#include <QThread>
#include <QDir>
#include <QScriptValueIterator>
#include <QMetaObject>
#include <QMetaProperty>

QxrdScriptEngine::QxrdScriptEngine(QxrdApplication* app, QxrdExperimentWPtr exp)
  : QScriptEngine(),
    m_Mutex(QMutex::Recursive),
    m_Application(app),
    m_Experiment(exp),
    m_Acquisition(),
    m_DataProcessor(),
    m_Window()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngine::QxrdScriptEngine(%p)\n", this);
  }
}

QxrdScriptEngine::~QxrdScriptEngine()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptEngine::~QxrdScriptEngine(%p)\n", this);
  }
}

QxrdExperimentWPtr QxrdScriptEngine::experiment() const
{
  return m_Experiment;
}

QxrdAcquisitionWPtr QxrdScriptEngine::acquisition() const
{
  return m_Acquisition;
}

QxrdWindow *QxrdScriptEngine::window() const
{
  return m_Window;
}

QxrdDataProcessorWPtr QxrdScriptEngine::dataProcessor() const
{
  return m_DataProcessor;
}

void QxrdScriptEngine::setWindow(QxrdWindow *win)
{
  m_Window = win;

  if (m_Window) {
    globalObject().setProperty("window",          newQObject(m_Window));
    globalObject().setProperty("imageGraph",      newQObject(m_Window->m_ImagePlot));
    globalObject().setProperty("centeringGraph",  newQObject(m_Window->m_CenterFinderPlot));
    globalObject().setProperty("integratorGraph", newQObject(m_Window->m_IntegratorPlot));
  }
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

void QxrdScriptEngine::loadScript(QString path)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "loadScript", Qt::QueuedConnection, Q_ARG(QString, path)));
  } else {
    QFile scriptFile(path);

    if (scriptFile.open(QFile::ReadOnly)) {
      QTextStream scriptStream(&scriptFile);
      QString script = scriptStream.readAll();

      QScriptEngine::evaluate(script, path);
    }
  }
}

void QxrdScriptEngine::evaluate(int src, QString expr)
{
  THREAD_CHECK;

  //  printf("QxrdScriptingEngine::evaluate(%s)\n", qPrintable(expr));

  QScriptValue result = QScriptEngine::evaluate(expr);

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
  abortEvaluation();
}

bool QxrdScriptEngine::hasUncaughtException() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return QScriptEngine::hasUncaughtException();
}

int  QxrdScriptEngine::uncaughtExceptionLineNumber() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return QScriptEngine::uncaughtExceptionLineNumber();
}

QString QxrdScriptEngine::uncaughtExceptionString() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return uncaughtException().toString();
}

QScriptValue QxrdScriptEngine::printFunc(QScriptContext *context, QScriptEngine *engine, void *u)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    int nArgs = context->argumentCount();
    QString msg;

    for (int i=0; i<nArgs; i++) {
      if (i != 0) {
        msg += " ";
      }

      msg += context -> argument(i).toString();
    }

    QxrdExperimentPtr expt(eng->experiment());

    if (expt) {
      expt->printMessage(msg);
    }
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdScriptEngine::acquireFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (!acq) return QScriptValue(engine, -1);

    int nArgs = context->argumentCount();

    if (nArgs < 6) {
      acq -> set_PhasesInGroup(1);
    }

    if (nArgs < 5) {
      acq -> set_PreTriggerFiles(0);
    }

    switch (nArgs) {
    default:

    case 6:
      acq -> set_PhasesInGroup(context -> argument(5).toUInt32());

    case 5:
      acq -> set_PreTriggerFiles(context -> argument(4).toUInt32());

    case 4:
      acq -> set_PostTriggerFiles(context -> argument(3).toUInt32());

    case 3:
      acq -> set_SummedExposures(context -> argument(2).toUInt32());

    case 2:
      acq -> set_ExposureTime(context -> argument(1).toNumber());

    case 1:
      acq -> set_FilePattern(context -> argument(0).toString());

    case 0:
      acq->acquire();
    }
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdScriptEngine::acquireDarkFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (!acq) return QScriptValue(engine, -1);

    int nArgs = context->argumentCount();

    switch (nArgs) {
    default:
    case 3:
      acq -> set_DarkSummedExposures(context -> argument(2).toUInt32());

    case 2:
      acq -> set_ExposureTime(context -> argument(1).toNumber());

    case 1:
      acq -> set_FilePattern(context -> argument(0).toString());

    case 0:
      acq -> acquireDark();
    }
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdScriptEngine::statusFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    if (!acq || !proc) return QScriptValue(engine, -1);

    double time=0;
    int status=0;

    if (context->argumentCount() > 0) {
      time = context->argument(0).toNumber();
    }

    status = acq -> acquisitionStatus(time);

    if (status == 1) {
      status = proc -> status(time);
    }

    return QScriptValue(engine, status);
  } else {
    return QScriptValue(engine, -1);
  }
}

QScriptValue QxrdScriptEngine::acquireStatusFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() == 0) {
        return QScriptValue(engine, acq -> acquisitionStatus(0));
      } else {
        double time = context->argument(0).toNumber();
        return QScriptValue(engine, acq -> acquisitionStatus(time));
      }
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::processStatusFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    if (proc) {
      if (context->argumentCount() == 0) {
        return QScriptValue(engine, proc -> status(0));
      } else {
        double time = context->argument(0).toNumber();
        return QScriptValue(engine, proc -> status(time));
      }
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::acquireCancelFunc(QScriptContext * /*context*/, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      acq -> cancel();

      return QScriptValue(engine, 1);
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::exposureTimeFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_ExposureTime(context->argument(0).toNumber());
      }

      return QScriptValue(engine, acq -> get_ExposureTime());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::summedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_SummedExposures(context->argument(0).toUInt32());
      }

      return QScriptValue(engine, acq -> get_SummedExposures());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::skippedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_SkippedExposures(context->argument(0).toUInt32());
      }

      return QScriptValue(engine, acq -> get_SkippedExposures());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::darkSummedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_DarkSummedExposures(context->argument(0).toUInt32());
      }

      return QScriptValue(engine, acq -> get_DarkSummedExposures());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::phasesInGroupFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_PhasesInGroup(context->argument(0).toUInt32());
      }

      return QScriptValue(engine, acq -> get_PhasesInGroup());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::preTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_PreTriggerFiles(context->argument(0).toUInt32());
      }

      return QScriptValue(engine, acq -> get_PreTriggerFiles());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::postTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_PostTriggerFiles(context->argument(0).toUInt32());
      }

      return QScriptValue(engine, acq -> get_PostTriggerFiles());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::filePatternFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_FilePattern(context->argument(0).toString());
      }

      return QScriptValue(engine, acq -> get_FilePattern());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::outputDirectoryFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdExperimentPtr doc(eng->experiment());

    if (doc) {
      if (context->argumentCount() != 0) {
        doc -> set_ExperimentDirectory(context->argument(0).toString());
      }

      return QScriptValue(engine, doc -> get_ExperimentDirectory());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::fileIndexFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      if (context->argumentCount() != 0) {
        acq -> set_FileIndex(context->argument(0).toUInt32());
      }

      return QScriptValue(engine, acq -> get_FileIndex());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::dataFunc(QScriptContext * /*context*/, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    if (proc) {
      return engine -> newQObject(proc -> data().data());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::darkFunc(QScriptContext * /*context*/, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    if (proc) {
      return engine -> newQObject(proc -> darkImage().data());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::maskFunc(QScriptContext * /*context*/, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    if (proc) {
      return engine -> newQObject(proc -> mask().data());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::overflowFunc(QScriptContext * /*context*/, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    if (proc) {
      return engine -> newQObject(proc -> overflow().data());
    }
  }

  return QScriptValue(engine, -1);
}

QScriptValue QxrdScriptEngine::helpFunc(QScriptContext * /*context*/, QScriptEngine * /*engine*/)
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

QScriptValue QxrdScriptEngine::extraChannelFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      QxrdAcquisitionExtraInputsPtr xtra(acq->acquisitionExtraInputs());

      if (xtra) {
        if (context->argumentCount() != 0) {
          int channel = context->argument(0).toInteger();

          QxrdAcquisitionExtraInputsChannelPtr chan(xtra->channel(channel));

          return engine->newQObject(chan.data());
        }
      }
    }
  }

  return QScriptValue();
}

QScriptValue QxrdScriptEngine::processFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    if (proc) {
      if (context->argumentCount() >= 1) {
        QString file = context->argument(0).toString();
        QList<double> normVals;

        for (int i=1; i<context->argumentCount(); i++) {
          normVals.append(context->argument(i).toNumber());
        }

        proc -> processNormalizedFile(file, normVals);
      }

      return QScriptValue(engine, 1);
    }
  }

  return QScriptValue(engine, -1);
}

static QString matchFilesProto =
    "matchFiles([pattern]...)";
static QString matchFilesDoc =
    "<p>Returns a list of file names matching the provided pattern(s).</p>"
    "<p>The patterns support 'wild card' characters sush as * and ?.</p>"
    "<p>Example:  matchFiles(\"*.tiff\")</p>";

QScriptValue QxrdScriptEngine::matchFilesFunc(QScriptContext *context, QScriptEngine *engine)
{
  QStringList result;

  if (context->argumentCount() >= 1) {
    QString dir = context->argument(0).toString();

    if (context->argumentCount() == 1) {
      QDir a(dir);
      a.setFilter(QDir::Dirs | QDir::Files);

      result.append(a.entryList());
    } else {
      for (int i=1; i<context->argumentCount(); i++) {
        QString patt = context->argument(i).toString();

        //        g_Application->printMessage(tr("Matching %1").arg(patt));

        QDir a(dir);

        a.setFilter(QDir::Dirs | QDir::Files);
        QStringList patts;
        patts<<patt;

        a.setNameFilters(patts);

        QStringList entries = a.entryList();
        //        int nEntries = entries.length();
        //        g_Application->printMessage(tr("Matched %1 entries").arg(nEntries));

        result.append(entries);
      }
    }
  }

  return engine->toScriptValue(result);
}

void QxrdScriptEngine::initialize()
{
  qScriptRegisterMetaType(this, ::QxrdRingFitToScriptValue, ::QxrdRingFitFromScriptValue);
  qScriptRegisterMetaType(this, ::QxrdRingSampledDataToScriptValue, ::QxrdRingSampledDataFromScriptValue);
  //  qScriptRegisterMetaType(this, ::QxrdRingSampledDataPtrToScriptValue, ::QxrdRingSampledDataPtrFromScriptValue);

  qScriptRegisterSequenceMetaType< QList<int> >(this);
  qScriptRegisterSequenceMetaType< QList<bool> >(this);
  qScriptRegisterSequenceMetaType< QList<double> >(this);
  qScriptRegisterSequenceMetaType< QList<QString> >(this);
  //  qScriptRegisterSequenceMetaType< QList<QxrdRingFitParameters*> >(this);
  qScriptRegisterSequenceMetaType< QVector<int> >(this);
  qScriptRegisterSequenceMetaType< QVector<bool> >(this);
  qScriptRegisterSequenceMetaType< QVector<double> >(this);
  qScriptRegisterSequenceMetaType< QVector<QString> >(this);
  //  qScriptRegisterSequenceMetaType< QVector<QxrdRingFitParameters*> >(this);

  if (m_Application) {
    globalObject().setProperty("application", newQObject(m_Application));

    QxrdAllocatorPtr alloc(m_Application->allocator());

    if (alloc) {
      globalObject().setProperty("allocator", newQObject(alloc.data()));
    }
  }

  QXRD_DOC_OBJECT("global", "The Global Object");

  globalObject().setProperty("global", globalObject());
  globalObject().setProperty("acquire", newFunction(acquireFunc));
  globalObject().setProperty("acquireDark", newFunction(acquireDarkFunc));
  globalObject().setProperty("status", newFunction(statusFunc));
  globalObject().setProperty("acquireStatus", newFunction(acquireStatusFunc));
  globalObject().setProperty("processStatus", newFunction(processStatusFunc, 1));
  globalObject().setProperty("acquireCancel", newFunction(acquireCancelFunc));
  globalObject().setProperty("exposureTime", newFunction(exposureTimeFunc, 1));
  globalObject().setProperty("summedExposures", newFunction(summedExposuresFunc, 1));
  globalObject().setProperty("skippedExposures", newFunction(skippedExposuresFunc, 1));
  globalObject().setProperty("darkSummedExposures", newFunction(darkSummedExposuresFunc, 1));
  globalObject().setProperty("phasesInGroup", newFunction(phasesInGroupFunc, 1));
  globalObject().setProperty("preTriggerFiles", newFunction(preTriggerFilesFunc, 1));
  globalObject().setProperty("postTriggerFiles", newFunction(postTriggerFilesFunc, 1));
  globalObject().setProperty("filePattern", newFunction(filePatternFunc, 1));
  globalObject().setProperty("outputDirectory", newFunction(outputDirectoryFunc, 1));
  globalObject().setProperty("fileIndex", newFunction(fileIndexFunc, 1));
  globalObject().setProperty("print", newFunction(printFunc, NULL));
  globalObject().setProperty("printMessage", newFunction(printFunc, NULL));
  globalObject().setProperty("data", newFunction(dataFunc));
  globalObject().setProperty("dark", newFunction(darkFunc));
  globalObject().setProperty("mask", newFunction(maskFunc));
  globalObject().setProperty("overflow", newFunction(overflowFunc));
  globalObject().setProperty("help", newFunction(helpFunc));
  globalObject().setProperty("process", newFunction(processFunc));
  globalObject().setProperty("typeName", newFunction(typeNameFunc));
  globalObject().setProperty("matchFiles", newFunction(matchFilesFunc));
  globalObject().setProperty("extraChannel", newFunction(extraChannelFunc, 1));

  if (m_Application) {
    QObject *plugin = dynamic_cast<QObject*>(m_Application->nidaqPlugin().data());

    if (plugin) {
      QXRD_DOC_OBJECT("nidaq", "NIDAQ Data Acquisition Plugin");
      globalObject().setProperty("nidaq", newQObject(plugin));
    }
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QXRD_DOC_OBJECT("experiment", "The current experiment");
    globalObject().setProperty("experiment", newQObject(expt.data()));

    m_Acquisition   = expt->acquisition();

    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq) {
      QXRD_DOC_OBJECT("acquisition", "The Acquisition Object");
      globalObject().setProperty("acquisition",     newQObject(acq.data()));

      QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());

      if (sync) {
        QXRD_DOC_OBJECT("synchronization", "Synchronized Acquisition");
        globalObject().setProperty("synchronization", newQObject(sync.data()));
      }

      QxrdAcquisitionTriggerPtr trig(acq->acquisitionTrigger());

      if (trig) {
        QXRD_DOC_OBJECT("trigger", "Acquisition Triggering");
        globalObject().setProperty("trigger", newQObject(trig.data()));
      }

      QxrdAcquisitionExtraInputsPtr extra(acq->acquisitionExtraInputs());

      if (extra) {
        QXRD_DOC_OBJECT("extraInputs", "Extra Inputs during Acquisition");
        globalObject().setProperty("extraInputs", newQObject(extra.data()));
      }
    }

    QxrdSimpleServerPtr ssrv(expt->simpleServer());

    if (ssrv) {
      QXRD_DOC_OBJECT("simpleServer", "Remote Control Text Based Socket Server");
      globalObject().setProperty("simpleServer", newQObject(ssrv.data()));
    }

    QxrdServerPtr srv(expt->specServer());

    if (srv) {
      QXRD_DOC_OBJECT("specServer", "Remote Control Server for use with Spec");
      globalObject().setProperty("specServer", newQObject(srv.data()));
    }

    m_DataProcessor = expt->dataProcessor();

    QxrdDataProcessorPtr dp(m_DataProcessor);

    if (dp) {
      QXRD_DOC_OBJECT("processor", "Control Data Processing Options");
      globalObject().setProperty("processor",       newQObject(dp.data()));

      QXRD_DOC_OBJECT("centering", "Beam Center and Detector Alignment Options");
      globalObject().setProperty("centering",       newQObject(dp->centerFinder().data()));

      QXRD_DOC_OBJECT("integrator", "Image Circular Integration Options");
      globalObject().setProperty("integrator",      newQObject(dp->integrator().data()));
      globalObject().setProperty("initialFit",      newQObject(dp->initialRingSetFitParameters().data()));
      globalObject().setProperty("refinedFit",      newQObject(dp->refinedRingSetFitParameters().data()));
      globalObject().setProperty("initialData",     newQObject(dp->initialRingSetData().data()));
      globalObject().setProperty("refinedData",     newQObject(dp->refinedRingSetData().data()));

      QxrdGenerateTestImagePtr gti(dp->generateTestImage());

      if (gti) {
        globalObject().setProperty("testImage",       newQObject(gti.data()));
      }
    }
  }
}

//#define DOC_PROPERTY "documentation"
//#define PROTO_PROPERTY "protodoc"

//QScriptValue QxrdScriptEngine::newQxrdObject(QObject *object, QString objectName)
//{
//  QScriptValue res = newQObject(object);

//  res.setProperty(DOC_PROPERTY, docString);

//  return res;
//}

//QScriptValue QxrdScriptEngine::newQxrdFunction(FunctionSignature fun, QString objectName)
//{
//  QScriptValue res = newFunction(fun);

//  res.setProperty(PROTO_PROPERTY, protoString);
//  res.setProperty(DOC_PROPERTY, docString);

//  return res;
//}

QString QxrdScriptEngine::documentationLink(QString base, QString subItem)
{
  QString item = base.isEmpty() ? subItem : base+"."+subItem;

  return tr("<a href=\"qrc:/help/autohelp?%1\">%2</a>").arg(item).arg(item);
}

QString QxrdScriptEngine::documentationText(QString item)
{
  //  QScriptValue val=QScriptEngine::evaluate("global." + item, QString(), 1);

  QScriptValue val;
  QString prefix  = "";
  QString itemName="Global Object";

  if (item.isEmpty()) {
    val = globalObject();
  } else {
    val = globalObject().property(item);
    prefix = item+".";
    itemName = item;
  }

  QString res;
  //  QString name  = val.toString();
  QString doc   = QxrdDocumentationDictionary::get_Doc(item);

  res.append(tr("<h2>Documentation for %1</h2>\n").arg(itemName));

  if (val.isFunction()) {
    QString proto = QxrdDocumentationDictionary::get_Proto(item);
    res.append(tr("<p>%1</p>\n").arg(prefix+proto));
    res.append(tr("%1\n").arg(doc));
  } else if (val.isObject()) {
    res.append(tr("%1\n").arg(doc));
    QObject *qobj = val.toQObject();

    if (qobj) {
      res.append(tr("<h3>QObject class %1 name %2</h3>\n")
                 .arg(qobj->metaObject()->className())
                 .arg(qobj->objectName()));

      const QMetaObject *meta = qobj->metaObject();

      if (meta->propertyCount() > QObject::staticMetaObject.propertyCount()) {
        res.append(tr("<h3>Properties of %1</h3>\n").arg(itemName));
        res.append(tr("<table>\n"));

        for (int i=QObject::staticMetaObject.propertyCount();
             i<meta->propertyCount(); i++) {
          const char* propName = meta->property(i).name();
          QVariant val = qobj->property(propName);

          res.append(tr("<tr>\n"));
          res.append(tr("<td>%1</td>\n").arg(prefix+propName));
          res.append(tr("<td>%1</td>\n").arg(val.typeName()));
          res.append(tr("<td>%1</td>\n").arg(val.toString()));
          res.append(tr("</tr>\n"));
        }

        res.append(tr("</table>\n"));
      }

      if (meta->methodCount() > QObject::staticMetaObject.methodCount()) {
        res.append(tr("<h3>Methods of %1</h3>\n").arg(itemName));
        res.append(tr("<table>\n"));

        for (int i=QObject::staticMetaObject.methodCount();
             i<meta->methodCount(); i++) {
          const char* methodSig = meta->method(i).signature();

          res.append(tr("<tr>\n"));
          res.append(tr("<td>%1</td>\n").arg(prefix+methodSig));
          res.append(tr("</tr>\n"));
        }

        res.append(tr("</table>\n"));
      }
    } else {
      QScriptValueIterator iter(val);

      QMap<QString, QScriptValue> subObjects, properties, functions;

      while (iter.hasNext()) {
        iter.next();

        QScriptValue v = iter.value();
        QString      n = iter.name();

        if (v.isFunction()) {
          functions.insert(n, v);
        } else if (v.isObject()) {
          subObjects.insert(n, v);
        } else {
          properties.insert(n, v);
        }
      }

      if (subObjects.count()) {
        QMapIterator<QString,QScriptValue> obj_iter(subObjects);

        res.append(tr("<h3>Sub Objects of %1</h3>\n").arg(itemName));
        res.append(tr("<table>\n"));

        while (obj_iter.hasNext()) {
          obj_iter.next();

          res.append(tr("<tr>\n"));
          res.append(tr("<td>%1</td>").arg(documentationLink(prefix+item, obj_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tr("</table>\n"));
      }

      if (properties.count()) {
        QMapIterator<QString,QScriptValue> prop_iter(properties);

        res.append(tr("<h3>Properties of %1</h3>\n").arg(itemName));
        res.append(tr("<table>\n"));

        while (prop_iter.hasNext()) {
          prop_iter.next();

          res.append(tr("<tr>\n"));
          res.append(tr("<td>%1</td>").arg(documentationLink(prefix+item, prop_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tr("</table>\n"));
      }

      if (functions.count()) {
        QMapIterator<QString,QScriptValue> func_iter(functions);

        res.append(tr("<h3>Methods of %1</h3>\n").arg(itemName));
        res.append(tr("<table>\n"));

        while (func_iter.hasNext()) {
          func_iter.next();

          res.append(tr("<tr>\n"));
          res.append(tr("<td>%1</td>").arg(documentationLink(prefix+item, func_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tr("</table>\n"));
      }
    }
  }

  QScriptValueIterator iter(val);

  QMap<QString,QObject*>      subObjects;
  QMap<QString,QcepProperty*> objectProperties;
  QMap<QString,QScriptValue>  memberFunctions;

  if (iter.hasNext()) {
    res.append("<table>\n");
    res.append("<tr>\n");
    res.append("<th>Name</th>\n");
    res.append("<th>Value</th>\n");
    res.append("<th>Data</th>\n");
    res.append("<th>Proto</th>\n");
    res.append("<th>Arr?</th>\n");
    res.append("<th>Bool?</th>\n");
    res.append("<th>Num?</th>\n");
    res.append("<th>Obj?</th>\n");
    res.append("<th>QObj?</th>\n");
    res.append("<th>QMta?</th>\n");
    res.append("<th>Fun?</th>\n");
    res.append("<th>Var?</th>\n");
    res.append("<th>Str?</th>\n");
    res.append("</tr>\n");

    while (iter.hasNext()) {
      iter.next();
      if (iter.name() != "documentation") {
        QScriptValue val = iter.value();

        if (val.isQObject()) {
          QObject *obj = val.toQObject();

          if (obj) {
            QcepProperty *prop = qobject_cast<QcepProperty*>(obj);

            if (prop) {
              objectProperties.insert(iter.name(),prop);
            } else {
              subObjects.insert(iter.name(),obj);
            }
          }
        } else if (val.isFunction()) {
          memberFunctions.insert(iter.name(),val);
        }
      }

      QScriptValue val = iter.value();
      res.append("<tr>\n");
      res.append(tr("<td>%1</td>\n").arg(documentationLink(item,iter.name())));
      res.append(tr("<td>%1</td>\n").arg(val.toString()));
      res.append(tr("<td>%1</td>\n").arg(val.data().toString()));
      res.append(tr("<td>%1</td>\n").arg(val.prototype().toString()));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isArray()?"ARR":""));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isBool()?"BOOL":""));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isNumber()?"NUM":""));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isObject()?"OBJ":""));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isQObject()?"QOBJ":""));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isQMetaObject()?"QMETA":""));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isFunction()?"FUNC":""));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isVariant()?"VRNT":""));
      res.append(tr("<td align=center>%1</td>\n").arg(val.isString()?"STRG":""));
      res.append("</tr>\n");
    }

    res.append("</table>\n");
  }

  if (objectProperties.count()) {
    res.append(tr("<h3>Properties</h3>\n"));
    res.append(tr("<dl>\n"));

    foreach(QcepProperty *prop, objectProperties) {
      res.append(tr("<dt>%1</dt>\n").arg(documentationLink(item,prop->name())));
      res.append(tr("<dd>%1</dd>\n").arg(prop->toolTip()));
    }

    res.append(tr("</dl>\n"));
  }

  if (memberFunctions.count()) {
    res.append(tr("<h3>Functions</h3>\n"));
    res.append(tr("<dl>\n"));

    foreach(QScriptValue val, memberFunctions) {
      res.append(tr("<dt>%1</dt>\n").arg(documentationLink(item,val.property("name").toString())));
      QString propDoc = val.property("documentation").toString();
      res.append(tr("<dd>%1</dd>\n").arg(propDoc));
    }

    res.append(tr("</dl>\n"));
  }

  if (subObjects.count()) {
    res.append(tr("<h3>Sub Objects</h3>\n"));
    res.append(tr("<dl>\n"));

    foreach(QObject *obj, subObjects) {
      res.append(tr("<dt>%1</dt>\n").arg(documentationLink(item,obj->objectName())));
      QString propDoc = obj->property("documentation").toString();
      res.append(tr("<dd>%1</dd>\n").arg(propDoc));
    }

    res.append(tr("</dl>\n"));
  }

  return res;
}

QByteArray QxrdScriptEngine::helpText(QString item)
{
  QString scriptItem;

//  if (item.isEmpty()) {
//    scriptItem = "global";
//  } else {
    scriptItem = item;
//  }

  QByteArray res;

  res.append(
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
        "<html>\n"
        "  <head>\n"
        "    <title>qxrd help</title>\n"
        "  </head>\n"
        "\n"
        "  <body>\n");

  res.append(documentationText(scriptItem));

  res.append(
        "  </body>\n"
        "</html>\n");

  return res;
}
