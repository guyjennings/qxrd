#include "qxrdfilebrowser.h"
#include "qxrddataprocessor.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include "qxrdmutexlocker.h"
#include "qxrdfilebrowsermodel.h"
#include "qxrdfilebrowserview.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"

QxrdFileBrowser::QxrdFileBrowser(QxrdSettingsSaverPtr saver,
                                 int isOutput,
                                 QxrdExperimentWPtr experiment,
                                 QxrdDataProcessorWPtr processor,
                                 QWidget *parent)
  : QDockWidget(parent),
    m_IsOutput(isOutput),
    m_Experiment(experiment),
    m_Processor(processor),
    m_Model(NULL)
{
  qRegisterMetaType<QFileInfo>("QFileInfo");

  setupUi(this);
  if (isOutput) {
    setWindowTitle("Output " + windowTitle());
  } else {
    setWindowTitle("Input " + windowTitle());
  }

  m_Model = QxrdFileBrowserModelPtr(
        new QxrdFileBrowserModel());

  m_Model -> initialize();
  m_Model -> setRootPath(QDir::currentPath());

  m_FileBrowser -> setModel(m_Model.data());

  m_FileBrowser -> setRootPath(QDir::currentPath());

  m_FileBrowser -> resizeColumnsToContents();
  m_FileBrowser -> resizeRowsToContents();

  m_FileBrowser->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_FileBrowser->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

  m_Model -> setNameFilters(QStringList("*.tif"));
  m_Model -> setNameFilterDisables(false);

  connect(m_Model.data(), SIGNAL(modelReset()), this, SLOT(onModelReset()));
  connect(m_Model.data(), SIGNAL(fileUpdated(QFileInfo)), this, SLOT(onFileUpdated(QFileInfo)));

//  connect(m_FilterChoices, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged(int)));
//  connect(m_FileSelector,  SIGNAL(textChanged(QString)), this, SLOT(onSelectorChanged(QString)));
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

  connect(this -> prop_RootDirectory(), SIGNAL(valueChanged(QString,int)), this, SLOT(onRootDirectoryChanged(QString)));
  connect(this -> prop_BrowserFilter(), SIGNAL(valueChanged(int,int)), this, SLOT(onFilterChanged(int)));
  connect(this -> prop_BrowserSelector(), SIGNAL(valueChanged(QString,int)), this, SLOT(onSelectorChanged(QString)));

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
//    g_Application->printMessage(tr("Testing %1").arg(path));

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
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    doPushDirectory(exp->get_ExperimentDirectory());
  }
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
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      foreach(index, rows) {
        if (!m_Model->isDir(index)) {
          //    printf("Open: %s\n", qPrintable(m_Model->filePath(index)));
          proc->loadData(m_Model->filePath(index));
        }
      }
    }
  }
}

void QxrdFileBrowser::doOpenDark()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
        proc->loadDark(m_Model->filePath(index));
      }
    }
  }
}

void QxrdFileBrowser::doOpenMask()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
        proc->loadMask(m_Model->filePath(index));
      }
    }
  }
}

void QxrdFileBrowser::doOpenGainMap()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
        proc->loadGainMap(m_Model->filePath(index));
      }
    }
  }
}

void QxrdFileBrowser::doProcess()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
        proc->processData(m_Model->filePath(index));
      }
    }
  }
}

void QxrdFileBrowser::doIntegrate()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
        proc->integrateData(m_Model->filePath(index));
      }
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

  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    proc->accumulateImages(paths);
  }
}

void QxrdFileBrowser::doRefreshBrowser()
{
  m_Model->refresh();
}

void QxrdFileBrowser::mousePressed(QModelIndex /*index*/)
{
  if (QApplication::mouseButtons() & Qt::RightButton) {
//    g_Application->printMessage("Right mouse pressed");

    QMenu *actions = new QMenu(this);
    QAction *open = actions->addAction("Open");
    QAction *openDark = actions->addAction("Open as Dark Image");
    QAction *openMask = actions->addAction("Open as Mask");
    QAction *openGainMap = actions->addAction("Open as Gain Map");
    QAction *accumulate = actions->addAction("Accumulate");
    QAction *integrate = actions->addAction("Integrate");
    QAction *process = actions->addAction("Process");

    QAction *action = actions->exec(QCursor::pos());

    if (action == open) {
        doOpen();
    } else if (action == openDark) {
        doOpenDark();
    } else if (action == openMask) {
        doOpenMask();
    } else if (action == openGainMap) {
        doOpenGainMap();
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
  QxrdExperimentPtr expt(m_Experiment);

  if (expt && qcepDebug(DEBUG_DISPLAY)) {
    expt->printMessage(
          tr("QxrdFileBrowser::onRowCountChanged(%1,%2)").arg(oldCount).arg(newCount));
  }

  m_FileBrowser->resizeColumnsToContents();
}

void QxrdFileBrowser::onModelReset()
{
  m_FileBrowser->resizeColumnsToContents();
  m_FileBrowser->resizeRowsToContents();
}

void QxrdFileBrowser::onFileUpdated(QFileInfo file)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    expt->printMessage(
          tr("QxrdFileBrowser::fileUpdated(\"%1\",\"%2\")").arg(file.filePath()).arg(file.lastModified().toString()));
  }
}

QxrdInputFileBrowser::QxrdInputFileBrowser(QxrdSettingsSaverPtr saver,
                                           QxrdExperimentWPtr experiment,
                                           QxrdDataProcessorWPtr processor,
                                           QWidget *parent)
  : QxrdFileBrowser(saver, false, experiment, processor, parent)
{
}

QxrdOutputFileBrowser::QxrdOutputFileBrowser(QxrdSettingsSaverPtr saver,
                                             QxrdExperimentWPtr experiment,
                                             QxrdDataProcessorWPtr processor,
                                             QWidget *parent)
  : QxrdFileBrowser(saver, true, experiment, processor, parent)
{
}

