#include "qxrdwelcomewindow.h"
#include "ui_qxrdwelcomewindow.h"
#include "qxrdwelcomerecentitem.h"
#include "qxrdapplication.h"
#include <QCloseEvent>
#include <QMessageBox>

QxrdWelcomeWindow::QxrdWelcomeWindow(QxrdApplication *app) :
  QMainWindow(NULL),
  ui(new Ui::QxrdWelcomeWindow),
  m_Application(app),
  m_InsertRow(4),
  m_SignalMapper(NULL),
  m_RecentExperimentsMenu(NULL)
{
  ui->setupUi(this);

  m_StatusMsg = new QLabel(NULL);
  m_StatusMsg -> setMinimumWidth(500);
  m_StatusMsg -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_StatusMsg -> setToolTip(tr("Status Messages"));

  statusBar() -> addPermanentWidget(m_StatusMsg);

  connect(&m_StatusTimer, SIGNAL(timeout()), this, SLOT(clearStatusMessage()));

  connect(ui->m_ActionEditApplicationPreferences, SIGNAL(triggered()), m_Application, SLOT(editGlobalPreferences()));
  connect(ui->m_ActionNewPerkinElmerAcquisition, SIGNAL(triggered()), m_Application, SLOT(doNewPerkinElmerAcquisition()));
  connect(ui->m_ActionNewPilatusAcquisition, SIGNAL(triggered()), m_Application, SLOT(doNewPilatusAcquisition()));
  connect(ui->m_ActionNewSimulatedAcquisition, SIGNAL(triggered()), m_Application, SLOT(doNewSimulatedAcquisition()));
  connect(ui->m_ActionNewPerkinElmerAnalysis, SIGNAL(triggered()), m_Application, SLOT(doNewPerkinElmerAnalysis()));
  connect(ui->m_ActionNewPilatusAnalysis, SIGNAL(triggered()), m_Application, SLOT(doNewPilatusAnalysis()));
  connect(ui->m_ActionNewGenericAnalysis, SIGNAL(triggered()), m_Application, SLOT(doNewGenericAnalysis()));
  connect(ui->m_ActionOpenExperiment, SIGNAL(triggered()), m_Application, SLOT(chooseExistingExperiment()));
  connect(ui->m_ActionExitApplication, SIGNAL(triggered()), m_Application, SLOT(possiblyQuit()));

  connect(ui->m_NewPerkinElmerAcquisition, SIGNAL(clicked()), ui->m_ActionNewPerkinElmerAcquisition, SLOT(trigger()));
  connect(ui->m_NewPilatusAcquisition, SIGNAL(clicked()), ui->m_ActionNewPilatusAcquisition, SLOT(trigger()));
  connect(ui->m_NewSimulatedAcquisition, SIGNAL(clicked()), ui->m_ActionNewSimulatedAcquisition, SLOT(trigger()));
  connect(ui->m_NewPerkinElmerAnalysis, SIGNAL(clicked()), ui->m_ActionNewPerkinElmerAnalysis, SLOT(trigger()));
  connect(ui->m_NewPilatusAnalysis, SIGNAL(clicked()), ui->m_ActionNewPilatusAnalysis, SLOT(trigger()));
  connect(ui->m_NewGenericAnalysis, SIGNAL(clicked()), ui->m_ActionNewGenericAnalysis, SLOT(trigger()));
  connect(ui->m_OpenExistingExperiment, SIGNAL(clicked()), ui->m_ActionOpenExperiment, SLOT(trigger()));

  m_SignalMapper = new QSignalMapper(this);

  connect(m_SignalMapper, SIGNAL(mapped(QString)), m_Application, SLOT(openRecentExperiment(QString)));

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

  connect(m_RecentExperimentsMenu, SIGNAL(aboutToShow()), this, SLOT(populateRecentExperimentsMenu()));
}

void QxrdWelcomeWindow::populateRecentExperimentsMenu()
{
//  printMessage("Populating recent experiments menu");

  m_RecentExperimentsMenu->clear();

  QStringList recent = m_Application->get_RecentExperiments();

  foreach (QString exp, recent) {
//    printf("Recent experiment: %s\n", qPrintable(exp));

    QAction *action = new QAction(exp, m_RecentExperimentsMenu);
    QSignalMapper *mapper = new QSignalMapper(action);
    connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(action, exp);

    connect(mapper, SIGNAL(mapped(const QString &)), m_Application, SLOT(openRecentExperiment(QString)));

    m_RecentExperimentsMenu -> addAction(action);
  }
}

void QxrdWelcomeWindow::appendRecentExperiment(QString title)
{
  QxrdWelcomeRecentItem *item = new QxrdWelcomeRecentItem(this);

  item->setText(title);

  ui->m_GridLayout->addWidget(item, m_InsertRow++, 0, 1, 2);

  connect(item, SIGNAL(clicked()), m_SignalMapper, SLOT(map()));

  m_SignalMapper->setMapping(item, title);

//  QAction *recentAction = new QAction(this);

//  recentAction->setText(title);

//  ui->m_OpenRecentExperimentMenu -> addAction(recentAction);

//  m_SignalMapper->setMapping(recentAction, title);

//  connect(recentAction, SIGNAL(triggered()), m_SignalMapper, SLOT(map()));
}

void QxrdWelcomeWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
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

    QcepProperty::readSettings(this, &staticMetaObject, section, settings);
  }
}

void QxrdWelcomeWindow::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    settings->setValue(section+"-geometry", saveGeometry());
    settings->setValue(section+"-state", saveState(1));

    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
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

