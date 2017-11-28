#include "qxrddebug.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qcepallocator.h"
#include "qxrdexperiment.h"
#include "qxrdserver.h"
#include "qxrdsimpleserver.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include "qcepdebug.h"
#include "qxrddetectorproxy.h"
#include "qxrddetectorproxywidget.h"
#include "qxrddetectorproxylistmodel.h"

#include <QMenu>

QxrdExperimentPreferencesDialog::QxrdExperimentPreferencesDialog(QxrdExperimentWPtr exptw, QWidget *parent, int initialPage) :
  QDialog(parent),
  m_Experiment(exptw)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentPreferencesDialog::QxrdExperimentPreferencesDialog(%p)\n", this);
  }

  setupUi(this);

  if (initialPage >= 0) {
    m_PreferencesPanes->setCurrentIndex(initialPage);
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    setWindowTitle(expt->experimentFilePath() + " Preferences");

    QxrdAcquisitionPtr acq = expt -> acquisition();
    QxrdDataProcessorPtr proc = expt->dataProcessor();
    //  QcepAllocator *alloc = g_Application->allocator();

    int runSpecServer = 0;
    int specServerPort = 0;

    QxrdServerPtr srv(expt -> specServer());

    if (srv) {
      runSpecServer = srv -> get_RunSpecServer();
      specServerPort = srv -> get_SpecServerPort();
    }

    int runSimpleServer = 0;
    int simpleServerPort = 0;

    QxrdSimpleServerPtr ssrv(expt -> simpleServer());

    if (ssrv) {
      runSimpleServer = ssrv -> get_RunSimpleServer();
      simpleServerPort = ssrv -> get_SimpleServerPort();
    }

    m_DetectorProxyModel =
        QxrdDetectorProxyListModelPtr(new QxrdDetectorProxyListModel());

    if (acq) {
      for (int i=0; i<acq->get_DetectorCount(); i++) {
        QxrdDetectorProxyPtr proxy =
            QxrdDetectorProxyPtr(new QxrdDetectorProxy(
                                   acq->detector(i), acq));

        appendDetectorProxy(proxy);
      }
    }

    m_DetectorsList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_DetectorsList->setModel(m_DetectorProxyModel.data());

    connect(m_AddDetector,      &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::addDetector);
    connect(m_RemoveDetector,   &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::removeDetector);
    connect(m_MoveDetectorDown, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::moveDetectorDown);
    connect(m_MoveDetectorUp,   &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::moveDetectorUp);
    connect(m_ConfigureDetector,&QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::configureDetector);
    connect(m_DetectorsList,    &QTreeView::doubleClicked, this, &QxrdExperimentPreferencesDialog::detectorDoubleClicked);

    m_ExperimentDirectory -> setText(expt->get_ExperimentDirectory());

    connect(m_CurrentLogFileBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::currentLogFileBrowse);
    m_CurrentLogFile -> setText(expt->get_LogFileName());

    connect(m_DataDirectoryBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::dataDirectoryBrowse);
    m_DataDirectory -> setText(expt->get_DataDirectory());

    connect(m_IntegratedScansFileBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::integratedScansFileBrowse);
    m_IntegratedScansFile -> setText(expt->get_ScanFileName());

    m_ScanFileExtension -> setText(expt->get_ScanFileExtension());

    m_ScanDataNegative -> setCurrentIndex(expt->get_ScanDataNegative());

    if (proc) {
      connect(m_SaveRawBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::saveRawBrowse);
      m_SaveRawInSubdir  -> setChecked(proc->get_SaveRawInSubdirectory());
      m_SaveRawSubdir    -> setText  (proc->get_SaveRawSubdirectory());

      connect(m_SaveDarkBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::saveDarkBrowse);
      m_SaveDarkInSubdir  -> setChecked(proc->get_SaveDarkInSubdirectory());
      m_SaveDarkSubdir    -> setText  (proc->get_SaveDarkSubdirectory());

      connect(m_SaveSubtractedBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::saveSubtractedBrowse);
      m_SaveSubtractedInSubdir  -> setChecked(proc->get_SaveSubtractedInSubdirectory());
      m_SaveSubtractedSubdir    -> setText  (proc->get_SaveSubtractedSubdirectory());

      connect(m_SaveIntegratedBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::saveIntegratedBrowse);
      m_SaveIntegratedInLogFile  -> setChecked(proc->get_SaveIntegratedData());
      m_SaveIntegratedInSeparateFiles  -> setChecked(proc->get_SaveIntegratedInSeparateFiles());
      m_SaveIntegratedInSubdir  -> setChecked(proc->get_SaveIntegratedInSubdirectory());
      m_SaveIntegratedSubdir    -> setText  (proc->get_SaveIntegratedSubdirectory());

      m_AccumulateIntegrated2D -> setChecked(proc->get_AccumulateIntegrated2D());
      m_AccumulateIntegratedName ->setText(proc->get_AccumulateIntegratedName());

      m_SaveOverflowFiles -> setChecked(proc->get_SaveOverflowFiles());
    }

    if (srv) {
      m_RunSpecServer -> setChecked(runSpecServer);
      m_SpecServerPort -> setRange(-1,65535);
      m_SpecServerPort -> setSpecialValueText(tr("Automatic"));
      m_SpecServerPort -> setValue(specServerPort);
    }

    if (ssrv) {
      m_RunSimpleServer -> setChecked(runSimpleServer);
      m_SimpleServerPort -> setRange(0,65535);
      m_SimpleServerPort -> setValue(simpleServerPort);
    }

    if (acq) {
      m_FileIndexWidth -> setValue(acq->get_FileIndexWidth());
      m_FilePhaseWidth -> setValue(acq->get_FilePhaseWidth());
      m_FileOverflowWidth -> setValue(acq->get_FileOverflowWidth());
      m_DetectorNumberWidth -> setValue(acq->get_DetectorNumberWidth());
    }

    m_FontSize -> setValue(expt->get_FontSize());
    m_Spacing  -> setValue(expt->get_Spacing());
  }
}

QxrdExperimentPreferencesDialog::~QxrdExperimentPreferencesDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentPreferencesDialog::~QxrdExperimentPreferencesDialog(%p)\n", this);
  }
}

QString QxrdExperimentPreferencesDialog::experimentDirectory()
{
  return m_ExperimentDirectory->text();
}

QString QxrdExperimentPreferencesDialog::dataDirectory()
{
  return QDir(experimentDirectory()).filePath(m_DataDirectory->text());
}

void QxrdExperimentPreferencesDialog::getRelativeDirectoryPath(QLineEdit *edit)
{
  QString initial = QDir(dataDirectory()).filePath(edit->text());

  QString dir = QFileDialog::getExistingDirectory(this, "", initial, QFileDialog::ShowDirsOnly);

  if (dir != "") {
    QString result = QDir(dataDirectory()).relativeFilePath(dir);

    edit -> setText(result);

//    printf("Initial: %s\nDir: %s\nResult: %s\n", qPrintable(initial), qPrintable(dir), qPrintable(result));
  }
}

void QxrdExperimentPreferencesDialog::saveRawBrowse()
{
  getRelativeDirectoryPath(m_SaveRawSubdir);
}

void QxrdExperimentPreferencesDialog::saveDarkBrowse()
{
  getRelativeDirectoryPath(m_SaveDarkSubdir);
}

void QxrdExperimentPreferencesDialog::saveSubtractedBrowse()
{
  getRelativeDirectoryPath(m_SaveSubtractedSubdir);
}

void QxrdExperimentPreferencesDialog::saveIntegratedBrowse()
{
  getRelativeDirectoryPath(m_SaveIntegratedSubdir);
}

void QxrdExperimentPreferencesDialog::dataDirectoryBrowse()
{
  QString initial = QDir(experimentDirectory()).filePath(m_DataDirectory->text());

  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory", initial, QFileDialog::ShowDirsOnly);

  if (dir != "") {
    m_DataDirectory->setText(QDir(experimentDirectory()).relativeFilePath(dir));
  }
}

void QxrdExperimentPreferencesDialog::currentLogFileBrowse()
{
  QString initial = QDir(experimentDirectory()).filePath(m_ExperimentDirectory->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial);

  if (file != "") {
    m_CurrentLogFile->setText(QDir(experimentDirectory()).relativeFilePath(file));
  }
}

void QxrdExperimentPreferencesDialog::integratedScansFileBrowse()
{
  QString initial = QDir(dataDirectory()).filePath(m_IntegratedScansFile->text());

  QString file = QFileDialog::getSaveFileName(this, "Scans File", initial);

  if (file != "") {
    m_IntegratedScansFile->setText(QDir(dataDirectory()).relativeFilePath(file));
  }
}

void QxrdExperimentPreferencesDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdExperimentPreferencesDialog::accept()
{
  int runSpecServer = m_RunSpecServer -> isChecked();
  int runSimpleServer = m_RunSimpleServer -> isChecked();
  int specServerPort = m_SpecServerPort -> value();
  int simpleServerPort = m_SimpleServerPort -> value();

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdAcquisitionPtr acq = expt -> acquisition();
    QxrdDataProcessorPtr proc = expt->dataProcessor();
    QxrdServerPtr srv(expt -> specServer());
    QxrdSimpleServerPtr ssrv(expt -> simpleServer());

//    expt->set_DetectorType(m_DetectorType->currentIndex());
//    expt->set_DetectorSubType(m_DetectorSubType->currentIndex());
//    expt->set_DetectorNumber(m_DetectorNumber->value());
//    expt->set_DetectorAddress(m_DetectorAddress->text());

    if (srv) {
      srv -> set_RunSpecServer(runSpecServer);
      srv -> set_SpecServerPort(specServerPort);
    }

    if (ssrv) {
      ssrv -> set_RunSimpleServer(runSimpleServer);
      ssrv -> set_SimpleServerPort(simpleServerPort);
    }

    if (proc) {
      proc -> set_SaveRawInSubdirectory(m_SaveRawInSubdir -> isChecked());
      proc -> set_SaveRawSubdirectory  (m_SaveRawSubdir   -> text());

      proc -> set_SaveDarkInSubdirectory(m_SaveDarkInSubdir  -> isChecked());
      proc -> set_SaveDarkSubdirectory  (m_SaveDarkSubdir    -> text());

      proc -> set_SaveSubtractedInSubdirectory(m_SaveSubtractedInSubdir -> isChecked());
      proc -> set_SaveSubtractedSubdirectory  (m_SaveSubtractedSubdir   -> text());

      proc -> set_SaveIntegratedData(m_SaveIntegratedInLogFile  -> isChecked());
      proc -> set_SaveIntegratedInSeparateFiles(m_SaveIntegratedInSeparateFiles -> isChecked());
      proc -> set_SaveIntegratedInSubdirectory (m_SaveIntegratedInSubdir  -> isChecked());
      proc -> set_SaveIntegratedSubdirectory   (m_SaveIntegratedSubdir    -> text());

      proc -> set_AccumulateIntegrated2D(m_AccumulateIntegrated2D -> isChecked());
      proc -> set_AccumulateIntegratedName(m_AccumulateIntegratedName -> text());

      proc -> set_SaveOverflowFiles(m_SaveOverflowFiles -> isChecked());
    }

    expt -> set_DataDirectory(m_DataDirectory -> text());
    expt -> set_LogFileName    (m_CurrentLogFile -> text());
    expt -> set_ScanFileName    (m_IntegratedScansFile -> text());
    expt -> set_ScanFileExtension(m_ScanFileExtension->text());
    expt -> set_ScanDataNegative(m_ScanDataNegative->currentIndex());

    if (acq) {
      acq  -> set_FileIndexWidth(m_FileIndexWidth -> value());
      acq  -> set_FilePhaseWidth(m_FilePhaseWidth -> value());
      acq  -> set_FileOverflowWidth(m_FileOverflowWidth -> value());
      acq  -> set_DetectorNumberWidth(m_DetectorNumberWidth -> value());

      int nDets = m_DetectorProxyModel->rowCount(QModelIndex());

      acq->clearDetectors();

      for (int i = 0; i<nDets; i++) {
        acq->appendDetectorProxy(m_DetectorProxyModel->detectorProxy(i));
      }
    }

    expt -> set_FontSize(m_FontSize -> value());
    expt -> set_Spacing(m_Spacing -> value());
  }

  QDialog::accept();
}

void QxrdExperimentPreferencesDialog::addDetector()
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdAcquisitionPtr acq = expt -> acquisition();

    if (acq) {
      QMenu menu;

      for (int i=0; i<QxrdDetectorSettings::detectorTypeCount(); i++) {
        QAction *a = new QAction(QxrdDetectorSettings::detectorTypeName(i), &menu);
        a->setData(i);

        menu.addAction(a);
      }

      QAction *choice = menu.exec(QCursor::pos());

      if (choice) {
        int type = choice->data().toInt();

        QxrdDetectorProxyPtr proxy =
            QxrdDetectorProxyPtr(new QxrdDetectorProxy(type, acq));

        appendDetectorProxy(proxy);
      }
    }
  }
}

void QxrdExperimentPreferencesDialog::removeDetector()
{
  int detCount = m_DetectorProxyModel->rowCount(QModelIndex());

  QItemSelectionModel *selected = m_DetectorsList->selectionModel();
  QVector<int> selectedRows;

  for (int i=0; i<detCount; i++) {
    if (selected->rowIntersectsSelection(i, QModelIndex())) {
      selectedRows.append(i);
    }
  }

  int res = QMessageBox::Cancel;

  if (selectedRows.count() == 0) {
    QMessageBox::information(this, "No Detectors Selected", "Select detectors to delete", QMessageBox::Ok);
  } else if (selectedRows.count() == 1) {
    res = QMessageBox::information(this, "Delete Detector?",
                             tr("Do you really want to delete detector %1").arg(selectedRows.value(0)),
                             QMessageBox::Ok, QMessageBox::Cancel);
  } else {
    QString dets;
    foreach (int i, selectedRows) {
      if (dets=="") {
        dets = tr("%1").arg(i);
      } else {
        dets += tr(", %1").arg(i);
      }
    }

    res = QMessageBox::information(this, "Delete Detectors?,",
                             tr("Do you really want to delete detectors %1").arg(dets),
                             QMessageBox::Ok, QMessageBox::Cancel);
  }

  if (res == QMessageBox::Ok) {
    for (int i=selectedRows.count()-1; i>=0; i--) {
//      printf("Want to remove row %d\n", selectedRows.value(i));
      m_DetectorProxyModel->removeDetector(selectedRows.value(i));
    }
  }
}

void QxrdExperimentPreferencesDialog::moveDetectorDown()
{
  int detCount = m_DetectorProxyModel->rowCount(QModelIndex());

  QItemSelectionModel *selected = m_DetectorsList->selectionModel();
  QVector<int> selectedRows;

  for (int i=0; i<detCount; i++) {
    if (selected->rowIntersectsSelection(i, QModelIndex())) {
      selectedRows.append(i);
    }
  }

  if (selectedRows.count() != 1) {
    QMessageBox::information(this, "Only move one", "Must have a single detector selected before moving it", QMessageBox::Ok);
  } else {
    m_DetectorProxyModel->moveDetectorDown(selectedRows.first());
  }
}

void QxrdExperimentPreferencesDialog::moveDetectorUp()
{
  int detCount = m_DetectorProxyModel->rowCount(QModelIndex());

  QItemSelectionModel *selected = m_DetectorsList->selectionModel();
  QVector<int> selectedRows;

  for (int i=0; i<detCount; i++) {
    if (selected->rowIntersectsSelection(i, QModelIndex())) {
      selectedRows.append(i);
    }
  }

  if (selectedRows.count() != 1) {
    QMessageBox::information(this, "Only move one", "Must have a single detector selected before moving it", QMessageBox::Ok);
  } else {
    m_DetectorProxyModel->moveDetectorUp(selectedRows.first());
  }
}

void QxrdExperimentPreferencesDialog::configureDetector()
{
  int detCount = m_DetectorProxyModel->rowCount(QModelIndex());

  QItemSelectionModel *selected = m_DetectorsList->selectionModel();
  QVector<int> selectedRows;

  for (int i=0; i<detCount; i++) {
    if (selected->rowIntersectsSelection(i, QModelIndex())) {
      selectedRows.append(i);
    }
  }

  if (selectedRows.count() != 1) {
    QMessageBox::information(this, "Only configure one", "Must have a single detector selected before configuring it", QMessageBox::Ok);
  } else {
    QxrdDetectorProxyPtr proxy = m_DetectorProxyModel->detectorProxy(selectedRows.first());

    if (proxy) {
      proxy->configureDetector();
    }
  }
}

void QxrdExperimentPreferencesDialog::appendDetectorProxy(QxrdDetectorProxyPtr proxy)
{
  if (proxy) {
    proxy->initialize();

    m_DetectorProxyModel->append(proxy);
  }
}

void QxrdExperimentPreferencesDialog::detectorDoubleClicked(const QModelIndex& item)
{
//  int row = item.row();
  int col = item.column();

  if (col == 0 || col == 2) {
    QxrdDetectorProxyPtr proxy = m_DetectorProxyModel->detectorProxy(item.row());

    if (proxy) {
      proxy->configureDetector();
    }
  }
}
