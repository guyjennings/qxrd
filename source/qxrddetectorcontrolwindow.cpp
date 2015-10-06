#include "qxrddetectorcontrolwindow.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacquisition.h"
#include "qxrddetectorprocessor.h"
#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicalculator-ptr.h"
#include "qxrdroicalculator.h"
#include <QMessageBox>
#include "qxrdroicoordinates.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"
#include "qxrddetector.h"
#include "qxrdroitypedelegate.h"

QxrdDetectorControlWindow::QxrdDetectorControlWindow(QcepSettingsSaverWPtr     saver,
                                                     QxrdExperimentWPtr        exp,
                                                     QxrdAcquisitionWPtr       acq,
                                                     QxrdDetectorWPtr          det,
                                                     QxrdDetectorProcessorWPtr proc,
                                                     QWidget                  *parent) :
  QxrdMainWindow(parent),
  m_Saver(saver),
  m_Experiment(exp),
  m_Acquisition(acq),
  m_Detector(det),
  m_Processor(proc)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose, false);

  QxrdDetectorProcessorPtr dp(m_Processor);
  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);
  QxrdExperimentPtr expt(m_Experiment);
  QxrdDetectorPtr dt(m_Detector);

  if (dp) {
    dp->prop_DetectorDisplayMode()     -> linkTo(m_DetectorDisplayMode);
    dp->prop_PerformDarkSubtraction()  -> linkTo(m_PerformDarkSubtraction);
    dp->prop_SaveRawImages()           -> linkTo(m_SaveRawData);
    dp->prop_SaveDarkImages()          -> linkTo(m_SaveDarkData);
    dp->prop_PerformBadPixels()        -> linkTo(m_BadPixelCorrection);
    dp->prop_PerformGainCorrection()   -> linkTo(m_GainCorrection);
    dp->prop_SaveSubtracted()          -> linkTo(m_SaveSubtractedData);

    dp->prop_DarkImagePath()           -> linkTo(m_DarkImagePath);
    dp->prop_RawDataSubdir()           -> linkTo(m_RawDataSubdir);
    dp->prop_DarkDataSubdir()          -> linkTo(m_DarkDataSubdir);
    dp->prop_BadPixelsPath()           -> linkTo(m_BadPixelPath);
    dp->prop_GainMapPath()             -> linkTo(m_GainCorrectionPath);
    dp->prop_SubtractedSubdir()        -> linkTo(m_SubtractedDataSubdir);

    dp->prop_MaskPath()                -> linkTo(m_MaskImagePath);

    dp->prop_PerformIntegration()      -> linkTo(m_PerformIntegration);
    dp->prop_DisplayIntegratedData()   -> linkTo(m_DisplayIntegratedData);
    dp->prop_SaveIntegratedData()      -> linkTo(m_SaveIntegratedData);
    dp->prop_IntegratedDataFile()      -> linkTo(m_IntegratedDataFile);
    dp->prop_SaveIntegratedDataSeparate() -> linkTo(m_SaveIntegratedDataSeparate);
    dp->prop_IntegratedDataSubdir()    -> linkTo(m_IntegratedDataSubdir);

    dp->prop_AccumulateIntegrated2D()  -> linkTo(m_AccumulateIntegratedData);
    dp->prop_AccumulateIntegratedName()-> linkTo(m_AccumulateIntegratedName);

    dp->prop_CalculateROICounts()      -> linkTo(m_CalculateROICounts);
    dp->prop_DisplayROIBorders()       -> linkTo(m_DisplayROIBorders);

    connect(dp->prop_DisplayROIBorders(), &QcepBoolProperty::valueChanged, this, &QxrdDetectorControlWindow::updateROIDisplay);

    updateROIDisplay(dp->get_DisplayROIBorders());

    QxrdROICalculatorPtr calc(dp->roiCalculator());

    if (calc) {
      m_ROIModel = calc->roiModel();

      m_ROIWidget->setItemDelegateForColumn(QxrdROICoordinatesListModel::TypeCol, new QxrdROITypeDelegate());
      m_ROIWidget->setModel(m_ROIModel.data());

      connect(m_NewROI,      &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doAppendROI);
      connect(m_DeleteROI,   &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doDeleteROI);
      connect(m_MoveROIDown, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doMoveROIDown);
      connect(m_MoveROIUp,   &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doMoveROIUp);

      m_ROIWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    m_DetectorImage->init(dp->imagePlotSettings());

    m_DetectorImage->setROIModel(m_ROIModel);
    m_DetectorImage->setROISelection(m_ROIWidget->selectionModel());
  }

  if (app) {
    connect(app->prop_UpdateIntervalMsec(), &QcepIntProperty::valueChanged,
            this, &QxrdDetectorControlWindow::onUpdateIntervalMsecChanged);

    connect(&m_UpdateTimer, &QTimer::timeout,
            this, &QxrdDetectorControlWindow::updateImageDisplay);

    m_UpdateTimer.start(app->get_UpdateIntervalMsec());
  }

  if (expt) {
    int fs = expt->get_FontSize();
    int sp = expt->get_Spacing();

    if (fs > 0) {
      setFontSize(fs);
    }

    if (sp >= 0) {
      setSpacing(sp);
    }

    connect(expt->prop_FontSize(), &QcepIntProperty::valueChanged, this, &QxrdDetectorControlWindow::setFontSize);
    connect(expt->prop_Spacing(), &QcepIntProperty::valueChanged, this, &QxrdDetectorControlWindow::setSpacing);
  }

  if (dt) {
    connect(dt->prop_DetectorNumber(),   &QcepIntProperty::valueChanged,    this, &QxrdDetectorControlWindow::updateWindowTitle);
    connect(dt->prop_DetectorTypeName(), &QcepStringProperty::valueChanged, this, &QxrdDetectorControlWindow::updateWindowTitle);
    connect(dt->prop_DetectorName(),     &QcepStringProperty::valueChanged, this, &QxrdDetectorControlWindow::updateWindowTitle);

    updateWindowTitle();
  }
}

QxrdDetectorControlWindow::~QxrdDetectorControlWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting detector control window\n");
#endif
}

void QxrdDetectorControlWindow::updateWindowTitle()
{
  QxrdDetectorPtr dt(m_Detector);

  if (dt) {
    setWindowTitle(tr("%1: Control %2 detector \"%3\"")
                   .arg(dt->get_DetectorNumber())
                   .arg(dt->get_DetectorTypeName())
                   .arg(dt->get_DetectorName()));
  }
}

void QxrdDetectorControlWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdDetectorControlWindow::printMessage(QString msg, QDateTime ts)
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq->printMessage(msg, ts);
  }
}

QVector<int> QxrdDetectorControlWindow::selectedROIs()
{
  QVector<int> res;

  if (m_ROIModel) {
    int roiCount = m_ROIModel->rowCount(QModelIndex());

    QItemSelectionModel *selected = m_ROIWidget->selectionModel();

    for (int i=0; i<roiCount; i++) {
      if (selected->rowIntersectsSelection(i, QModelIndex())) {
        res.append(i);
      }
    }
  }

  return res;
}

void QxrdDetectorControlWindow::doAppendROI()
{
  QVector<int> rois = selectedROIs();

  if (m_ROIModel) {
    QMenu menu;

    for (int i=0; i<QxrdROICoordinates::roiTypeCount(); i++) {
      QAction *a = new QAction(QxrdROICoordinates::roiTypeName(i), &menu);
      a->setData(i);

      menu.addAction(a);
    }

    QAction *choice = menu.exec(QCursor::pos());

    if (choice) {
      int roiType = choice->data().toInt();

      QxrdROICoordinatesPtr roi =
          QxrdROICoordinatesPtr(new QxrdROICoordinates(m_Saver, m_Experiment, roiType));

      m_ROIModel->append(roi);
    }
  }
}

void QxrdDetectorControlWindow::doDeleteROI()
{
  QVector<int> rois = selectedROIs();

  int res = QMessageBox::Cancel;

  if (rois.count() == 0) {
    QMessageBox::information(this, "No ROIs Selected", "Select ROIs to delete", QMessageBox::Ok);
  } else if (rois.count() == 1) {
    res = QMessageBox::information(this, "Delete ROI?",
                                   tr("Do you really want to delete ROI %1").arg(rois.value(0)),
                                   QMessageBox::Ok, QMessageBox::Cancel);
  } else {
    QString roimsg;
    foreach (int i, rois) {
      if (roimsg=="") {
        roimsg = tr("%1").arg(i);
      } else {
        roimsg += tr(", %1").arg(i);
      }
    }

    res = QMessageBox::information(this, "Delete ROIs?",
                                   tr("Do you really want to delete ROIs %1").arg(roimsg));
  }

  if (res == QMessageBox::Ok && m_ROIModel) {
    for (int i=rois.count()-1; i>=0; i--) {
      m_ROIModel->removeROI(rois.value(i));
    }
  }
}

void QxrdDetectorControlWindow::doMoveROIDown()
{
  QVector<int> rois = selectedROIs();

  if (rois.count() != 1) {
    QMessageBox::information(this, "Only Move One", "Must have a single ROI selected before moving it", QMessageBox::Ok);
  } else {
    if (m_ROIModel) {
      m_ROIModel->moveROIDown(rois.first());
    }
  }
}

void QxrdDetectorControlWindow::doMoveROIUp()
{
  QVector<int> rois = selectedROIs();

  if (rois.count() != 1) {
    QMessageBox::information(this, "Only Move One", "Must have a single ROI selected before moving it", QMessageBox::Ok);
  } else {
    if (m_ROIModel) {
      m_ROIModel->moveROIUp(rois.first());
    }
  }
}

void QxrdDetectorControlWindow::updateImageDisplay()
{
  if (m_NewDataAvailable.fetchAndStoreOrdered(0)) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_UpdateMutex);

    m_DisplayedImage = m_NewImage;
    m_NewImage       = QcepImageDataBasePtr();

    m_DisplayedOverflow = m_NewOverflow;
    m_DisplayedOverflow = QcepMaskDataPtr();

    QxrdDetectorProcessorPtr proc(m_Processor);

    if (proc) {
      if (proc->get_DetectorDisplayMode() == QxrdDetectorProcessor::ImageDisplayMode) {
        m_DetectorImage->onProcessedImageAvailable(m_DisplayedImage, m_DisplayedOverflow);
      }
    }
  } else if (m_NewMaskAvailable.fetchAndStoreOrdered(0)) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_UpdateMutex);

    m_DisplayedMask = m_NewMask;
    m_NewMask       = QcepMaskDataPtr();

    QxrdDetectorProcessorPtr proc(m_Processor);

    if (proc) {
      if (proc->get_DetectorDisplayMode() == QxrdDetectorProcessor::ImageDisplayMode) {
        m_DetectorImage->onMaskedImageAvailable(m_DisplayedImage, m_DisplayedMask);
      }
    }
  }
}

void QxrdDetectorControlWindow::onUpdateIntervalMsecChanged(int newVal)
{
  m_UpdateTimer.setInterval(newVal);
}

void QxrdDetectorControlWindow::displayNewData(QcepImageDataBasePtr img, QcepMaskDataPtr overflow)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_UpdateMutex);

  m_NewImage    = img;
  m_NewOverflow = overflow;

  m_NewDataAvailable.fetchAndStoreOrdered(1);
}

void QxrdDetectorControlWindow::displayNewMask(QcepMaskDataPtr mask)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_UpdateMutex);

  m_NewMask = mask;

  m_NewMaskAvailable.fetchAndStoreOrdered(1);
}

void QxrdDetectorControlWindow::updateROIDisplay(bool show)
{
  m_DetectorImage -> enableROIDisplay(show);
}
