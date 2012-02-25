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
#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdallocator.h"

#include <QThread>
#include <QDir>

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

  globalObject().setProperty("acquire", newFunction(acquireFunc));
  globalObject().setProperty("acquireDark", newFunction(acquireDarkFunc));
  globalObject().setProperty("status", newFunction(statusFunc));
  globalObject().setProperty("acquireStatus", newFunction(acquireStatusFunc));
  globalObject().setProperty("processStatus", newFunction(processStatusFunc));
  globalObject().setProperty("acquireCancel", newFunction(acquireCancelFunc));
  globalObject().setProperty("exposureTime", newFunction(exposureTimeFunc));
  globalObject().setProperty("summedExposures", newFunction(summedExposuresFunc));
  globalObject().setProperty("skippedExposures", newFunction(skippedExposuresFunc));
  globalObject().setProperty("darkSummedExposures", newFunction(darkSummedExposuresFunc));
  globalObject().setProperty("phasesInGroup", newFunction(phasesInGroupFunc));
  globalObject().setProperty("preTriggerFiles", newFunction(preTriggerFilesFunc));
  globalObject().setProperty("postTriggerFiles", newFunction(postTriggerFilesFunc));
  globalObject().setProperty("filePattern", newFunction(filePatternFunc));
  globalObject().setProperty("outputDirectory", newFunction(outputDirectoryFunc));
  globalObject().setProperty("fileIndex", newFunction(fileIndexFunc));
  globalObject().setProperty("print", newFunction(printFunc));
  globalObject().setProperty("printMessage", newFunction(printFunc));
  globalObject().setProperty("data", newFunction(dataFunc));
  globalObject().setProperty("dark", newFunction(darkFunc));
  globalObject().setProperty("mask", newFunction(maskFunc));
  globalObject().setProperty("overflow", newFunction(overflowFunc));
  globalObject().setProperty("help", newFunction(helpFunc));
  globalObject().setProperty("process", newFunction(processFunc));
  globalObject().setProperty("typeName", newFunction(typeNameFunc));
  globalObject().setProperty("matchFiles", newFunction(matchFilesFunc));

  if (m_Application) {
    QObject *plugin = dynamic_cast<QObject*>(m_Application->nidaqPlugin().data());

    if (plugin) {
      globalObject().setProperty("nidaq", newQObject(plugin));
    }
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    globalObject().setProperty("experiment", newQObject(expt.data()));

    m_Acquisition   = expt->acquisition();

    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq) {
      globalObject().setProperty("acquisition",     newQObject(acq.data()));
      globalObject().setProperty("synchronization", newQObject(acq->synchronizedAcquisition()));
    }

    m_DataProcessor = expt->dataProcessor();

    QxrdDataProcessorPtr dp(m_DataProcessor);

    if (dp) {
      globalObject().setProperty("processor",       newQObject(dp.data()));
      globalObject().setProperty("centering",       newQObject(dp->centerFinder().data()));
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

    QxrdWindow *wp = expt->window();

    if (wp) {
      globalObject().setProperty("window",          newQObject(wp));
      globalObject().setProperty("imageGraph",      newQObject(wp->m_Plot));
      globalObject().setProperty("centeringGraph",  newQObject(wp->m_CenterFinderPlot));
      globalObject().setProperty("integratorGraph", newQObject(wp->m_IntegratorPlot));
    }
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

  return hasUncaughtException();
}

int  QxrdScriptEngine::uncaughtExceptionLineNumber() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return uncaughtExceptionLineNumber();
}

QString QxrdScriptEngine::uncaughtExceptionString() const
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return uncaughtException().toString();
}

QScriptValue QxrdScriptEngine::printFunc(QScriptContext *context, QScriptEngine *engine)
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
