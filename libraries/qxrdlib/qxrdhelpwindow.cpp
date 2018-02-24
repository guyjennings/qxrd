#include "qxrdhelpwindow.h"
#include "ui_qxrdhelpwindow.h"
#include "qxrdtodolist.h"
#include <QSortFilterProxyModel>
#include <QThread>
#include "qxrdexperiment.h"

QxrdHelpWindow::QxrdHelpWindow(QString name) :
  inherited(name)
{
}

void QxrdHelpWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  inherited::initialize(parent);

  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  connect(m_HelpHomeButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::home);
  connect(m_HelpForwardButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::forward);
  connect(m_HelpBackButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::backward);

  connect(m_HelpBrowser, &QTextBrowser::forwardAvailable, m_HelpForwardButton, &QWidget::setEnabled);
  connect(m_HelpBrowser, &QTextBrowser::backwardAvailable, m_HelpBackButton, &QWidget::setEnabled);

  m_HelpBrowser->initialize(QxrdExperiment::findExperiment(m_Parent));

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
  inherited::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
