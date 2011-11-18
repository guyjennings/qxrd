#ifndef QXRDDEFAULTAPPLICATION_H
#define QXRDDEFAULTAPPLICATION_H

#include "qcepmacros.h"

#include <QApplication>
#include <QMenu>
#include "qcepproperty.h"

class QxrdDefaultApplication : public QApplication
{
  Q_OBJECT
public:
  QxrdDefaultApplication(int &argc, char **argv);
  ~QxrdDefaultApplication();

  void readDefaultSettings();
  void writeDefaultSettings();
  void appendRecentExperiment(QString path);

  virtual void loadPreferences(QString path) = 0;
  virtual void savePreferences(QString path) = 0;
  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;

  QString newAnalysisExperiment(QString path);
  QString newPerkinElmerExperiment(QString path);
  QString newPilatusExperiment(QString path);

  QString normalizeExperimentName(QString filename);
  void setNewExperimentSettings(QSettings &settings, int type, QString filename);

  void setupRecentExperimentsMenu(QAction *action);

signals:

public slots:
  void chooseNewExperiment();
  void chooseExistingExperiment();
  void openRecentExperiment(QString path);

  void openExperiment(int kind, QString path);
  void populateRecentExperimentsMenu();

public:
  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments STORED false)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(int recentExperimentsSize READ get_RecentExperimentsSize WRITE set_RecentExperimentsSize STORED false)
  QCEP_INTEGER_PROPERTY(RecentExperimentsSize)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment STORED false)
  QCEP_STRING_PROPERTY(CurrentExperiment)

private:
    QMenu   *m_RecentExperimentsMenu;
};

#endif // QXRDDEFAULTAPPLICATION_H
