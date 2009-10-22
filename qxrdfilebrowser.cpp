/******************************************************************
*
*  $Id: qxrdfilebrowser.cpp,v 1.2 2009/10/22 21:50:58 jennings Exp $
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
    SOURCE_IDENT("$Id: qxrdfilebrowser.cpp,v 1.2 2009/10/22 21:50:58 jennings Exp $")
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

  connect(m_FilterChoices, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged(int)));
  connect(m_FileSelector,  SIGNAL(textChanged(QString)), this, SLOT(onSelectorChanged(QString)));
  connect(m_OpenButton, SIGNAL(clicked()), this, SLOT(doOpen()));
  connect(m_ProcessButton, SIGNAL(clicked()), this, SLOT(doProcess()));

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

void QxrdFileBrowser::onSelectorChanged(QString str)
{
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
*  Revision 1.2  2009/10/22 21:50:58  jennings
*  More code for file browser, basic operation now works
*
*  Revision 1.1  2009/10/22 19:22:28  jennings
*  Initial file browser
*
*
*******************************************************************/
