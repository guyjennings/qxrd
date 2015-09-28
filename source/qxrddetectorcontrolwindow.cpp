#include "qxrddetectorcontrolwindow.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacquisition.h"
#include "qxrddetectorprocessor.h"
#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicalculator-ptr.h"
#include "qxrdroicalculator.h"
#include <QMessageBox>
#include "qxrdroicoordinates.h"

QxrdDetectorControlWindow::QxrdDetectorControlWindow(QcepSettingsSaverWPtr saver,
                                                     QxrdExperimentWPtr        exp,
                                                     QxrdAcquisitionWPtr       acq,
                                                     QxrdDetectorProcessorWPtr proc,
                                                     QWidget                  *parent) :
  QxrdMainWindow(parent),
  m_Saver(saver),
  m_Experiment(exp),
  m_Acquisition(acq),
  m_Processor(proc)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose, false);

  QxrdDetectorProcessorPtr dp(m_Processor);

  if (dp) {
    dp->prop_DetectorDisplayMode()     -> linkTo(m_DetectorDisplayMode);
    dp->prop_PerformDarkSubtraction()  -> linkTo(m_PerformDarkSubtraction);
    dp->prop_SaveRawImages()           -> linkTo(m_SaveRawData);
    dp->prop_SaveDarkImages()          -> linkTo(m_SaveDarkData);
    dp->prop_PerformBadPixels()        -> linkTo(m_BadPixelCorrection);
    dp->prop_PerformGainCorrection()   -> linkTo(m_GainCorrection);
    dp->prop_SaveSubtracted()          -> linkTo(m_SaveSubtractedData);
    dp->prop_PerformIntegration()      -> linkTo(m_PerformIntegration);
    dp->prop_DisplayIntegratedData()   -> linkTo(m_DisplayIntegratedData);
    dp->prop_SaveIntegratedData()      -> linkTo(m_SaveIntegratedData);
    dp->prop_AccumulateIntegrated2D()  -> linkTo(m_AccumulateIntegratedData);
    dp->prop_CalculateROICounts()      -> linkTo(m_CalculateROICounts);
    dp->prop_DisplayROIBorders()       -> linkTo(m_DisplayROIBorders);

    QxrdROICalculatorPtr calc(dp->roiCalculator());

    if (calc) {
      m_ROIModel = QxrdROICoordinatesListModelPtr(
            new QxrdROICoordinatesListModel(calc->coordinates()));

      m_ROIWidget->setModel(m_ROIModel.data());

      connect(m_NewROI,      &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doAppendROI);
      connect(m_DeleteROI,   &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doDeleteROI);
      connect(m_MoveROIDown, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doMoveROIDown);
      connect(m_MoveROIUp,   &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doMoveROIUp);
    }
  }
}

QxrdDetectorControlWindow::~QxrdDetectorControlWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting detector control window\n");
#endif
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
      m_ROIModel->removeROI(i);
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
