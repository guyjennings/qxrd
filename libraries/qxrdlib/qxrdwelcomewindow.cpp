#include "qxrdwelcomewindow.h"
#include "ui_qxrdwelcomewindow.h"
#include "qxrdwelcomerecentitem.h"
#include "qxrdapplication.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>
#include "qxrdapplicationsettings.h"

QxrdWelcomeWindow::QxrdWelcomeWindow(QxrdApplicationWPtr appw) :
  QMainWindow(NULL),
  ui(new Ui::QxrdWelcomeWindow),
  m_Application(appw),
  m_InsertRow(5),
  m_SignalMapper(NULL),
  m_RecentExperimentsMenu(NULL)
{
  QxrdApplicationPtr app(m_Application);

  if (app) {
    ui->setupUi(this);

    m_StatusMsg = new QLabel(NULL);
    m_StatusMsg -> setMinimumWidth(500);
    m_StatusMsg -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_StatusMsg -> setToolTip(tr("Status Messages"));

    statusBar() -> addPermanentWidget(m_StatusMsg);

    connect(&m_StatusTimer, &QTimer::timeout, this, &QxrdWelcomeWindow::clearStatusMessage);

    connect(ui->m_ActionEditApplicationPreferences, &QAction::triggered, app.data(), &QxrdApplication::editGlobalPreferences);
    connect(ui->m_ActionNewExperiment, &QAction::triggered, app.data(), &QxrdApplication::createNewExperiment);
    connect(ui->m_ActionOpenExperiment, &QAction::triggered, app.data(), &QxrdApplication::chooseExistingExperiment);
    connect(ui->m_ActionExitApplication, &QAction::triggered, app.data(), &QxrdApplication::possiblyQuit);

    connect(ui->m_NewExperiment, &QAbstractButton::clicked, ui->m_ActionNewExperiment, &QAction::trigger);
    connect(ui->m_OpenExistingExperiment, &QAbstractButton::clicked, ui->m_ActionOpenExperiment, &QAction::trigger);

    connect(&m_SignalMapper, (void (QSignalMapper::*)(const QString&)) &QSignalMapper::mapped,
            app.data(), &QxrdApplication::openRecentExperiment);

    QxrdApplicationSettingsPtr settings(app->settings());

    if (settings) {
      QStringList recents = settings->get_RecentExperiments();

      foreach (QString recent, recents) {
        appendRecentExperiment(recent);
      }
    }

    setupRecentExperimentsMenu(ui->m_ActionOpenRecentExperiment);
  }
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

  QxrdApplicationPtr app(m_Application);

  if (app) {
    m_RecentExperimentsMenu->clear();

    QxrdApplicationSettingsPtr settings(app->settings());

    if (settings) {
      QStringList recent = settings->get_RecentExperiments();

      foreach (QString exp, recent) {
        //    printf("Recent experiment: %s\n", qPrintable(exp));

        QAction *action = new QAction(exp, m_RecentExperimentsMenu);
        connect(action, &QAction::triggered, &m_SignalMapper, (void (QSignalMapper::*) ()) &QSignalMapper::map);
        m_SignalMapper.setMapping(action, exp);

        m_RecentExperimentsMenu -> addAction(action);
      }
    }
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

void QxrdWelcomeWindow::readSettings(QSettings *settings)
{
  if (settings) {
    QByteArray geometry = settings->value("geometry").toByteArray();
    QByteArray winstate = settings->value("state").toByteArray();

    if (!geometry.isEmpty() && !winstate.isEmpty()) {
      restoreGeometry(geometry);
      restoreState(winstate,1);
    }

    settings->beginGroup("properties");
    QcepProperty::readSettings(this, settings);
    settings->endGroup();
  }
}

void QxrdWelcomeWindow::writeSettings(QSettings *settings)
{
  if (settings) {
    settings->setValue("geometry", saveGeometry());
    settings->setValue("state", saveState(1));

    settings->beginGroup("properties");
    QcepProperty::writeSettings(this, settings);
    settings->endGroup();
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
  QxrdApplicationPtr app(m_Application);

  if (app) {
    QxrdApplicationSettingsPtr settings(app->settings());

    if (settings) {
      QString experiment = settings->get_RecentExperiments().value(0);

      if (experiment.length()) {
        app->openExperiment(experiment);
      }
    }
  }
}
