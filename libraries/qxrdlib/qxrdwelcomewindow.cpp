#include "qxrdwelcomewindow.h"
#include "ui_qxrdwelcomewindow.h"
#include "qxrdwelcomerecentitem.h"
#include "qxrdappcommon.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>

QxrdWelcomeWindow::QxrdWelcomeWindow(QxrdAppCommonWPtr appw) :
  QMainWindow(NULL),
  m_Application(appw),
  m_InsertRow(5),
  m_RecentExperimentsMenu(NULL)
{
  QxrdAppCommonPtr app(m_Application);

  if (app) {
    setupUi(this);

    m_StatusMsg = new QLabel(NULL);
    m_StatusMsg -> setMinimumWidth(500);
    m_StatusMsg -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_StatusMsg -> setToolTip(tr("Status Messages"));

    statusBar() -> addPermanentWidget(m_StatusMsg);

    connect(&m_StatusTimer, &QTimer::timeout, this, &QxrdWelcomeWindow::clearStatusMessage);

    connect(m_ActionEditApplicationPreferences, &QAction::triggered, app.data(), &QxrdAppCommon::editGlobalPreferences);
    connect(m_ActionNewExperiment, &QAction::triggered, app.data(), &QxrdAppCommon::createNewExperiment);
    connect(m_ActionOpenExperiment, &QAction::triggered, app.data(), &QxrdAppCommon::chooseExistingExperiment);
    connect(m_ActionExitApplication, &QAction::triggered, app.data(), &QxrdAppCommon::possiblyQuit);

    connect(m_NewExperiment, &QAbstractButton::clicked, m_ActionNewExperiment, &QAction::trigger);
    connect(m_OpenExistingExperiment, &QAbstractButton::clicked, m_ActionOpenExperiment, &QAction::trigger);

    QStringList recents = app->get_RecentExperiments();

    foreach (QString recent, recents) {
      appendRecentExperiment(recent);
    }

    setupRecentExperimentsMenu(m_ActionOpenRecentExperiment);
  }
}

QxrdWelcomeWindow::~QxrdWelcomeWindow()
{
}

void QxrdWelcomeWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
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

  QxrdAppCommonPtr app(m_Application);

  if (app) {
    m_RecentExperimentsMenu->clear();

    QStringList recent = app->get_RecentExperiments();

    foreach (QString exp, recent) {
      QAction *action = new QAction(exp, m_RecentExperimentsMenu);

      connect(action, &QAction::triggered, [=] {app->openRecentExperiment(exp);});

      m_RecentExperimentsMenu -> addAction(action);
    }
  }
}

void QxrdWelcomeWindow::appendRecentExperiment(QString title)
{
  QxrdAppCommonPtr app(m_Application);

  if (app) {
    QxrdWelcomeRecentItem *item = new QxrdWelcomeRecentItem(NULL);

    item->setText(title);

    m_GridLayout->addWidget(item, m_InsertRow++, 0, 1, 2);

    connect(item, &QAbstractButton::clicked, [=] {app->openRecentExperiment(title);});
  }
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
  QxrdAppCommonPtr app(m_Application);

  if (app) {
    QString experiment = app->get_RecentExperiments().value(0);

    if (experiment.length()) {
      app->openExperiment(experiment);
    }
  }
}
