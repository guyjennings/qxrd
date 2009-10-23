/******************************************************************
*
*  $Id: qxrdfilebrowser.cpp,v 1.4 2009/10/23 19:42:01 jennings Exp $
*
*******************************************************************/

#include "qxrdfilebrowser.h"
#include "qxrddataprocessor.h"
#include <QFileSystemModel>
#include <QFileDialog>

QxrdFileBrowser::QxrdFileBrowser(QxrdDataProcessor *processor, QWidget *parent)
  : QWidget(parent),
    m_BrowserFilter(this, "BrowserFilter",1),
    m_BrowserSelector(this, "BrowserSelector",""),
    m_Processor(processor),
    m_Model(NULL),
    SOURCE_IDENT("$Id: qxrdfilebrowser.cpp,v 1.4 2009/10/23 19:42:01 jennings Exp $")
{
  setupUi(this);

  m_Model = new QFileSystemModel();
  m_Model -> setRootPath(QDir::currentPath());
  m_FileBrowser -> setModel(m_Model);
  m_FileBrowser -> setRootIndex(m_Model->index(QDir::currentPath()));

  m_FileBrowser -> setColumnHidden(1,true);
  m_FileBrowser -> setColumnHidden(2,true);
  m_FileBrowser -> setColumnHidden(3,true);

  m_Model -> setNameFilters(QStringList("*.tif"));
  m_Model -> setNameFilterDisables(false);

  connect(this, SIGNAL(printMessage(QString)), m_Processor, SIGNAL(printMessage(QString)));
  connect(this, SIGNAL(statusMessage(QString)), m_Processor, SIGNAL(statusMessage(QString)));
  connect(this, SIGNAL(criticalMessage(QString)), m_Processor, SIGNAL(criticalMessage(QString)));

  connect(m_FilterChoices, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged(int)));
  connect(m_FileSelector,  SIGNAL(textChanged(QString)), this, SLOT(onSelectorChanged(QString)));
  connect(m_OpenButton, SIGNAL(clicked()), this, SLOT(doOpen()));
  connect(m_ProcessButton, SIGNAL(clicked()), this, SLOT(doProcess()));
  connect(m_Processor -> prop_OutputDirectory(), SIGNAL(changedValue(QString)), this, SLOT(onRootDirectoryChanged(QString)));

  prop_BrowserFilter() -> linkTo(m_FilterChoices);
  prop_BrowserSelector() -> linkTo(m_FileSelector);
}

void QxrdFileBrowser::onFilterChanged(int newfilter)
{
  switch (newfilter) {
  case 0: // Image Files (*.tif)
    m_Model -> setNameFilters(QStringList("*.tif"));
    break;

  case 1: // Processed Image Files (*nnnnn.tif)
    m_Model -> setNameFilters(QStringList("*[0-9][0-9][0-9][0-9][0-9].tif"));
    break;

  case 2: // Raw image files
    m_Model -> setNameFilters(QStringList("*.raw.tif"));
    break;

  case 3: // Dark image files
    m_Model -> setNameFilters(QStringList("*.dark.tif"));
    break;

  case 4: // All files
    m_Model -> setNameFilters(QStringList());
    break;
  }
}

void QxrdFileBrowser::onSelectorChanged(QString str, QModelIndex parent)
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QRegExp pattern(str, Qt::CaseSensitive, QRegExp::Wildcard);

  int rows = m_Model -> rowCount(parent);

  for (int i=0; i<rows; i++) {
    QModelIndex index = m_Model -> index(i,0, parent);

    QString path = m_Model->fileName(index);
//    emit printMessage(tr("Testing %1").arg(path));

    if (pattern.exactMatch(path)) {
      sel -> select(index, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    } else {
      sel -> select(index, QItemSelectionModel::Rows | QItemSelectionModel::Deselect);
    }

    if (m_Model -> hasChildren(index)) {
//      emit printMessage(tr("%1 has children").arg(path));

      onSelectorChanged(str, index);
    }
  }
}

void QxrdFileBrowser::onRootDirectoryChanged(QString str)
{
  m_Model -> setRootPath(str);
  m_FileBrowser -> setRootIndex(m_Model->index(str));
}

void QxrdFileBrowser::doOpen()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  foreach(index, rows) {
//    printf("Open: %s\n", qPrintable(m_Model->filePath(index)));
    QMetaObject::invokeMethod(m_Processor, "loadData", Qt::QueuedConnection, Q_ARG(QString, m_Model->filePath(index)));
  }
}

void QxrdFileBrowser::doProcess()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  foreach(index, rows) {
//    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
    QMetaObject::invokeMethod(m_Processor, "processData", Qt::QueuedConnection, Q_ARG(QString, m_Model->filePath(index)));
  }
}

void QxrdFileBrowser::writeSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdFileBrowser::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

/******************************************************************
*
*  $Log: qxrdfilebrowser.cpp,v $
*  Revision 1.4  2009/10/23 19:42:01  jennings
*  Implement file selector box for file browser, change file browser root when output directory is changed
*
*  Revision 1.3  2009/10/23 04:39:58  jennings
*  Unfiltered files hidden, rather than dimmed
*
*  Revision 1.2  2009/10/22 21:50:58  jennings
*  More code for file browser, basic operation now works
*
*  Revision 1.1  2009/10/22 19:22:28  jennings
*  Initial file browser
*
*
*******************************************************************/
