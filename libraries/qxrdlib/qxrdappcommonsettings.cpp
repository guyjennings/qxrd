#include "qxrdappcommonsettings.h"
#include "qxrdappcommon.h"
#include "qcepallocator.h"
#include <QDir>

QxrdAppCommonSettings::QxrdAppCommonSettings(QString name,
                                             int argc,
                                             char **argv) :
  inherited(name),
  m_Argc(this, "argc", argc, "Number of Command Line Arguments"),
  m_Argv(this, "argv", makeStringListFromArgs(argc, argv), "Command Line Arguments"),
  m_GuiWanted(this, "guiWanted", 1, "GUI Wanted?"),
  m_CmdList(this, "cmdList", QStringList(), "Commands to Execute"),
  m_FileList(this, "fileList", QStringList(), "Files to Process"),
  m_WatcherList(this, "watcherList", QStringList(), "File patterns to watch for"),
  m_PluginList(this, "pluginList", QStringList(), "Plugin directories"),
  m_Debug(this,"debug", 0, "Debug Level"),
  m_OpenNew(this,"openNew", 0, "Open a new experiment"),
  m_FreshStart(this,"freshStart", 0, "Do a Fresh Start"),
  m_StartDetectors(this, "startDetectors", 1, "Start Detectors when opening experiments"),
  m_CurrentExperiment(this, "currentExperiment", "", "Current Experiment"),
  m_RecentExperiments(this, "recentExperiments", QStringList(), "Recent Experiments"),
  m_RecentExperimentsSize(this,"recentExperimentsSize", 8, "Number of Recent Experiments to Remember"),
  m_ExperimentCount(this, "experimentCount", 0, "Number of open experiments"),
  m_CurrentDirectory(this, "currentDirectory", QDir::homePath(), "Current Directory"),
  //  m_OpenDirectly(m_Saver, this,"openDirectly", false, "Open Last Experiment at Startup"),
  m_FileBrowserLimit(this, "fileBrowserLimit", 1000, "Max Number of Files in Browser Windows (0 = unlimited)"),
  m_MessageWindowLines(this, "messageWindowLines", 1000, "Number of Lines in Message Window (0 = unlimited)"),
  m_UpdateIntervalMsec(this, "updateIntervalMsec", 1000, "Time Intervale for Updates (in msec)")
{
  m_Allocator = QcepAllocatorPtr(
        new QcepAllocator("allocator"));
}

QxrdAppCommonSettings::~QxrdAppCommonSettings()
{
}

void QxrdAppCommonSettings::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Allocator->initialize(parent);
}

QcepAllocatorWPtr QxrdAppCommonSettings::allocator() const
{
  return m_Allocator;
}

void QxrdAppCommonSettings::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    settings->beginGroup("allocator");
    alloc->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdAppCommonSettings::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  QcepAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    settings->beginGroup("allocator");
    alloc->writeSettings(settings);
    settings->endGroup();
  }
}

QStringList QxrdAppCommonSettings::makeStringListFromArgs(int argc, char **argv)
{
  QStringList res;

  for (int i=0; i<argc; i++) {
    res.append(argv[i]);
  }

  return res;
}

void QxrdAppCommonSettings::appendCommand(QString cmd)
{
  prop_CmdList()->appendValue(cmd);
}

void QxrdAppCommonSettings::appendScript(QString script)
{
  prop_CmdList()->appendValue(tr("loadScript(\"%1\")").arg(script));
}

void QxrdAppCommonSettings::appendFile(QString file)
{
  prop_FileList()->appendValue(file);
}

void QxrdAppCommonSettings::appendWatcher(QString patt)
{
  prop_WatcherList()->appendValue(patt);
}

void QxrdAppCommonSettings::appendPlugin(QString dir)
{
  prop_PluginList()->appendValue(dir);
}
