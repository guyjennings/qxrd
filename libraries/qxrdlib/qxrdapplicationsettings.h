#ifndef QXRDAPPLICATIONSETTINGS_H
#define QXRDAPPLICATIONSETTINGS_H

#include "qxrdlib_global.h"
#include "qxrdappcommonsettings.h"
#include "qxrdapplication-ptr.h"

class QXRD_EXPORT QxrdApplicationSettings : public QxrdAppCommonSettings
{
  Q_OBJECT

private:
  typedef QxrdAppCommonSettings inherited;

public:
  QxrdApplicationSettings(QxrdApplicationWPtr app, int argc, char **argv);
  virtual ~QxrdApplicationSettings();
  void init();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

public:
  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)

  Q_PROPERTY(QString currentDirectory READ get_CurrentDirectory WRITE set_CurrentDirectory)
  QCEP_STRING_PROPERTY(CurrentDirectory)

//  Q_PROPERTY(bool   openDirectly  READ get_OpenDirectly WRITE set_OpenDirectly)
//  QCEP_BOOLEAN_PROPERTY(OpenDirectly)

  Q_PROPERTY(int    fileBrowserLimit   READ get_FileBrowserLimit WRITE set_FileBrowserLimit)
  QCEP_INTEGER_PROPERTY(FileBrowserLimit)

  Q_PROPERTY(int    messageWindowLines   READ get_MessageWindowLines WRITE set_MessageWindowLines)
  QCEP_INTEGER_PROPERTY(MessageWindowLines)

  Q_PROPERTY(int    updateIntervalMsec   READ get_UpdateIntervalMsec WRITE set_UpdateIntervalMsec)
  QCEP_INTEGER_PROPERTY(UpdateIntervalMsec)

  Q_PROPERTY(int lockerCount READ get_LockerCount WRITE set_LockerCount STORED false)
  QCEP_INTEGER_PROPERTY(LockerCount)

  Q_PROPERTY(double lockerRate READ get_LockerRate WRITE set_LockerRate STORED false)
  QCEP_DOUBLE_PROPERTY(LockerRate)

  Q_PROPERTY(int experimentCount READ get_ExperimentCount WRITE set_ExperimentCount STORED false)
  QCEP_INTEGER_PROPERTY(ExperimentCount)
};

#endif // QXRDAPPLICATIONSETTINGS_H
