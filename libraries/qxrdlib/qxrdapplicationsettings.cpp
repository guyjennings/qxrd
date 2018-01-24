#include "qxrdapplicationsettings.h"
#include "qxrdapplication.h"
#include "qcepallocator.h"
#include <QDir>

QxrdApplicationSettings::QxrdApplicationSettings(QxrdApplicationWPtr app, int argc, char **argv) :
  QcepApplicationSettings(app),
  m_RecentExperiments(this, "recentExperiments", QStringList(), "Recent Experiments"),
  m_RecentExperimentsSize(this,"recentExperimentsSize", 8, "Number of Recent Experiments to Remember"),
  m_CurrentExperiment(this, "currentExperiment", "", "Current Experiment"),
  m_CurrentDirectory(this, "currentDirectory", QDir::homePath(), "Current Directory"),
  //  m_OpenDirectly(m_Saver, this,"openDirectly", false, "Open Last Experiment at Startup"),
  m_Debug(this,"debug", 0, "Debug Level"),
  m_OpenNew(this,"openNew", 0, "Open a new experiment"),
  m_FreshStart(this,"freshStart", 0, "Do a Fresh Start"),
  m_FileBrowserLimit(this, "fileBrowserLimit", 1000, "Max Number of Files in Browser Windows (0 = unlimited)"),
  m_MessageWindowLines(this, "messageWindowLines", 1000, "Number of Lines in Message Window (0 = unlimited)"),
  m_UpdateIntervalMsec(this, "updateIntervalMsec", 1000, "Time Intervale for Updates (in msec)"),
  m_Argc(this, "argc", argc, "Number of Command Line Arguments"),
  m_Argv(this, "argv", makeStringListFromArgs(argc, argv), "Command Line Arguments"),
  m_GuiWanted(this, "guiWanted", 1, "GUI Wanted?"),
  m_StartDetectors(this, "startDetectors", 1, "Start Detectors when opening experiments"),
  m_CmdList(this, "cmdList", QStringList(), "Commands to Execute"),
  m_FileList(this, "fileList", QStringList(), "Files to Process"),
  m_WatcherList(this, "watcherList", QStringList(), "File patterns to watch for"),
  m_LockerCount(this, "lockerCount", 0, "Number of mutex locks taken"),
  m_LockerRate(this, "lockerRate", 0, "Mutex Locking Rate"),
  m_ExperimentCount(this, "experimentCount", 0, "Number of open experiments")
{
#ifndef QT_NO_DEBUG
  printf("Constructing application settings\n");
#endif
}

QxrdApplicationSettings::~QxrdApplicationSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting application settings\n");
#endif
}

void QxrdApplicationSettings::init()
{
  m_Allocator = QcepAllocatorPtr(
        new QcepAllocator("allocator"));
}

QStringList QxrdApplicationSettings::makeStringListFromArgs(int argc, char **argv)
{
  QStringList res;

  for (int i=0; i<argc; i++) {
    res.append(argv[i]);
  }

  return res;
}

QcepAllocatorWPtr QxrdApplicationSettings::allocator() const
{
  return m_Allocator;
}

void QxrdApplicationSettings::readSettings(QSettings *settings)
{
  QcepApplicationSettings::readSettings(settings);

  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    settings->beginGroup("allocator");
    alloc->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdApplicationSettings::writeSettings(QSettings *settings)
{
  QcepApplicationSettings::writeSettings(settings);

  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    settings->beginGroup("allocator");
    alloc->writeSettings(settings);
    settings->endGroup();
  }
}

void QxrdApplicationSettings::appendCommand(QString cmd)
{
  prop_CmdList()->appendValue(cmd);
}

void QxrdApplicationSettings::appendScript(QString script)
{
  prop_CmdList()->appendValue(tr("loadScript(\"%1\")").arg(script));
}

void QxrdApplicationSettings::appendFile(QString file)
{
  prop_FileList()->appendValue(file);
}

void QxrdApplicationSettings::appendWatcher(QString patt)
{
  prop_WatcherList()->appendValue(patt);
}
