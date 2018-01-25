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
};

#endif // QXRDAPPCOMMONSETTINGS_H
