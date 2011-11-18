#include "qxrddefaultapplication.h"
#include <QSettings>
#include "qxrddefaultapplicationdialog.h"
#include <stdio.h>
#include <QFileDialog>
#include <QStringList>

QxrdDefaultApplication::QxrdDefaultApplication(int &argc, char **argv) :
  QApplication(argc, argv),
  m_RecentExperiments(this, "recentExperiments", QStringList()),
  m_RecentExperimentsSize(this,"recentExperimentsSize", 8),
  m_CurrentExperiment(this, "currentExperiment", "")
{
  readDefaultSettings();
}

void QxrdDefaultApplication::readDefaultSettings()
{
  QSettings settings("cep.xor.aps.anl.gov", "qxrd-defaults");

  set_RecentExperiments(settings.value("recentExperiments").toStringList());
  set_RecentExperimentsSize(settings.value("recentExperimentsSize").toInt());
  set_CurrentExperiment(settings.value("currentExperiment").toString());
}

void QxrdDefaultApplication::writeDefaultSettings()
{
  QSettings settings("cep.xor.aps.anl.gov", "qxrd-defaults");

  settings.setValue("recentExperiments", get_RecentExperiments());
  settings.setValue("recentExperimentsSize", get_RecentExperimentsSize());
  settings.setValue("currentExperiment", get_CurrentExperiment());
}

QxrdDefaultApplication::~QxrdDefaultApplication()
{
  writeDefaultSettings();
}

void QxrdDefaultApplication::chooseNewExperiment()
{
  QxrdDefaultApplicationDialog *chooser =  new QxrdDefaultApplicationDialog(this);

  if (chooser->choose()) {
    openExperiment(chooser->chosenKind(), chooser->chosenPath());
  }
}

void QxrdDefaultApplication::chooseExistingExperiment()
{
  QString res = QFileDialog::getOpenFileName(NULL,
                                             "Open an existing experiment...",
                                             get_CurrentExperiment(),
                                             "QXRD Experiments (*.qxrdp);;Other Files (*)");

  if (res.length() > 0) {
    openExperiment(QxrdDefaultApplicationDialog::Existing, res);
  }
}

void QxrdDefaultApplication::openExperiment(int kind, QString path)
{
  printf("Open Experiment %s\n", qPrintable(path));

  switch(kind) {
  case QxrdDefaultApplicationDialog::Analysis:
    newAnalysisExperiment(path);
    break;

  case QxrdDefaultApplicationDialog::PerkinElmer:
    newPerkinElmerExperiment(path);
    break;

  case QxrdDefaultApplicationDialog::Pilatus:
    newPilatusExperiment(path);
    break;
  }

  appendRecentExperiment(path);

  writeDefaultSettings();

  loadPreferences(path);
}

void QxrdDefaultApplication::appendRecentExperiment(QString path)
{
  QStringList recent = get_RecentExperiments();

  recent.prepend(path);
  recent.removeDuplicates();

  while(recent.length() > get_RecentExperimentsSize()) {
    recent.removeLast();
  }

  set_RecentExperiments(recent);
}

void QxrdDefaultApplication::newAnalysisExperiment(QString path)
{
  savePreferences(path);
}

void QxrdDefaultApplication::newPerkinElmerExperiment(QString path)
{
  savePreferences(path);
}

void QxrdDefaultApplication::newPilatusExperiment(QString path)
{
  savePreferences(path);
}
