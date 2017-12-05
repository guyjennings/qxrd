#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qcepmacros.h"
#include "qcepallocator.h"
#include "qxrdexperiment.h"
#include "qxrdexperimentthread.h"
#include "qxrdapplication.h"
#include "qxrdapplicationsettings.h"
#include "qxrddataprocessorthread.h"
#include "qxrddataprocessor.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrdcalibrantlibrarymodel.h"
#include "qxrdcalibrantdspacings.h"
#include "qxrdcalibrantdspacingsmodel.h"
#include "qxrdwindow.h"
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
#include "qxrdfilesaver.h"
#include "qcepmutexlocker.h"
#include "qxrdacquisition-ptr.h"
#include "qcepdataset.h"
#include "qcepdatasetmodel.h"
#include "qcepdatagroup.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepfileformattertext.h"
#include "qxrdacquisitionexecution.h"
#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdsynchronizedacquisition.h"
#include "qcepdataexportparameters.h"
#include "qcepdataimportparameters.h"
#include <QHostInfo>
#include <QColorDialog>
#include <QProcess>
#include "qxrdwindowsettings.h"
#include "qxrdgeneratetestimage.h"
#include "qxrdjsengine.h"

#include "qxrdmainwindowsettings-ptr.h"
#include "qxrdacquisitionwindowsettings.h"
#include "qxrdextraiowindowsettings.h"
#include "qxrdinfowindowsettings.h"
#include "qxrdanalysiswindowsettings.h"
#include "qxrdintegrationwindowsettings.h"
#include "qxrdcalculatorwindowsettings.h"
#include "qxrdmainwindowsettings.h"
#include "qxrdcalibrantwindowsettings.h"
#include "qxrdmaskingwindowsettings.h"
#include "qxrdcenteringwindowsettings.h"
#include "qxrdscriptingwindowsettings.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrdwindowsettings.h"
#include "qxrdhelpwindowsettings.h"

QxrdExperiment::QxrdExperiment(QString name) :
  QcepExperiment("", name),
  m_Application(),
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
//  m_ScriptEngineDebugger(NULL),
  m_ScriptEngineJS(),
  m_LogFile(NULL),
  m_ScanFile(NULL),
  m_ExperimentFileMutex(),

  m_QxrdVersion(this, "qxrdVersion", STR(QXRD_VERSION), "Qxrd Version"),
  m_DataDirectory(this, "dataDirectory", defaultDataDirectory(""), "Saved Data Directory"),
  m_LogFileName(this, "logFileName", defaultLogName(""), "Log File Name"),
  m_ScanFileName(this, "scanFileName", defaultScanName(""), "Scan File Name"),
  m_ScanFileExtension(this, "scanFileExtension", ".avg", "Scan File Extension"),
  m_ScanDataNegative(this, "scanDataNegative", 0, "Scan Data Negative Value Handling"),
  m_DefaultLayout(this,"defaultLayout",0, "Default Layout Used?"),
  m_WorkCompleted(this, "workCompleted", 0, "Amount of Work Completed"),
  m_WorkTarget(this, "workTarget", 0, "Amount of Work Targetted"),
  m_CompletionPercentage(this, "completionPercentage", 0, "Percentage of Work Completed"),
  m_DefaultScript(this, "defaultScript", "", "Default script for experiment"),
  m_ExtraScriptFiles(this, "extraScriptFiles", QStringList(), "Additional script files for experiment"),
  m_FontSize(this, "fontSize", -1, "Suggested font size"),
  m_Spacing(this, "spacing", -1, "Suggested widget spacing"),
  m_IsReading(this, "isReading", 0, "Is being read?")
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

QxrdExperimentPtr QxrdExperiment::newExperiment(QString path, QxrdApplicationWPtr app, QxrdExperimentSettingsPtr set)
{
  QxrdExperimentPtr expt(new QxrdExperiment("experiment"));

  if (expt) {
    expt->setExperimentFilePath(path);
    expt->setExperimentApplication(app);
    expt->initialize(set);
  }

  return expt;
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

  onAutoSaveTimer();
//  m_SettingsSaver->performSave();

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

QxrdExperimentThreadPtr QxrdExperiment::experimentThread() const
{
  QxrdExperimentThread* t =
      qobject_cast<QxrdExperimentThread*>(thread());

  return QxrdExperimentThreadPtr(t);
}

void QxrdExperiment::initialize(QxrdExperimentSettingsPtr settings)
{
  QxrdApplicationPtr app(m_Application);

  if (app) {
    QxrdExperimentPtr myself(qSharedPointerDynamicCast<QxrdExperiment>(sharedFromThis()));

    QcepExperiment::initialize();

    splashMessage("Initializing File Saver");

    m_FileSaverThread = QxrdFileSaverThreadPtr(
          new QxrdFileSaverThread(sharedFromThis()));

    m_FileSaverThread -> start();
    m_FileSaver = m_FileSaverThread -> fileSaver();

    splashMessage("Initializing Data Processing");

    addChildPtr(QxrdDataProcessor::newDataProcessor());

    QxrdFileSaverPtr saver(m_FileSaver);

    if (saver) {
      saver -> setProcessor(m_DataProcessor);
      saver -> setExperiment(myself);
    }

//    addChildPtr(m_FileSaver);

    splashMessage("Initializing Data Acquisition");

    addChildPtr(QxrdAcquisition::newAcquisition());

    m_Acquisition -> initialize();

    m_CalibrantLibrary = QxrdCalibrantLibrary::newCalibrantLibrary();

    m_CalibrantLibraryModel = QxrdCalibrantLibraryModelPtr(
          new QxrdCalibrantLibraryModel(m_CalibrantLibrary));

    addChildPtr(m_CalibrantLibrary);

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

    addChildPtr(QcepAllocator::newDataset("dataset"));

    m_DatasetModel = QcepDatasetModelPtr(
          new QcepDatasetModel(myself, m_DataProcessor, m_Dataset));

//    m_DatasetModel -> newGroup("/group1");
//    m_DatasetModel -> newGroup("/group2");
//    m_DatasetModel -> newGroup("/group3");
//    m_DatasetModel -> newGroup("/group1/group1.1");
//    m_DatasetModel -> newGroup("/group1/group1.2");

//    QStringList cols;
//    cols << "x" << "y" << "z" << "v" << "sdev";

//    m_DatasetModel -> newColumnScan("/group1/scan1", cols, 1000);
//    m_DatasetModel -> newColumnScan("/group2/scan2", cols, 1000);
//    m_DatasetModel -> newColumnScan("/group3/scan3", cols, 1000);

//    m_DatasetModel -> newColumn("/group4/x", 1000);
//    m_DatasetModel -> newColumn("/group4/y", 1000);
//    m_DatasetModel -> newColumn("/group4/z", 1000);
//    m_DatasetModel -> newColumn("/group4/t", 1000);
//    m_DatasetModel -> newColumn("/group4/sdev", 1000);

    addChildPtr(QxrdWindowSettings::newWindowSettings());

    splashMessage("Starting SPEC Server");

    m_ServerThread = QxrdServerThreadPtr(
          new QxrdServerThread(myself, "qxrd"));

    m_ServerThread -> start();
    m_Server = m_ServerThread -> server();

    addChildPtr(m_Server);

    splashMessage("Starting Simple Socket Server");

    m_SimpleServerThread = QxrdSimpleServerThreadPtr(
          new QxrdSimpleServerThread(myself, "simpleserver"));

    m_SimpleServerThread -> start();
    m_SimpleServer = m_SimpleServerThread -> server();

    addChildPtr(m_SimpleServer);

//    m_ScriptEngineThread = QxrdScriptEngineThreadPtr(
//          new QxrdScriptEngineThread(m_Application, m_Experiment));
//    m_ScriptEngineThread -> setObjectName("script");
//    m_ScriptEngineThread -> start();
//    m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

    m_ScriptEngine = QxrdScriptEnginePtr(
          new QxrdScriptEngine(app, myself));

    m_ScriptEngine -> initialize();

    m_ScriptEngineJS = QxrdJSEnginePtr(
          new QxrdJSEngine(app, myself));

    m_ScriptEngineJS -> initialize();

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

//    settings->beginGroup("experiment");
    readSettings(settings.data());
//    settings->endGroup();

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

//    m_SettingsSaver->start();

    connect(&m_AutoSaveTimer, &QTimer::timeout, this, &QxrdExperiment::onAutoSaveTimer);

    m_AutoSaveTimer.start(5000);
  }
}

void QxrdExperiment::addChildPtr(QcepObjectPtr child)
{
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

//  else if (checkPointer<QxrdCalibrantDSpacings>(child, m_CalibrantDSpacings)) {
//    if (m_CalibrantDSpacingsModel == NULL) {
//      printMessage(("Calibrant D Spacings Model == NULL"));
//    } else {
//      m_CalibrantDSpacingsModel->setCalibrantDSpacings(m_CalibrantDSpacings);
//    }
//  }

  else if (checkPointer<QxrdAcquisition>(child, m_Acquisition)) {}

//  else if (checkPointer<QxrdFileSaver>(child, m_FileSaver)) {
//    QxrdFileSaverPtr fsav(m_FileSaver);
//    fsav -> moveToThread(m_FileSaverThread.data());
//  }

//  else if (checkPointer<QxrdScriptEngine>(child, m_ScriptEngine)) {}

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
  qRegisterMetaType< QList<QwtLegendData> >("QList<QwtLegendData>");
  qRegisterMetaType<QxrdExperiment*>("QxrdExperiment*");
  qRegisterMetaType<QxrdAcquisition*>("QxrdAcquisition*");
  qRegisterMetaType<QxrdAcquisitionExtraInputs*>("QxrdAcquisitionExtraInputs*");
  qRegisterMetaType<QxrdAcquisitionExtraInputsChannel*>("QxrdAcquisitionExtraInputsChannel*");
  qRegisterMetaType<QxrdCalibrantWPtr>("QxrdCalibrantWPtr");
  qRegisterMetaType<QxrdCalibrantDSpacing>("QxrdCalibrantDSpacing");
  qRegisterMetaType<QxrdCalibrantDSpacings>("QxrdCalibrantDSpacings");
  qRegisterMetaType<QxrdCalibrantLibrary*>("QxrdCalibrantLibrary*");
  qRegisterMetaType<QxrdCenterFinder*>("QxrdCenterFinder*");
  qRegisterMetaType<QxrdDataProcessor*>("QxrdDataProcessor*");
  qRegisterMetaType<QxrdFileBrowserModelUpdater*>("QxrdFileBrowserModelUpdater*");
  qRegisterMetaType<QxrdFileSaver*>("QxrdFileSaver*");
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
  qRegisterMetaType<QxrdGenerateTestImage*>("QxrdGenerateTestImage*");
  qRegisterMetaType<QxrdMainWindowSettings*>("QxrdMainWindowSettings*");
  qRegisterMetaType<QxrdAcquisitionWindowSettings*>("QxrdAcquisitionWindowSettings*");
  qRegisterMetaType<QxrdAnalysisWindowSettings*>("QxrdAnalysisWindowSettings*");
  qRegisterMetaType<QxrdExtraIOWindowSettings*>("QxrdExtraIOWindowSettings*");
  qRegisterMetaType<QxrdInfoWindowSettings*>("QxrdInfoWindowSettings*");
  qRegisterMetaType<QxrdCalculatorWindowSettings*>("QxrdCalculatorWindowSettings*");
  qRegisterMetaType<QxrdCalibrantWindowSettings*>("QxrdCalibrantWindowSettings*");
  qRegisterMetaType<QxrdHelpWindowSettings*>("QxrdHelpWindowSettings*");
  qRegisterMetaType<QxrdCenteringWindowSettings*>("QxrdCenteringWindowSettings*");
  qRegisterMetaType<QxrdIntegrationWindowSettings*>("QxrdIntegrationWindowSettings*");
  qRegisterMetaType<QxrdMaskingWindowSettings*>("QxrdMaskingWindowSettings*");
  qRegisterMetaType<QxrdScriptingWindowSettings*>("QxrdScriptingWindowSettings*");
  qRegisterMetaType<QxrdWindowSettings*>("QxrdWindowSettings*");
}

void QxrdExperiment::setExperimentApplication(QxrdApplicationWPtr app)
{
  m_Application = app;
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

          acq->openWindows(); // Open detector control windows...
        }

        if (eng) {
          eng -> setWindow(m_Window);
        }

        if (m_ScriptEngineJS) {
          m_ScriptEngineJS -> setWindow(m_Window);
        }

        m_Window -> onAcquisitionInit();

        if (eng) {
          connect(m_Window.data(),   &QxrdWindow::executeCommand,
                  eng.data(),   &QxrdScriptEngine::evaluateAppCommand);

          connect(eng.data(),   &QxrdScriptEngine::appResultAvailable,
                  m_Window.data(),   &QxrdWindow::finishedCommand);
        }

        if (m_ScriptEngineJS) {
          connect(m_Window.data(), &QxrdWindow::executeCommandJS,
                  m_ScriptEngineJS.data(), &QxrdJSEngine::evaluateAppCommandJS);

          connect(m_ScriptEngineJS.data(), &QxrdJSEngine::appResultAvailableJS,
                  m_Window.data(), &QxrdWindow::finishedCommandJS);
        }

        readInitialLogFile();

        if (m_Window && set && set->get_GuiWanted()) {
          m_Window -> show();
        }
      }

      for (int i=0; i<windowSettingsCount(); i++) {
        QcepMainWindowSettingsPtr set =
            windowSettings(i);

        if (set) {
          set->openWindow();
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

void QxrdExperiment::criticalMessage(QString msg, QDateTime /*ts*/) const
{
  QxrdApplicationPtr app(m_Application);
  QxrdWindowPtr      win(m_Window);

  if (win) {
    displayPushedMessages();
    win->displayCriticalMessage(msg);
  } else if (app) {
    pushMessage(msg);
  }
}

void QxrdExperiment::statusMessage(QString msg, QDateTime /*ts*/) const
{
  QxrdApplicationPtr app(m_Application);
  QxrdWindowPtr      win(m_Window);

  if (win) {
    displayPushedMessages();
    win->displayStatusMessage(msg);
  } else {
    pushMessage(msg);
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
      displayPushedMessages();

      win->displayMessage(message);
    } else {
      pushMessage(message);
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
    displayPushedMessages();

    win->displayMessage(msg);
  } else {
    pushMessage(msg);
  }
}

void QxrdExperiment::displayPushedMessages() const
{
  QxrdWindowPtr win = m_Window;

  if (win) {
    foreach (QString m, m_PushedMessages) {
      win->displayMessage(m);
    }

    m_PushedMessages.clear();
  }
}

void QxrdExperiment::pushMessage(QString msg) const
{
  m_PushedMessages.append(msg);
}

QxrdApplicationWPtr QxrdExperiment::application() const
{
  return m_Application;
}

QxrdExperimentWPtr QxrdExperiment::experiment()
{
  return qSharedPointerDynamicCast<QxrdExperiment>(sharedFromThis());
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

QxrdJSEngineWPtr QxrdExperiment::jsEngine()
{
  return m_ScriptEngineJS;
}

void QxrdExperiment::executeCommandJS(QString cmd)
{
  if (m_ScriptEngineJS) {
    m_ScriptEngineJS->evaluateAppCommandJS(cmd);
  }
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
  prop_IsReading()->incValue(1);

  //  if (qcepDebug(DEBUG_PREFS)) {
  printMessage("started QxrdExperiment::readSettings");
  printf("started QxrdExperiment::readSettings\n");
  //  }

  QcepMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

  QString docPath = experimentFilePath();

  if (docPath.length()>0) {
    QSettings settings(docPath, QSettings::IniFormat);

    //    settings.beginGroup("experiment");
    readSettings(&settings);
    //    settings.endGroup();
  } else {
    QxrdExperimentSettings settings;

    //    settings.beginGroup("experiment");
    readSettings(&settings);
    //    settings.endGroup();
  }

  //  if (qcepDebug(DEBUG_PREFS)) {
  printMessage("finished QxrdExperiment::readSettings");
  printf("finished QxrdExperiment::readSettings\n");
  //  }

  prop_IsReading()->incValue(-1);
}

void QxrdExperiment::readSettings(QSettings *settings)
{
  prop_IsReading()->incValue(1);
  //  if (qcepDebug(DEBUG_PREFS)) {
  printMessage("started QxrdExperiment::readSettings(QSettings*)");
  printf("started QxrdExperiment::readSettings(QSettings*)\n");
  //  }

  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (settings) {
    QcepExperiment::readSettings(settings);

    QxrdAcquisitionPtr acq(m_Acquisition);
    QxrdDataProcessorPtr proc(m_DataProcessor);
    QxrdServerPtr srv(m_Server);
    QxrdSimpleServerPtr ssrv(m_SimpleServer);

    if (m_WindowSettings) {
      settings->beginGroup("window");
      m_WindowSettings -> readSettings(settings);
      settings->endGroup();
    }

    if (acq) {
      settings->beginGroup("acquisition");
      acq -> readSettings(settings);
      settings->endGroup();
    }

    settings->beginGroup("experiment");
    if (settings->contains("detectorType")) {
      int detType = settings->value("detectorType", -1).toInt();

      if (detType == QxrdDetectorSettings::PerkinElmerDetector) {
        if (acq) {
          if (acq->get_DetectorCount() == 0) {
            acq->appendDetector(detType);
          }
        }
      }
    }
    settings->endGroup();

    if (proc) {
      settings->beginGroup("processor");
      proc -> readSettings(settings);
      settings->endGroup();
    }

    if (m_CalibrantLibrary) {
      settings->beginGroup("calibrantLibrary");
      m_CalibrantLibrary->readSettings(settings);
      settings->endGroup();
    }

    if (srv) {
      settings->beginGroup("specserver");
      srv  -> readSettings(settings);
      settings->endGroup();
    }

    if (ssrv) {
      settings->beginGroup("simpleserver");
      ssrv -> readSettings(settings);
      settings->endGroup();
    }

    //    if (m_DatasetModel) {
    //      settings->beginGroup("dataset");
    //      m_DatasetModel->readSettings(settings, "dataset");
    //      settings->endGroup();
    //    }

    if (m_Dataset) {
      settings->beginGroup("dataset");
      m_Dataset->readSettings(settings);
      settings->endGroup();
    }

    if (get_QxrdVersion() != STR(QXRD_VERSION)) {
      set_QxrdVersion(STR(QXRD_VERSION));
    }

    defaultWindowSettings();
  }

  //  if (qcepDebug(DEBUG_PREFS)) {
  printMessage("finished QxrdExperiment::readSettings(QSettings*)");
  printf("finished QxrdExperiment::readSettings(QSettings*)\n");
  //  }

  prop_IsReading()->incValue(-1);
}

void QxrdExperiment::writeSettings()
{
//  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("started QxrdExperiment::writeSettings");
    printf("started QxrdExperiment::writeSettings\n");
//  }

  QcepMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

  QString docPath = experimentFilePath();

  if (docPath.length()>0) {
    QFile::remove(docPath+".new");

    {
      QSettings settings(docPath+".new", QSettings::IniFormat);

//      settings.beginGroup("experiment");
      writeSettings(&settings);
//      settings.endGroup();
    }

    QFile::remove(docPath+".bak");
    QFile::rename(docPath, docPath+".bak");
    QFile::rename(docPath+".new", docPath);
  } else {
    QxrdExperimentSettings settings;

//    settings.beginGroup("experiment");
    writeSettings(&settings);
//    settings.endGroup();
  }

  setChanged(0);

//  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("finished QxrdExperiment::writeSettings");
    printf("finished QxrdExperiment::writeSettings\n");
//  }
}

void QxrdExperiment::writeSettings(QSettings *settings)
{
//  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("started QxrdExperiment::writeSettings(QSettings*)");
    printf("started QxrdExperiment::writeSettings(QSettings*)\n");
//  }

    QTime tic;

    tic.start();

  if (settings) {
    QcepExperiment::writeSettings(settings);

    QxrdAcquisitionPtr acq(m_Acquisition);
    QxrdDataProcessorPtr proc(m_DataProcessor);
    QxrdServerPtr srv(m_Server);
    QxrdSimpleServerPtr ssrv(m_SimpleServer);

    if (m_Window) {
      m_Window->captureSize();
    }

    if (m_WindowSettings) {
      settings->beginGroup("window");
      m_WindowSettings -> writeSettings(settings);
      settings->endGroup();
    }

    if (acq) {
      settings->beginGroup("acquisition");
      acq -> writeSettings(settings);
      settings->endGroup();
    }

    if (proc) {
      settings->beginGroup("processor");
      proc -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_CalibrantLibrary) {
      settings->beginGroup("calibrantLibrary");
      m_CalibrantLibrary->writeSettings(settings);
      settings->endGroup();
    }

    if (srv) {
      settings->beginGroup("specserver");
      srv  -> writeSettings(settings);
      settings->endGroup();
    }

    if (ssrv) {
      settings->beginGroup("simpleserver");
      ssrv -> writeSettings(settings);
      settings->endGroup();
    }

//    if (m_DatasetModel) {
//      settings->beginGroup("dataset");
//      m_DatasetModel -> writeSettings(settings, "dataset");
//      settings->endGroup();
//    }

    if (m_Dataset) {
      settings->beginGroup("dataset");
      m_Dataset -> writeSettings(settings);
      settings->endGroup();
    }
  }

//  if (qcepDebug(DEBUG_PREFS)) {
    printMessage(tr("finished QxrdExperiment::writeSettings(QSettings*) after %1 msec").arg(tic.elapsed()));
    printf("finished QxrdExperiment::writeSettings(QSettings*) after %d msec\n", tic.elapsed());
//  }
}

void QxrdExperiment::onAutoSaveTimer()
{
//  printMessage("Auto save experiment");

//  if (m_Window) {
  //    m_Window->setChanged(isChanged());
  //  }
  if (!get_IsReading()) {
    if (isChanged()) {
      printMessage(tr("QxrdExperiment::onAutoSaveTimer saved because %1 changed").arg(changedBy()));
      printf("QxrdExperiment::onAutoSaveTimer saved because %s changed\n", qPrintable(changedBy()));

      writeSettings();
    }
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

//  settings.beginGroup("experiment");
  writeSettings(&settings);
//  settings.endGroup();
}

void QxrdExperiment::saveExperiment2(QString path)
{
  QxrdExperimentSettings settings(path);

  QcepObject::writeObject(&settings);
}

void QxrdExperiment::saveExperimentAsText(QString filePath)
{
  QcepFileFormatterPtr fmt = QcepFileFormatterPtr(new QcepFileFormatterText(filePath));

  fmt -> beginWriteFile();

  writeObjectFmt(fmt);

  fmt -> endWriteFile();
}

void QxrdExperiment::saveExperimentCopyAs(QString path)
{
  printMessage(tr("Save experiment copy as %1").arg(path));

  QxrdExperimentSettings settings(path);

//  settings.beginGroup("experiment");
  writeSettings(&settings);
//  settings.endGroup();

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

void QxrdExperiment::plotImage(QcepDoubleImageDataPtr img)
{
  if (m_Window) {
    m_Window->newDataAvailable(img, QcepMaskDataPtr());
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

void QxrdExperiment::defaultWindowSettings()
{
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdAcquisitionWindowSettings("Acquisition")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdAnalysisWindowSettings("Analysis")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdCalculatorWindowSettings("Calculator")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdCalibrantWindowSettings("Calibrant")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdCenteringWindowSettings("Centering")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdExtraIOWindowSettings("Extra IO")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdHelpWindowSettings("Help")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdInfoWindowSettings("Info")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdIntegrationWindowSettings("Integration")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdMaskingWindowSettings("Masking")));
  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdScriptingWindowSettings("Scripting")));

  for (int i=0; i<windowSettingsCount(); i++) {
    QxrdMainWindowSettingsPtr set =
        qSharedPointerDynamicCast<QxrdMainWindowSettings>(windowSettings(i));

    if (set) {
      set -> initialize(application(), experiment());
    }
  }
}
