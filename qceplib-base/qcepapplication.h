#ifndef QCEPAPPLICATION_H
#define QCEPAPPLICATION_H

#include "qceplib_global.h"
#include <QApplication>
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepdataobject-ptr.h"
#include "qcepobject.h"
#include "qapplication-ptr.h"
#include "qcepapplication-ptr.h"

class QCEP_EXPORT QcepApplication : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  explicit QcepApplication(int &argc, char **argv);
  void initializeRoot();

  static QcepApplicationWPtr findApplication(QcepObjectWPtr p);

  virtual void setDefaultObjectData(QcepDataObject *obj) = 0;

  int exec();
  void processEvents() const;
  void exit();
  void quit();

  virtual void openStartupWindow() = 0;
  virtual void closeStartupWindow() = 0;

  virtual QString applicationDescription() = 0;
  virtual QIcon   applicationIcon() = 0;

  virtual void openExperiment(QString path) = 0;
  virtual void openRecentExperiment(QString path) = 0;

  virtual void editGlobalPreferences() = 0;
  virtual void createNewExperiment() = 0;
  virtual void chooseExistingExperiment() = 0;

  virtual void readApplicationSettings() = 0;
  virtual void writeApplicationSettings() = 0;

  virtual void possiblyQuit() = 0;

public slots:
  void openObjectBrowserWindow(QcepObjectWPtr obj);

protected:
  QApplicationPtr m_Application;

private:
  QStringList makeStringListFromArgs(int argc, char **argv);

public:
  Q_PROPERTY(int    argc       READ get_Argc      WRITE set_Argc STORED false)
  QCEP_INTEGER_PROPERTY(Argc)

  Q_PROPERTY(QStringList argv READ get_Argv WRITE set_Argv STORED false)
  QCEP_STRING_LIST_PROPERTY(Argv)

  Q_PROPERTY(int    guiWanted  READ get_GuiWanted WRITE set_GuiWanted STORED false)
  QCEP_INTEGER_PROPERTY(GuiWanted)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)

  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(int recentExperimentsSize READ get_RecentExperimentsSize WRITE set_RecentExperimentsSize)
  QCEP_INTEGER_PROPERTY(RecentExperimentsSize)

  Q_PROPERTY(int experimentCount READ get_ExperimentCount WRITE set_ExperimentCount STORED false)
  QCEP_INTEGER_PROPERTY(ExperimentCount)

  Q_PROPERTY(int    updateIntervalMsec   READ get_UpdateIntervalMsec WRITE set_UpdateIntervalMsec)
  QCEP_INTEGER_PROPERTY(UpdateIntervalMsec)
};

extern QCEP_EXPORT QcepApplication *g_Application;

#endif // QCEPAPPLICATION_H
