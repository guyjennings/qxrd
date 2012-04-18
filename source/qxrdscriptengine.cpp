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
#include <QRegExp>

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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

QXRD_DOC_FUNCTION(
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

//QScriptValue QxrdScriptEngine::typeNameFunc(QScriptContext *context, QScriptEngine *engine)
//{
//  if (context->argumentCount() != 0) {
//    return QScriptValue(engine, QMetaType::typeName(context->argument(0).toUInt32()));
//  } else {
//    return QScriptValue(engine,"?");
//  }
//}

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

QXRD_DOC_FUNCTION(
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
  globalObject().setProperty("help", newFunction(helpFunc));
  globalObject().setProperty("process", newFunction(processFunc));
//  globalObject().setProperty("typeName", newFunction(typeNameFunc));
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
        QXRD_DOC_OBJECT("triggering", "Acquisition Triggering Parameters");
        globalObject().setProperty("triggering", newQObject(trig.data()));
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
  QString res = tr("<a href=\"qrc:/help/autohelp?%1\">%2</a>").arg(item).arg(item);

  QString proto = QxrdDocumentationDictionary::get_Proto(item);
  QString doc   = QxrdDocumentationDictionary::get_Doc(item);

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
  QString proto = QxrdDocumentationDictionary::get_Proto(item);
  QString longDoc = QxrdDocumentationDictionary::get_LongDoc(item);

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
        res.append(tr("<h3>Properties of %1</h3>\n").arg(itemName));

        for (int i=QObject::staticMetaObject.propertyCount();
             i<meta->propertyCount(); i++) {
          const char* propName = meta->property(i).name();
          QVariant val = qobj->property(propName);

          res.append("<div style=\"background-color:#e0e0e0;\">\n");
          res.append(tr("<p><i>%1</i> : %2 = %3</p>\n")
                     .arg(prefix+propName)
                     .arg(val.typeName())
                     .arg(val.toString()));
          res.append(tr("</div>\n"));

          QString doc = QxrdDocumentationDictionary::get_Doc(prefix+propName);

          if (doc.length()) {
            res.append(tr("<div style=\"margin-left:20px\">\n"));
            res.append(tr("<p>%1</p>\n").arg(doc));
            res.append(tr("</div>\n"));
          }
        }
      }

      if (meta->methodCount() > QObject::staticMetaObject.methodCount()) {
        res.append(tr("<h3>Methods of %1</h3>\n").arg(itemName));

        for (int i=QObject::staticMetaObject.methodCount();
             i<meta->methodCount(); i++) {
          const char* methodSig = meta->method(i).signature();

          res.append(tr("<dl>\n"));
          res.append(tr("<dt><i>%1</i></dt>\n").arg(prefix+methodSig));

          QString proto = QxrdDocumentationDictionary::get_Proto(prefix+methodSig);
          QString doc   = QxrdDocumentationDictionary::get_Doc(prefix+methodSig);

          if (proto.length()) {
            res.append(tr("<dd><p>%1</p></dd>\n").arg(proto));
          }

          if (doc.length()) {
            res.append(tr("<dd>%1</dd>\n").arg(doc));
          }

          res.append(tr("</dl>\n"));
        }
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
        res.append(tableHeader());

        int i=0;

        while (obj_iter.hasNext()) {
          obj_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(prefix+item, obj_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }

      if (properties.count()) {
        QMapIterator<QString,QScriptValue> prop_iter(properties);

        res.append(tr("<h3>Properties of %1</h3>\n").arg(itemName));
        res.append(tableHeader());

        int i=0;

        while (prop_iter.hasNext()) {
          prop_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(prefix+item, prop_iter.key())));
          res.append(tr("</tr>\n"));
        }

        res.append(tableFooter());
      }

      if (functions.count()) {
        QMapIterator<QString,QScriptValue> func_iter(functions);

        res.append(tr("<h3>Methods of %1</h3>\n").arg(itemName));
        res.append(tableHeader());

        int i=0;

        while (func_iter.hasNext()) {
          func_iter.next();

          if ((i++%2)) {
            res.append(tr("<tr bgcolor=\"#e0e0e0\">\n"));
          } else {
            res.append(tr("<tr bgcolor=\"white\">\n"));
          }
          res.append(tr("<td>%1</td>").arg(documentationLink(prefix+item, func_iter.key())));
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

  QHashIterator<QString, QString> iter(QxrdDocumentationDictionary::docs());

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
