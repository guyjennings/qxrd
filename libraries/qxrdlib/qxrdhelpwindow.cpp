#include "qxrdhelpwindow.h"
#include "ui_qxrdhelpwindow.h"
#include "qxrdtodolist.h"
#include <QSortFilterProxyModel>

QxrdHelpWindow::QxrdHelpWindow(QxrdHelpWindowSettingsWPtr set, QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acqw, QxrdProcessorWPtr procw) :
  QxrdMainWindow(name, app, expt, acqw, procw),
  m_HelpWindowSettings(set)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  connect(m_HelpHomeButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::home);
  connect(m_HelpForwardButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::forward);
  connect(m_HelpBackButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::backward);

  connect(m_HelpBrowser, &QTextBrowser::forwardAvailable, m_HelpForwardButton, &QWidget::setEnabled);
  connect(m_HelpBrowser, &QTextBrowser::backwardAvailable, m_HelpBackButton, &QWidget::setEnabled);

  m_HelpBrowser->init(m_Experiment);

  QxrdToDoList *toDoList = new QxrdToDoList(this);
  QSortFilterProxyModel *sorted = new QSortFilterProxyModel(this);

  sorted->setSourceModel(toDoList);
  sorted->sort(0, Qt::DescendingOrder);

  m_ToDoList->setModel(sorted);
  m_ToDoList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_ToDoList->horizontalHeader()->setStretchLastSection(true);
  m_ToDoList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_ToDoList->setSelectionBehavior(QAbstractItemView::SelectRows);
}

QxrdHelpWindow::~QxrdHelpWindow()
{
}

void QxrdHelpWindow::changeEvent(QEvent *e)
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
