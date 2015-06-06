#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qcepmacros.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"
#include "qxrddataprocessorthread.h"
#include "qxrddataprocessor.h"
#include "qxrdcalibrantlibrary.h"
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
#include "qcepdataset.h"
#include "qcepdatagroup-ptr.h"
#include <QHostInfo>
#include <QColorDialog>

QxrdExperiment::QxrdExperiment(
    QString path,
    QxrdApplicationWPtr app) :
  QcepExperiment("experiment", NULL),
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
  m_ExperimentDirectory(QxrdSettingsSaverPtr()/*m_SettingsSaver*/, this, "experimentDirectory", defaultExperimentDirectory(path), "Experiment Directory"),
  m_ExperimentFileName(QxrdSettingsSaverPtr()/*m_SettingsSaver*/, this, "experimentFileName", defaultExperimentFileName(path), "Experiment File"),
  m_ExperimentName(QxrdSettingsSaverPtr()/*m_SettingsSaver*/, this, "experimentName", defaultExperimentName(path), "Experiment Name"),
  m_ExperimentDescription(m_SettingsSaver, this, "experimentDescription", "", "Experiment Description"),
  m_DataDirectory(m_SettingsSaver, this, "dataDirectory", defaultDataDirectory(path), "Saved Data Directory"),
  m_LogFileName(m_SettingsSaver, this, "logFileName", defaultLogName(path), "Log File Name"),
  m_ScanFileName(m_SettingsSaver, this, "scanFileName", defaultScanName(path), "Scan File Name"),
  m_ScanFileExtension(m_SettingsSaver, this, "scanFileExtension", ".avg", "Scan File Extension"),
  m_ScanDataNegative(m_SettingsSaver, this, "scanDataNegative", 0, "Scan Data Negative Value Handling"),
  m_DetectorType(m_SettingsSaver, this, "detectorType", 0, "Detector Type"),
  m_DetectorTypeName(QxrdSettingsSaverPtr(), this,"detectorTypeName", QxrdDetectorThread::detectorTypeName(get_DetectorType()), "Name of Detector Type"),
  m_DetectorNumber(m_SettingsSaver, this, "detectorNumber", 0, "Detector Number"),
  m_DetectorSubType(m_SettingsSaver, this, "detectorSubType", 0, "Detector Sub Type"),
  m_DetectorAddress(m_SettingsSaver, this, "detectorAddress", "", "Detector Address"),
  m_ProcessorType(m_SettingsSaver, this,"processorType", 0, "Data Processor Type"),
  m_DefaultLayout(QxrdSettingsSaverWPtr(), this,"defaultLayout",0, "Default Layout Used?"),
  m_WorkCompleted(QxrdSettingsSaverWPtr(), this, "workCompleted", 0, "Amount of Work Completed"),
  m_WorkTarget(QxrdSettingsSaverWPtr(), this, "workTarget", 0, "Amount of Work Targetted"),
  m_CompletionPercentage(QxrdSettingsSaverWPtr(), this, "completionPercentage", 0, "Percentage of Work Completed"),
  m_DefaultScript(m_SettingsSaver, this, "defaultScript", "", "Default script for experiment"),
  m_ExtraScriptFiles(m_SettingsSaver, this, "extraScriptFiles", QStringList(), "Additional script files for experiment"),
  m_FontSize(m_SettingsSaver, this, "fontSize", -1, "Suggested font size"),
  m_Spacing(m_SettingsSaver, this, "spacing", -1, "Suggested widget spacing")
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperiment::QxrdExperiment(%p)\n", this);
  }
}

void QxrdExperiment::initialize(QxrdExperimentThreadWPtr expthrd, QxrdExperimentWPtr exp, QSettings *settings)
{
  m_ExperimentThread = expthrd;
  m_Experiment       = exp;

  QxrdApplicationPtr app(m_Application);

  if (app) {
    QThread::currentThread()->setObjectName("experiment");

    splashMessage("Initializing File Saver");

    m_FileSaverThread = QxrdFileSaverThreadPtr(
          new QxrdFileSaverThread(app->allocator()));
    m_FileSaverThread -> setObjectName("saver");
    m_FileSaverThread -> start();
    m_FileSaver = m_FileSaverThread -> fileSaver();

    splashMessage("Initializing Data Processing");

    m_DataProcessorThread = QxrdDataProcessorThreadPtr(
          new QxrdDataProcessorThread(m_SettingsSaver,
                                      m_Experiment,
                                      QxrdAcquisitionPtr(),
                                      app->allocator(),
                                      m_FileSaver));
    m_DataProcessorThread -> setObjectName("proc");
    m_DataProcessorThread -> start();
    m_DataProcessor = m_DataProcessorThread -> dataProcessor();

    QxrdFileSaverPtr saver(m_FileSaver);

    if (saver) {
      saver -> setProcessor(m_DataProcessor);
      saver -> setExperiment(m_Experiment);
    }

    splashMessage("Initializing Data Acquisition");

    m_AcquisitionThread = QxrdAcquisitionThreadPtr(
          new QxrdAcquisitionThread(m_SettingsSaver,
                                    m_Experiment,
                                    m_DataProcessor,
                                    app->allocator(),
                                    get_DetectorType()));
    m_AcquisitionThread -> setObjectName("acqu");
    m_AcquisitionThread -> start();
    m_Acquisition = m_AcquisitionThread -> acquisition();

    m_CalibrantLibrary = QxrdCalibrantLibraryPtr(
          new QxrdCalibrantLibrary(m_SettingsSaver, m_Experiment));

    m_CalibrantLibrary -> initialize(m_CalibrantLibrary);

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

    m_Dataset = QcepDatasetPtr(new QcepDataset(m_SettingsSaver, "dataset", QcepDatasetPtr()));

    m_Dataset->append(QcepDataGroupPtr(new QcepDataGroup(m_SettingsSaver, "group1", m_Dataset)));
    m_Dataset->append(QcepDataGroupPtr(new QcepDataGroup(m_SettingsSaver, "group2", m_Dataset)));
    m_Dataset->append(QcepDataGroupPtr(new QcepDataGroup(m_SettingsSaver, "group3", m_Dataset)));

    m_WindowSettings = QxrdWindowSettingsPtr(new QxrdWindowSettings(m_SettingsSaver, NULL));

    splashMessage("Starting SPEC Server");

    m_ServerThread = QxrdServerThreadPtr(
          new QxrdServerThread(m_SettingsSaver, m_Experiment, "qxrd"));
    m_ServerThread -> setObjectName("server");
    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    splashMessage("Starting Simple Socket Server");

    m_SimpleServerThread = QxrdSimpleServerThreadPtr(
          new QxrdSimpleServerThread(m_SettingsSaver, m_Experiment, "simpleserver"));
    m_SimpleServerThread -> setObjectName("smpsrv");
    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    m_ScriptEngineThread = QxrdScriptEngineThreadPtr(
          new QxrdScriptEngineThread(m_Application, m_Experiment));
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

    splashMessage("Loading Preferences");

    readSettings(settings, "experiment");

    splashMessage("Loading Background Images");

    if (proc) {
      proc -> loadDefaultImages();
    }

#ifdef Q_OS_WIN32
    QDir::setCurrent(QDir::homePath());
#endif

    printMessage(tr("------- Starting new session -------"));
    printMessage(tr("Experiment name: %1").arg(get_ExperimentName()));
    printMessage(tr("Experiment Directory: %1").arg(get_ExperimentDirectory()));
    printMessage(tr("Stored in file: %1").arg(get_ExperimentFileName()));

    QString about = STR(QXRD_VERSION);

    if (sizeof(void*) == 4) {
      about += " - 32 Bit";
    } else {
      about += " - 64 Bit";
    }

#ifdef Q_CC_MSVC
    about += " MSVC";
#endif

#ifdef Q_CC_GNU
    about += " gcc";
#endif

#ifdef QT_NO_DEBUG
    about += " Release";
#else
    about += " Debug";
#endif

#ifdef Q_OS_MAC
    about += " : Macintosh";
#elif defined(Q_OS_WIN32)
    about += " : Windows";
#elif defined(Q_OS_UNIX)
    about += " : Unix";
#endif

    printMessage(tr("Qxrd Version : %1").arg(about));
    printMessage(tr("Qt Version %1").arg(qVersion()));
    printMessage(tr("Qceplib Version %1").arg(STR(QCEPLIB_VERSION)));
    printMessage(tr("QWT Version %1").arg(STR(QCEPLIB_QWT_VERSION)));
    printMessage(tr("Mar345 Version %1").arg(STR(QCEPLIB_MAR345_VERSION)));
    printMessage(tr("CBF Version %1").arg(STR(QCEPLIB_CBF_VERSION)));
    printMessage(tr("TIFF Version %1").arg(STR(QCEPLIB_TIFF_VERSION)));
    printMessage(tr("LevMar Version %1").arg(STR(QCEPLIB_LEVMAR_VERSION)));
#ifdef QCEPLIB_HDF5_VERSION
    printMessage(tr("HDF5 Version %1").arg(STR(QCEPLIB_HDF5_VERSION)));
#endif
    printMessage(tr("Spec Server Version %1").arg(STR(QCEPLIB_SPECSERVER_VERSION)));
    printMessage(tr("Running on host %1").arg(QHostInfo::localHostName()));
    printMessage(tr("Current directory %1").arg(QDir::currentPath()));

    connect(prop_WorkCompleted(),   SIGNAL(valueChanged(int,int)), this, SLOT(updateCompletionPercentage(int,int)));
    connect(prop_WorkTarget(),      SIGNAL(valueChanged(int,int)), this, SLOT(updateCompletionPercentage(int,int)));
    connect(prop_DetectorType(),    SIGNAL(valueChanged(int,int)), this, SLOT(onDetectorTypeChanged()));
    connect(prop_DetectorSubType(), SIGNAL(valueChanged(int,int)), this, SLOT(onDetectorTypeChanged()));
    connect(prop_DetectorNumber(),  SIGNAL(valueChanged(int,int)), this, SLOT(onDetectorTypeChanged()));
    connect(prop_DetectorAddress(), SIGNAL(valueChanged(QString,int)), this, SLOT(onDetectorTypeChanged()));

    m_SettingsSaver->start();

    onDetectorTypeChanged();
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
                         m_Experiment,
                         m_Acquisition,
                         m_DataProcessor,
                         app->allocator(),
                         NULL));

    QxrdDataProcessorPtr proc(m_DataProcessor);
    QxrdAcquisitionPtr acq(m_Acquisition);
    QxrdScriptEnginePtr eng(m_ScriptEngine);

    if (m_Window) {
      m_Window -> initialize(m_Window);

      if (proc) {
        proc -> setWindow(m_Window);
      }

      if (acq) {
        acq -> setWindow(m_Window);
      }

      if (eng) {
        eng -> setWindow(m_Window);
      }

      m_Window -> onAcquisitionInit();

      if (eng) {
        connect(m_Window.data(),   SIGNAL(executeCommand(QString)),
                eng.data(),   SLOT(evaluateAppCommand(QString)));

        connect(eng.data(),   SIGNAL(appResultAvailable(QScriptValue)),
                m_Window.data(),   SLOT(finishedCommand(QScriptValue)));
      }

      readInitialLogFile();

      if (m_Window && app && app->get_GuiWanted()) {
        m_Window -> show();
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

void QxrdExperiment::criticalMessage(QString msg, QDateTime ts) const
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

void QxrdExperiment::statusMessage(QString msg, QDateTime ts) const
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

void QxrdExperiment::printMessage(QString msg, QDateTime ts) const
{
  if (qcepDebug(DEBUG_NOMESSAGES)) {
  } else {
    QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+
        QThread::currentThread()->objectName()+": "+
        msg.trimmed();

    message = message.replace("\n", " : ");

#ifndef QT_NO_DEBUG
    printf("%s\n", qPrintable(message));
#endif

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

void QxrdExperiment::printLine(QString msg)
{
#ifndef QT_NO_DEBUG
    printf("%s\n", qPrintable(msg));
#endif

  QxrdWindowPtr win = m_Window;

  if (win) {
    win->displayMessage(msg);
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

QxrdServerThreadWPtr QxrdExperiment::specServerThread()
{
  return m_ServerThread;
}

QxrdSimpleServerWPtr QxrdExperiment::simpleServer()
{
  return m_SimpleServer;
}

QxrdSimpleServerThreadWPtr QxrdExperiment::simpleServerThread()
{
  return m_SimpleServerThread;
}

QxrdDataProcessorWPtr QxrdExperiment::dataProcessor() const
{
  return m_DataProcessor;
}

QxrdCalibrantLibraryWPtr QxrdExperiment::calibrantLibrary() const
{
  return m_CalibrantLibrary;
}

QcepDatasetWPtr QxrdExperiment::dataset() const
{
  return m_Dataset;
}

QxrdCenterFinderWPtr QxrdExperiment::centerFinder() const
{
  QxrdDataProcessorPtr dp(m_DataProcessor);

  if (dp) {
    return dp->centerFinder();
  } else {
    return QxrdCenterFinderWPtr();
  }
}

QxrdIntegratorWPtr QxrdExperiment::integrator() const
{
  QxrdDataProcessorPtr dp(m_DataProcessor);

  if (dp) {
    return dp->integrator();
  } else {
    return QxrdIntegratorWPtr();
  }
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

void QxrdExperiment::openNewLogFile() const
{
  {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

    if (m_LogFile) {
      fclose(m_LogFile);
      m_LogFile = NULL;
    }
  }

  openLogFile();
}

void QxrdExperiment::openLogFile() const
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

void QxrdExperiment::readInitialLogFile()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  FILE *logFile = fopen(qPrintable(logFilePath()), "r");

  if (logFile) {
    fseek(logFile, -10000, SEEK_END);

    char buff[10001];
    fgets(buff, 10000, logFile);

    while (!feof(logFile)) {
      fgets(buff, 10000, logFile);

      m_Window->initialLogEntry(buff);
    }

    fclose(logFile);
  }
}

FILE* QxrdExperiment::logFile()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  return m_LogFile;
}

void QxrdExperiment::logMessage(QString msg) const
{
  openLogFile();

  QxrdMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdExperiment::closeLogFile() const
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

    readSettings(&settings, "experiment");
  } else {
    QxrdExperimentSettings settings;

    readSettings(&settings, "experiment");
  }
}

void QxrdExperiment::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepExperiment::readSettings(settings, section);

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

    if (m_CalibrantLibrary) {
      m_CalibrantLibrary->readSettings(settings, "calibrantLibrary");
    }

    if (srv) {
      srv  -> readSettings(settings, "specserver");
    }

    if (ssrv) {
      ssrv -> readSettings(settings, "simpleserver");
    }

//    onDetectorTypeChanged();
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

      writeSettings(&settings, "experiment");
    }

    QFile::remove(docPath+".bak");
    QFile::rename(docPath, docPath+".bak");
    QFile::rename(docPath+".new", docPath);
  } else {
    QxrdExperimentSettings settings;

    writeSettings(&settings, "experiment");
  }
}

void QxrdExperiment::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepExperiment::writeSettings(settings, section);

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

    if (m_CalibrantLibrary) {
      m_CalibrantLibrary->writeSettings(settings, "calibrantLibrary");
    }

    if (srv) {
      srv  -> writeSettings(settings, "specserver");
    }

    if (ssrv) {
      ssrv -> writeSettings(settings, "simpleserver");
    }
  }
}

QString QxrdExperiment::defaultExperimentDirectory(QString path) const
{
  QFileInfo info(path);

  QString directory = info.dir().canonicalPath();

  return directory;
}

QString QxrdExperiment::defaultExperimentFileName(QString path) const
{
  QFileInfo info(path);

  if (info.suffix() == "qxrdp") {
    return info.fileName();
  } else {
    return info.fileName()+".qxrdp";
  }
}

QString QxrdExperiment::defaultExperimentName(QString path) const
{
  QFileInfo info(path);

  if (info.suffix() == "qxrdp") {
    return info.completeBaseName();
  } else {
    return info.fileName();
  }
}

QString QxrdExperiment::defaultDataDirectory(QString /*path*/) const
{
  return "";
}

QString QxrdExperiment::defaultLogName(QString path) const
{
  return defaultExperimentName(path)+".log";
}

QString QxrdExperiment::defaultScanName(QString path) const
{
  return defaultExperimentName(path)+".scans";
}

QString QxrdExperiment::experimentFilePath() const
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

QString QxrdExperiment::logFilePath() const
{
  QDir dir(get_ExperimentDirectory());

  return dir.filePath(get_LogFileName());
}

QString QxrdExperiment::scanFilePath() const
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

  writeSettings(&settings, "experiment");
}

void QxrdExperiment::saveExperimentCopyAs(QString path)
{
  printMessage(tr("Save experiment copy as %1").arg(path));

  QxrdExperimentSettings settings(path);

  writeSettings(&settings, "experiment");

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
  int newNum  = get_DetectorNumber();

  if (qcepDebug(DEBUG_ACQUIRE)) {
    printMessage(tr("Detector type changed to %1, Number %2").arg(newType).arg(newNum));
  }

  m_Detector       = QxrdDetectorPtr();
  m_DetectorThread = QxrdDetectorThreadPtr();

  m_DetectorThread = QxrdDetectorThreadPtr(new QxrdDetectorThread(m_Experiment, m_Acquisition));
  m_DetectorThread -> start();

  if (m_DetectorThread) {
    m_Detector = m_DetectorThread->detector();
  }

  QxrdDetectorPtr det(m_Detector);

  if (det) {
    set_DetectorType(det->detectorType());
    set_DetectorSubType(det->detectorSubType());
    set_DetectorTypeName(det->detectorTypeName());
    set_DetectorAddress(det->detectorAddress());

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

QColor QxrdExperiment::pickColor(QColor start)
{
  QColor res =  QColorDialog::getColor(start);

  return res;
}

void QxrdExperiment::evaluateScriptFiles(QStringList files)
{
  foreach(QString file, files) {
    evaluateScriptFile(file);
  }
}

void QxrdExperiment::evaluateScriptFile(QString path)
{
  QxrdScriptEnginePtr eng(m_ScriptEngine);

  if (eng) {
    printMessage(tr("Loading script %1").arg(path));

    eng->loadScript(path);
  }
}
