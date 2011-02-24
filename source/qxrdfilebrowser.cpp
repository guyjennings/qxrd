#include "qxrdfilebrowser.h"
#include "qxrddataprocessor.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMenu>
#include "qxrdmutexlocker.h"
#include "qxrdfilebrowsermodel.h"
#include "qxrdfilebrowserview.h"

QxrdFileBrowser::QxrdFileBrowser(int isOutput, QxrdDataProcessorPtr processor, QWidget *parent)
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

  m_Model = new QxrdFileBrowserModel();
  m_Model -> setRootPath(QDir::currentPath());
  m_FileBrowser -> setModel(m_Model);
  m_FileBrowser -> setRootIndex(m_Model->index(QDir::currentPath()));
  m_FileBrowser -> setUniformRowHeights(true);
  m_FileBrowser -> setExpandsOnDoubleClick(false);

//  connect(m_FileBrowser, SIGNAL(rowCountChanged(int,int)), this, SLOT(onRowCountChanged(int,int)));

//  m_FileBrowser -> setColumnHidden(1,true); // Size
  m_FileBrowser -> setColumnHidden(2,true); // Type
//  m_FileBrowser -> setColumnHidden(3,true); // Modified

  m_Model -> setNameFilters(QStringList("*.tif"));
  m_Model -> setNameFilterDisables(false);

  connect(this, SIGNAL(printMessage(QDateTime,QString)), m_Processor, SIGNAL(printMessage(QDateTime,QString)));
  connect(this, SIGNAL(statusMessage(QDateTime,QString)), m_Processor, SIGNAL(statusMessage(QDateTime,QString)));
  connect(this, SIGNAL(criticalMessage(QDateTime,QString)), m_Processor, SIGNAL(criticalMessage(QDateTime,QString)));

  connect(m_FilterChoices, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged(int)));
  connect(m_FileSelector,  SIGNAL(textChanged(QString)), this, SLOT(onSelectorChanged(QString)));
  connect(m_PrevDirectoryButton, SIGNAL(clicked()), this, SLOT(doPreviousDirectory()));
  connect(m_UpDirectoryButton, SIGNAL(clicked()), this, SLOT(doUpDirectory()));
  connect(m_ChangeDirectoryButton, SIGNAL(clicked()), this, SLOT(doChangeDirectory()));
  connect(m_HomeDirectoryButton, SIGNAL(clicked()), this, SLOT(doHomeDirectory()));
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
  m_FileBrowser -> setRootIndex(m_Model->index(str));

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

void QxrdFileBrowser::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdFileBrowser::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdFileBrowser::mousePressed(QModelIndex /*index*/)
{
  if (QApplication::mouseButtons() & Qt::RightButton) {
//    emit printMessage(QDateTime::currentDateTime(), "Right mouse pressed");

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

  m_FileBrowser->resizeColumnToContents(0);
}

QxrdInputFileBrowser::QxrdInputFileBrowser(QxrdDataProcessorPtr processor, QWidget *parent)
  : QxrdFileBrowser(false, processor, parent)
{
}

QxrdOutputFileBrowser::QxrdOutputFileBrowser(QxrdDataProcessorPtr processor, QWidget *parent)
  : QxrdFileBrowser(true, processor, parent)
{
}
