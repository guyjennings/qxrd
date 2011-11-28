#include "qxrdwelcomewindow.h"
#include "ui_qxrdwelcomewindow.h"
#include "qxrdwelcomerecentitem.h"
#include "qxrdapplication.h"

QxrdWelcomeWindow::QxrdWelcomeWindow(QxrdApplication *app) :
  QMainWindow(NULL),
  ui(new Ui::QxrdWelcomeWindow),
  m_Application(app),
  m_InsertRow(4),
  m_SignalMapper(NULL)
{
  ui->setupUi(this);

  QStringList recents = m_Application->get_RecentExperiments();

  connect(ui->m_ActionEditApplicationPreferences, SIGNAL(triggered()), m_Application, SLOT(editGlobalPreferences()));
  connect(ui->m_ActionNewPerkinElmerAcquisition, SIGNAL(triggered()), m_Application, SLOT(doNewPerkinElmerAcquisition()));
  connect(ui->m_ActionNewPilatusAcquisition, SIGNAL(triggered()), m_Application, SLOT(doNewPilatusAcquisition()));
  connect(ui->m_ActionNewSimulatedAcquisition, SIGNAL(triggered()), m_Application, SLOT(doNewSimulatedAcquisition()));
  connect(ui->m_ActionNewPerkinElmerAnalysis, SIGNAL(triggered()), m_Application, SLOT(doNewPerkinElmerAnalysis()));
  connect(ui->m_ActionNewPilatusAnalysis, SIGNAL(triggered()), m_Application, SLOT(doNewPilatusAnalysis()));
  connect(ui->m_ActionNewGenericAnalysis, SIGNAL(triggered()), m_Application, SLOT(doNewGenericAnalysis()));
  connect(ui->m_ActionOpenExperiment, SIGNAL(triggered()), m_Application, SLOT(chooseExistingExperiment()));

  connect(ui->m_NewPerkinElmerAcquisition, SIGNAL(clicked()), ui->m_ActionNewPerkinElmerAcquisition, SLOT(trigger()));
  connect(ui->m_NewPilatusAcquisition, SIGNAL(clicked()), ui->m_ActionNewPilatusAcquisition, SLOT(trigger()));
  connect(ui->m_NewSimulatedAcquisition, SIGNAL(clicked()), ui->m_ActionNewSimulatedAcquisition, SLOT(trigger()));
  connect(ui->m_NewPerkinElmerAnalysis, SIGNAL(clicked()), ui->m_ActionNewPerkinElmerAnalysis, SLOT(trigger()));
  connect(ui->m_NewPilatusAnalysis, SIGNAL(clicked()), ui->m_ActionNewPilatusAnalysis, SLOT(trigger()));
  connect(ui->m_NewGenericAnalysis, SIGNAL(clicked()), ui->m_ActionNewGenericAnalysis, SLOT(trigger()));
  connect(ui->m_OpenExistingExperiment, SIGNAL(clicked()), ui->m_ActionOpenExperiment, SLOT(trigger()));

  m_SignalMapper = new QSignalMapper(this);

  connect(m_SignalMapper, SIGNAL(mapped(QString)), m_Application, SLOT(openRecentExperiment(QString)));

  foreach (QString recent, recents) {
    appendRecentDocument(recent);
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

void QxrdWelcomeWindow::appendRecentDocument(QString title)
{
  QxrdWelcomeRecentItem *item = new QxrdWelcomeRecentItem(this);

  item->setText(title);

  ui->m_GridLayout->addWidget(item, m_InsertRow++, 0, 1, 2);

  connect(item, SIGNAL(clicked()), m_SignalMapper, SLOT(map()));

  m_SignalMapper->setMapping(item, title);
}
