#include "qxrdappcommonsettings.h"
#include "qxrdappcommon.h"
#include "qcepallocator.h"

QxrdAppCommonSettings::QxrdAppCommonSettings(QxrdAppCommonWPtr app, int argc, char **argv)
  : inherited(app),
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
    m_RecentExperiments(this, "recentExperiments", QStringList(), "Recent Experiments"),
    m_RecentExperimentsSize(this,"recentExperimentsSize", 8, "Number of Recent Experiments to Remember")
{

}

QxrdAppCommonSettings::~QxrdAppCommonSettings()
{
}

void QxrdAppCommonSettings::init()
{
  m_Allocator = QcepAllocatorPtr(
        new QcepAllocator("allocator"));
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
