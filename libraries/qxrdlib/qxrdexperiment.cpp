#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qcepmacros.h"
#include "qcepallocator.h"
#include "qxrdexperiment.h"
#include "qxrdexperimentthread.h"
#include "qxrdapplication-ptr.h"
#include "qxrdapplication.h"
#include "qxrdprocessor.h"
#include "qxrdprocessorexecution.h"
#include "qcepcenterfinder.h"
#include "qxrdpolartransform.h"
#include "qxrdpolarnormalization.h"
#include "qceppowderpoint.h"
#include "qcepcalibrantlibrary.h"
#include "qcepcalibrantlibrarymodel.h"
#include "qcepcalibrantdspacings.h"
#include "qcepcalibrantdspacingsmodel.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdacqdummy.h"
#include "qcepspecserversettings.h"
#include "qcepspecserverthread.h"
#include "qcepspecserver.h"
#include "qcepsimpleserversettings.h"
#include "qcepsimpleserverthread.h"
#include "qcepsimpleserver.h"
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
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdsynchronizedacquisition.h"
#include "qcepdataexportparameters.h"
#include "qcepdataimportparameters.h"
#include <QHostInfo>
#include <QColorDialog>
#include <QProcess>
#include "qxrdwindowsettings.h"
#include "qxrdgeneratetestimage.h"
#include "qxrdjsengine.h"
#include "qxrdintegrator.h"
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
#include "qxrdwatcherwindowsettings.h"
#include "qxrdscriptingwindowsettings.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrdwindowsettings.h"
#include "qxrdhelpwindowsettings.h"
#include "qcepstartupwindowsettings.h"
#include "qxrdwatcherwindowsettings-ptr.h"
#include "qxrdwatcherwindowsettings.h"
#include "qxrdhistogramdialogsettings.h"

QxrdExperiment::QxrdExperiment(QString name) :
  inherited(name),
  m_Application(),
  m_SpecServerThread(NULL),
  m_SpecServer(),
  m_SimpleServerThread(NULL),
  m_SimpleServer(),
  m_Processor(),
  m_Acquisition(),
  m_FileSaverThread(NULL),
  m_FileSaver(),
  m_ScriptEngine(),
  m_ScriptEngineJS(),
  m_LogFile(NULL),
  m_ScanFile(NULL),
  m_ExperimentFileMutex(),
  m_WasAutoSaved(false),

  m_ExperimentMode(this, "experimentMode", -1, "Experiment Mode"),
  m_QxrdVersion(this, "qxrdVersion", g_Application -> applicationVersion(), "Qxrd Version"),
  m_DataDirectory(this, "dataDirectory", defaultDataDirectory(""), "Saved Data Directory"),
  m_LogFileName(this, "logFileName", defaultLogName(""), "Log File Name"),
  m_LogMaxLengthMB(this, "logMaxLengthMB", 0, "Max log file length in MB (or 0 for unlimited)"),
  m_LogMaxSaved(this, "logMaxSaved", 0, "Number of saved log files (or 0 for unlimited)"),
  m_ScanFileName(this, "scanFileName", defaultScanName(""), "Scan File Name"),
  m_ScanFileExtension(this, "scanFileExtension", ".avg", "Scan File Extension"),
  m_ScanDataNegative(this, "scanDataNegative", 0, "Scan Data Negative Value Handling"),
  m_ScanMaxLengthMB(this, "scanMaxLengthMB", 0, "Max scan file length in MB (or 0 for unlimited)"),
  m_ScanMaxSaved(this, "scanMaxSaved", 0, "Number of saved scan files (or 0 for unlimited)"),
  m_DefaultLayout(this,"defaultLayout",0, "Default Layout Used?"),
  m_WorkCompleted(this, "workCompleted", 0, "Amount of Work Completed"),
  m_WorkTarget(this, "workTarget", 0, "Amount of Work Targetted"),
  m_CompletionPercentage(this, "completionPercentage", 0, "Percentage of Work Completed"),
  m_DefaultScript(this, "defaultScript", "", "Default script for experiment"),
  m_ExtraScriptFiles(this, "extraScriptFiles", QStringList(), "Additional script files for experiment"),
  m_IsReading(this, "isReading", 0, "Is being read?"),
  m_CompressOutputBZ2(this, "compressOutputBZ2", 0, "Compress output files with bzip2"),
  m_CompressLevelBZ2(this, "compressLevelBZ2", 3, "Compression level (1-9) for output files with bzip2")
{
#ifndef QT_NO_DEBUG
  printf("Constructing experiment\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperiment::QxrdExperiment(%p)\n", this);
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

  closeScanFile();
  closeLogFile();

  QxrdAppCommonPtr app(m_Application);

  if (app) {
    app->prop_ExperimentCount()->incValue(-1);
  }
}

QxrdExperimentWPtr QxrdExperiment::findExperiment(QcepObjectWPtr p)
{
  QxrdExperimentWPtr res =
      qSharedPointerDynamicCast<QxrdExperiment>(p);

  if (res == NULL) {
    QcepObjectPtr objp(p);

    if (objp) {
      res = findExperiment(objp->parentPtr());
    }
  }

  return res;
}

QxrdExperimentThreadWPtr QxrdExperiment::experimentThread() const
{
  QxrdExperimentThread* t =
      qobject_cast<QxrdExperimentThread*>(thread());

  return QxrdExperimentThreadPtr(t);
}

void QxrdExperiment::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QxrdExperiment::initialize(QcepObjectWPtr parent,
                                QString path,
                                int mode)
{
  set_ExperimentMode(mode);

  inherited::initialize(parent, path);

  m_Application =
      QxrdAppCommon::findApplication(parent);

  m_ExperimentThread =
      QxrdExperimentThread::findExperimentThread(parent);

  QxrdAppCommonPtr appl(m_Application);

  if (appl) {
    appl->prop_ExperimentCount()->incValue(1);
  }

  m_Processor =
      QxrdProcessorPtr(
        new QxrdProcessor("processor"));

  if (get_ExperimentMode() == QxrdExperiment::AcquisitionAllowed) {
    m_Acquisition =
        QxrdAcqCommonPtr(
          new QxrdAcquisition("acquisition"));
  } else {
    m_Acquisition =
        QxrdAcqCommonPtr(
          new QxrdAcqDummy("dummyAcq"));
  }

  m_CalibrantLibrary =
      QcepCalibrantLibraryPtr(
        new QcepCalibrantLibrary("calibrantLibrary"));

  m_CalibrantLibraryModel =
      QcepCalibrantLibraryModelPtr(
        new QcepCalibrantLibraryModel(m_CalibrantLibrary));

  m_CalibrantDSpacings =
      QcepCalibrantDSpacingsPtr(
        new QcepCalibrantDSpacings());

  m_CalibrantDSpacingsModel =
      QcepCalibrantDSpacingsModelPtr(
        new QcepCalibrantDSpacingsModel(m_CalibrantLibrary,
                                        m_CalibrantDSpacings));

  QxrdAppCommonPtr app(m_Application);

  if (app) {
    QxrdExperimentPtr myself(qSharedPointerDynamicCast<QxrdExperiment>(sharedFromThis()));

    splashMessage("Initializing File Saver");

    m_FileSaverThread = QxrdFileSaverThreadPtr(
          new QxrdFileSaverThread("fileSaverThread"));
    m_FileSaverThread -> initialize(sharedFromThis());
    m_FileSaverThread -> start();

    m_FileSaver = m_FileSaverThread -> fileSaver();

    splashMessage("Initializing Data Processing");

    m_Processor -> initialize(sharedFromThis());

    splashMessage("Initializing Data Acquisition");

    m_Acquisition -> initialize(sharedFromThis());

    m_CalibrantLibrary -> initialize(sharedFromThis());

    m_CalibrantDSpacings -> initialize(sharedFromThis());

//    QxrdApplicationPtr appp(
//          qSharedPointerDynamicCast<QxrdApplication>(app));

    if (m_Acquisition) {
      connect(m_Acquisition.data(), &QxrdAcqCommon::acquireStarted, this, &QxrdExperiment::acquireStarted);
      connect(m_Acquisition.data(), &QxrdAcqCommon::acquireComplete, this, &QxrdExperiment::acquireComplete);
      connect(m_Acquisition.data(), &QxrdAcqCommon::acquiredFrame, this, &QxrdExperiment::acquiredFrame);
    }

    m_Dataset = QcepAllocator::newDataset(sharedFromThis(), "dataset");

    m_DatasetModel = QcepDatasetModelPtr(
          new QcepDatasetModel(myself, m_Dataset));

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

    m_ScriptEngine = QxrdScriptEnginePtr(
          new QxrdScriptEngine("scriptEngine"));
    m_ScriptEngine -> initialize(sharedFromThis());

    m_ScriptEngineJS = QxrdJSEnginePtr(
          new QxrdJSEngine("jsEngine"));
    m_ScriptEngineJS -> initialize(sharedFromThis());

    splashMessage("Starting SPEC Server");

    m_SpecServerSettings =
        QcepSpecServerSettingsPtr(
          new QcepSpecServerSettings("specServerSettings"));
    m_SpecServerSettings -> initialize(sharedFromThis());

    m_SpecServerThread = QcepSpecServerThreadPtr(
          new QcepSpecServerThread("specServerThread"));
    m_SpecServerThread -> initialize(sharedFromThis(),
                                     m_SpecServerSettings,
                                     m_ScriptEngine);
    m_SpecServerThread -> start();

    m_SpecServer = m_SpecServerThread -> specServer();

    splashMessage("Starting Simple Socket Server");

    m_SimpleServerSettings =
        QcepSimpleServerSettingsPtr(
          new QcepSimpleServerSettings("simpleServerSettings"));
    m_SimpleServerSettings -> initialize(sharedFromThis());

    m_SimpleServerThread = QcepSimpleServerThreadPtr(
          new QcepSimpleServerThread("simpleServerThread"));
    m_SimpleServerThread -> initialize(sharedFromThis(),
                                       m_SimpleServerSettings,
                                       m_ScriptEngine);
    m_SimpleServerThread -> start();

    m_SimpleServer = m_SimpleServerThread -> simpleServer();

    QxrdFileSaverPtr saver(m_FileSaver);

    if (saver) {
      saver -> initialize(m_FileSaverThread);
      saver -> setProcessor(m_Processor);
      saver -> setExperiment(myself);
      saver -> setAcquisition(m_Acquisition);
    }

#ifdef Q_OS_WIN32
    QDir::setCurrent(QDir::homePath());
#endif

    printMessage(tr("------- Starting new session -------"));
    printMessage(tr("Experiment name: %1").arg(get_ExperimentName()));
    printMessage(tr("Experiment Directory: %1").arg(get_ExperimentDirectory()));
    printMessage(tr("Stored in file: %1").arg(get_ExperimentFileName()));

    QString about = app -> applicationVersion();

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
  }
}

void QxrdExperiment::registerMetaTypes()
{
  inherited::registerMetaTypes();

  qRegisterMetaType< QList<QwtLegendData> >("QList<QwtLegendData>");
  qRegisterMetaType<QxrdExperiment*>("QxrdExperiment*");

  QxrdAcquisition::registerMetaTypes();
  QxrdAcqDummy::registerMetaTypes();

  qRegisterMetaType<QcepCalibrantWPtr>("QcepCalibrantWPtr");
  qRegisterMetaType<QcepCalibrantDSpacing>("QcepCalibrantDSpacing");
  qRegisterMetaType<QcepCalibrantDSpacings>("QcepCalibrantDSpacings");
  qRegisterMetaType<QcepCalibrantLibrary*>("QcepCalibrantLibrary*");
  qRegisterMetaType<QcepCenterFinder*>("QcepCenterFinder*");

  QxrdProcessor::registerMetaTypes();
  QxrdProcessorExecution::registerMetaTypes();

  qRegisterMetaType<QxrdFileBrowserModelUpdater*>("QxrdFileBrowserModelUpdater*");
  qRegisterMetaType<QxrdFileSaver*>("QxrdFileSaver*");
  qRegisterMetaType<QxrdIntegrator*>("QxrdIntegrator*");
  qRegisterMetaType<QcepDetectorGeometry*>("QxrdDetectorGeometry*");
  qRegisterMetaType<QxrdPolarNormalization*>("QxrdPolarNormalization*");
  qRegisterMetaType<QxrdPolarTransform*>("QxrdPolarTransform*");
  qRegisterMetaType<QcepPowderPoint>("QxrdPowderPoint");
  qRegisterMetaType<QcepPowderPointVector>("QxrdPowderPointVector");
  qRegisterMetaType<QxrdScriptEngine*>("QxrdScriptEngine*");
  qRegisterMetaType<QcepServer*>("QcepServer*");
  qRegisterMetaType<QcepSpecServer*>("QcepSpecServer*");
  qRegisterMetaType<QcepSpecServerThread*>("QcepSpecServerThread*");
  qRegisterMetaType<QcepSimpleServer*>("QcepSimpleServer*");
  qRegisterMetaType<QcepSimpleServerThread*>("QcepSimpleServerThread*");

  qRegisterMetaType<QxrdWindowSettings*>("QxrdWindowSettings*");
  qRegisterMetaType<QxrdGenerateTestImage*>("QxrdGenerateTestImage*");
  qRegisterMetaType<QxrdMainWindowSettings*>("QxrdMainWindowSettings*");
  qRegisterMetaType<QxrdAcquisitionWindowSettings*>("QxrdAcquisitionWindowSettings*");
  qRegisterMetaType<QxrdFileSaverThread*>("QxrdFileSaverThread*");

  QxrdExtraIOWindowSettings::registerMetaTypes();
  QxrdHistogramDialogSettings::registerMetaTypes();

  qRegisterMetaType<QxrdAnalysisWindowSettings*>("QxrdAnalysisWindowSettings*");
  qRegisterMetaType<QxrdInfoWindowSettings*>("QxrdInfoWindowSettings*");
  qRegisterMetaType<QxrdCalculatorWindowSettings*>("QxrdCalculatorWindowSettings*");
  qRegisterMetaType<QxrdCalibrantWindowSettings*>("QxrdCalibrantWindowSettings*");
  qRegisterMetaType<QxrdHelpWindowSettings*>("QxrdHelpWindowSettings*");
  qRegisterMetaType<QxrdCenteringWindowSettings*>("QxrdCenteringWindowSettings*");
  qRegisterMetaType<QxrdWatcherWindowSettings*>("QxrdWatcherWindowSettings*");
  qRegisterMetaType<QxrdIntegrationWindowSettings*>("QxrdIntegrationWindowSettings*");
  qRegisterMetaType<QxrdMaskingWindowSettings*>("QxrdMaskingWindowSettings*");
  qRegisterMetaType<QxrdScriptingWindowSettings*>("QxrdScriptingWindowSettings*");
  qRegisterMetaType<QxrdWindowSettings*>("QxrdWindowSettings*");
  qRegisterMetaType<QcepStartupWindowSettings*>("QxrdStartupWindowSettings*");
}

void QxrdExperiment::openWindow(QxrdMainWindowSettingsWPtr set)
{
  QxrdAppCommonPtr app(m_Application);

  if (app && set) {
    QxrdAppCommon *appp = app.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(appp, [=]() { app->openWindow(set); })
    )
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(appp, "openWindow", Q_ARG(QxrdMainWindowSettingsWPtr, set))
    )
#endif
  }
}

void QxrdExperiment::openWindows()
{
  GUI_THREAD_CHECK;

  inherited::openWindows();
}

void QxrdExperiment::closeWindows()
{
  GUI_THREAD_CHECK;

  inherited::closeWindows();
}

QxrdAppCommonWPtr QxrdExperiment::application() const
{
  return m_Application;
}

QxrdAcqCommonWPtr QxrdExperiment::acquisition() const
{
  return m_Acquisition;
}

QxrdFileSaverWPtr QxrdExperiment::fileSaver() const
{
  return m_FileSaver;
}

QcepSpecServerSettingsWPtr QxrdExperiment::specServerSettings()
{
  return m_SpecServerSettings;
}

QcepSpecServerWPtr QxrdExperiment::specServer()
{
  return m_SpecServer;
}

QcepSpecServerThreadWPtr QxrdExperiment::specServerThread()
{
  return m_SpecServerThread;
}

QcepSimpleServerSettingsWPtr QxrdExperiment::simpleServerSettings()
{
  return m_SimpleServerSettings;
}

QcepSimpleServerWPtr QxrdExperiment::simpleServer()
{
  return m_SimpleServer;
}

QcepSimpleServerThreadWPtr QxrdExperiment::simpleServerThread()
{
  return m_SimpleServerThread;
}

QxrdProcessorWPtr QxrdExperiment::processor() const
{
  return m_Processor;
}

QcepCalibrantLibraryWPtr QxrdExperiment::calibrantLibrary() const
{
  return m_CalibrantLibrary;
}

QcepCalibrantLibraryModelWPtr QxrdExperiment::calibrantLibraryModel() const
{
  return m_CalibrantLibraryModel;
}

QcepCalibrantDSpacingsWPtr QxrdExperiment::calibrantDSpacings() const
{
  return m_CalibrantDSpacings;
}

QcepCalibrantDSpacingsModelWPtr QxrdExperiment::calibrantDSpacingsModel() const
{
  return m_CalibrantDSpacingsModel;
}

QcepDatasetModelPtr QxrdExperiment::dataset()
{
  return m_DatasetModel;
}

QcepCenterFinderWPtr QxrdExperiment::centerFinder() const
{
  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    return dp->centerFinder();
  } else {
    return QcepCenterFinderWPtr();
  }
}

QxrdIntegratorWPtr QxrdExperiment::integrator() const
{
  QxrdProcessorPtr dp(m_Processor);

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

void QxrdExperiment::openNewLogFile()
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

void QxrdExperiment::openLogFile()
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

  if (backupFileIfNeeded(m_LogFile, get_LogMaxLengthMB(), logFilePath(), get_LogMaxSaved())) {
    m_LogFile = fopen(qPrintable(logFilePath()), "a");

    if (m_LogFile) {
      fprintf(m_LogFile, "#F %s\n", qPrintable(get_LogFileName()));
      fprintf(m_LogFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
      fprintf(m_LogFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
      fflush(m_LogFile);
    }
  }
}

QString QxrdExperiment::backupFilePath(QString path, int n)
{
  return path+tr(".%1.bak").arg(n);
}

bool QxrdExperiment::backupFileIfNeeded(FILE *f, int maxLenMB, QString path, int nBackups)
{
  bool needsReopen = false;

  if (maxLenMB && f) {
    QFile file;
    file.open(f, QIODevice::ReadWrite);

    qint64 pos = file.size();

    if (pos > (((qint64)maxLenMB)*QcepAllocator::MegaBytes)) {
      fclose(f);

      int maxSave = nBackups;

      if (nBackups == 0) {
        for (int i=0; ;i++) {
          QFileInfo info(backupFilePath(path, i));

          if (info.exists()) {
            maxSave++;
          } else {
            break;
          }
        }
      }

      if (nBackups) {
        remove(qPrintable(backupFilePath(path, maxSave)));
      }

      for (int i=maxSave; i>=1; i--) {
        rename(qPrintable(backupFilePath(path, i-1)),
               qPrintable(backupFilePath(path, i)));
      }

      rename(qPrintable(path),
             qPrintable(backupFilePath(path, 0)));

      needsReopen = true;
    }
  }

  return needsReopen;
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

      //TODO: replace
//      m_Window->initialLogEntry(buff);
    }

    fclose(logFile);
  }
}

FILE* QxrdExperiment::logFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  return m_LogFile;
}

void QxrdExperiment::logMessage(QString msg)
{
  openLogFile();

  QcepMutexLocker lock(__FILE__, __LINE__, &m_LogFileMutex);

  if (m_LogFile) {
    fprintf(m_LogFile, "#CX %s\n", qPrintable(msg));
    fflush(m_LogFile);
  }
}

void QxrdExperiment::closeLogFile()
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
      fprintf(m_ScanFile, "#F %s\n", qPrintable(get_ScanFileName()));
      fprintf(m_ScanFile, "#E %d\n", QDateTime::currentDateTime().toTime_t());
      fprintf(m_ScanFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
      fflush(m_ScanFile);
    }
  }

  if (backupFileIfNeeded(m_ScanFile, get_ScanMaxLengthMB(), scanFilePath(), get_ScanMaxSaved())) {
    m_ScanFile = fopen(qPrintable(scanFilePath()), "a");

    if (m_ScanFile) {
      fprintf(m_ScanFile, "#F %s\n", qPrintable(get_ScanFileName()));
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

void QxrdExperiment::readExperimentSettings()
{
  prop_IsReading()->incValue(1);

  splashMessage("Loading Experiment");

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("started QxrdExperiment::readExperimentSettings");
  }

  QcepMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

  QString docPath = experimentFilePath();

  if (docPath.length()>0) {
    QSettings settings(docPath, QSettings::IniFormat);

    readSettings(&settings);
  } else {
    QxrdExperimentSettings settings;

    readSettings(&settings);
  }

  splashMessage("Loading Background Images");

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    proc -> loadDefaultImages();
  }

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("finished QxrdExperiment::readExperimentSettings");
  }

  prop_IsReading()->incValue(-1);

  setChanged(0);

  splashMessage("Experiment loaded");
}

void QxrdExperiment::readSettings(QSettings *settings)
{
  prop_IsReading()->incValue(1);

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("started QxrdExperiment::readSettings(QSettings*)");
  }

  if (settings) {
    inherited::readSettings(settings);

    splashMessage("Reading Acquisition Settings");

    if (m_Acquisition) {
      settings->beginGroup("acquisition");
      m_Acquisition -> readSettings(settings);
      settings->endGroup();
    }

    settings->beginGroup("experiment");
    if (settings->contains("detectorType")) {
      int detType = settings->value("detectorType", -1).toInt();

      if (detType == QxrdDetectorSettings::PerkinElmer) {
        if (m_Acquisition) {
          if (m_Acquisition->detectorCount() == 0) {
            QxrdDetectorSettingsPtr det = m_Acquisition->newDetector(detType);

            m_Acquisition->appendDetector(det);
          }
        }
      }
    }
    settings->endGroup();

    splashMessage("Reading Processor Settings");

    if (m_Processor) {
      settings->beginGroup("processor");
      m_Processor -> readSettings(settings);
      settings->endGroup();
    }

    splashMessage("Reading Calibrant Library");

    if (m_CalibrantLibrary) {
      settings->beginGroup("calibrantLibrary");
      m_CalibrantLibrary->readSettings(settings);
      settings->endGroup();
    }

    splashMessage("Reading Spec Server Settings");

    if (m_SpecServerSettings) {
      settings->beginGroup("specServerSettings");
      m_SpecServerSettings  -> readSettings(settings);
      settings->endGroup();
    }

    splashMessage("Reading Simple Server Settings");

    if (m_SimpleServerSettings) {
      settings->beginGroup("simpleServerSettings");
      m_SimpleServerSettings -> readSettings(settings);
      settings->endGroup();
    }

    splashMessage("Reading Dataset Settings");

    if (m_Dataset) {
      settings->beginGroup("dataset");
      m_Dataset->readSettings(settings);
      settings->endGroup();
    }

    if (get_QxrdVersion() != g_Application->applicationVersion()) {
      set_QxrdVersion(g_Application->applicationVersion());
    }

    int nWin  = windowSettingsCount();
    int nOpened = 0;

    for (int i=0; i<nWin; i++) {
      QcepMainWindowSettingsPtr set = windowSettings(i);

      if (set) {
        if (set->get_WindowOpen()) {
          nOpened += 1;
        }
      }
    }

    if (nOpened == 0) {
      QcepMainWindowSettingsPtr set = windowSettings(0);

      if (set) {
        set -> set_WindowOpen(true);
      }
    }
  }

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("finished QxrdExperiment::readSettings(QSettings*)");
  }

  prop_IsReading()->incValue(-1);

  connect(&m_AutoSaveTimer, &QTimer::timeout, this, &QxrdExperiment::autoSaveExperiment);

  m_AutoSaveTimer.start(5000);
}

void QxrdExperiment::writeExperimentSettings()
{
  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("started QxrdExperiment::writeExperimentSettings");
  }

  QcepMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

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

  setChanged(0);

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("finished QxrdExperiment::writeExperimentSettings");
  }
}

void QxrdExperiment::writeSettings(QSettings *settings)
{
  if (qcepDebug(DEBUG_PREFS)) {
    printMessage("started QxrdExperiment::writeSettings(QSettings*)");
  }

  QTime tic;

  tic.start();

  if (settings) {
    inherited::writeSettings(settings);

    if (m_Acquisition) {
      settings->beginGroup("acquisition");
      m_Acquisition -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_Processor) {
      settings->beginGroup("processor");
      m_Processor -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_CalibrantLibrary) {
      settings->beginGroup("calibrantLibrary");
      m_CalibrantLibrary->writeSettings(settings);
      settings->endGroup();
    }

    if (m_SpecServerSettings) {
      settings->beginGroup("specServerSettings");
      m_SpecServerSettings  -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_SimpleServerSettings) {
      settings->beginGroup("simpleServerSettings");
      m_SimpleServerSettings -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_Dataset) {
      settings->beginGroup("dataset");
      m_Dataset -> writeSettings(settings);
      settings->endGroup();
    }
  }

  if (qcepDebug(DEBUG_PREFS)) {
    printMessage(tr("finished QxrdExperiment::writeSettings(QSettings*) after %1 msec").arg(tic.elapsed()));
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
  set_ExperimentPath(path);
  set_ExperimentDirectory(defaultExperimentDirectory(path));
  set_ExperimentFileName(defaultExperimentFileName(path));
  set_ExperimentName(defaultExperimentName(path));
  set_LogFileName(defaultLogName(path));
  set_ScanFileName(defaultScanName(path));

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
  writeExperimentSettings();
}

void QxrdExperiment::saveExperimentAs(QString path)
{
  QxrdExperimentSettings settings(path);

  setExperimentFilePath(path);

  writeSettings(&settings);
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

  writeSettings(&settings);
}

void QxrdExperiment::shutdownAndSave()
{
  THREAD_CHECK;

  haltAutosaveTimer();

  if (isChanged() || wasAutoSaved()) {
    saveExperiment();
  }
}

void QxrdExperiment::haltAutosaveTimer()
{
  m_AutoSaveTimer.stop();
}

void QxrdExperiment::autoSaveExperiment()
{
  THREAD_CHECK;

  if (isChanged()) {
    QString cb = changedBy();

    printMessage(tr("Autosave experiment because %1 changed").arg(cb));

    if (qcepDebug(DEBUG_PREFS)) {
      printMessage("started QxrdExperiment::autoSaveExperiment");
    }

    QcepMutexLocker lock(__FILE__, __LINE__, &m_ExperimentFileMutex);

    QString docPath = experimentFilePath()+".auto";

    if (docPath.length()>0) {
      QFile::remove(docPath);

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

    setChanged(0);

    m_WasAutoSaved = true;

    if (qcepDebug(DEBUG_PREFS)) {
      printMessage("finished QxrdExperiment::autoSaveExperiment");
    }
  }
}

int QxrdExperiment::wasAutoSaved()
{
  return m_WasAutoSaved;
}

void QxrdExperiment::openWatcher(QString patt)
{
  THREAD_CHECK;

  QxrdWatcherWindowSettingsPtr set(
        new QxrdWatcherWindowSettings("watcherWindowSettings"));

  if (set) {
    set -> initialize(sharedFromThis());

    set -> set_WindowOpen(true);
    set -> set_Pattern(patt);
  }

  appendWindowSettings(set);

  openWindow(set);
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

void QxrdExperiment::acquireStarted()
{
  commenceWork(100);
}

void QxrdExperiment::acquireComplete()
{
  finishedWork(100);
}

void QxrdExperiment::acquiredFrame(QString fileName,
                                   int iphase,
                                   int nphases,
                                   int isum,
                                   int nsum,
                                   int igroup,
                                   int ngroup)
{
  int totalFrames = (nphases*nsum*ngroup <= 0 ? 1 : nphases*nsum*ngroup);
  int thisFrame   = igroup*nphases*nsum + isum*nphases + iphase + 1;
  int pctComplete = thisFrame*100/totalFrames;

  set_WorkCompleted(pctComplete);
}

void QxrdExperiment::setupAcquisition()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    acq -> setupAcquisition();
  }
}
void QxrdExperiment::plotImage(QcepDoubleImageDataPtr img)
{
  //TODO: replace
//  if (m_Window) {
//    m_Window->newDataAvailable(img, QcepMaskDataPtr());
//  }
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

void QxrdExperiment::defaultWindowSettings()
{
  THREAD_CHECK;

  inherited::defaultWindowSettings();

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdWindowSettings(
                           "mainWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdAcquisitionWindowSettings(
                           "acquisitionWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdAnalysisWindowSettings(
                           "analysisWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdCalculatorWindowSettings(
                           "calculatorWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdCalibrantWindowSettings(
                           "calibrantWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdCenteringWindowSettings(
                           "centeringWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdWatcherWindowSettings(
                           "watcherWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdExtraIOWindowSettings(
                           "extraIOWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdHelpWindowSettings(
                           "helpWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdInfoWindowSettings(
                           "infoWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdIntegrationWindowSettings(
                           "integrationWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdMaskingWindowSettings(
                           "maskingWindowSettings")));

  appendWindowSettings(QcepMainWindowSettingsPtr(
                         new QxrdScriptingWindowSettings(
                           "scriptingWindowSettings")));
}
