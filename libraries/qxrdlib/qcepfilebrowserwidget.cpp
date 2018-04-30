#include "qxrddebug.h"
#include "qcepfilebrowserwidget.h"
#include "qcepfilebrowsersettings.h"
#include "qcepfilebrowsermodel.h"
#include <QMenu>
#include <QDir>
#include <QFileDialog>
#include "qxrdexperiment.h"
#include "qxrdprocessor.h"
#include "qcepdatasetmodel.h"

QcepFileBrowserWidget::QcepFileBrowserWidget(QWidget *parent) :
  QWidget(parent)
{
  setupUi(this);
}

QcepFileBrowserWidget::~QcepFileBrowserWidget()
{
  haltBrowser();
}

void QcepFileBrowserWidget::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QcepFileBrowserWidget::initialize(QcepFileBrowserSettingsWPtr settings,
                                 QxrdExperimentWPtr          experiment,
                                 QxrdProcessorWPtr       processor)
{
  m_FileBrowserSettings = settings;
  m_Experiment          = experiment;
  m_Processor           = processor;
  m_Model = QcepFileBrowserModelPtr(
        NEWPTR(QcepFileBrowserModel("fileBrowserModel")));

  m_Model -> initialize(experiment);
  m_Model -> setRootPath(QDir::currentPath());

  m_FileBrowser -> setModel(m_Model.data());

//  m_FileBrowser -> resizeColumnsToContents();
//  m_FileBrowser -> resizeRowsToContents();

  m_FileBrowser->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_FileBrowser->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  m_Model -> setNameFilters(QStringList("*.tif"));
  m_Model -> setNameFilterDisables(false);

  CONNECT_CHECK(connect(m_Model.data(), &QAbstractItemModel::modelReset, this, &QcepFileBrowserWidget::onModelReset));
  CONNECT_CHECK(connect(m_Model.data(), &QcepFileBrowserModel::fileUpdated, this, &QcepFileBrowserWidget::onFileUpdated));

  CONNECT_CHECK(connect(m_PrevDirectoryButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doPreviousDirectory));
  CONNECT_CHECK(connect(m_UpDirectoryButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doUpDirectory));
  CONNECT_CHECK(connect(m_ChangeDirectoryButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doChangeDirectory));
  CONNECT_CHECK(connect(m_HomeDirectoryButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doHomeDirectory));
  CONNECT_CHECK(connect(m_AcquisitionDirectoryButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doAcquisitionDirectory));
  CONNECT_CHECK(connect(m_RefreshButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doRefreshBrowser));
  CONNECT_CHECK(connect(m_OpenButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doOpen));
  CONNECT_CHECK(connect(m_ProcessButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doProcess));
  CONNECT_CHECK(connect(m_IntegrateButton, &QAbstractButton::clicked, this, &QcepFileBrowserWidget::doIntegrate));
  CONNECT_CHECK(connect(m_AccumulateButton, &QPushButton::clicked, this, &QcepFileBrowserWidget::doSumImages));

  QcepFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set) {
    CONNECT_CHECK(connect(set -> prop_RootDirectory(), &QcepStringProperty::valueChanged, this, &QcepFileBrowserWidget::onRootDirectoryChanged));
    CONNECT_CHECK(connect(set -> prop_BrowserFilter(), &QcepIntProperty::valueChanged, this, &QcepFileBrowserWidget::onFilterChanged));
    CONNECT_CHECK(connect(set -> prop_BrowserSelector(), &QcepStringProperty::valueChanged, this, &QcepFileBrowserWidget::onSelectorChanged));

    onRootDirectoryChanged(set->get_RootDirectory());
    onFilterChanged(set->get_BrowserFilter());
    onSelectorChanged(set->get_BrowserSelector());
  }

  CONNECT_CHECK(connect(m_FileBrowser, &QAbstractItemView::pressed, this, &QcepFileBrowserWidget::mousePressed));
  CONNECT_CHECK(connect(m_FileBrowser, &QAbstractItemView::doubleClicked, this, &QcepFileBrowserWidget::doubleClicked));

  CONNECT_CHECK(connect(m_RootDirectoryCombo, (void (QComboBox::*)(int)) &QComboBox::activated, this, &QcepFileBrowserWidget::doSelectComboItem));

  if (set) {
    set->prop_BrowserFilter() -> linkTo(m_FilterChoices);
    set->prop_BrowserSelector() -> linkTo(m_FileSelector);
  }

  m_PrevDirectoryButton->setEnabled(false);

  if (set) {
    m_RootDirectoryCombo -> setItemText(0, set->get_RootDirectory());
  }
}

void QcepFileBrowserWidget::haltBrowser()
{
  if (m_Model) {
    m_Model -> haltFileBrowser();
  }
}

void QcepFileBrowserWidget::onFilterChanged(int newfilter)
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

void QcepFileBrowserWidget::onSelectorChanged(QString str)
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QRegExp pattern(str, Qt::CaseSensitive, QRegExp::Wildcard);

  int rows = m_Model -> rowCount(QModelIndex());

  for (int i=0; i<rows; i++) {
    QModelIndex index = m_Model -> index(i, 0, QModelIndex());

    QString path = m_Model->fileName(index);
    //    g_Application->printMessage(tr("Testing %1").arg(path));

    if (pattern.exactMatch(path)) {
      sel -> select(index, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    } else {
      sel -> select(index, QItemSelectionModel::Rows | QItemSelectionModel::Deselect);
    }
  }
}

void QcepFileBrowserWidget::doSelectComboItem(int index)
{
  QString path = m_RootDirectoryCombo->itemText(index);

  if (path != "") {
    doPushDirectory(path);
  }
}

void QcepFileBrowserWidget::onRootDirectoryChanged(QString str)
{
  m_Model -> setRootPath(str);

  QDir dir(str);

  m_RootDirectoryCombo -> clear();
  m_RootDirectoryCombo -> insertItem(0, str);

  while (dir.cdUp()) {
    m_RootDirectoryCombo -> insertItem(0, dir.path());
  }
}

void QcepFileBrowserWidget::doPushDirectory(QString newDir)
{
  QcepFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set && newDir != "") {
    m_DirectoryStack.append(set->get_RootDirectory());

    set->set_RootDirectory(newDir);

    m_PrevDirectoryButton->setEnabled(true);
  }
}

void QcepFileBrowserWidget::doPreviousDirectory()
{
  QcepFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set && !m_DirectoryStack.isEmpty()) {
    set->set_RootDirectory(m_DirectoryStack.takeLast());

    m_PrevDirectoryButton->setEnabled(!m_DirectoryStack.isEmpty());
  }
}

void QcepFileBrowserWidget::doChangeDirectory()
{
  QcepFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set) {
    QString newRoot = QFileDialog::getExistingDirectory(this, "New browser directory...", set->get_RootDirectory(), 0);

    if (newRoot != "") {
      doPushDirectory(newRoot);
    }
  }
}

void QcepFileBrowserWidget::doUpDirectory()
{
  QcepFileBrowserSettingsPtr set(m_FileBrowserSettings);

  if (set) {
    QDir dir(set->get_RootDirectory());

    if (dir.cdUp()) {
      doPushDirectory(dir.path());
    }
  }
}

void QcepFileBrowserWidget::doHomeDirectory()
{
  doPushDirectory(QDir::currentPath());
}

void QcepFileBrowserWidget::doAcquisitionDirectory()
{
  QcepExperimentPtr exp(m_Experiment);

  if (exp) {
    doPushDirectory(exp->get_ExperimentDirectory());
  }
}

void QcepFileBrowserWidget::doOpen()
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
    QxrdProcessorPtr proc(m_Processor);

    if (proc) {
      QxrdProcessor *p = proc.data();

      foreach(index, rows) {
        if (!m_Model->isDir(index)) {
          //    printf("Open: %s\n", qPrintable(m_Model->filePath(index)));

#if       QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
            INVOKE_CHECK(
              QMetaObject::invokeMethod(p, [=]() { p->loadData(m_Model->filePath(index)); })
            )
#else
            INVOKE_CHECK(
              QMetaObject::invokeMethod(p, "loadData", Q_ARG(QString, m_Model->filePath(index)))
            )
#endif
        }
      }
    }
  }
}

void QcepFileBrowserWidget::doOpenDark()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
#if     QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, [=]() { p->loadDark(m_Model->filePath(index));})
          )
#else
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, "loadDark", Q_ARG(QString, m_Model->filePath(index)))
          )
#endif
      }
    }
  }
}

void QcepFileBrowserWidget::doOpenMask()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
#if     QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, [=]() { p->loadMask(m_Model->filePath(index));})
          )
#else
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, "loadMask", Q_ARG(QString, m_Model->filePath(index)))
          )
#endif
      }
    }
  }
}

void QcepFileBrowserWidget::doOpenGainMap()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
#if     QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, [=]() { p->loadGainMap(m_Model->filePath(index));})
          )
#else
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, "loadGainMap", Q_ARG(QString, m_Model->filePath(index)))
          )
#endif
      }
    }
  }
}

void QcepFileBrowserWidget::doProcess()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
#if     QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, [=]() { p->processData(m_Model->filePath(index));})
          )
#else
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, "processData", Q_ARG(QString, m_Model->filePath(index)))
          )
#endif
      }
    }
  }
}

void QcepFileBrowserWidget::doIntegrate()
{
  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
  QModelIndexList rows = sel->selectedRows();
  QModelIndex index;

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

    foreach(index, rows) {
      //    printf("Process: %s\n", qPrintable(m_Model->filePath(index)));
      if (!m_Model->isDir(index)) {
#if     QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, [=]() { p->integrateData(m_Model->filePath(index));})
          )
#else
          INVOKE_CHECK(
            QMetaObject::invokeMethod(p, "integrateData", Q_ARG(QString, m_Model->filePath(index)))
          )
#endif
      }
    }
  }
}

void QcepFileBrowserWidget::doSumImages()
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

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, [=]() { p->sumImages(paths);})
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "sumImages", Q_ARG(QStringList, paths))
      )
#endif
  }
}

void QcepFileBrowserWidget::doClearAccumulator()
{
  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, &QxrdProcessor::clearAccumulator)
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "clearAccumulator")
      )
#endif
  }
}

void QcepFileBrowserWidget::doIntegrateAndAccumulate()
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

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, [=]() { p->integrateAndAccumulate(paths);})
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "integrateAndAccumulate", Q_ARG(QStringList, paths))
      )
#endif
  }
}

void QcepFileBrowserWidget::doSaveAccumulator()
{
//  QItemSelectionModel *sel = m_FileBrowser->selectionModel();
//  QModelIndexList rows = sel->selectedRows();
//  QModelIndex index;
//  QString path;

//  if (rows.count() >= 1) {
//    path = m_Model->filePath(rows.first());
//  } else {
//    path = m_Model->rootPath();
//  }

  QxrdExperimentPtr expt(m_Experiment);
  QxrdProcessorPtr proc(m_Processor);

  if (expt && proc) {
    QcepDatasetModelPtr ds(expt->dataset());

    if (ds) {
      QcepDataObjectPtr obj = ds->item(proc->get_AccumulateIntegratedName());
      if (obj) {
        QString selectedFilter = proc->get_AccumulateIntegratedFormat();
        QString path = proc->get_AccumulateIntegratedDirectory() + "/" +
            proc->get_AccumulateIntegratedFileName();

        QString file = QFileDialog::getSaveFileName(this,
                                                    "Save accumulated data in...", path,
                                                    obj->fileFormatFilterString(), &selectedFilter);

        if (file.length() > 0) {
          proc -> saveAccumulator(file, selectedFilter);

          proc -> set_AccumulateIntegratedFormat(selectedFilter);

          QFileInfo finfo(file);

          proc -> set_AccumulateIntegratedDirectory(finfo.dir().absolutePath());
          proc -> set_AccumulateIntegratedFileName(finfo.fileName());

          QString message = tr("Accumulator saved in %1, Format %2").arg(file).arg(selectedFilter);
          expt -> statusMessage(message);
          expt -> printMessage(message);
        }
      }
    }
  }
}

void QcepFileBrowserWidget::doAdd()
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

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, [=]() { p->addImages(paths);})
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "addImages", Q_ARG(QStringList, paths))
      )
#endif
  }
}

void QcepFileBrowserWidget::doSubtract()
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

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, [=]() { p->subtractImages(paths);})
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "subtractImages", Q_ARG(QStringList, paths))
      )
#endif
  }
}

void QcepFileBrowserWidget::doProjectX()
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

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, [=]() { p->projectImages(paths, 1, 0, 0);})
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "projectImages", Q_ARG(int, 1), Q_ARG(int, 0), Q_ARG(int, 0))
      )
#endif
  }
}

void QcepFileBrowserWidget::doProjectY()
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

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, [=]() { p->projectImages(paths, 0, 1, 0);})
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "projectImages", Q_ARG(int, 0), Q_ARG(int, 1), Q_ARG(int, 0))
      )
#endif
  }
}

void QcepFileBrowserWidget::doProjectZ()
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

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, [=]() { p->projectImages(paths, 0, 0, 1);})
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "projectImages", Q_ARG(int, 0), Q_ARG(int, 0), Q_ARG(int, 1))
      )
#endif
  }
}

void QcepFileBrowserWidget::doCorrelate()
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

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdProcessor *p = proc.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, [=]() { p->correlateImages(paths);})
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(p, "correlateImages", Q_ARG(QStringList, paths))
      )
#endif
  }
}

void QcepFileBrowserWidget::doEvaluate(QString filePath)
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
    QxrdExperiment *expp = exp.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
      INVOKE_CHECK(
        QMetaObject::invokeMethod(expp, [=]() { expp->evaluateScriptFile(filePath); })
      )
#else
      INVOKE_CHECK(
        QMetaObject::invokeMethod(expp, "evaluateScriptFile", Q_ARG(QString, filePath))
      )
#endif
  }
}

void QcepFileBrowserWidget::doRefreshBrowser()
{
  m_Model->refresh();
}

void QcepFileBrowserWidget::mousePressed(QModelIndex index)
{
  if (QApplication::mouseButtons() & Qt::RightButton) {
    //    g_Application->printMessage("Right mouse pressed");

    QString filePath = m_Model->filePath(index);

    QMenu *actions = new QMenu(this);
    QAction *title = actions->addAction("File operations...");
    QAction *open = actions->addAction("Open");
    QAction *openDark = actions->addAction("Open as Dark Image");
    QAction *openMask = actions->addAction("Open as Mask");
    QAction *openGainMap = actions->addAction("Open as Gain Map");
    QAction *integrate = actions->addAction("Integrate");
    QAction *clearAccum = actions->addAction("Clear accumulator");
    QAction *integAccum = actions->addAction("Integrate and accumulate");
    QAction *saveAccum = actions->addAction("Save accumulator in...");
    QAction *process = actions->addAction("Process");
    QAction *sumImages = actions->addAction("Sum Images to make new Current Image");
    QAction *add       = actions->addAction("Add Images to existing Current Image");
    QAction *subtract = actions->addAction("Subtract Images from Current Image");
    QAction *projectX = actions->addAction("Project Along X");
    QAction *projectY = actions->addAction("Project Along Y");
    QAction *projectZ = actions->addAction("Project Along Z");
    QAction *correlate = actions->addAction("Correlate Images with Current Image");
    QAction *evaluate = actions->addAction("Evaluate scripts");

    title->setDisabled(true);

    QAction *action = actions->exec(QCursor::pos(), title);

    if (action == open) {
      doOpen();
    } else if (action == openDark) {
      doOpenDark();
    } else if (action == openMask) {
      doOpenMask();
    } else if (action == openGainMap) {
      doOpenGainMap();
    } else if (action == sumImages) {
      doSumImages();
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

void QcepFileBrowserWidget::doubleClicked(QModelIndex /*index*/)
{
  doOpen();
}

void QcepFileBrowserWidget::onRowCountChanged(int oldCount, int newCount)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt && qcepDebug(DEBUG_DISPLAY)) {
    expt->printMessage(
          tr("QcepFileBrowserWidget::onRowCountChanged(%1,%2)").arg(oldCount).arg(newCount));
  }

//  m_FileBrowser->resizeColumnsToContents();
}

void QcepFileBrowserWidget::onModelReset()
{
//  m_FileBrowser->resizeColumnsToContents();
//  m_FileBrowser->resizeRowsToContents();
}

void QcepFileBrowserWidget::onFileUpdated(QFileInfo file)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt && qcepDebug(DEBUG_DISPLAY)) {
    expt->printMessage(
          tr("QcepFileBrowserWidget::onFileUpdated(\"%1\",\"%2\")").arg(file.filePath()).arg(file.lastModified().toString()));
  }
}
