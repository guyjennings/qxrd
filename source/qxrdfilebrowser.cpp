#include "qxrddebug.h"
#include "qxrdfilebrowser.h"
#include "qxrddataprocessor.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include <QMetaObject>
#include "qcepmutexlocker.h"
#include "qxrdfilebrowsermodel.h"
#include "qxrdfilebrowserview.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qcepsettingssaver.h"

QxrdFileBrowser::QxrdFileBrowser(QxrdFileBrowserSettingsWPtr settings,
                                 int isOutput,
                                 QxrdExperimentWPtr experiment,
                                 QxrdDataProcessorWPtr processor,
                                 QWidget *parent)
  : QDockWidget(parent),
    m_FileBrowserSettings(settings),
    m_IsOutput(isOutput),
    m_Experiment(experiment),
    m_Processor(processor),
    m_Model(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileBrowser::QxrdFileBrowser(%p)\n", this);
  }

  qRegisterMetaType<QFileInfo>("QFileInfo");

  setupUi(this);
  if (isOutput) {
    setWindowTitle("Output " + windowTitle());
  } else {
    setWindowTitle("Input " + windowTitle());
  }

  m_Model = QxrdFileBrowserModelPtr(
        new QxrdFileBrowserModel());

  m_Model -> initialize(m_Model);
  m_Model -> setRootPath(QDir::currentPath());

  m_FileBrowser -> setModel(m_Model.data());

  m_FileBrowser -> resizeColumnsToContents();
  m_FileBrowser -> resizeRowsToContents();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  m_FileBrowser->horizontalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
  m_FileBrowser->verticalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
#else
  m_FileBrowser->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_FileBrowser->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif

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

  QxrdFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set) {
    connect(set -> prop_RootDirectory(), SIGNAL(valueChanged(QString,int)), this, SLOT(onRootDirectoryChanged(QString)));
    connect(set -> prop_BrowserFilter(), SIGNAL(valueChanged(int,int)), this, SLOT(onFilterChanged(int)));
    connect(set -> prop_BrowserSelector(), SIGNAL(valueChanged(QString,int)), this, SLOT(onSelectorChanged(QString)));

    onRootDirectoryChanged(set->get_RootDirectory());
    onFilterChanged(set->get_BrowserFilter());
    onSelectorChanged(set->get_BrowserSelector());
  }

  connect(m_FileBrowser, SIGNAL(pressed(QModelIndex)), this, SLOT(mousePressed(QModelIndex)));
  connect(m_FileBrowser, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));

  connect(m_RootDirectoryCombo, SIGNAL(activated(int)), this, SLOT(doSelectComboItem(int)));

  if (set) {
    set->prop_BrowserFilter() -> linkTo(m_FilterChoices);
    set->prop_BrowserSelector() -> linkTo(m_FileSelector);
  }

  m_PrevDirectoryButton->setEnabled(false);

  if (set) {
    m_RootDirectoryCombo -> setItemText(0, set->get_RootDirectory());
  }
}

QxrdFileBrowser::~QxrdFileBrowser()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileBrowser::~QxrdFileBrowser(%p)\n", this);
  }
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
  QxrdFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set && newDir != "") {
    m_DirectoryStack.append(set->get_RootDirectory());

    set->set_RootDirectory(newDir);

    m_PrevDirectoryButton->setEnabled(true);
  }
}

void QxrdFileBrowser::doPreviousDirectory()
{
  QxrdFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set && !m_DirectoryStack.isEmpty()) {
    set->set_RootDirectory(m_DirectoryStack.takeLast());

    m_PrevDirectoryButton->setEnabled(!m_DirectoryStack.isEmpty());
  }
}

void QxrdFileBrowser::doChangeDirectory()
{
  QxrdFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set) {
    QString newRoot = QFileDialog::getExistingDirectory(this, "New browser directory...", set->get_RootDirectory(), 0);

    if (newRoot != "") {
      doPushDirectory(newRoot);
    }
  }
}

void QxrdFileBrowser::doUpDirectory()
{
  QxrdFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set) {
    QDir dir(set->get_RootDirectory());

    if (dir.cdUp()) {
      doPushDirectory(dir.path());
    }
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
    QMetaObject::invokeMethod(proc.data(), "accumulateImages", Q_ARG(QStringList, paths));
  }
}

void QxrdFileBrowser::doClearAccumulator()
{
  QxrdDataProcessorPtr proc(m_Processor);

  if (proc) {
    QMetaObject::invokeMethod(proc.data(), "clearAccumulator");
  }
}

void QxrdFileBrowser::doIntegrateAndAccumulate()
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
    QMetaObject::invokeMethod(proc.data(), "integrateAndAccumulate", Q_ARG(QStringList, paths));
  }
}

void QxrdFileBrowser::doSaveAccumulator()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;
  QString path;

  if (rows.count() >= 1) {
    path = m_Model->filePath(rows.first());
  } else {
    path = m_Model->rootPath();
  }

  QString file = QFileDialog::getSaveFileName(this, "Save accumulated data in...", path);

  if (file.length() > 0) {
    QxrdDataProcessorPtr proc(m_Processor);

    if (proc) {
      QMetaObject::invokeMethod(proc.data(), "saveAccumulator", Q_ARG(QString, file));
    }
  }
}

void QxrdFileBrowser::doAdd()
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
    QMetaObject::invokeMethod(proc.data(), "addImages", Q_ARG(QStringList, paths));
  }
}

void QxrdFileBrowser::doSubtract()
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
    QMetaObject::invokeMethod(proc.data(), "subtractImages", Q_ARG(QStringList, paths));
  }
}

void QxrdFileBrowser::doProjectX()
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
    QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, paths), Q_ARG(int, 1), Q_ARG(int, 0), Q_ARG(int, 0));
  }
}

void QxrdFileBrowser::doProjectY()
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
    QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, paths), Q_ARG(int, 0), Q_ARG(int, 1), Q_ARG(int, 0));
  }
}

void QxrdFileBrowser::doProjectZ()
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
    QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, paths), Q_ARG(int, 0), Q_ARG(int, 0), Q_ARG(int, 1));
  }
}

void QxrdFileBrowser::doCorrelate()
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
    QMetaObject::invokeMethod(proc.data(), "correlateImages", Q_ARG(QStringList, paths));
  }
}

void QxrdFileBrowser::doEvaluate(QString filePath)
{
//  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
//  QModelIndexList rows = sel->selectedRows();
//  QModelIndex index;
//  QStringList paths;

//  foreach(index, rows) {
//    if (!m_Model->isDir(index)) {
//      paths.append(m_Model->filePath(index));
//    }
//  }

  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    INVOKE_CHECK(QMetaObject::invokeMethod(exp.data(), "evaluateScriptFile", Q_ARG(QString, filePath)));
  }
}

void QxrdFileBrowser::doRefreshBrowser()
{
  m_Model->refresh();
}

void QxrdFileBrowser::mousePressed(QModelIndex index)
{
  if (QApplication::mouseButtons() & Qt::RightButton) {
    //    g_Application->printMessage("Right mouse pressed");

    QString filePath = m_Model->filePath(index);

    QMenu *actions = new QMenu(this);
    QAction *open = actions->addAction("Open");
    QAction *openDark = actions->addAction("Open as Dark Image");
    QAction *openMask = actions->addAction("Open as Mask");
    QAction *openGainMap = actions->addAction("Open as Gain Map");
    QAction *accumulate = actions->addAction("Accumulate");
    QAction *integrate = actions->addAction("Integrate");
    QAction *clearAccum = actions->addAction("Clear accumulator");
    QAction *integAccum = actions->addAction("Integrate and accumulate");
    QAction *saveAccum = actions->addAction("Save accumulator in...");
    QAction *process = actions->addAction("Process");
    QAction *add = actions->addAction("Add Images to Current Image");
    QAction *subtract = actions->addAction("Subtract Images from Current Image");
    QAction *projectX = actions->addAction("Project Along X");
    QAction *projectY = actions->addAction("Project Along Y");
    QAction *projectZ = actions->addAction("Project Along Z");
    QAction *correlate = actions->addAction("Correlate Images with Current Image");
    QAction *evaluate = actions->addAction("Evaluate scripts");

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
    } else if (action == clearAccum) {
      doClearAccumulator();
    } else if (action == integAccum) {
      doIntegrateAndAccumulate();
    } else if (action == saveAccum) {
      doSaveAccumulator();
    } else if (action == process) {
      doProcess();
    } else if (action == add) {
      doAdd();
    } else if (action == subtract) {
      doSubtract();
    } else if (action == projectX) {
      doProjectX();
    } else if (action == projectY) {
      doProjectY();
    } else if (action == projectZ) {
      doProjectZ();
    } else if (action == correlate) {
      doCorrelate();
    } else  if (action == evaluate) {
      doEvaluate(filePath);
    }
  }
}

void QxrdFileBrowser::doubleClicked(QModelIndex /*index*/)
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

//  m_FileBrowser->resizeColumnsToContents();
}

void QxrdFileBrowser::onModelReset()
{
//  m_FileBrowser->resizeColumnsToContents();
//  m_FileBrowser->resizeRowsToContents();
}

void QxrdFileBrowser::onFileUpdated(QFileInfo file)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt && qcepDebug(DEBUG_DISPLAY)) {
    expt->printMessage(
          tr("QxrdFileBrowser::fileUpdated(\"%1\",\"%2\")").arg(file.filePath()).arg(file.lastModified().toString()));
  }
}

QxrdInputFileBrowser::QxrdInputFileBrowser(QxrdFileBrowserSettingsWPtr settings,
                                           QxrdExperimentWPtr experiment,
                                           QxrdDataProcessorWPtr processor,
                                           QWidget *parent)
  : QxrdFileBrowser(settings, false, experiment, processor, parent)
{
}

QxrdOutputFileBrowser::QxrdOutputFileBrowser(QxrdFileBrowserSettingsWPtr settings,
                                             QxrdExperimentWPtr experiment,
                                             QxrdDataProcessorWPtr processor,
                                             QWidget *parent)
  : QxrdFileBrowser(settings, true, experiment, processor, parent)
{
}

