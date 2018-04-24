#include "qxrddebug.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdacqcommon.h"
#include "qxrdprocessor.h"
#include "qxrdapplication.h"
#include "qcepallocator.h"
#include "qxrdexperiment.h"
#include "qcepspecserversettings.h"
#include "qcepsimpleserversettings.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include "qcepdebug.h"
#include "qxrddetectorsettings.h"
#include "qxrddetectorlistmodel.h"

#include <QMenu>

//TODO: move detector options to separate dialog
//TODO: move server options to separate dialog
//TODO: remove detector related stuff when called from viewer...
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

    QxrdAcqCommonPtr acq(expt->acquisition());

    QxrdProcessorPtr proc = expt->processor();
    //  QcepAllocator *alloc = g_Application->allocator();

    int runSpecServer = 0;
    int specServerPort = 0;

    QcepSpecServerSettingsPtr srv(expt -> specServerSettings());

    if (srv) {
      runSpecServer = srv -> get_RunSpecServer();
      specServerPort = srv -> get_SpecServerPort();
    }

    int runSimpleServer = 0;
    int simpleServerPort = 0;

    QcepSimpleServerSettingsPtr ssrv(expt -> simpleServerSettings());

    if (ssrv) {
      runSimpleServer = ssrv -> get_RunSimpleServer();
      simpleServerPort = ssrv -> get_SimpleServerPort();
    }

    m_DetectorsModel =
        QxrdDetectorListModelPtr(new QxrdDetectorListModel());


    if (acq) {
      for (int i=0; i<acq->detectorCount(); i++) {
        QxrdDetectorSettingsPtr det = acq->detector(i);

        m_DetectorsModel -> append(det);
      }
    }

    m_DetectorsList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_DetectorsList->setModel(m_DetectorsModel.data());

    CONNECT_CHECK(connect(m_AddDetector,      &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::addDetector));
    CONNECT_CHECK(connect(m_RemoveDetector,   &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::removeDetector));
    CONNECT_CHECK(connect(m_MoveDetectorDown, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::moveDetectorDown));
    CONNECT_CHECK(connect(m_MoveDetectorUp,   &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::moveDetectorUp));
    CONNECT_CHECK(connect(m_ConfigureDetector,&QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::configureDetector));
    CONNECT_CHECK(connect(m_DetectorsList,    &QTreeView::doubleClicked, this, &QxrdExperimentPreferencesDialog::detectorDoubleClicked));

    CONNECT_CHECK(connect(m_CurrentLogFileBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::currentLogFileBrowse));
    CONNECT_CHECK(connect(m_DataDirectoryBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::dataDirectoryBrowse));
    CONNECT_CHECK(connect(m_IntegratedScansFileBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::integratedScansFileBrowse));

    expt -> prop_ExperimentDirectory() -> copyTo(m_ExperimentDirectory);
    expt -> prop_DataDirectory()       -> copyTo(m_DataDirectory);
    expt -> prop_LogFileName()         -> copyTo(m_CurrentLogFile);
    expt -> prop_LogMaxLengthMB()      -> copyTo(m_LogFileMaxMB);
    expt -> prop_LogMaxSaved()         -> copyTo(m_LogFileBackups);

    expt -> prop_ScanFileName()        -> copyTo(m_IntegratedScansFile);
    expt -> prop_ScanFileExtension()   -> copyTo(m_ScanFileExtension);
    expt -> prop_ScanDataNegative()    -> copyTo(m_ScanDataNegative);
    expt -> prop_ScanMaxLengthMB()     -> copyTo(m_ScanFileMaxMB);
    expt -> prop_ScanMaxSaved()        -> copyTo(m_ScanFileBackups);

    expt -> prop_CompressOutputBZ2()   -> copyTo(m_CompressOutputBZ2);
    expt -> prop_CompressLevelBZ2()    -> copyTo(m_CompressLevelBZ2);

    if (proc) {
      CONNECT_CHECK(connect(m_SaveRawBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::saveRawBrowse));
      CONNECT_CHECK(connect(m_SaveDarkBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::saveDarkBrowse));
      CONNECT_CHECK(connect(m_SaveSubtractedBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::saveSubtractedBrowse));
      CONNECT_CHECK(connect(m_SaveIntegratedBrowse, &QAbstractButton::clicked, this, &QxrdExperimentPreferencesDialog::saveIntegratedBrowse));

      proc -> prop_SaveRawInSubdirectory()         -> copyTo(m_SaveRawInSubdir);
      proc -> prop_SaveRawSubdirectory()           -> copyTo(m_SaveRawSubdir);

      proc -> prop_SaveDarkInSubdirectory()        -> copyTo(m_SaveDarkInSubdir);
      proc -> prop_SaveDarkSubdirectory()          -> copyTo(m_SaveDarkSubdir);

      proc -> prop_SaveSubtractedInSubdirectory()  -> copyTo(m_SaveSubtractedInSubdir);
      proc -> prop_SaveSubtractedSubdirectory()    -> copyTo(m_SaveSubtractedSubdir);

      proc -> prop_SaveIntegratedData()            -> copyTo(m_SaveIntegratedInLogFile);
      proc -> prop_SaveIntegratedInSeparateFiles() -> copyTo(m_SaveIntegratedInSeparateFiles);
      proc -> prop_SaveIntegratedInSubdirectory()  -> copyTo(m_SaveIntegratedInSubdir);
      proc -> prop_SaveIntegratedSubdirectory()    -> copyTo(m_SaveIntegratedSubdir);

      proc -> prop_AccumulateIntegrated2D()        -> copyTo(m_AccumulateIntegrated2D);
      proc -> prop_AccumulateIntegratedName()      -> copyTo(m_AccumulateIntegratedName);

      proc -> prop_SaveOverflowFiles()             -> copyTo(m_SaveOverflowFiles);
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
      initializeFormatMenu(m_FileNameFormat1);
      initializeFormatMenu(m_FileNameFormat2);
      initializeFormatMenu(m_FileNameFormat3);
      initializeFormatMenu(m_FileNameFormat4);

      acq -> prop_FileIndexWidth()      -> copyTo(m_FileIndexWidth);
      acq -> prop_FilePhaseWidth()      -> copyTo(m_FilePhaseWidth);
      acq -> prop_FileOverflowWidth()   -> copyTo(m_FileOverflowWidth);
      acq -> prop_DetectorNumberWidth() -> copyTo(m_DetectorNumberWidth);
      acq -> prop_FileNumberWidth()     -> copyTo(m_FileNumberWidth);
      acq -> prop_FileNameFormat1()     -> copyTo(m_FileNameFormat1);
      acq -> prop_FileNameFormat2()     -> copyTo(m_FileNameFormat2);
      acq -> prop_FileNameFormat3()     -> copyTo(m_FileNameFormat3);
      acq -> prop_FileNameFormat4()     -> copyTo(m_FileNameFormat4);
    }
  }
}

QxrdExperimentPreferencesDialog::~QxrdExperimentPreferencesDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentPreferencesDialog::~QxrdExperimentPreferencesDialog(%p)\n", this);
  }
}

void QxrdExperimentPreferencesDialog::initializeFormatMenu(QComboBox *cb)
{
  cb -> clear();
  cb -> addItem("(empty)");
  cb -> addItem("_i<index>");
  cb -> addItem("_d<det>");
  cb -> addItem("_p<phase>");
  cb -> addItem("_n<num>");
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
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_DataDirectory->text());

  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory", initial.absoluteFilePath(), QFileDialog::ShowDirsOnly);

  if (dir != "") {
    m_DataDirectory->setText(pwd.relativeFilePath(dir));
  }
}

void QxrdExperimentPreferencesDialog::currentLogFileBrowse()
{
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_CurrentLogFile->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial.absoluteFilePath());

  if (file != "") {
    m_CurrentLogFile->setText(pwd.relativeFilePath(file));
  }
}

void QxrdExperimentPreferencesDialog::integratedScansFileBrowse()
{
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_IntegratedScansFile->text());

  QString file = QFileDialog::getSaveFileName(this, "Scans File", initial.absoluteFilePath());

  if (file != "") {
    m_IntegratedScansFile->setText(pwd.relativeFilePath(file));
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
//  int mask = 0;

//  mask |= 1 << m_FileNameFormat1 -> currentIndex();
//  mask |= 1 << m_FileNameFormat2 -> currentIndex();
//  mask |= 1 << m_FileNameFormat3 -> currentIndex();
//  mask |= 1 << m_FileNameFormat4 -> currentIndex();

//  if (mask != 15) {
//    QMessageBox::warning(this,
//                         "File Format",
//                         "File Format invalid");
//    return;
//  }

  int runSpecServer = m_RunSpecServer -> isChecked();
  int runSimpleServer = m_RunSimpleServer -> isChecked();
  int specServerPort = m_SpecServerPort -> value();
  int simpleServerPort = m_SimpleServerPort -> value();

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdAcqCommonPtr acq(expt->acquisition());
    QxrdProcessorPtr proc = expt->processor();
    QcepSpecServerSettingsPtr srv(expt -> specServerSettings());
    QcepSimpleServerSettingsPtr ssrv(expt -> simpleServerSettings());

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
      proc -> prop_SaveRawInSubdirectory()         -> copyFrom(m_SaveRawInSubdir);
      proc -> prop_SaveRawSubdirectory()           -> copyFrom(m_SaveRawSubdir);

      proc -> prop_SaveDarkInSubdirectory()        -> copyFrom(m_SaveDarkInSubdir);
      proc -> prop_SaveDarkSubdirectory()          -> copyFrom(m_SaveDarkSubdir);

      proc -> prop_SaveSubtractedInSubdirectory()  -> copyFrom(m_SaveSubtractedInSubdir);
      proc -> prop_SaveSubtractedSubdirectory()    -> copyFrom(m_SaveSubtractedSubdir);

      proc -> prop_SaveIntegratedData()            -> copyFrom(m_SaveIntegratedInLogFile);
      proc -> prop_SaveIntegratedInSeparateFiles() -> copyFrom(m_SaveIntegratedInSeparateFiles);
      proc -> prop_SaveIntegratedInSubdirectory()  -> copyFrom(m_SaveIntegratedInSubdir);
      proc -> prop_SaveIntegratedSubdirectory()    -> copyFrom(m_SaveIntegratedSubdir);

      proc -> prop_AccumulateIntegrated2D()        -> copyFrom(m_AccumulateIntegrated2D);
      proc -> prop_AccumulateIntegratedName()      -> copyFrom(m_AccumulateIntegratedName);

      proc -> prop_SaveOverflowFiles()             -> copyFrom(m_SaveOverflowFiles);
    }

    expt -> prop_DataDirectory()     -> copyFrom(m_DataDirectory);
    expt -> prop_LogFileName()       -> copyFrom(m_CurrentLogFile);
    expt -> prop_LogMaxLengthMB()    -> copyFrom(m_LogFileMaxMB);
    expt -> prop_LogMaxSaved()       -> copyFrom(m_LogFileBackups);

    expt -> prop_ScanFileName()      -> copyFrom(m_IntegratedScansFile);
    expt -> prop_ScanFileExtension() -> copyFrom(m_ScanFileExtension);
    expt -> prop_ScanDataNegative()  -> copyFrom(m_ScanDataNegative);
    expt -> prop_ScanMaxLengthMB()   -> copyFrom(m_ScanFileMaxMB);
    expt -> prop_ScanMaxSaved()      -> copyFrom(m_ScanFileBackups);

    expt -> prop_CompressOutputBZ2() -> copyFrom(m_CompressOutputBZ2);
    expt -> prop_CompressLevelBZ2()  -> copyFrom(m_CompressLevelBZ2);

    if (acq) {
      acq -> prop_FileIndexWidth()      -> copyFrom(m_FileIndexWidth);
      acq -> prop_FilePhaseWidth()      -> copyFrom(m_FilePhaseWidth);
      acq -> prop_FileOverflowWidth()   -> copyFrom(m_FileOverflowWidth);
      acq -> prop_DetectorNumberWidth() -> copyFrom(m_DetectorNumberWidth);
      acq -> prop_FileNumberWidth()     -> copyFrom(m_FileNumberWidth);
      acq -> prop_FileNameFormat1()     -> copyFrom(m_FileNameFormat1);
      acq -> prop_FileNameFormat2()     -> copyFrom(m_FileNameFormat2);
      acq -> prop_FileNameFormat3()     -> copyFrom(m_FileNameFormat3);
      acq -> prop_FileNameFormat4()     -> copyFrom(m_FileNameFormat4);

      int nDets = m_DetectorsModel->rowCount(QModelIndex());

      acq->clearDetectors();

      for (int i = 0; i<nDets; i++) {
        acq->appendDetector(m_DetectorsModel->detector(i));
      }
    }
  }

  QDialog::accept();
}

void QxrdExperimentPreferencesDialog::addDetector()
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdAcqCommonPtr acq(expt->acquisition());

    if (acq) {
      QMenu menu;

      for (int i=0; i<QxrdDetectorSettings::detectorTypeCount(); i++) {
        menu.addAction(QxrdDetectorSettings::detectorTypeName(i),
                       [=]() { addDetectorOfType(i);});
      }

      menu.exec(QCursor::pos());
    }
  }
}

void QxrdExperimentPreferencesDialog::addDetectorOfType(int t)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdAcqCommonPtr acq(expt->acquisition());

    if (acq) {
      QxrdDetectorSettingsPtr det(acq->newDetector(t));

      m_DetectorsModel->append(det);
    }
  }
}

void QxrdExperimentPreferencesDialog::removeDetector()
{
  int detCount = m_DetectorsModel->rowCount(QModelIndex());

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
      m_DetectorsModel->removeDetector(selectedRows.value(i));
    }
  }
}

void QxrdExperimentPreferencesDialog::moveDetectorDown()
{
  int detCount = m_DetectorsModel->rowCount(QModelIndex());

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
    m_DetectorsModel->moveDetectorDown(selectedRows.first());
  }
}

void QxrdExperimentPreferencesDialog::moveDetectorUp()
{
  int detCount = m_DetectorsModel->rowCount(QModelIndex());

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
    m_DetectorsModel->moveDetectorUp(selectedRows.first());
  }
}

void QxrdExperimentPreferencesDialog::configureDetector()
{
  int detCount = m_DetectorsModel->rowCount(QModelIndex());

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
    QxrdDetectorSettingsPtr proxy = m_DetectorsModel->detector(selectedRows.first());

    if (proxy) {
      proxy->configureDetector();
    }
  }
}

//void QxrdExperimentPreferencesDialog::appendDetector(QxrdDetectorSettingsPtr det)
//{
//  if (det) {
//    m_DetectorsModel->append(det);
//  }
//}

void QxrdExperimentPreferencesDialog::detectorDoubleClicked(const QModelIndex& item)
{
//  int row = item.row();
  int col = item.column();

  if (col == 0 || col == 2) {
    QxrdDetectorSettingsPtr proxy = m_DetectorsModel->detector(item.row());

    if (proxy) {
      proxy->configureDetector();
    }
  }
}
