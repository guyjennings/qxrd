#ifndef QXRDAPPCOMMONSETTINGS_H
#define QXRDAPPCOMMONSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepapplicationsettings.h"
#include "qxrdappcommon-ptr.h"
#include "qcepallocator-ptr.h"

class QXRD_EXPORT QxrdAppCommonSettings : public QcepApplicationSettings
{
  Q_OBJECT

private:
  typedef QcepApplicationSettings inherited;

public:
  QxrdAppCommonSettings(QxrdAppCommonWPtr app, int argc, char **argv);
  virtual ~QxrdAppCommonSettings();
  virtual void init();

  QcepAllocatorWPtr allocator() const;

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void appendCommand(QString cmd);
  void appendScript(QString script);
  void appendFile(QString file);
  void appendWatcher(QString patt);
  void appendPlugin(QString dir);

private:
  QStringList makeStringListFromArgs(int argc, char **argv);

private:
  QcepAllocatorPtr                m_Allocator;

public:
  Q_PROPERTY(int    argc       READ get_Argc      WRITE set_Argc STORED false)
  QCEP_INTEGER_PROPERTY(Argc)

  Q_PROPERTY(QStringList argv READ get_Argv WRITE set_Argv STORED false)
  QCEP_STRING_LIST_PROPERTY(Argv)

  Q_PROPERTY(int    guiWanted  READ get_GuiWanted WRITE set_GuiWanted STORED false)
  QCEP_INTEGER_PROPERTY(GuiWanted)

  Q_PROPERTY(QStringList cmdList READ get_CmdList WRITE set_CmdList STORED false)
  QCEP_STRING_LIST_PROPERTY(CmdList)

  Q_PROPERTY(QStringList fileList READ get_FileList WRITE set_FileList STORED false)
  QCEP_STRING_LIST_PROPERTY(FileList)

  Q_PROPERTY(QStringList watcherList READ get_WatcherList WRITE set_WatcherList STORED false)
  QCEP_STRING_LIST_PROPERTY(WatcherList)

  Q_PROPERTY(QStringList pluginList READ get_PluginList WRITE set_PluginList STORED false)
  QCEP_STRING_LIST_PROPERTY(PluginList)

  Q_PROPERTY(qint64    debug         READ get_Debug WRITE set_Debug)
  QCEP_INTEGER64_PROPERTY(Debug)

  Q_PROPERTY(int    openNew         READ get_OpenNew WRITE set_OpenNew STORED false)
  QCEP_INTEGER_PROPERTY(OpenNew)

  Q_PROPERTY(int    freshStart         READ get_FreshStart WRITE set_FreshStart STORED false)
  QCEP_INTEGER_PROPERTY(FreshStart)

  Q_PROPERTY(int    startDetectors READ get_StartDetectors WRITE set_StartDetectors STORED false)
  QCEP_INTEGER_PROPERTY(StartDetectors)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)

  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(int recentExperimentsSize READ get_RecentExperimentsSize WRITE set_RecentExperimentsSize)
  QCEP_INTEGER_PROPERTY(RecentExperimentsSize)
};

#endif // QXRDAPPCOMMONSETTINGS_H
