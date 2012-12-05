#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qcepmacros.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"
#include "qxrddataprocessorthread.h"
#include "qxrddataprocessor.h"
#include "qxrdwindow.h"
#include "qxrdacquisitionthread.h"
#include "qxrdacquisition.h"
#include "qxrdserverthread.h"
#include "qxrdserver.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdsimpleserver.h"
#include "qxrdscriptenginethread.h"
#include "qxrdscriptengine.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdexperimentsettings.h"
#include <QFileDialog>
#include "qxrdfilesaverthread.h"
#include "qxrdsettingssaver.h"
#include "qxrdfilesaver.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisition-ptr.h"

QxrdExperiment::QxrdExperiment(
    QString path,
    QxrdApplicationWPtr app) :
  QObject(NULL),
  m_ObjectNamer(this, "experiment"),
  m_Application(app),
  m_ExperimentThread(),
  m_SettingsSaver(new QxrdSettingsSaver(this)),
  m_WindowSettings(NULL),
  m_Window(),
  m_ServerThread(NULL),
  m_Server(),
  m_SimpleServerThread(NULL),
  m_SimpleServer(),
  m_DataProcessorThread(NULL),
  m_DataProcessor(),
  m_AcquisitionThread(NULL),
  m_Acquisition(),
  m_FileSaverThread(NULL),
  m_FileSaver(),
  m_ScriptEngineThread(NULL),
  m_ScriptEngine(),
  m_ScriptEngineDebugger(NULL),
  m_LogFile(NULL),
  m_ScanFile(NULL),
  m_ExperimentFileMutex(),

  m_ExperimentKind(m_SettingsSaver, this, "experimentKind", -1, "Kind of Experiment"),
  m_ExperimentDirectory(m_SettingsSaver, this, "experimentDirectory", defaultExperimentDirectory(path), "Experiment Directory"),
  m_ExperimentFileName(m_SettingsSaver, this, "experimentFileName", defaultExperimentFileName(path), "Experiment File"),
  m_ExperimentName(m_SettingsSaver, this, "experimentName", defaultExperimentName(path), "Experiment Name"),
  m_ExperimentDescription(m_SettingsSaver, this, "experimentDescription", "", "Experiment Description"),
  m_LogFileName(m_SettingsSaver, this, "logFileName", defaultLogName(path), "Log File Name"),
  m_ScanFileName(m_SettingsSaver, this, "scanFileName", defaultScanName(path), "Scan File Name"),
  m_DetectorType(m_SettingsSaver, this,"detectorType", 0, "Detector Type"),
  m_DetectorTypeName(QxrdSettingsSaverPtr(), this,"detectorTypeName", QxrdDetectorThread::detectorTypeName(get_DetectorType()), "Name of Detector Type"),
  m_ProcessorType(m_SettingsSaver, this,"processorType", 0, "Data Processor Type"),
  m_DefaultLayout(QxrdSettingsSaverWPtr(), this,"defaultLayout",0, "Default Layout Used?"),
  m_WorkCompleted(QxrdSettingsSaverWPtr(), this, "workCompleted", 0, "Amount of Work Completed"),
  m_WorkTarget(QxrdSettingsSaverWPtr(), this, "workTarget", 0, "Amount of Work Targetted"),
  m_CompletionPercentage(QxrdSettingsSaverWPtr(), this, "completionPercentage", 0, "Percentage of Work Completed")
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperiment::QxrdExperiment(%p)\n", this);
  }
}

void QxrdExperiment::initialize(QxrdExperimentThreadWPtr expthrd, QSettings *settings)
{
  m_ExperimentThread = expthrd;

  QxrdApplicationPtr app(m_Application);

  if (app) {
    splashMessage("Initializing File Saver");

    m_FileSaverThread = QxrdFileSaverThreadPtr(
          new QxrdFileSaverThread(app->allocator()));
    m_FileSaverThread -> setObjectName("saver");
    m_FileSaverThread -> start();
    m_FileSaver = m_FileSaverThread -> fileSaver();

    splashMessage("Initializing Data Processing");

    m_DataProcessorThread = QxrdDataProcessorThreadPtr(
          new QxrdDataProcessorThread(m_SettingsSaver,
                                      QxrdExperimentWPtr(this),
                                      QxrdAcquisitionPtr(),
                                      app->allocator(),
                                      m_FileSaver));
    m_DataProcessorThread -> setObjectName("proc");
    m_DataProcessorThread -> start();
    m_DataProcessor = m_DataProcessorThread -> dataProcessor();

    QxrdFileSaverPtr saver(m_FileSaver);

    if (saver) {
      saver -> setProcessor(m_DataProcessor);
    }

    splashMessage("Initializing Data Acquisition");

    m_AcquisitionThread = QxrdAcquisitionThreadPtr(
          new QxrdAcquisitionThread(m_SettingsSaver,
                                    QxrdExperimentWPtr(this),
                                    m_DataProcessor,
                                    app->allocator(),
                                    get_DetectorType()));
    m_AcquisitionThread -> setObjectName("acqu");
    m_AcquisitionThread -> start();
    m_Acquisition = m_AcquisitionThread -> acquisition();

    QxrdDataProcessorPtr proc(m_DataProcessor);

    if (proc) {
      proc -> setAcquisition(m_Acquisition);
    }

    if (saver) {
      saver -> setAcquisition(m_Acquisition);
    }

    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq) {
      acq -> setNIDAQPlugin(app->nidaqPlugin());
    }

    m_WindowSettings = QxrdWindowSettingsPtr(new QxrdWindowSettings(m_SettingsSaver, NULL));

    splashMessage("Starting SPEC Server");

    m_ServerThread = QxrdServerThreadPtr(
          new QxrdServerThread(m_SettingsSaver, this, "qxrd"));
    m_ServerThread -> setObjectName("server");
    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    splashMessage("Starting Simple Socket Server");

    m_SimpleServerThread = QxrdSimpleServerThreadPtr(
          new QxrdSimpleServerThread(m_SettingsSaver, this, "simpleserver"));
    m_SimpleServerThread -> setObjectName("smpsrv");
    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    m_ScriptEngineThread = QxrdScriptEngineThreadPtr(
          new QxrdScriptEngineThread(m_Application, this));
    m_ScriptEngineThread -> setObjectName("script");
    m_ScriptEngineThread -> start();
    m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();


    QxrdServerPtr srv(m_Server);
    QxrdScriptEnginePtr eng(m_ScriptEngine);

//    if (eng) {
//      m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
//      m_ScriptEngineDebugger -> attachTo(eng.data());
//      m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);
//    }

    if (srv && eng) {
      connect(srv.data(),   SIGNAL(executeCommand(QString)),
              eng.data(),   SLOT(evaluateSpecCommand(QString)));

      connect(eng.data(),   SIGNAL(specResultAvailable(QScriptValue)),
              srv.data(),   SLOT(finishedCommand(QScriptValue)));
    }

    QxrdSimpleServerPtr ssrv(m_SimpleServer);

    if (ssrv && eng) {
      connect(ssrv.data(),  SIGNAL(executeCommand(QString)),
              eng.data(),   SLOT(evaluateSimpleServerCommand(QString)));

      connect(eng.data(),   SIGNAL(simpleServerResultAvailable(QScriptValue)),
              ssrv.data(),  SLOT(finishedCommand(QScriptValue)));
    }

    connect(prop_WorkCompleted(), SIGNAL(valueChanged(int,int)), this, SLOT(updateCompletionPercentage(int,int)));
    connect(prop_WorkTarget(),    SIGNAL(valueChanged(int,int)), this, SLOT(updateCompletionPercentage(int,int)));
    connect(prop_DetectorType(),  SIGNAL(valueChanged(int,int)), this, SLOT(onDetectorTypeChanged()));

    splashMessage("Loading Preferences");

    readSettings(settings);

    splashMessage("Loading Background Images");

    if (proc) {
      proc -> loadDefaultImages();
    }

#ifdef Q_OS_WIN32
    QDir::setCurrent(QDir::homePath());
#endif

    printMessage(tr("Current directory %1").arg(QDir::currentPath()));

    m_SettingsSaver->start();
  }
}

QxrdExperimentThreadWPtr QxrdExperiment::experimentThread()
{
  return m_ExperimentThread;
}

QxrdSettingsSaverPtr QxrdExperiment::settingsSaver()
{
  return m_SettingsSaver;
}

void QxrdExperiment::openWindows()
{
  GUI_THREAD_CHECK;

  QxrdApplicationPtr app(m_Application);

  if (app && app->get_GuiWanted()) {
    splashMessage("Opening Main Window");
    m_Window = QxrdWindowPtr(
          new QxrdWindow(m_WindowSettings,
                         m_Application,
                         this,
                         m_Acquisition,
                         m_DataProcessor,
                         app->allocator(),
                         NULL));

    QxrdWindowPtr win = m_Window;
    QxrdDataProcessorPtr proc(m_DataProcessor);
    QxrdAcquisitionPtr acq(m_Acquisition);
    QxrdScriptEnginePtr eng(m_ScriptEngine);

    if (win) {
      win -> initialize();

      if (proc) {
        proc -> setWindow(win);
      }

      if (acq) {
        acq -> setWindow(win);
      }

      if (eng) {
        eng -> setWindow(win);
      }

      win -> onAcquisitionInit();

      if (eng) {
        connect(win.data(),   SIGNAL(executeCommand(QString)),
                eng.data(),   SLOT(evaluateAppCommand(QString)));

        connect(eng.data(),   SIGNAL(appResultAvailable(QScriptValue)),
                win.data(),   SLOT(finishedCommand(QScriptValue)));
      }

      if (win && app && app->get_GuiWanted()) {
        win -> show();
      }
    }
  }
}

QxrdExperiment::~QxrdExperiment()
{
  QxrdApplicationPtr app(m_Application);

  if (app && qcepDebug(DEBUG_APP)) {
    app->printMessage("QxrdExperiment::~QxrdExperiment");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperiment::~QxrdExperiment(%p)\n", this);
  }

  m_SettingsSaver->performSave();

//  delete m_Window;

  closeScanFile();
  closeLogFile();
}

void QxrdExperiment::splashMessage(QString msg)
{
  QxrdApplicationPtr app(m_Application);

  if (app) {
    app->splashMessage(msg);
  }
}

void QxrdExperiment::criticalMessage(QString msg)
{
  QxrdApplicationPtr app(m_Application);
  QxrdWindowPtr      win(m_Window);

  if (win) {
    win->displayCriticalMessage(msg);
  } else if (app) {
    app->criticalMessage(msg);
  } else {
    printf("%s\n", qPrintable(msg));
  }
}

void QxrdExperiment::statusMessage(QString msg)
{
  QxrdApplicationPtr app(m_Application);
  QxrdWindowPtr      win(m_Window);

  if (win) {
    win->displayStatusMessage(msg);
  } else if (app) {
    app->statusMessage(msg);
  } else {
    printf("%s\n", qPrintable(msg));
  }
}

void QxrdExperiment::printMessage(QString msg, QDateTime ts)
{
  if (qcepDebug(DEBUG_NOMESSAGES)) {
  } else {
    QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+
        QThread::currentThread()->objectName()+": "+
        msg.trimmed();

    message = message.replace("\n", " : ");

    logMessage(message);

    QxrdWindowPtr win = m_Window;

    if (win) {
      win->displayMessage(message);
//    } else if (m_Application) {
//      m_Application->printMessage(message);
    } else {
//      printf("%s\n", qPrintable(message));
    }
  }
}

QxrdWindowPtr QxrdExperiment::window()
{
  return m_Window;
}

QxrdAcquisitionThreadPtr QxrdExperiment::acquisitionThread()
{
  return m_AcquisitionThread;
}

QxrdAcquisitionWPtr QxrdExperiment::acquisition() const
{
  return m_Acquisition;
}

QxrdServerWPtr QxrdExperiment::specServer()
{
  return m_Server;
}

QxrdSimpleServerWPtr QxrdExperiment::simpleServer()
{
  return m_SimpleServer;
}

QxrdDataProcessorWPtr QxrdExperiment::dataProcessor() const
{
  return m_DataProcessor;
}

QxrdScriptEngineWPtr QxrdExperiment::scriptEngine()
{
  return m_ScriptEngine;
}

void QxrdExperiment::executeCommand(QString cmd)
{
  QxrdScriptEnginePtr eng(m_ScriptEngine);

  if (eng) {
    eng->evaluateAppCommand(cmd);
  }
}

void QxrdExperiment::newLogFile(QString path)
{
  {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

    if (m_LogFile) {
      fclose(m_LogFile);
      m_LogFile = NULL;
    }
  }

  set_LogFileName(path);

  openLogFile();
}

void QxrdExperiment::openLogFile()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  if (m_LogFile == NULL) {
    m_LogFile = fopen(qPrintable(logFilePath()), "a");

    if (m_LogFile) {
      fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFileName()));
      fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
      fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
      fflush(m_LogFile);
    }
  }
}

FILE* QxrdExperiment::logFile()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  return m_LogFile;
}

void QxrdExperiment::logMessage(QString msg)
{
  openLogFile();

  QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdExperiment::closeLogFile()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s ------- shutdown --------\n",
            qPrintable(QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ")));

    fclose(m_LogFile);
    m_LogFile = NULL;
  }
}

void QxrdExperiment::newScanFile(QString path)
{
  {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_ScanFileMutex);

    if (m_ScanFile) {
      fclose(m_ScanFile);
      m_ScanFile = NULL;
    }
  }

  set_ScanFileName(path);

  openScanFile();
}

void QxrdExperiment::openScanFile()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_ScanFileMutex);

  if (m_ScanFile == NULL) {
    m_ScanFile = fopen(qPrintable(scanFilePath()), "a");

    if (m_ScanFile) {
      fprintf(m_ScanFile, "#F %s\n", qPrintable(get_LogFileName()));
      fprintf(m_ScanFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
      fprintf(m_ScanFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
      fflush(m_ScanFile);
    }
  }
}

FILE* QxrdExperiment::scanFile()
{
  return m_ScanFile;
}

void QxrdExperiment::closeScanFile()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_ScanFileMutex);

  if (m_ScanFile) {
    fclose(m_ScanFile);
    m_ScanFile = NULL;
  }
}

void QxrdExperiment::readSettings()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

  QString docPath = experimentFilePath();

  if (docPath.length()>0) {
    QSettings settings(docPath, QSettings::IniFormat);

    readSettings(&settings);
  } else {
    QxrdExperimentSettings settings;

    readSettings(&settings);
  }
}

void QxrdExperiment::readSettings(QSettings *settings)
{
  if (settings) {
    QcepProperty::readSettings(this, &staticMetaObject, "experiment", settings);

    QxrdAcquisitionPtr acq(m_Acquisition);
    QxrdDataProcessorPtr proc(m_DataProcessor);
    QxrdServerPtr srv(m_Server);
    QxrdSimpleServerPtr ssrv(m_SimpleServer);

    m_WindowSettings -> readSettings(settings, "window");

    if (acq) {
      acq  -> readSettings(settings, "acquisition");
    }

    if (proc) {
      proc -> readSettings(settings, "processor");
    }

    if (srv) {
      srv  -> readSettings(settings, "specserver");
    }

    if (ssrv) {
      ssrv -> readSettings(settings, "simpleserver");
    }

    onDetectorTypeChanged();
  }
}

void QxrdExperiment::writeSettings()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

  QString docPath = experimentFilePath();

  if (docPath.length()>0) {
    QFile::remove(docPath+".new");

    {
      QSettings settings(docPath+".new", QSettings::IniFormat);

      writeSettings(&settings);
    }

    QFile::remove(docPath+".bak");
    QFile::rename(docPath, docPath+".bak");
    QFile::rename(docPath+".new", docPath);
  } else {
    QxrdExperimentSettings settings;

    writeSettings(&settings);
  }
}

void QxrdExperiment::writeSettings(QSettings *settings)
{
  if (settings) {
    QcepProperty::writeSettings(this, &staticMetaObject, "experiment", settings);

    QxrdAcquisitionPtr acq(m_Acquisition);
    QxrdDataProcessorPtr proc(m_DataProcessor);
    QxrdServerPtr srv(m_Server);
    QxrdSimpleServerPtr ssrv(m_SimpleServer);

    m_WindowSettings -> writeSettings(settings, "window");

    if (acq) {
      acq  -> writeSettings(settings, "acquisition");
    }

    if (proc) {
      proc -> writeSettings(settings, "processor");
    }

    if (srv) {
      srv  -> writeSettings(settings, "specserver");
    }

    if (ssrv) {
      ssrv -> writeSettings(settings, "simpleserver");
    }
  }
}

QString QxrdExperiment::defaultExperimentDirectory(QString path)
{
  QFileInfo info(path);

  QString directory = info.dir().canonicalPath();

  return directory;
}

QString QxrdExperiment::defaultExperimentFileName(QString path)
{
  QFileInfo info(path);

  if (info.suffix() == "qxrdp") {
    return info.fileName();
  } else {
    return info.fileName()+".qxrdp";
  }
}

QString QxrdExperiment::defaultExperimentName(QString path)
{
  QFileInfo info(path);

  if (info.suffix() == "qxrdp") {
    return info.completeBaseName();
  } else {
    return info.fileName();
  }
}

QString QxrdExperiment::defaultLogName(QString path)
{
  return defaultExperimentName(path)+".log";
}

QString QxrdExperiment::defaultScanName(QString path)
{
  return defaultExperimentName(path)+".scans";
}

QString QxrdExperiment::experimentFilePath()
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_ExperimentFileName());
}

void QxrdExperiment::setExperimentFilePath(QString path)
{
  set_ExperimentDirectory(defaultExperimentDirectory(path));
  set_ExperimentFileName(defaultExperimentFileName(path));
  set_ExperimentName(defaultExperimentName(path));
  set_LogFileName(defaultLogName(path));
  set_ScanFileName(defaultScanName(path));

  QxrdWindowPtr win = m_Window;

  if (win) {
    win -> updateTitle();
  }

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage(tr("setExperimentFilePath %1").arg(path));
    printMessage(tr("  experimentDirectory: %1").arg(get_ExperimentDirectory()));
    printMessage(tr("  experimentFileName: %1").arg(get_ExperimentFileName()));
    printMessage(tr("  experimentName: %1").arg(get_ExperimentName()));
    printMessage(tr("  logFilePath: %1").arg(get_LogFileName()));
    printMessage(tr("  scanFilePath: %1").arg(get_ScanFileName()));
  }

  newLogFile(get_LogFileName());
  newScanFile(get_ScanFileName());
}

QString QxrdExperiment::logFilePath()
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_LogFileName());
}

QString QxrdExperiment::scanFilePath()
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_ScanFileName());
}

void QxrdExperiment::saveExperiment()
{
  writeSettings();
}

void QxrdExperiment::saveExperimentAs(QString path)
{
  QxrdExperimentSettings settings(path);

  setExperimentFilePath(path);

  writeSettings(&settings);
}

void QxrdExperiment::saveExperimentCopyAs(QString path)
{
  printMessage(tr("Save experiment copy as %1").arg(path));

  QxrdExperimentSettings settings(path);

  writeSettings(&settings);

//  QxrdExperiment *exp = new QxrdExperiment(path, m_Application, &settings);

//  exp -> setExperimentFilePath(path);

//  exp -> writeSettings(&settings);

//  delete exp;
}

void QxrdExperiment::updateCompletionPercentage(int, int)
{
  int completed = get_WorkCompleted();
  int target    = get_WorkTarget();

  if (target > 0) {
    set_CompletionPercentage(100.0*completed/target);
  } else {
    set_CompletionPercentage(0);
  }
}

void QxrdExperiment::completeWork(int amt)
{
  prop_WorkCompleted()->incValue(amt);
}

void QxrdExperiment::commenceWork(int amt)
{
  prop_WorkTarget()->incValue(amt);
}

void QxrdExperiment::finishedWork(int amt)
{
  prop_WorkCompleted()->incValue(-amt);
  prop_WorkTarget()->incValue(-amt);
}

void QxrdExperiment::onDetectorTypeChanged()
{
  int newType = get_DetectorType();

  if (qcepDebug(DEBUG_ACQUIRE)) {
    printMessage(tr("Detector type changed to %1").arg(newType));
  }

  m_Detector       = QxrdDetectorPtr();
  m_DetectorThread = QxrdDetectorThreadPtr();

  m_DetectorThread = QxrdDetectorThreadPtr(new QxrdDetectorThread(this, m_Acquisition));
  m_DetectorThread -> start();

  if (m_DetectorThread) {
    m_Detector = m_DetectorThread->detector();
  }

  QxrdDetectorPtr det(m_Detector);

  if (det) {
    set_DetectorType(det->detectorType());
    set_DetectorTypeName(det->detectorTypeName());

    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq) {
      acq->setDetector(det);
    }
  }
}

void QxrdExperiment::dump()
{
  printf("About to dump\n");
  dumpObjectInfo();
  dumpObjectTree();
  printf("Dumped\n");
}
