#include "qxrdwelcomewindow.h"
#include "ui_qxrdwelcomewindow.h"
#include "qxrdwelcomerecentitem.h"
#include "qxrdapplication.h"

QxrdWelcomeWindow::QxrdWelcomeWindow(QxrdApplication *app) :
  QMainWindow(NULL),
  ui(new Ui::QxrdWelcomeWindow),
  m_Application(app),
  m_InsertRow(4)
{
  ui->setupUi(this);

  QStringList recents = m_Application->get_RecentExperiments();

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
}
