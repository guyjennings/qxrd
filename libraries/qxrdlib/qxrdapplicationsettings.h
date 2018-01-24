#ifndef QXRDAPPLICATIONSETTINGS_H
#define QXRDAPPLICATIONSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepapplicationsettings.h"
#include "qxrdapplication-ptr.h"
#include "qcepallocator-ptr.h"

class QXRD_EXPORT QxrdApplicationSettings : public QcepApplicationSettings
{
  Q_OBJECT

public:
  QxrdApplicationSettings(QxrdApplicationWPtr app, int argc, char **argv);
  virtual ~QxrdApplicationSettings();
  void init();

  QStringList makeStringListFromArgs(int argc, char **argv);

  QcepAllocatorWPtr allocator() const;

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void appendCommand(QString cmd);
  void appendScript(QString script);
  void appendFile(QString file);
  void appendWatcher(QString patt);

private:
  QcepAllocatorPtr                m_Allocator;

public:
  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(int recentExperimentsSize READ get_RecentExperimentsSize WRITE set_RecentExperimentsSize)
  QCEP_INTEGER_PROPERTY(RecentExperimentsSize)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)

  Q_PROPERTY(QString currentDirectory READ get_CurrentDirectory WRITE set_CurrentDirectory)
  QCEP_STRING_PROPERTY(CurrentDirectory)

//  Q_PROPERTY(bool   openDirectly  READ get_OpenDirectly WRITE set_OpenDirectly)
//  QCEP_BOOLEAN_PROPERTY(OpenDirectly)

  Q_PROPERTY(qint64    debug         READ get_Debug WRITE set_Debug)
  QCEP_INTEGER64_PROPERTY(Debug)

  Q_PROPERTY(int    openNew         READ get_OpenNew WRITE set_OpenNew STORED false)
  QCEP_INTEGER_PROPERTY(OpenNew)

  Q_PROPERTY(int    freshStart         READ get_FreshStart WRITE set_FreshStart STORED false)
  QCEP_INTEGER_PROPERTY(FreshStart)

  Q_PROPERTY(int    fileBrowserLimit   READ get_FileBrowserLimit WRITE set_FileBrowserLimit)
  QCEP_INTEGER_PROPERTY(FileBrowserLimit)

  Q_PROPERTY(int    messageWindowLines   READ get_MessageWindowLines WRITE set_MessageWindowLines)
  QCEP_INTEGER_PROPERTY(MessageWindowLines)

  Q_PROPERTY(int    updateIntervalMsec   READ get_UpdateIntervalMsec WRITE set_UpdateIntervalMsec)
  QCEP_INTEGER_PROPERTY(UpdateIntervalMsec)

  Q_PROPERTY(int    argc       READ get_Argc      WRITE set_Argc STORED false)
  QCEP_INTEGER_PROPERTY(Argc)

  Q_PROPERTY(QStringList argv READ get_Argv WRITE set_Argv STORED false)
  QCEP_STRING_LIST_PROPERTY(Argv)

  Q_PROPERTY(int    guiWanted  READ get_GuiWanted WRITE set_GuiWanted STORED false)
  QCEP_INTEGER_PROPERTY(GuiWanted)

  Q_PROPERTY(int    startDetectors READ get_StartDetectors WRITE set_StartDetectors STORED false)
  QCEP_INTEGER_PROPERTY(StartDetectors)

  Q_PROPERTY(QStringList cmdList READ get_CmdList WRITE set_CmdList STORED false)
  QCEP_STRING_LIST_PROPERTY(CmdList)

  Q_PROPERTY(QStringList fileList READ get_FileList WRITE set_FileList STORED false)
  QCEP_STRING_LIST_PROPERTY(FileList)

  Q_PROPERTY(QStringList watcherList READ get_WatcherList WRITE set_WatcherList STORED false)
  QCEP_STRING_LIST_PROPERTY(WatcherList)

  Q_PROPERTY(int lockerCount READ get_LockerCount WRITE set_LockerCount STORED false)
  QCEP_INTEGER_PROPERTY(LockerCount)

  Q_PROPERTY(double lockerRate READ get_LockerRate WRITE set_LockerRate STORED false)
  QCEP_DOUBLE_PROPERTY(LockerRate)

  Q_PROPERTY(int experimentCount READ get_ExperimentCount WRITE set_ExperimentCount STORED false)
  QCEP_INTEGER_PROPERTY(ExperimentCount)
};

#endif // QXRDAPPLICATIONSETTINGS_H
