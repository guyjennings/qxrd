#include "qxrdfilebrowser.h"
#include "qxrddataprocessor.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMenu>
#include "qxrdmutexlocker.h"
#include "qxrdfilebrowsermodelthread.h"
#include "qxrdfilebrowsermodel.h"
#include "qxrdfilebrowserview.h"

QxrdFileBrowser::QxrdFileBrowser(int isOutput, QxrdDataProcessor *processor, QWidget *parent)
  : QDockWidget(parent),
    m_IsOutput(isOutput),
    m_BrowserFilter(this, "browserFilter",1),
    m_BrowserSelector(this, "browserSelector",""),
    m_RootDirectory(this, "rootDirectory",""),
    m_Processor(processor),
    m_Model(NULL)
{
  setupUi(this);
  if (isOutput) {
    setWindowTitle("Output " + windowTitle());
  } else {
    setWindowTitle("Input " + windowTitle());
  }

//  m_ModelThread = new QxrdFileBrowserModelThread();
//  m_ModelThread -> start();

//  m_Model = m_ModelThread ->fileBrowserModel();  /*new QxrdFileBrowserModel();*/
//  m_Model -> setRootPath(QDir::currentPath());
  m_Model = new QxrdFileBrowserModel();
  m_Model -> setRootPath(QDir::currentPath());
//  m_Model -> moveToThread(m_ModelThread);

  m_FileBrowser -> setModel(m_Model);

  m_FileBrowser -> setRootPath(QDir::currentPath());

  m_FileBrowser -> resizeColumnsToContents();
  m_FileBrowser -> resizeRowsToContents();

  m_FileBrowser->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_FileBrowser->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

  m_Model -> setNameFilters(QStringList("*.tif"));
  m_Model -> setNameFilterDisables(false);

  connect(m_Model, SIGNAL(modelReset()), this, SLOT(onModelReset()));

  connect(this, SIGNAL(printMessage(QString,QDateTime)), m_Processor, SIGNAL(printMessage(QString,QDateTime)));
  connect(this, SIGNAL(statusMessage(QString,QDateTime)), m_Processor, SIGNAL(statusMessage(QString,QDateTime)));
  connect(this, SIGNAL(criticalMessage(QString,QDateTime)), m_Processor, SIGNAL(criticalMessage(QString,QDateTime)));

  connect(m_FilterChoices, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged(int)));
  connect(m_FileSelector,  SIGNAL(textChanged(QString)), this, SLOT(onSelectorChanged(QString)));
  connect(m_PrevDirectoryButton, SIGNAL(clicked()), this, SLOT(doPreviousDirectory()));
  connect(m_UpDirectoryButton, SIGNAL(clicked()), this, SLOT(doUpDirectory()));
  connect(m_ChangeDirectoryButton, SIGNAL(clicked()), this, SLOT(doChangeDirectory()));
  connect(m_HomeDirectoryButton, SIGNAL(clicked()), this, SLOT(doHomeDirectory()));
  connect(m_AcquisitionDirectoryButton, SIGNAL(clicked()), this, SLOT(doAcquisitionDirectory()));
  connect(m_RefreshButton, SIGNAL(clicked()), this, SLOT(doRefreshBrowser()));
  connect(m_OpenButton, SIGNAL(clicked()), this, SLOT(doOpen()));
  connect(m_ProcessButton, SIGNAL(clicked()), this, SLOT(doProcess()));
  connect(m_IntegrateButton, SIGNAL(clicked()), this, SLOT(doIntegrate()));
  connect(m_AccumulateButton, SIGNAL(clicked()), this, SLOT(doAccumulate()));

  connect(this -> prop_RootDirectory(), SIGNAL(changedValue(QString)), this, SLOT(onRootDirectoryChanged(QString)));

  connect(m_FileBrowser, SIGNAL(pressed(QModelIndex)), this, SLOT(mousePressed(QModelIndex)));
  connect(m_FileBrowser, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));

  connect(m_RootDirectoryCombo, SIGNAL(activated(int)), this, SLOT(doSelectComboItem(int)));

  prop_BrowserFilter() -> linkTo(m_FilterChoices);
  prop_BrowserSelector() -> linkTo(m_FileSelector);

  m_PrevDirectoryButton->setEnabled(false);
  m_RootDirectoryCombo -> setItemText(0, get_RootDirectory());
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

  case 4: // Integrated data files
    m_Model -> setNameFilters(QStringList("*.avg"));
    break;

  case 5: // Metadata files
    m_Model -> setNameFilters(QStringList("*.metadata"));
    break;

  case 6: // All files
    m_Model -> setNameFilters(QStringList());
    break;
  }
}

void QxrdFileBrowser::onSelectorChanged(QString str, const QModelIndex &parent)
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QRegExp pattern(str, Qt::CaseSensitive, QRegExp::Wildcard);

  int rows = m_Model -> rowCount(parent);

  for (int i=0; i<rows; i++) {
    QModelIndex index = m_Model -> index(i, 0, parent);

    QString path = m_Model->fileName(index);
//    emit printMessage(tr("Testing %1").arg(path));

    if (pattern.exactMatch(path)) {
      sel -> select(index, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    } else {
      sel -> select(index, QItemSelectionModel::Rows | QItemSelectionModel::Deselect);
    }
  }
}

void QxrdFileBrowser::doSelectComboItem(int index)
{
  QString path = m_RootDirectoryCombo->itemText(index);

  if (path != "") {
    doPushDirectory(path);
  }
}

void QxrdFileBrowser::onRootDirectoryChanged(QString str)
{
  m_Model -> setRootPath(str);

  QDir dir(str);

  m_RootDirectoryCombo -> clear();
  m_RootDirectoryCombo -> insertItem(0, str);

  while (dir.cdUp()) {
    m_RootDirectoryCombo -> insertItem(0, dir.path());
  }
}

void QxrdFileBrowser::doPushDirectory(QString newDir)
{
  if (newDir != "") {
    m_DirectoryStack.append(get_RootDirectory());

    set_RootDirectory(newDir);

    m_PrevDirectoryButton->setEnabled(true);
  }
}

void QxrdFileBrowser::doPreviousDirectory()
{
  if (!m_DirectoryStack.isEmpty()) {
    set_RootDirectory(m_DirectoryStack.takeLast());

    m_PrevDirectoryButton->setEnabled(!m_DirectoryStack.isEmpty());
  }
}

void QxrdFileBrowser::doChangeDirectory()
{
  QString newRoot = QFileDialog::getExistingDirectory(this, "New browser directory...", get_RootDirectory(), 0);

  if (newRoot != "") {
    doPushDirectory(newRoot);
  }
}

void QxrdFileBrowser::doUpDirectory()
{
  QDir dir(get_RootDirectory());

  if (dir.cdUp()) {
    doPushDirectory(dir.path());
  }
}

void QxrdFileBrowser::doHomeDirectory()
{
  doPushDirectory(QDir::currentPath());
}

void QxrdFileBrowser::doAcquisitionDirectory()
{
  doPushDirectory(m_Processor->get_OutputDirectory());
}

void QxrdFileBrowser::doOpen()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;
  int ndirs = 0, nfiles = 0;
  foreach(index, rows) {
    if (m_Model->isDir(index)) {
      ndirs += 1;
    } else {
      nfiles += 1;
    }
  }

  if (nfiles == 0) {
    foreach(index, rows) {
      if (m_Model->isDir(index)) {
        doPushDirectory(m_Model->filePath(index));
        return;
      }
    }
  } else {
    foreach(index, rows) {
      if (!m_Model->isDir(index)) {
        //    printf("Open: %s\n", qPrintable(m_Model->filePath(index)));
        INVOKE_CHECK(QMetaObject::invokeMethod(m_Processor, "loadData", Qt::QueuedConnection, Q_ARG(QString, m_Model->filePath(index))));
      }
    }
  }
}

void QxrdFileBrowser::doProcess()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  foreach(index, rows) {
//    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
    if (!m_Model->isDir(index)) {
      INVOKE_CHECK(QMetaObject::invokeMethod(m_Processor, "processData", Qt::QueuedConnection, Q_ARG(QString, m_Model->filePath(index))));
    }
  }
}

void QxrdFileBrowser::doIntegrate()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  foreach(index, rows) {
//    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
    if (!m_Model->isDir(index)) {
      INVOKE_CHECK(QMetaObject::invokeMethod(m_Processor, "integrateData", Qt::QueuedConnection, Q_ARG(QString, m_Model->filePath(index))));
    }
  }
}

void QxrdFileBrowser::doAccumulate()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;
  QStringList paths;

  foreach(index, rows) {
    if (!m_Model->isDir(index)) {
      paths.append(m_Model->filePath(index));
    }
  }

  INVOKE_CHECK(QMetaObject::invokeMethod(m_Processor, "accumulateImages", Qt::QueuedConnection, Q_ARG(QStringList, paths)));
}

void QxrdFileBrowser::doRefreshBrowser()
{
  m_Model->refresh();
}

void QxrdFileBrowser::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  if (m_FileBrowser) {
    settings.beginWriteArray(section+"/colWidths",4);

    for (int i=0; i<3; i++) {
      settings.setArrayIndex(i);
      settings.setValue("width", m_FileBrowser->columnWidth(i));
    }

    settings.endArray();
  }
}

void QxrdFileBrowser::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  int sz = settings.beginReadArray(section+"/colWidths");

  for (int i=0; i<sz; i++) {
    settings.setArrayIndex(i);
    int width = settings.value("width", -1).toInt();

    if (m_FileBrowser) {
      if (width > 5) {
        m_FileBrowser->setColumnWidth(i, width);
      } else {
        m_FileBrowser->setColumnWidth(i, 5);
      }
    }
  }

  settings.endArray();
}

void QxrdFileBrowser::mousePressed(QModelIndex /*index*/)
{
  if (QApplication::mouseButtons() & Qt::RightButton) {
//    emit printMessage("Right mouse pressed");

    QMenu *actions = new QMenu(this);
    QAction *open = actions->addAction("Open");
    QAction *accumulate = actions->addAction("Accumulate");
    QAction *integrate = actions->addAction("Integrate");
    QAction *process = actions->addAction("Process");

    QAction *action = actions->exec(QCursor::pos());

    if (action == open) {
      doOpen();
    } else if (action == accumulate) {
      doAccumulate();
    } else if (action == integrate) {
      doIntegrate();
    } else if (action == process) {
      doProcess();
    }
  }
}

void QxrdFileBrowser::doubleClicked(QModelIndex index)
{
  doOpen();
}

void QxrdFileBrowser::onRowCountChanged(int oldCount, int newCount)
{
  printf("QxrdFileBrowser::onRowCountChanged(%d,%d)\n", oldCount, newCount);

  m_FileBrowser->resizeColumnsToContents();
}

void QxrdFileBrowser::onModelReset()
{
  m_FileBrowser->resizeColumnsToContents();
  m_FileBrowser->resizeRowsToContents();
}

QxrdInputFileBrowser::QxrdInputFileBrowser(QxrdDataProcessor *processor, QWidget *parent)
  : QxrdFileBrowser(false, processor, parent)
{
}

QxrdOutputFileBrowser::QxrdOutputFileBrowser(QxrdDataProcessor *processor, QWidget *parent)
  : QxrdFileBrowser(true, processor, parent)
{
}

