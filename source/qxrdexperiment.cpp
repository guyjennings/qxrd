#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qcepmacros.h"
#include "qcepallocator.h"
#include "qxrdexperiment.h"
#include "qxrdapplication.h"
#include "qxrdapplicationsettings.h"
#include "qxrddataprocessorthread.h"
#include "qxrddataprocessor.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrdcalibrantlibrarymodel.h"
#include "qxrdcalibrantdspacings.h"
#include "qxrdcalibrantdspacingsmodel.h"
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
#include "qcepsettingssaver.h"
#include "qxrdfilesaver.h"
#include "qxrdexperimentthread.h"
#include "qxrddetectorthread.h"
#include "qcepmutexlocker.h"
#include "qxrdacquisition-ptr.h"
#include "qcepdataset.h"
#include "qcepdatasetmodel.h"
#include "qcepdatagroup.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepsettingssaver.h"
//#include "qxrddetectorcontrolwindow.h"
#include "qcepfileformattertext.h"
#include "qxrdacquisitionexecution.h"
#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdfitparameter.h"
#include "qcepdataexportparameters.h"
#include "qcepdataimportparameters.h"
#include <QHostInfo>
#include <QColorDialog>
#include <QProcess>

QxrdExperiment::QxrdExperiment(QxrdExperimentThreadWPtr expthrd,
    QString path,
    QxrdApplicationWPtr app) :
  QcepExperiment(path, "experiment", QcepDataObjectWPtr()),
  m_Application(app),
  m_ExperimentThread(expthrd),
  m_WindowSettings(NULL),
  m_Window(),
//  m_AcquisitionWindow(),
  m_ServerThread(NULL),
  m_Server(),
  m_SimpleServerThread(NULL),
  m_SimpleServer(),
  m_DataProcessor(),
  m_Acquisition(),
  m_FileSaverThread(NULL),
  m_FileSaver(),
  m_ScriptEngine(),
  m_ScriptEngineDebugger(NULL),
  m_LogFile(NULL),
  m_ScanFile(NULL),
  m_ExperimentFileMutex(),

  m_DataDirectory(this, "dataDirectory", defaultDataDirectory(path), "Saved Data Directory"),
  m_LogFileName(this, "logFileName", defaultLogName(path), "Log File Name"),
  m_ScanFileName(this, "scanFileName", defaultScanName(path), "Scan File Name"),
  m_ScanFileExtension(this, "scanFileExtension", ".avg", "Scan File Extension"),
  m_ScanDataNegative(this, "scanDataNegative", 0, "Scan Data Negative Value Handling"),
  m_DefaultLayout(this,"defaultLayout",0, "Default Layout Used?"),
  m_WorkCompleted(this, "workCompleted", 0, "Amount of Work Completed"),
  m_WorkTarget(this, "workTarget", 0, "Amount of Work Targetted"),
  m_CompletionPercentage(this, "completionPercentage", 0, "Percentage of Work Completed"),
  m_DefaultScript(this, "defaultScript", "", "Default script for experiment"),
  m_ExtraScriptFiles(this, "extraScriptFiles", QStringList(), "Additional script files for experiment"),
  m_FontSize(this, "fontSize", -1, "Suggested font size"),
  m_Spacing(this, "spacing", -1, "Suggested widget spacing")
{
#ifndef QT_NO_DEBUG
  printf("Constructing experiment\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperiment::QxrdExperiment(%p)\n", this);
  }

  QxrdApplicationPtr appl(m_Application);

  if (appl) {
    QxrdApplicationSettingsPtr set(appl->settings());

    if (set) {
      set->prop_ExperimentCount()->incValue(1);
    }
  }
}

QxrdExperiment::~QxrdExperiment()
{
#ifndef QT_NO_DEBUG
  printf("Deleting experiment\n");
#endif

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdExperiment::~QxrdExperiment");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperiment::~QxrdExperiment(%p)\n", this);
  }

  m_SettingsSaver->performSave();

  closeScanFile();
  closeLogFile();

  QxrdApplicationPtr app(m_Application);

  if (app) {
    QxrdApplicationSettingsPtr set(app->settings());

    if (set) {
      set->prop_ExperimentCount()->incValue(-1);
    }
  }
}

void QxrdExperiment::initialize(QSettings *settings)
{
  QxrdApplicationPtr app(m_Application);

  if (app) {
    QxrdExperimentPtr myself(qSharedPointerDynamicCast<QxrdExperiment>(sharedFromThis()));

    QcepExperiment::initialize(settings);

    splashMessage("Initializing File Saver");

    m_FileSaverThread = QxrdFileSaverThreadPtr(
          new QxrdFileSaverThread(sharedFromThis()));

    m_FileSaverThread -> start();
    m_FileSaver = m_FileSaverThread -> fileSaver();

    splashMessage("Initializing Data Processing");

    m_DataProcessor = QxrdDataProcessorPtr(
          new QxrdDataProcessor(myself,
                                QxrdAcquisitionWPtr(),
                                m_FileSaver));

    m_DataProcessor -> initialize();

    QxrdFileSaverPtr saver(m_FileSaver);

    if (saver) {
      saver -> setProcessor(m_DataProcessor);
      saver -> setExperiment(myself);
    }

    splashMessage("Initializing Data Acquisition");

    m_Acquisition = QxrdAcquisitionPtr(
          new QxrdAcquisition(myself,
                              m_DataProcessor));

    m_Acquisition -> initialize();

    m_CalibrantLibrary = QxrdCalibrantLibraryPtr(
          new QxrdCalibrantLibrary(myself));

    m_CalibrantLibraryModel = QxrdCalibrantLibraryModelPtr(
          new QxrdCalibrantLibraryModel(m_CalibrantLibrary));

    m_CalibrantDSpacings = QxrdCalibrantDSpacingsPtr(
          new QxrdCalibrantDSpacings());

    m_CalibrantDSpacingsModel = QxrdCalibrantDSpacingsModelPtr(
          new QxrdCalibrantDSpacingsModel(m_CalibrantLibrary, m_CalibrantDSpacings));

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

    m_Dataset      = QcepAllocator::newDataset("dataset", myself);
    m_DatasetModel = QcepDatasetModelPtr(
          new QcepDatasetModel(myself, m_DataProcessor, m_Dataset));

//    m_DatasetModel -> newGroup("group1");
//    m_DatasetModel -> newGroup("group2");
//    m_DatasetModel -> newGroup("group3");

//    QStringList cols;
//    cols << "x" << "y" << "z" << "v" << "sdev";

//    m_DatasetModel -> newColumnScan("group1/scan1", cols, 1000);
//    m_DatasetModel -> newColumnScan("group2/scan2", cols, 1000);
//    m_DatasetModel -> newColumnScan("group3/scan3", cols, 1000);

//    m_DatasetModel -> newColumn("group4/x", 1000);
//    m_DatasetModel -> newColumn("group4/y", 1000);
//    m_DatasetModel -> newColumn("group4/z", 1000);
//    m_DatasetModel -> newColumn("group4/t", 1000);
//    m_DatasetModel -> newColumn("group4/sdev", 1000);

    addChildPtr(new QxrdWindowSettings("windowSettings"));

    splashMessage("Starting SPEC Server");

    m_ServerThread = QxrdServerThreadPtr(
          new QxrdServerThread(myself, "qxrd"));

    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    splashMessage("Starting Simple Socket Server");

    m_SimpleServerThread = QxrdSimpleServerThreadPtr(
          new QxrdSimpleServerThread(myself, "simpleserver"));

    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

//    m_ScriptEngineThread = QxrdScriptEngineThreadPtr(
//          new QxrdScriptEngineThread(m_Application, m_Experiment));
//    m_ScriptEngineThread -> setObjectName("script");
//    m_ScriptEngineThread -> start();
//    m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

    m_ScriptEngine = QxrdScriptEnginePtr(
          new QxrdScriptEngine(app, myself));

    m_ScriptEngine -> initialize();

    QxrdServerPtr srv(m_Server);
    QxrdScriptEnginePtr eng(m_ScriptEngine);

//    if (eng) {
//      m_ScriptEngineDebugger = new QScriptEngineDebugger(this);
//      m_ScriptEngineDebugger -> attachTo(eng.data());
//      m_ScriptEngineDebugger -> setAutoShowStandardWindow(true);
//    }

    if (srv && eng) {
      connect(srv.data(),   &QxrdServer::executeCommand,
              eng.data(),   &QxrdScriptEngine::evaluateSpecCommand);

      connect(eng.data(),   &QxrdScriptEngine::specResultAvailable,
              srv.data(),   &QxrdServer::finishedCommand);
    }

    QxrdSimpleServerPtr ssrv(m_SimpleServer);

    if (ssrv && eng) {
      connect(ssrv.data(),  &QxrdSimpleServer::executeCommand,
              eng.data(),   &QxrdScriptEngine::evaluateSimpleServerCommand);

      connect(eng.data(),   &QxrdScriptEngine::simpleServerResultAvailable,
              ssrv.data(),  &QxrdSimpleServer::finishedCommand);
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

    connect(prop_WorkCompleted(),   &QcepIntProperty::valueChanged, this, &QxrdExperiment::updateCompletionPercentage);
    connect(prop_WorkTarget(),      &QcepIntProperty::valueChanged, this, &QxrdExperiment::updateCompletionPercentage);

    m_SettingsSaver->start();
  }
}

void QxrdExperiment::addChildPtr(QcepObjectPtr child)
{
  printMessage("QxrdExperiment::addChildPtr");

  QcepExperiment::addChildPtr(child);

  if (checkPointer<QxrdWindowSettings>(child, m_WindowSettings)) {}

  else if (checkPointer<QxrdServer>(child, m_Server)) {
    QxrdServerPtr srv(m_Server);
    srv -> moveToThread(m_ServerThread.data());
  }

  else if (checkPointer<QxrdSimpleServer>(child, m_SimpleServer)) {
    QxrdSimpleServerPtr ssrv(m_SimpleServer);
    ssrv -> moveToThread(m_SimpleServerThread.data());
  }

  else if (checkPointer<QxrdDataProcessor>(child, m_DataProcessor)) {}

  else if (checkPointer<QxrdCalibrantLibrary>(child, m_CalibrantLibrary)) {
    if (m_CalibrantLibraryModel == NULL) {
      printMessage("Calibrant Library Model == NULL");
    } else {
      m_CalibrantLibraryModel->setCalibrantLibrary(m_CalibrantLibrary);
    }
  }

  else if (checkPointer<QxrdCalibrantDSpacings>(child, m_CalibrantDSpacings)) {
    if (m_CalibrantDSpacingsModel == NULL) {
      printMessage(("Calibrant D Spacings Model == NULL"));
    } else {
      m_CalibrantDSpacingsModel->setCalibrantDSpacings(m_CalibrantDSpacings);
    }
  }

  else if (checkPointer<QxrdAcquisition>(child, m_Acquisition)) {}

  else if (checkPointer<QxrdFileSaver>(child, m_FileSaver)) {
    QxrdFileSaverPtr fsav(m_FileSaver);
    fsav -> moveToThread(m_FileSaverThread.data());
  }

  else if (checkPointer<QxrdScriptEngine>(child, m_ScriptEngine)) {}

  else if (checkPointer<QcepDataset>(child, m_Dataset)) {
    if (m_DatasetModel == NULL) {
      printMessage("Dataset Model == NULL");
    } else {
      m_DatasetModel->setDataset(m_Dataset);
    }
  }
}

void QxrdExperiment::removeChildPtr(QcepObjectPtr child)
{
  printMessage("Need to write QxrdExperiment::removeChildPtr");

  QcepExperiment::removeChildPtr(child);
}

void QxrdExperiment::registerMetaTypes()
{
  qRegisterMetaType<QxrdExperiment*>("QxrdExperiment*");
  qRegisterMetaType<QxrdAcquisition*>("QxrdAcquisition*");
  qRegisterMetaType<QxrdAcquisitionExecution*>("QxrdAcquisitionExecution*");
  qRegisterMetaType<QxrdAcquisitionExtraInputs*>("QxrdAcquisitionExtraInputs*");
  qRegisterMetaType<QxrdAcquisitionExtraInputsChannel*>("QxrdAcquisitionExtraInputsChannel*");
  qRegisterMetaType<QxrdCalibrantWPtr>("QxrdCalibrantWPtr");
  qRegisterMetaType<QxrdCalibrantDSpacing>("QxrdCalibrantDSpacing");
  qRegisterMetaType<QxrdCalibrantDSpacings>("QxrdCalibrantDSpacings");
  qRegisterMetaType<QxrdCalibrantLibrary*>("QxrdCalibrantLibrary*");
  qRegisterMetaType<QxrdCenterFinder*>("QxrdCenterFinder*");
  qRegisterMetaType<QxrdDataProcessor*>("QxrdDataProcessor*");
  qRegisterMetaType<QxrdDistortionCorrection*>("QxrdDistortionCorrection*");
  qRegisterMetaType<QxrdFileBrowserModelUpdater*>("QxrdFileBrowserModelUpdater*");
  qRegisterMetaType<QxrdFileSaver*>("QxrdFileSaver*");
  qRegisterMetaType<QxrdFitParameterPtr>("QxrdFitParameterPtr");
  qRegisterMetaType<QxrdIntegrator*>("QxrdIntegrator*");
  qRegisterMetaType<QxrdDetectorGeometry*>("QxrdDetectorGeometry*");
  qRegisterMetaType<QxrdPolarNormalization*>("QxrdPolarNormalization*");
  qRegisterMetaType<QxrdPolarTransform*>("QxrdPolarTransform*");
  qRegisterMetaType<QxrdPowderPoint>("QxrdPowderPoint");
  qRegisterMetaType<QxrdPowderPointVector>("QxrdPowderPointVector");
  qRegisterMetaType<QxrdScriptEngine*>("QxrdScriptEngine*");
  qRegisterMetaType<QxrdServer*>("QxrdServer*");
  qRegisterMetaType<QxrdSimpleServer*>("QxrdSimpleServer*");
  qRegisterMetaType<QxrdSynchronizedAcquisition*>("QxrdSynchronizedAcquisition*");
  qRegisterMetaType<QxrdWindowSettings*>("QxrdWindowSettings*");
  qRegisterMetaType<QcepDataset*>("QcepDataset*");
  qRegisterMetaType<QcepDataColumn*>("QcepDataColumn*");
  qRegisterMetaType<QcepDataGroup*>("QcepDataGroup*");
  qRegisterMetaType<QcepDataArray*>("QcepDataArray*");
  qRegisterMetaType<QcepDoubleImageData*>("QcepDoubleImageData*");
  qRegisterMetaType<QcepFloatImageData*>("QcepFloatImageData*");
  qRegisterMetaType<QcepInt32ImageData*>("QcepInt32ImageData*");
  qRegisterMetaType<QcepUInt32ImageData*>("QcepUInt32ImageData*");
  qRegisterMetaType<QcepInt16ImageData*>("QcepInt16ImageData*");
  qRegisterMetaType<QcepUInt16ImageData*>("QcepUInt16ImageData*");
  qRegisterMetaType<QcepMaskData*>("QcepMaskData*");
  qRegisterMetaType<QcepIntegratedData*>("QcepIntegratedData*");
  qRegisterMetaType<QcepDataColumnScan*>("QcepDataColumnScan*");
  qRegisterMetaType<QcepDataExportParameters*>("QcepDataExportParameters*");
  qRegisterMetaType<QcepDataImportParameters*>("QcepDataImportParameters*");
}

QxrdExperimentThreadWPtr QxrdExperiment::experimentThread()
{
  return m_ExperimentThread;
}

void QxrdExperiment::openWindows()
{
  GUI_THREAD_CHECK;

  QxrdApplicationPtr app(m_Application);

  if (app) {
    QxrdApplicationSettingsPtr set(app->settings());
    if (set && set->get_GuiWanted()) {
      splashMessage("Opening Main Window");
      m_Window = QxrdWindowPtr(
            new QxrdWindow(m_WindowSettings,
                           m_Application,
                           qSharedPointerDynamicCast<QxrdExperiment>(sharedFromThis()),
                           m_Acquisition,
                           m_DataProcessor,
                           NULL),
            &QObject::deleteLater);

      QxrdDataProcessorPtr proc(m_DataProcessor);
      QxrdAcquisitionPtr acq(m_Acquisition);
      QxrdScriptEnginePtr eng(m_ScriptEngine);

      if (m_Window) {
        m_Window -> initialize();

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
          connect(m_Window.data(),   &QxrdWindow::executeCommand,
                  eng.data(),   &QxrdScriptEngine::evaluateAppCommand);

          connect(eng.data(),   &QxrdScriptEngine::appResultAvailable,
                  m_Window.data(),   &QxrdWindow::finishedCommand);
        }

        readInitialLogFile();

        if (m_Window && set && set->get_GuiWanted()) {
          m_Window -> show();
        }
      }
    }
  }
}

void QxrdExperiment::closeWindows()
{
  m_Window = QxrdWindowPtr();
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

//#ifndef QT_NO_DEBUG
//    printf("%s\n", qPrintable(message));
//#endif

    logMessage(message);

    QxrdWindowPtr win = m_Window;

    if (win) {
      win->displayMessage(message);
//    } else if (m_Application) {
//      m_Application->printMessage(message);
//    } else {
//      printf("%s\n", qPrintable(message));
    }
  }
}

void QxrdExperiment::printLine(QString msg) const
{
//#ifndef QT_NO_DEBUG
//    printf("%s\n", qPrintable(msg));
//#endif

  QxrdWindowPtr win = m_Window;

  if (win) {
    win->displayMessage(msg);
  }
}

QxrdWindowPtr QxrdExperiment::window()
{
  return m_Window;
}

QxrdAcquisitionWPtr QxrdExperiment::acquisition() const
{
  return m_Acquisition;
}

QxrdFileSaverWPtr QxrdExperiment::fileSaver() const
{
  return m_FileSaver;
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

QxrdCalibrantLibraryModelWPtr QxrdExperiment::calibrantLibraryModel() const
{
  return m_CalibrantLibraryModel;
}

QxrdCalibrantDSpacingsWPtr QxrdExperiment::calibrantDSpacings() const
{
  return m_CalibrantDSpacings;
}

QxrdCalibrantDSpacingsModelWPtr QxrdExperiment::calibrantDSpacingsModel() const
{
  return m_CalibrantDSpacingsModel;
}

QcepDatasetModelPtr QxrdExperiment::dataset()
{
  return m_DatasetModel;
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

QString QxrdExperiment::systemCommand(QString cmd)
{
  QProcess process;

  process.start(cmd);
  process.waitForReadyRead();

  return process.readAllStandardOutput();
}

void QxrdExperiment::newLogFile(QString path)
{
  {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

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
    QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

    if (m_LogFile) {
      fclose(m_LogFile);
      m_LogFile = NULL;
    }
  }

  openLogFile();
}

void QxrdExperiment::openLogFile() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  return m_LogFile;
}

void QxrdExperiment::logMessage(QString msg) const
{
  openLogFile();

  QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdExperiment::closeLogFile() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

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
    QcepMutexLocker lock(__FILE__, __LINE__, &m_ScanFileMutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_ScanFileMutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_ScanFileMutex);

  if (m_ScanFile) {
    fclose(m_ScanFile);
    m_ScanFile = NULL;
  }
}

void QxrdExperiment::readSettings()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

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
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (settings) {
    QcepExperiment::readSettings(settings, section);

    QxrdAcquisitionPtr acq(m_Acquisition);
    QxrdDataProcessorPtr proc(m_DataProcessor);
    QxrdServerPtr srv(m_Server);
    QxrdSimpleServerPtr ssrv(m_SimpleServer);

    m_WindowSettings -> readSettings(settings, "window");

    if (acq) {
      acq -> readSettings(settings, "acquisition");
    }

    if (settings->contains("experiment/detectorType")) {
      int detType = settings->value("experiment/detectorType", -1).toInt();

      if (detType == QxrdDetectorThread::PerkinElmerDetector) {
        if (acq) {
          if (acq->get_DetectorCount() == 0) {
            acq->appendDetector(detType);
          }
        }
      }
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

//    if (m_DatasetModel) {
//      settings->beginGroup("dataset");
//      m_DatasetModel->readSettings(settings, "dataset");
//      settings->endGroup();
//    }
  }
}

void QxrdExperiment::writeSettings()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

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
      acq -> writeSettings(settings, "acquisition");
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

//    if (m_DatasetModel) {
//      settings->beginGroup("dataset");
//      m_DatasetModel -> writeSettings(settings, "dataset");
//      settings->endGroup();
//    }
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

void QxrdExperiment::saveExperimentAsText(QString filePath)
{
  QcepFileFormatterPtr fmt = QcepFileFormatterPtr(new QcepFileFormatterText(filePath));

  fmt -> beginWriteFile();

  writeObject(fmt);

  fmt -> endWriteFile();
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

//void QxrdExperiment::openAcquisitionWindow()
//{
//  GUI_THREAD_CHECK;

//  if (m_AcquisitionWindow == NULL) {
//    m_AcquisitionWindow = QxrdDetectorControlWindowPtr(
//          new QxrdDetectorControlWindow(acquisition(), NULL));
//  }

//  m_AcquisitionWindow->show();
//}

void QxrdExperiment::readObjectTreeFromText(QString filePath)
{
  printMessage(tr("Reading Object Tree from %1 ...").arg(filePath));

  QcepFileFormatterPtr fmt = QcepFileFormatterPtr(
        new QcepFileFormatterText(filePath));

  fmt -> beginReadFile();

  QcepObjectPtr obj = QcepObject::readDataObject(fmt);

  fmt -> endReadFile();

  if (obj) {
    obj -> dumpObjectTreePtr();
  } else {
    printMessage("NULL");
  }
}
