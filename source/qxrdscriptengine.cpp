#include "qxrddebug.h"
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
#include "qxrdacquisitionextrainputs.h"
#include "qxrdallocator.h"
#include "qxrdserver.h"
#include "qxrdsimpleserver.h"
#include "qcepdocumentationdictionary.h"

#include <QThread>
#include <QDir>
#include <QScriptValueIterator>
#include <QMetaObject>
#include <QMetaProperty>
#include <QRegExp>

QxrdScriptEngine::QxrdScriptEngine(QxrdApplicationWPtr app, QxrdExperimentWPtr exp)
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

QxrdApplicationWPtr QxrdScriptEngine::application() const
{
  return m_Application;
}

QxrdExperimentWPtr QxrdScriptEngine::experiment() const
{
  return m_Experiment;
}

QxrdAcquisitionWPtr QxrdScriptEngine::acquisition() const
{
  return m_Acquisition;
}

QxrdWindowWPtr QxrdScriptEngine::window() const
{
  return m_Window;
}

QxrdDataProcessorWPtr QxrdScriptEngine::dataProcessor() const
{
  return m_DataProcessor;
}

void QxrdScriptEngine::setWindow(QxrdWindowWPtr win)
{
  m_Window = win;

  QxrdWindowPtr w = m_Window;

  if (w) {
    QCEP_DOC_OBJECT("window", "The Experiment Main Window");
    globalObject().setProperty("window",          newQObject(w.data()));

    QCEP_DOC_OBJECT("imageGraph", "The Image Plot in the Main Experiment Window");
    globalObject().setProperty("imageGraph",      newQObject(w->m_ImagePlot));

    QCEP_DOC_OBJECT("centeringGraph", "The Center Finder Plot");
    globalObject().setProperty("centeringGraph",  newQObject(w->m_CenterFinderPlot));

    QCEP_DOC_OBJECT("integratorGraph", "The Integrated Data Plot");
    globalObject().setProperty("integratorGraph", newQObject(w->m_IntegratorPlot));
  }
}

void QxrdScriptEngine::lock()
{
  m_Mutex.lock();
}

void QxrdScriptEngine::unlock()
{
  m_Mutex.unlock();
}

void QxrdScriptEngine::evaluateAppCommand(QString expr)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  //  printf("QxrdScriptingEngine::evaluateAppCommand(%s)\n", qPrintable(expr));

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "evaluateScript", Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(QString, expr)));
}

void QxrdScriptEngine::evaluateSimpleServerCommand(QString expr)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  //  printf("QxrdScriptingEngine::evaluateServerCommand(%s)\n", qPrintable(expr));

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "evaluateScript", Qt::QueuedConnection, Q_ARG(int, 1), Q_ARG(QString, expr)));
}

void QxrdScriptEngine::evaluateSpecCommand(QString expr)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  //  printf("QxrdScriptingEngine::evaluateSpecCommand(%s)\n", qPrintable(expr));

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "evaluateScript", Qt::QueuedConnection, Q_ARG(int, 2), Q_ARG(QString, expr)));
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

QString QxrdScriptEngine::convertToString(QScriptValue result)
{
  if (result.isError()) {
    return "ERROR : "+result.property("error").toString();
  } else if (result.isArray()) {
    int len = result.property("length").toInteger();

    QString s = "[";

    for (int i=0; i<len; i++) {
      s += convertToString(result.property(tr("%1").arg(i)));

      if (i<len-1) {
        s += ", ";
      }
    }

    s += "]";

    return s;

  } else if (result.isObject()) {
    QScriptValueIterator it(result);

    QString s = "{";

    while(it.hasNext()) {
      it.next();

      s += it.name()+":";
      s += convertToString(it.value());

      if (it.hasNext()) {
        s += ", ";
      }
    }

    s += "}";

    return s;

  } else {
    return result.toString();
  }
}

void QxrdScriptEngine::evaluateScript(int src, QString expr)
{
  THREAD_CHECK;

  //  printf("QxrdScriptingEngine::evaluateScript(%s)\n", qPrintable(expr));

  QScriptValue result = QScriptEngine::evaluate(expr);

  if(qcepDebug(DEBUG_LOGSCRIPTS)) {
    QxrdExperimentPtr exp(m_Experiment);

    if (exp) {
      exp->printMessage(tr("Evaluate[%1] %2 = %3").arg(src).arg(expr).arg(result.toString()));
    }
  }

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

QCEP_DOC_FUNCTION(
    "print",
    "print([value]...)",
    "Print values to the log file and message window",
    "<p>The values of the arguments are catenated into a single string which is "
    "printed to the log file and to the message window</p>\n"
    "<p>The following is a typical use: print out the names and values of the "
    "elements of an object:</p>\n"
    "<code>"
    "for(i in acquisition) print(i, acquisition[i])"
    "</code>"
    )

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

QCEP_DOC_FUNCTION(
    "acquire",

    "acquire([fileName[, exposure[, summedExposures[, postTriggerFiles[, preTriggerFiles[, nPhases]]]]]])",
    "Start acquisition of a sequence of images",

    "<p>The arguments are optional and may be successively omitted from the "
    "right.  If <i>preTriggerFiles</i> is omitted, zero is used instead. "
    "If <i>nPhases</i> is omitted, one is used. "
    "Any other argument which is omitted will take its value "
    "instead from the values entered in the acquire dialog.  Any "
    "argument which is given will replace the corresponding value "
    "in the acquire dialog.</p>\n"
    "<p>Note that the script function merely starts the acquisition "
    "- you should use the separate \"status\" function to wait for "
    "acquisition and processing to be completed.</p>\n"
    "<p>The following is a typical example of the use of this "
    "command from spec:</p>\n"
    "<code>"
    "def PEexp(filename,exposure,subframes,frames) '{<br/>"
    "&nbsp;&nbsp;remote_eval(PEHOST,"
    "sprintf(\"acquire(\\\"%s\\\",%g,%d,%d,0)\",filename,exposure,subframes,frames));<br/>"
    "&nbsp;&nbsp;<br/> &nbsp;&nbsp;PEwait()<br/> }'<br/>"
    "</code>"
    )

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

QCEP_DOC_FUNCTION(
    "acquireDark",
    "acquireDark([filename [,exposure [,darkSummedExposures]]])",
    "Start acquisition of a dark image",
    "<p>Arguments are optional and, if given, will replace the "
    "corresponding value in the acquire dialog, if not given the "
    "dialog values are used.</p>\n"
    "<p>The following is a typical example of the use of this "
    "command from spec:</p>\n"
    "<code>\n"
    "def PEexpd(filename, exposure, subframes) '{<br/>\n"
    "&nbsp;&nbsp;remote_eval(PEHOST,"
    "sprintf(\"acquireDark(\\\"%s\\\",%g,%d)\",filename,exposure,subframes));<br/>\n"
    "&nbsp;&nbsp;<br/> &nbsp;&nbsp;PEwait()<br/> }'<br/>\n"
    "</code>\n"
    )

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

QCEP_DOC_FUNCTION(
    "status",
    "status([time])",
    "Test if acquisition and processing have finished",
    "<p>"
    "If the argument is given the function will wait up to that "
    "many seconds - if acquisition and processing finish before "
    "the time elapsed then <code>status</code> will return at "
    "that time, otherwise at the end of the timeout period.  The "
    "function returns a non-zero result if acquisition and "
    "processing are complete, or zero if they are not."
    "</p>\n"
    "<p>"
    "If no argument is given the function tests if acquisition "
    "and processing are complete and returns the result of the "
    "test immediately."
    "</p>\n"
    "<p>"
    "The following is a typical use of this command from spec:"
    "</p>\n"
    "<code>"
    "def PEwait() '{<br/>"
    "&nbsp;&nbsp;while(remote_eval(PEHOST,\"status(1.0)\")==0) {<br/>"
    "&nbsp;&nbsp;}<br/> }'<br/>"
    "</code>\n"
    )

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

QCEP_DOC_FUNCTION(
    "acquireStatus",
    "acquireStatus([time])",
    "Test if acquisition has finished",
    "<p>"
    "Similar to 'status' "
    "except that it only tests for the acquisition operation being complete."
    "</p>"
    )

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

QCEP_DOC_FUNCTION(
    "processStatus",
    "processStatus([time])",
    "Test if processing has finished",
    "<p>"
    "Similar to 'status' "
    "except that it only tests for the processing operation being complete."
    "</p>"
    )

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

QCEP_DOC_FUNCTION(
    "acquireCancel",
    "acquireCancel()",
    "Cancel the current acquisition operation",
    ""
    )

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

QCEP_DOC_FUNCTION(
    "trigger",
    "trigger()",
    "Trigger triggered acquisition",
    "<p>If 'preTriggerFiles' is greater than zero then acquisition operations proceed in the "
    "'triggered' mode.  This acts much like a digital oscilloscope trigger where a certain number "
    "('preTriggerFiles') of acquired images are held in the computer RAM until the trigger operation occurs "
    "and then the most recent acquired images are written to disk, along with a number of post-trigger images</p>"
    )

QScriptValue QxrdScriptEngine::triggerFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdAcquisitionPtr acq(eng->acquisition());

    if (acq) {
      acq -> trigger();

      return QScriptValue(engine, 1);
    }
  }

  return QScriptValue(engine, -1);
}

QCEP_DOC_FUNCTION(
    "exposureTime",
    "exposureTime([time])",
    "Get or set the acquisition exposure time (also for dark exposures)",
    "<p>If the time argument is given, set the exposure time, otherwise return the "
    "current exposure time</p>"
    "<p>This function provides a convenient method to access the "
    "<code>acquisition.exposureTime</code> property</p>"
    )

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

QCEP_DOC_FUNCTION(
    "summedExposures",
    "summedExposures([n])",
    "Get or set the number of summed exposures for acquisition",
    "<p>If the n argument is given, set the number of summed exposures, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.summedExposures</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "skippedExposures",
    "skippedExposures([n])",
    "Get or set the number of skipped exposures for acquisition",
    "<p>If the n argument is given, set the number of skipped exposures, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.skippedExposures</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "darkSummedExposures",
    "darkSummedExposures([n])",
    "Get or set the number of summed exposures for dark acquisition",
    "<p>If the n argument is given, set the number of summed exposures, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.darkSummedExposures</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "phasesInGroup",
    "phasesInGroup([n])",
    "Get or set the number of phases for synchronized acquisition",
    "<p>If the n argument is given, set the number of phases, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.phasesInGroup</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "preTriggerFiles",
    "preTriggerFiles([n])",
    "Get or set the number of pre-trigger file groups for triggered acquisition",
    "<p>If the n argument is given, set the number of pre-trigger file groups, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.preTriggerFiles</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "postTriggerFiles",
    "postTriggerFiles([n])",
    "Get or set the number of post-trigger file groups for triggered acquisition",
    "<p>If the n argument is given, set the number of post-trigger file groups, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.postTriggerFiles</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "filePattern",
    "filePattern([pattern])",
    "Get or set the acquisition file name pattern",
    "<p>If the pattern argument is given, set the file name pattern, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.filePattern</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "outputDirectory",
    "outputDirectory([dir])",
    "Get or set the acquisition output directory",
    "<p>If the dir argument is given, set the output directory, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.outputDirectory</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "fileIndex",
    "fileIndex([n])",
    "Get or set the acquisition file index",
    "<p>If the n argument is given, set the file index, otherwise return the "
    "current value.</p>"
    "<p>Easy access to <code>acquisition.fileIndex</code></p>"
    )

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

QCEP_DOC_FUNCTION(
    "data",
    "data()",
    "Get the current image",
    "<p>Returns a reference to the most recently acquired or most recently loaded image.</p>"
    "<p>The returned object can have its properties queried.</p>"
    "<p>Note that the value returned by this function will change as each "
    "new image is acquired so be careful if calling this function during acquisition.</p>"
    )

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

QCEP_DOC_FUNCTION(
    "dark",
    "dark()",
    "Get the dark image (or null if none has been taken)",
    "<p>Returns a reference to the current dark image.</p>"
    "<p>The returned object can have its properties queried.</p>"
    "<p>For example, to get the file name of the dark image:</p>"
    "<code>print(dark().fileName)</code>"
    "<p>Or to show a list of properties of the dark image:</p>"
    "<code>for(i in dark()) print(i, dark()[i])</code>"
    )

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

QCEP_DOC_FUNCTION(
    "mask",
    "mask()",
    "Get the top item of the mask stack",
    "<p>Returns a reference to the top of the mask stack, or null if the "
    "mask stack is empty.</p>"
    )

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

QCEP_DOC_FUNCTION(
    "overflow",
    "overflow()",
    "Get the overflow pixel map for acquired images",
    "<p>Returns the overflow pixels map for the most recently acquired image "
    "(if overflow processing is enabled)</p>\n"
    "<p>The overflow map is non-zero wherever an image pixel exceeded the overflow "
    "threshold during acquisition.  Where more than one exposures are summed, the overflow "
    "map will count the number of exposures for which the pixel overflowed.</p>"
    )

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

QCEP_DOC_FUNCTION(
    "liveData",
    "liveData()",
    "Get the current live view image",
    "<p>Returns a reference to the most recently acquired live view image.</p>"
    "<p>The returned object can have its properties queried.</p>"
    "<p>Note that the value returned by this function will change as each "
    "new live view image is acquired so be careful when calling this function.</p>"
    )

QScriptValue QxrdScriptEngine::liveDataFunc(QScriptContext * /*context*/, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    if (proc) {
      return engine -> newQObject(proc -> liveData().data());
    }
  }

  return QScriptValue(engine, -1);
}

QCEP_DOC_FUNCTION(
    "help",
    "help([name...])",
    "Returns help text for a given name or names",
    "<p>Returns a string containing an html representation of the help text "
    "for a name or names.  If more than one name is given the result is the "
    "concatenation of the help for each name in turn</p>"
    "<p>The names may contain wildcard characters, the result will contain help "
    "for all available matching the wildcard patterns.</p>"
    "<p>If no name is given the command returns help for the help command itself.</p>"
    "<p>If the command is executed in the QXRD script window the html result will "
    "be properly formatted in the message window.</p>"
    "<p>For example:</p>"
    "<code>help(\"help\")</code>"
    )

QScriptValue QxrdScriptEngine::helpFunc(QScriptContext * context, QScriptEngine * engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    int n=context->argumentCount();
    QString res="";

    if (n==0) {
      res = eng->documentationText("help");
    } else {
      for (int i=0; i<n; i++) {
        QString name = context->argument(i).toString();

        QRegExp re(name, Qt::CaseInsensitive, QRegExp::Wildcard);

        res.append(eng->documentationText(re));
      }
    }

    return QScriptValue(engine, res);
  } else {
    return QScriptValue(engine, QString());
  }
}

QCEP_DOC_FUNCTION(
    "extraChannel",
    "extraChannel(n)",
    "Access channels for extra inputs",
    "<p>Returns a reference to configuration data for an extra input channel.</p>"
    "<p>Example: to access the acquired waveform for extra channel 0</p>"
    "<code>extraChannel(0).waveform</code>"
    )

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

QCEP_DOC_FUNCTION(
    "process",
    "process(filename [, norm...])",
    "Load and process an image file",
    "<p>Load and process the file filename.  The norm arguments are used as "
    "normalization values during processing</p>"
    "<p>The function is closely related to</p>"
    "<code>processor.processNormalizedFile(QString,QDoubleList)</code>"
    )

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

QCEP_DOC_FUNCTION(
    "setFileNormalization",
    "setFileNormalization(filename [, norm...])",
    "Set the normalization values for an image file",
    "<p>Loads the file filename, sets its normalization values and "
    "rewrites the metadata file</p>"
    "<p>The function is closely related to</p>"
    "<code>processor.setFileNormalization(QString,QDoubleList)</code>"
    )

QScriptValue QxrdScriptEngine::setFileNormalizationFunc(QScriptContext *context, QScriptEngine *engine)
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

        proc -> setFileNormalization(file, normVals);
      }

      return QScriptValue(engine, 1);
    }
  }

  return QScriptValue(engine, -1);
}

QCEP_DOC_FUNCTION(
    "matchFiles",
    "matchFiles([pattern]...)",
    "Return a list of files matching a pattern",
    "<p>Returns a list of file names matching the provided pattern(s).</p>"
    "<p>The patterns support 'wild card' characters sush as * and ?.</p>"
    "<p>Example - to calculate the sum of the pixel intensity of pixel 100,100 in "
    "all the TIFF files in the current directory:</p>"
    "<code>var sum=0;<br/>\n"
    "for(f in matchFiles(\"*.tif\") {<br/>\n"
    "&nbsp;&nbsp;processor.loadData(f);<br/>\n"
    "&nbsp;&nbsp;sum += data().getImageData(100,100);<br/>\n"
    "}<br>"
    )

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

QCEP_DOC_FUNCTION(
    "mapUserFunction",
    "mapUserFunction(functionname)",
    "Map a user function over current data",
    ""
    )

QScriptValue QxrdScriptEngine::mapUserFunctionFunc(QScriptContext *context, QScriptEngine *engine)
{
  QxrdScriptEngine *eng = qobject_cast<QxrdScriptEngine*>(engine);

  if (eng) {
    QxrdDataProcessorPtr proc(eng->dataProcessor());

    QxrdDoubleImageDataPtr d = proc->data();

    if (d) {
      int ht = d->get_Height();
      int wd = d->get_Width();

      if (context->argumentCount() == 1) {
        QScriptValue func = context->argument(0);

        if (!func.isFunction()) {
          func = eng->globalObject().property(func.toString());
        }

        if (func.isFunction()) {
          for (int i=0; i<wd; i++) {
            for (int j=0; j<ht; j++) {
              d->setValue(i,j, func.call(QScriptValue(), QScriptValueList() << i << j).toNumber());
            }
          }

          proc->newData(d, QxrdMaskDataPtr());
        }
      }
    }
  }

  return engine->toScriptValue(0);
}

QCEP_DOC_OBJECT(
    "JSON",
    "Qt Built-in JSON Parser"
    )

QCEP_DOC_FUNCTION(
    "JSON.parse",
    "JSON.parse(string)",
    "Parse a string as a JSON object",
    "<p>A built-in function in the Qt script system</p>"
    )

QCEP_DOC_FUNCTION(
    "JSON.stringify",
    "JSON.stringify(object)",
    "Produce JSON string representation of an object",
    "<p>A built-in function in the Qt script system</p>"
    )

QCEP_DOC_FUNCTION(
    "Math",
    "var x = Math.sqrt(2)",
    "Qt Built-in Math Module",
    "<p>Qt Script mathematical functions module</p>"
    "<p>See also: <a href=\"http://www.w3schools.com/jsref/jsref_obj_math.asp\">JavaScript Math Object</a></p>"
    )

QCEP_DOC_OBJECT(
    "Infinity",
    "Qt Script built-in object with infinite value"
    )

QCEP_DOC_OBJECT(
    "NaN",
    "Qt Script built-in object with NaN (Not-a-Number) Value"
    )

QCEP_DOC_OBJECT(
    "undefined",
    "Qt Script built-in object with undefined value"
    )


QCEP_DOC_FUNCTION(
    "Array",
    "var x=Array([value...])",
    "Qt Script built in array constructor function (<a href=\"http://www.w3schools.com/jsref/jsref_obj_array.asp\">JavaScript Array Object</a>)",
    "<p>Construct a QtScript array containing the indicated elements</p>"
    "<p>Example:</p>"
    "<code>Array(1,2,3,4)</code>"
    )

QCEP_DOC_FUNCTION(
    "Boolean",
    "var bool=Boolean(value)",
    "Qt Script built in Boolean constructor function",
    "<p>Construct a QtScript boolean with the given value<p>"
    "<p>Example:</p>"
    "<code>Boolean(0)</code>"
    )

QCEP_DOC_FUNCTION(
    "Date",
    "var d=new Date(|msec|string|year,month[,day[,hr[,min[,sec[,msec]]]]])",
    "Qt Script built in date object",
    "<p>Construct dates given various inputs</p>"
    "<code>var d = new Date() // current date<br/>"
    "var d = new Date(msec)  // Date from millisecs<br/>"
    "var d = new Date(\"October 13, 1975 11:13:00\") // Date from a string<br/>"
    "var d = new Date(2001,5,24) // Date from numeric values</code>"
    "<p>See also: <a href=\"http://www.w3schools.com/jsref/jsref_obj_date.asp\">JavaScript Date Object</a></p>"
    )

QCEP_DOC_FUNCTION(
    "RegExp",
    "var patt=new RegExp(pattern,modifiers) or var patt=/pattern/modifiers",
    "Qt Script built in Regular Expression Object",
    "<p>Construct regular expression matching objects</p>"
    "<p>Examples:</p>"
    "<code>var p1 = new RegExp(\".*\\.tif\",\"i\")<br/>"
    "var p2=/.*\\.tif/i</code>"
    "<p>See also: <a href=\"http://www.w3schools.com/jsref/jsref_obj_regexp.asp\">JavaScript RegExp Object</a></p>"
    )

QCEP_DOC_FUNCTION(
    "String",
    "var txt = new String(\"string\"); var t2=\"hhh\";",
    "Qt Script built in String Object",
    "<p>Construct and manipulate string values.</p>"
    "<p>See also: <a href=\"http://www.w3schools.com/jsref/jsref_obj_string.asp\">JavaScript String Object</a></p>"
    )

QCEP_DOC_FUNCTION(
    "Number",
    "var num = new Number(42)",
    "Qt Script Built in Number Object",
    "<p>Construct and manipulate numeric values.</p>"
    "<p>See also: <a href=\"http://www.w3schools.com/jsref/jsref_obj_number.asp\">JavaScript Number Object</a></p>"
    )

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

  QxrdApplicationPtr app(m_Application);

  if (app) {
    QCEP_DOC_OBJECT("application", "The QXRD Application Object");
    globalObject().setProperty("application", newQObject(app.data()));

    QxrdAllocatorPtr alloc(app->allocator());

    if (alloc) {
      QCEP_DOC_OBJECT("allocator", "The QXRD Memory Allocator");
      globalObject().setProperty("allocator", newQObject(alloc.data()));
    }
  }

  QCEP_DOC_OBJECT("global", "The Global Object");
  globalObject().setProperty("global", globalObject());

  QCEP_DOC_OBJECT("scripting", "The Script Engine");
  globalObject().setProperty("scripting", newQObject(this));

  globalObject().setProperty("acquire", newFunction(acquireFunc));
  globalObject().setProperty("acquireDark", newFunction(acquireDarkFunc));
  globalObject().setProperty("status", newFunction(statusFunc));
  globalObject().setProperty("acquireStatus", newFunction(acquireStatusFunc));
  globalObject().setProperty("processStatus", newFunction(processStatusFunc, 1));
  globalObject().setProperty("acquireCancel", newFunction(acquireCancelFunc));
  globalObject().setProperty("trigger", newFunction(triggerFunc));
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
  globalObject().setProperty("liveData", newFunction(liveDataFunc));
  globalObject().setProperty("help", newFunction(helpFunc));
  globalObject().setProperty("process", newFunction(processFunc));
  globalObject().setProperty("setFileNormalization", newFunction(setFileNormalizationFunc));
  globalObject().setProperty("matchFiles", newFunction(matchFilesFunc));
  globalObject().setProperty("extraChannel", newFunction(extraChannelFunc, 1));
  globalObject().setProperty("mapUserFunction", newFunction(mapUserFunctionFunc, 1));

  if (app) {
    QObject *plugin = dynamic_cast<QObject*>(app->nidaqPlugin().data());

    if (plugin) {
      QCEP_DOC_OBJECT("nidaq", "NIDAQ Data Acquisition Plugin");
      globalObject().setProperty("nidaq", newQObject(plugin));
    }
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QCEP_DOC_OBJECT("experiment", "The current experiment");
    globalObject().setProperty("experiment", newQObject(expt.data()));

    m_Acquisition   = expt->acquisition();

    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq) {
      QCEP_DOC_OBJECT("acquisition", "The Acquisition Object");
      globalObject().setProperty("acquisition",     newQObject(acq.data(), QtOwnership, QScriptEngine::AutoCreateDynamicProperties));

      QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());

      if (sync) {
        QCEP_DOC_OBJECT("synchronization", "Synchronized Acquisition");
        globalObject().setProperty("synchronization", newQObject(sync.data()));
      }

      QxrdAcquisitionExtraInputsPtr extra(acq->acquisitionExtraInputs());

      if (extra) {
        QCEP_DOC_OBJECT("extraInputs", "Extra Inputs during Acquisition");
        globalObject().setProperty("extraInputs", newQObject(extra.data()));
      }
    }

    QxrdSimpleServerPtr ssrv(expt->simpleServer());

    if (ssrv) {
      QCEP_DOC_OBJECT("simpleServer", "Remote Control Text Based Socket Server");
      globalObject().setProperty("simpleServer", newQObject(ssrv.data()));
    }

    QxrdServerPtr srv(expt->specServer());

    if (srv) {
      QCEP_DOC_OBJECT("specServer", "Remote Control Server for use with Spec");
      globalObject().setProperty("specServer", newQObject(srv.data()));
    }

    m_DataProcessor = expt->dataProcessor();

    QxrdDataProcessorPtr dp(m_DataProcessor);

    if (dp) {
      QCEP_DOC_OBJECT("processor", "Control Data Processing Options");
      globalObject().setProperty("processor",       newQObject(dp.data()));

      QCEP_DOC_OBJECT("centering", "Beam Center and Detector Alignment Options");
      globalObject().setProperty("centering",       newQObject(dp->centerFinder().data()));

      QCEP_DOC_OBJECT("integrator", "Image Circular Integration Options");
      globalObject().setProperty("integrator",      newQObject(dp->integrator().data()));

      QCEP_DOC_OBJECT("initialFit", "Initial Powder Ring Fitting Parameters");
      globalObject().setProperty("initialFit",      newQObject(dp->initialRingSetFitParameters().data()));

      QCEP_DOC_OBJECT("refinedFit", "Refined Powder Ring Fitting Parameters");
      globalObject().setProperty("refinedFit",      newQObject(dp->refinedRingSetFitParameters().data()));

      QCEP_DOC_OBJECT("initialData", "Initial Powder Ring Fitting Data");
      globalObject().setProperty("initialData",     newQObject(dp->initialRingSetData().data()));

      QCEP_DOC_OBJECT("refinedData", "Refined Power Ring Fitting Data");
      globalObject().setProperty("refinedData",     newQObject(dp->refinedRingSetData().data()));

      QxrdGenerateTestImagePtr gti(dp->generateTestImage());

      if (gti) {
        QCEP_DOC_OBJECT("testImage", "Object for generating test images");
        globalObject().setProperty("testImage",       newQObject(gti.data()));
      }
    }
  }
}

QString QxrdScriptEngine::documentationLink(QString base, QString subItem)
{
  QString item = base.isEmpty() ? subItem : base+"."+subItem;
  QString res = tr("<a href=\"qrc:/help/autohelp?%1\">%2</a>").arg(item).arg(item);

  QString proto = QcepDocumentationDictionary::get_Proto(item);
  QString doc   = QcepDocumentationDictionary::get_Doc(item);

  res.append("</td>\n<td width=\"75%\">\n");

  if (proto.length()) {
    res.append(tr("<p><i>%1</i></p>\n").arg(proto));
  }

  if (doc.length()) {
    res.append(tr("<p>%1</p>\n").arg(doc));
  }

  return res;
}

QString QxrdScriptEngine::tableHeader()
{
  return
      "<table border=\"0\" cellpadding=\"0\" cellspacing=\"1\" bgcolor=\"black\" width=\"100%\">\n"
      "<tr>\n"
      "<td bgcolor=\"black\">\n"
      "<table border=\"0\" cellpadding=\"1\" cellspacing=\"1\" bgcolor=\"black\" width=\"100%\">\n";
}

QString QxrdScriptEngine::tableFooter()
{
  return
      "</table>\n"
      "</td>\n"
      "</tr>\n"
      "</table>\n";
}

QString QxrdScriptEngine::documentationText(QString item)
{
  //  QScriptValue val=QScriptEngine::evaluate("global." + item, QString(), 1);

  QScriptValue val;
  QString prefix  = "";
  QString itemName="Global";

  if (item.isEmpty()) {
    val = globalObject();
  } else {
    val = globalObject().property(item);
    prefix = item+".";
    itemName = item;
  }

  QString res;
  //  QString name  = val.toString();
  QString doc   = QcepDocumentationDictionary::get_Doc(item);
  QString proto = QcepDocumentationDictionary::get_Proto(item);
  QString longDoc = QcepDocumentationDictionary::get_LongDoc(item);

  res.append(tr("<h2>Documentation for %1</h2>\n").arg(itemName));

  if (val.isFunction()) {
    if (proto.length()) {
      res.append(tr("<p><i>%1</i></p>\n").arg(proto));
    }

    if (doc.length()) {
      res.append(tr("%1\n").arg(doc));
    }

    if (longDoc.length()) {
      res.append(tr("%1\n").arg(longDoc));
    }
  } else if (val.isObject()) {
    res.append(tr("%1\n").arg(doc));
    res.append(tr("%1\n").arg(longDoc));

    QObject *qobj = val.toQObject();

    if (qobj) {
      res.append(tr("<h3>QObject class %1 name %2</h3>\n")
                 .arg(qobj->metaObject()->className())
                 .arg(qobj->objectName()));

      const QMetaObject *meta = qobj->metaObject();

      if (meta->propertyCount() > QObject::staticMetaObject.propertyCount()) {
        res.append(tr("<h3>%1 Properties</h3>\n").arg(itemName));
        res.append(tableHeader());

        for (int i=QObject::staticMetaObject.propertyCount();
             i<meta->propertyCount(); i++) {
          const char* propName = meta->property(i).name();
          QVariant val = qobj->property(propName);

          if ((i%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td><td>%2</td><td>%3</td>\n")
                     .arg(prefix+propName)
                     .arg(val.typeName())
                     .arg(val.toString()));

          QString doc = QcepDocumentationDictionary::get_Doc(prefix+propName);

          res.append(tr("<td width=\"75%\">%1</td>\n").arg(doc));

          res.append("</tr>\n");
        }

        res.append(tableFooter());
      }

      if (qobj->dynamicPropertyNames().count()) {
        res.append(tr("<h3>%1 User Defined Properties</h3>\n").arg(itemName));
        res.append(tableHeader());

        QList<QByteArray> props = qobj->dynamicPropertyNames();

        for (int i=0; i<props.count(); i++) {
          QString propName = props[i];
          QVariant val = qobj->property(qPrintable(propName));

          if ((i%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }

          res.append(tr("<td>%1</td><td>%2</td><td>%3</td>\n")
                     .arg(prefix+propName)
                     .arg(val.typeName())
                     .arg(val.toString()));

          QString doc = QcepDocumentationDictionary::get_Doc(prefix+propName);

          res.append(tr("<td width=\"75%\">%1</td>\n").arg(doc));

          res.append("</tr>\n");
        }

        res.append(tableFooter());
      }

      if (meta->methodCount() > QObject::staticMetaObject.methodCount()) {
        res.append(tr("<h3>%1 Functions</h3>\n").arg(itemName));
        res.append(tableHeader());

        for (int i=QObject::staticMetaObject.methodCount();
             i<meta->methodCount(); i++) {
          const char* methodSig = meta->method(i).signature();
          QMetaMethod::MethodType methodType = meta->method(i).methodType();

          if ((i%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td><i>%1</i></td>\n").arg(prefix+methodSig));

          QString proto = QcepDocumentationDictionary::get_Proto(methodSig);
          QString doc   = QcepDocumentationDictionary::get_Doc(methodSig);

          res.append("<td>");
          switch (methodType) {
          case QMetaMethod::Signal:
            res.append("<i>Signal</i><br/>\n");
            break;
          case QMetaMethod::Slot:
            res.append("<i>Slot</i><br/>\n");
            break;
          default:
            break;
          }

          res.append(tr("%1<br/>\n").arg(proto));
          res.append(tr("%1</td>\n").arg(doc));

          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
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

        res.append(tr("<h3>%1 Objects</h3>\n").arg(itemName));
        res.append(tableHeader());

        int i=0;

        while (obj_iter.hasNext()) {
          obj_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(item, obj_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }

      if (properties.count()) {
        QMapIterator<QString,QScriptValue> prop_iter(properties);

        res.append(tr("<h3>%1 Properties</h3>\n").arg(itemName));
        res.append(tableHeader());

        int i=0;

        while (prop_iter.hasNext()) {
          prop_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(item, prop_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }

      if (functions.count()) {
        QMapIterator<QString,QScriptValue> func_iter(functions);

        res.append(tr("<h3>%1 Functions</h3>\n").arg(itemName));
        res.append(tableHeader());

        int i=0;

        while (func_iter.hasNext()) {
          func_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(item, func_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }
    }
  }

  return res;
}

QString QxrdScriptEngine::documentationText(QRegExp pattern)
{
  QString res="";

  QHashIterator<QString, QString> iter(QcepDocumentationDictionary::docs());

  while (iter.hasNext()) {
    iter.next();
    if (pattern.exactMatch(iter.key())) {
      res.append(documentationText(iter.key()));
    }
  }

  return res;
}

QByteArray QxrdScriptEngine::helpText(QString item)
{
  QString scriptItem;

  scriptItem = item;

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

void QxrdScriptEngine::dumpLocks()
{
  QcepMutexLocker::dumpLocks();
}
