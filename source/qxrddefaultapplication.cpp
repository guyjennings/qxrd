#include "qxrddefaultapplication.h"
#include <QSettings>
#include "qxrddefaultapplicationdialog.h"
#include <stdio.h>
#include <QFileDialog>
#include <QStringList>
#include <QSignalMapper>

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
  set_RecentExperimentsSize(settings.value("recentExperimentsSize", 8).toInt());
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
  printMessage("");
  printMessage(tr("===== Open Experiment %1").arg(path));

  switch(kind) {
  case QxrdDefaultApplicationDialog::Analysis:
    path = newAnalysisExperiment(path);
    break;

  case QxrdDefaultApplicationDialog::PerkinElmer:
    path = newPerkinElmerExperiment(path);
    break;

  case QxrdDefaultApplicationDialog::Pilatus:
    path = newPilatusExperiment(path);
    break;
  }

  if (path.length() > 0) {
    appendRecentExperiment(path);

    writeDefaultSettings();

    loadPreferences(path);

    printMessage("");
    printMessage("New experiment loaded");
    printMessage("");
  }
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

QString QxrdDefaultApplication::normalizeExperimentName(QString filename)
{
  QFileInfo info(filename);
  QString exten = info.suffix();

  if (exten != "qxrdp") {
    return filename+".qxrdp";
  } else {
    return filename;
  }
}

void QxrdDefaultApplication::setNewExperimentSettings(QSettings &settings, int type, QString filename)
{
  QFileInfo info(filename);
  QString  path = info.path();
  QString  base = info.completeBaseName();

  settings.setValue("application/detectorType", type);
  settings.setValue("processor/outputDirectory", path);
  settings.setValue("application/logFilePath", base+".log");
  settings.setValue("processor/integratedFilePath", base+".avg");
  settings.setValue("window/inputFileBrowser/rootDirectory", path);
  settings.setValue("window/outputFileBrowser/rootDirectory", path);

  switch(type) {
  case 1: // Perkin Elmer
    break;

  case 2: // Pilatus
    break;

  case 4: // Analysis
    settings.setValue("application/runSpecServer", false);
    settings.setValue("application/runSimpleServer", false);
    break;
  }
}

QString QxrdDefaultApplication::newAnalysisExperiment(QString path)
{
  path = normalizeExperimentName(path);

  if (path.length() > 0) {
    savePreferences(path);

    QSettings settings(path, QSettings::IniFormat);

    setNewExperimentSettings(settings, 4, path);

    return path;
  } else {
    return QString();
  }
}

QString QxrdDefaultApplication::newPerkinElmerExperiment(QString path)
{
  path = normalizeExperimentName(path);

  if (path.length() > 0) {
    savePreferences(path);

    QSettings settings(path, QSettings::IniFormat);

    setNewExperimentSettings(settings, 1, path);

    return path;
  } else {
    return QString();
  }
}

QString QxrdDefaultApplication::newPilatusExperiment(QString path)
{
  path = normalizeExperimentName(path);

  if (path.length() > 0) {
    savePreferences(path);

    QSettings settings(path, QSettings::IniFormat);

    setNewExperimentSettings(settings, 2, path);

    return path;
  } else {
    return QString();
  }
}

void QxrdDefaultApplication::setupRecentExperimentsMenu(QAction *action)
{
  m_RecentExperimentsMenu = new QMenu();

  action->setMenu(m_RecentExperimentsMenu);

  connect(m_RecentExperimentsMenu, SIGNAL(aboutToShow()), this, SLOT(populateRecentExperimentsMenu()));
}

void QxrdDefaultApplication::populateRecentExperimentsMenu()
{
//  printMessage("Populating recent experiments menu");

  m_RecentExperimentsMenu->clear();

  foreach (QString exp, get_RecentExperiments()) {
    QAction *action = new QAction(exp, m_RecentExperimentsMenu);
    QSignalMapper *mapper = new QSignalMapper(action);
    connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(action, exp);

    connect(mapper, SIGNAL(mapped(const QString &)), this, SLOT(openRecentExperiment(QString)));

    m_RecentExperimentsMenu -> addAction(action);
  }
}

void QxrdDefaultApplication::openRecentExperiment(QString path)
{
//  printMessage(tr("Attempt to open experiment %1").arg(path));

  QFileInfo info(path);

  if (info.exists()) {
    openExperiment(QxrdDefaultApplicationDialog::Existing, path);
  } else {
    printMessage(tr("Experiment %1 does not exist").arg(path));
  }
}

void QxrdDefaultApplication::doNewPerkinElmerAcquisition()
{
}

void QxrdDefaultApplication::doNewPilatusAcquisition()
{
}

void QxrdDefaultApplication::doNewSimulatedAcquisition()
{
}

void QxrdDefaultApplication::doNewPerkinElmerAnalysis()
{
}

void QxrdDefaultApplication::doNewPilatusAnalysis()
{
}

void QxrdDefaultApplication::doNewGenericAnalysis()
{
}
