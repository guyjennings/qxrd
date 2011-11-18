#ifndef QXRDDEFAULTAPPLICATION_H
#define QXRDDEFAULTAPPLICATION_H

#include "qcepmacros.h"

#include <QApplication>
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

  void newAnalysisExperiment(QString path);
  void newPerkinElmerExperiment(QString path);
  void newPilatusExperiment(QString path);

signals:

public slots:
  void chooseNewExperiment();
  void chooseExistingExperiment();

  void openExperiment(int kind, QString path);

public:
  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(int recentExperimentsSize READ get_RecentExperimentsSize WRITE set_RecentExperimentsSize)
  QCEP_INTEGER_PROPERTY(RecentExperimentsSize)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)
};

#endif // QXRDDEFAULTAPPLICATION_H
