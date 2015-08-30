#include "qxrdwelcomewindow.h"
#include "ui_qxrdwelcomewindow.h"
#include "qxrdwelcomerecentitem.h"
#include "qxrdapplication.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>

QxrdWelcomeWindow::QxrdWelcomeWindow(QxrdApplication *app) :
  QMainWindow(NULL),
  ui(new Ui::QxrdWelcomeWindow),
  m_Application(app),
  m_InsertRow(5),
  m_SignalMapper(NULL),
  m_RecentExperimentsMenu(NULL)
{
  ui->setupUi(this);

  m_StatusMsg = new QLabel(NULL);
  m_StatusMsg -> setMinimumWidth(500);
  m_StatusMsg -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_StatusMsg -> setToolTip(tr("Status Messages"));

  statusBar() -> addPermanentWidget(m_StatusMsg);

  connect(&m_StatusTimer, &QTimer::timeout, this, &QxrdWelcomeWindow::clearStatusMessage);

  connect(ui->m_ActionEditApplicationPreferences, &QAction::triggered, m_Application, &QxrdApplication::editGlobalPreferences);
  connect(ui->m_ActionNewPerkinElmerAcquisition, &QAction::triggered, m_Application, &QxrdApplication::doNewPerkinElmerAcquisition);
  connect(ui->m_ActionNewPilatusAcquisition, &QAction::triggered, m_Application, &QxrdApplication::doNewPilatusAcquisition);
  connect(ui->m_ActionNewSimulatedAcquisition, &QAction::triggered, m_Application, &QxrdApplication::doNewSimulatedAcquisition);
  connect(ui->m_ActionNewPerkinElmerAnalysis, &QAction::triggered, m_Application, &QxrdApplication::doNewPerkinElmerAnalysis);
  connect(ui->m_ActionNewPilatusAnalysis, &QAction::triggered, m_Application, &QxrdApplication::doNewPilatusAnalysis);
  connect(ui->m_ActionNewGenericAnalysis, &QAction::triggered, m_Application, &QxrdApplication::doNewGenericAnalysis);
  connect(ui->m_ActionOpenExperiment, &QAction::triggered, m_Application, &QxrdApplication::chooseExistingExperiment);
  connect(ui->m_ActionExitApplication, &QAction::triggered, m_Application, &QxrdApplication::possiblyQuit);

  connect(ui->m_NewPerkinElmerAcquisition, &QAbstractButton::clicked, ui->m_ActionNewPerkinElmerAcquisition, &QAction::trigger);
  connect(ui->m_NewPilatusAcquisition, &QAbstractButton::clicked, ui->m_ActionNewPilatusAcquisition, &QAction::trigger);
  connect(ui->m_NewSimulatedAcquisition, &QAbstractButton::clicked, ui->m_ActionNewSimulatedAcquisition, &QAction::trigger);
  connect(ui->m_NewPerkinElmerAnalysis, &QAbstractButton::clicked, ui->m_ActionNewPerkinElmerAnalysis, &QAction::trigger);
  connect(ui->m_NewPilatusAnalysis, &QAbstractButton::clicked, ui->m_ActionNewPilatusAnalysis, &QAction::trigger);
  connect(ui->m_NewGenericAnalysis, &QAbstractButton::clicked, ui->m_ActionNewGenericAnalysis, &QAction::trigger);
  connect(ui->m_OpenExistingExperiment, &QAbstractButton::clicked, ui->m_ActionOpenExperiment, &QAction::trigger);

  connect(&m_SignalMapper, (void (QSignalMapper::*)(const QString&)) &QSignalMapper::mapped,
          m_Application, &QxrdApplication::openRecentExperiment);

  QStringList recents = m_Application->get_RecentExperiments();

  foreach (QString recent, recents) {
    appendRecentExperiment(recent);
  }

  setupRecentExperimentsMenu(ui->m_ActionOpenRecentExperiment);
}

QxrdWelcomeWindow::~QxrdWelcomeWindow()
{
  delete ui;
}

void QxrdWelcomeWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdWelcomeWindow::setupRecentExperimentsMenu(QAction *action)
{
  m_RecentExperimentsMenu = new QMenu(this);

  action->setMenu(m_RecentExperimentsMenu);

  connect(m_RecentExperimentsMenu, &QMenu::aboutToShow, this, &QxrdWelcomeWindow::populateRecentExperimentsMenu);
}

void QxrdWelcomeWindow::populateRecentExperimentsMenu()
{
//  printMessage("Populating recent experiments menu");

  m_RecentExperimentsMenu->clear();

  QStringList recent = m_Application->get_RecentExperiments();

  foreach (QString exp, recent) {
//    printf("Recent experiment: %s\n", qPrintable(exp));

    QAction *action = new QAction(exp, m_RecentExperimentsMenu);
    connect(action, &QAction::triggered, &m_SignalMapper, (void (QSignalMapper::*) ()) &QSignalMapper::map);
    m_SignalMapper.setMapping(action, exp);

    m_RecentExperimentsMenu -> addAction(action);
  }
}

void QxrdWelcomeWindow::appendRecentExperiment(QString title)
{
  QxrdWelcomeRecentItem *item = new QxrdWelcomeRecentItem(NULL);

  item->setText(title);

  ui->m_GridLayout->addWidget(item, m_InsertRow++, 0, 1, 2);

  connect(item, &QAbstractButton::clicked, &m_SignalMapper, (void (QSignalMapper::*) ()) &QSignalMapper::map);

  m_SignalMapper.setMapping(item, title);
}

void QxrdWelcomeWindow::closeEvent ( QCloseEvent * event )
{
  QMainWindow::closeEvent(event);
////  if (wantToClose()) {
//    event -> accept();
//  } else {
//    event -> ignore();
//  }
}

void QxrdWelcomeWindow::possiblyClose()
{
  //   printf("QxrdWindow::possiblyClose()\n");
  if (wantToClose()) {
    close();
  }
}

bool QxrdWelcomeWindow::wantToClose()
{
  THREAD_CHECK;

  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window?"),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

void QxrdWelcomeWindow::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QByteArray geometry = settings->value(section+"-geometry").toByteArray();
    QByteArray winstate = settings->value(section+"-state").toByteArray();

    if (!geometry.isEmpty() && !winstate.isEmpty()) {
      restoreGeometry(geometry);
      restoreState(winstate,1);
    }

    QcepProperty::readSettings(this, settings, section);
  }
}

void QxrdWelcomeWindow::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    settings->setValue(section+"-geometry", saveGeometry());
    settings->setValue(section+"-state", saveState(1));

    QcepProperty::writeSettings(this, settings, section);
  }
}

void QxrdWelcomeWindow::displayMessage(QString msg)
{
  m_StatusMsg -> setText(msg);

  m_StatusTimer.start(5000);
}

void QxrdWelcomeWindow::clearStatusMessage()
{
  m_StatusMsg -> setText("");
}

void QxrdWelcomeWindow::openMostRecent()
{
  QString experiment = m_Application->get_RecentExperiments().value(0);

  if (experiment.length()) {
    m_Application->openExperiment(experiment);
  }
}
