#include "qxrddefaultapplication.h"
#include <QSettings>
#include "qxrddefaultapplicationdialog.h"
#include <stdio.h>
#include <QFileDialog>
#include <QStringList>

QxrdDefaultApplication::QxrdDefaultApplication(int &argc, char **argv) :
  QApplication(argc, argv),
  m_RecentExperiments(this, "recentExperiments", QStringList()),
  m_CurrentExperiment(this, "currentExperiment", "")
{
  prop_RecentExperiments()->appendValue("One");
  prop_RecentExperiments()->appendValue("Two");
  prop_RecentExperiments()->appendValue("Three");
  prop_RecentExperiments()->appendValue("Four");
  prop_RecentExperiments()->appendValue("Five");
}

void QxrdDefaultApplication::readDefaultSettings()
{
  QSettings settings("cep.xor.aps.anl.gov", "qxrd-defaults");

  set_RecentExperiments(settings.value("recentExperiments").toStringList());
  set_CurrentExperiment(settings.value("currentExperiment").toString());
}

void QxrdDefaultApplication::writeDefaultSettings()
{
  QSettings settings("cep.xor.aps.anl.gov", "qxrd-defaults");

  settings.setValue("recentExperiments", QVariant((QStringList) get_RecentExperiments()));
  settings.setValue("currentExperiment", get_CurrentExperiment());
}

QxrdDefaultApplication::~QxrdDefaultApplication()
{
}

void QxrdDefaultApplication::chooseNewExperiment()
{
  QxrdDefaultApplicationDialog *chooser =  new QxrdDefaultApplicationDialog(this);

  QString res = chooser->choose();

  if (res.length() > 0) {
    openExperiment(res);
  }
}

void QxrdDefaultApplication::chooseExistingExperiment()
{
  QString res = QFileDialog::getOpenFileName(NULL,
                                             "Open an existing experiment...",
                                             get_CurrentExperiment(),
                                             "QXRD Experiments (*.qxrdp);;Other Files (*)");
}

void QxrdDefaultApplication::openExperiment(QString path)
{
  printf("Open Experiment %s\n", qPrintable(path));

  appendRecentExperiment(path);
}

void QxrdDefaultApplication::appendRecentExperiment(QString path)
{
}

