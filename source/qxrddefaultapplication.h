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

signals:

public slots:
  void chooseNewExperiment();
  void chooseExistingExperiment();

  void openExperiment(QString path);

public:
  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)
};

#endif // QXRDDEFAULTAPPLICATION_H
