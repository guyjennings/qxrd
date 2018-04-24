#include "qxrddetectorcontrolwindow.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacqcommon.h"
#include "qxrdprocessor.h"
#include "qceproimodel.h"
#include "qceproicalculator-ptr.h"
#include "qceproicalculator.h"
#include <QMessageBox>
#include <QFileDialog>
#include "qceproi.h"
#include "qxrdappcommon.h"
#include "qcepmutexlocker.h"
#include "qxrddetectorsettings.h"
#include "qceproitypedelegate.h"
#include "qceproishape.h"
#include "qxrddetectorcontrolwindowsettings.h"
#include "qxrdwindow.h"
#include "qceproieditordialog.h"
#include <QThread>
#include "qxrdexperiment.h"
#include "qcepdataobjectpropertiesmodel.h"
#include "qxrdinfowindow.h"
#include "qxrdacquisitioneventlogwindow.h"

QxrdDetectorControlWindow::QxrdDetectorControlWindow(QString name) :
  inherited("detector"),
  m_Experiment(),
  m_Acquisition(),
  m_Detector(),
  m_Processor()
{
}

void QxrdDetectorControlWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  m_Experiment = QxrdExperiment::findExperiment(m_Parent);
  m_Acquisition = QxrdAcqCommon::findAcquisition(m_Parent);
  m_Detector = QxrdDetectorSettings::findDetectorSettings(m_Parent);
  m_Processor = QxrdProcessor::findProcessor(m_Parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  setAttribute(Qt::WA_DeleteOnClose, false);

  QxrdDetectorSettingsPtr dt(m_Detector);

  if (dt) {
    m_Processor = dt->processor();
  }

  QxrdProcessorPtr dp(m_Processor);
  QxrdAppCommon *app = qobject_cast<QxrdAppCommon*>(g_Application);
  QxrdExperimentPtr expt(m_Experiment);
  QxrdAcqCommonPtr acqp(m_Acquisition);

  if (dp) {
    dp->prop_DetectorDisplayMode()     -> linkTo(m_DetectorDisplayMode);
    dp->prop_PerformDarkSubtraction()  -> linkTo(m_PerformDarkSubtraction);
    dp->prop_SaveRawImages()           -> linkTo(m_SaveRawData);
    dp->prop_SaveDarkImages()          -> linkTo(m_SaveDarkData);
    dp->prop_PerformBadPixels()        -> linkTo(m_BadPixelCorrection);
    dp->prop_PerformGainCorrection()   -> linkTo(m_GainCorrection);
    dp->prop_SaveSubtracted()          -> linkTo(m_SaveSubtractedData);

    dp->prop_DarkImagePath()           -> linkTo(m_DarkImagePath);
    dp->prop_SaveRawSubdirectory()     -> linkTo(m_RawDataSubdir);
    dp->prop_SaveDarkSubdirectory()    -> linkTo(m_DarkDataSubdir);
    dp->prop_BadPixelsPath()           -> linkTo(m_BadPixelPath);
    dp->prop_GainMapPath()             -> linkTo(m_GainCorrectionPath);
    dp->prop_SaveSubtractedSubdirectory()
                                       -> linkTo(m_SubtractedDataSubdir);

    dp->prop_PerformIntegration()      -> linkTo(m_PerformIntegration);
    dp->prop_DisplayIntegratedData()   -> linkTo(m_DisplayIntegratedData);
    dp->prop_SaveIntegratedData()      -> linkTo(m_SaveIntegratedData);
    dp->prop_SaveIntegratedPath()      -> linkTo(m_IntegratedDataFile);
    dp->prop_SaveIntegratedInSeparateFiles() -> linkTo(m_SaveIntegratedDataSeparate);
    dp->prop_SaveIntegratedSubdirectory()    -> linkTo(m_IntegratedDataSubdir);

    dp->prop_AccumulateIntegrated2D()  -> linkTo(m_AccumulateIntegratedData);
    dp->prop_AccumulateIntegratedName()-> linkTo(m_AccumulateIntegratedName);

    dp->prop_CalculateROICounts()      -> linkTo(m_CalculateROICounts);
    dp->prop_DisplayROIBorders()       -> linkTo(m_DisplayROIBorders);

    CONNECT_CHECK(connect(dp->prop_DisplayROIBorders(), &QcepBoolProperty::valueChanged, this, &QxrdDetectorControlWindow::updateROIDisplay));

    CONNECT_CHECK(connect(m_BrowseDarkImage,     &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doBrowseDark));
    CONNECT_CHECK(connect(m_ClearDarkImage,      &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doClearDark));
    CONNECT_CHECK(connect(m_BrowseBadPixels,     &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doBrowseBadPixels));
    CONNECT_CHECK(connect(m_ClearBadPixels,      &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doClearBadPixels));
    CONNECT_CHECK(connect(m_BrowseGainImage,     &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doBrowseGainCorrection));
    CONNECT_CHECK(connect(m_ClearGainImage,      &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doClearGainCorrection));
    CONNECT_CHECK(connect(m_ActionLoadDarkImage, &QAction::triggered, this, &QxrdDetectorControlWindow::doBrowseDark));
    CONNECT_CHECK(connect(m_ActionLoadMask,      &QAction::triggered, this, &QxrdDetectorControlWindow::doBrowseMask));

    updateROIDisplay(dp->get_DisplayROIBorders());

    QcepROICalculatorPtr calc(dp->roiCalculator());

    if (calc) {
      m_ROIModel = calc->roiModel();

//      m_ROIWidget->setItemDelegateForColumn(QxrdROIModel::OuterTypeCol, new QxrdROITypeDelegate());
//      m_ROIWidget->setItemDelegateForColumn(QxrdROIModel::InnerTypeCol, new QxrdROITypeDelegate());
      m_ROIWidget->setModel(m_ROIModel.data());

      CONNECT_CHECK(connect(m_NewROI,      &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doAppendROI));
      CONNECT_CHECK(connect(m_DeleteROI,   &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doDeleteROI));
      CONNECT_CHECK(connect(m_MoveROIDown, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doMoveROIDown));
      CONNECT_CHECK(connect(m_MoveROIUp,   &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doMoveROIUp));

      CONNECT_CHECK(connect(m_EditROIButton,          &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doEditROI));
      CONNECT_CHECK(connect(m_RecalculateButton,      &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doRecalculate));
      CONNECT_CHECK(connect(m_VisualizeROIBackground, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doVisualizeBackground));
      CONNECT_CHECK(connect(m_VisualizeROIPeak,       &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::doVisualizePeak));

      m_ROIWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    CONNECT_CHECK(
          connect(dp.data(),       &QxrdProcessor::dataAvailable,
                  m_DetectorImage, &QcepImagePlotWidget::newImage));

    CONNECT_CHECK(
          connect(dp.data(),       &QxrdProcessor::maskAvailable,
                  m_DetectorImage, &QcepImagePlotWidget::newMask));

    CONNECT_CHECK(
          connect(dp.data(),       &QxrdProcessor::dataAvailable,
                  this,            &QxrdDetectorControlWindow::newImage));

    CONNECT_CHECK(
          connect(dp.data(),       &QxrdProcessor::darkAvailable,
                  this,            &QxrdDetectorControlWindow::newImage));

    m_ImageInfoTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::ResizeToContents);

    //TODO: Initialise

//    m_DetectorImage->init(dp->imagePlotSettings(), expt);

//    m_DetectorImage->setROIModel(m_ROIModel);
//    m_DetectorImage->setROISelection(m_ROIWidget->selectionModel());

//    CONNECT_CHECK(connect(m_ImageZoomInButton, &QAbstractButton::clicked, m_DetectorImage, &QxrdDetectorImagePlot::enableZooming));
//    CONNECT_CHECK(connect(m_ImageZoomOutButton, &QAbstractButton::clicked, m_DetectorImage, &QcepPlot::zoomOut));
//    CONNECT_CHECK(connect(m_ImageZoomAllButton, &QAbstractButton::clicked, m_DetectorImage, &QxrdImagePlot::autoScale));

//    CONNECT_CHECK(connect(m_ROICreateButton, &QAbstractButton::clicked, m_DetectorImage, &QxrdDetectorImagePlot::enableROICreate));
//    CONNECT_CHECK(connect(m_ROISelectButton, &QAbstractButton::clicked, m_DetectorImage, &QxrdDetectorImagePlot::enableROISelect));
//    CONNECT_CHECK(connect(m_ROIAddNodeButton, &QAbstractButton::clicked, m_DetectorImage, &QxrdDetectorImagePlot::enableROIAddNode));
//    CONNECT_CHECK(connect(m_ROIRemoveNodeButton, &QAbstractButton::clicked, m_DetectorImage, &QxrdDetectorImagePlot::enableROIRemoveNode));

//    CONNECT_CHECK(connect(m_DetectorImage,   &QxrdDetectorImagePlot::changeROICreateType, this, &QxrdDetectorControlWindow::onChangeROICreateType));
//    CONNECT_CHECK(connect(m_ROIRotateButton, &QAbstractButton::clicked, m_DetectorImage, &QxrdDetectorImagePlot::enableROIRotate));
//    CONNECT_CHECK(connect(m_ROIResizeButton, &QAbstractButton::clicked, m_DetectorImage, &QxrdDetectorImagePlot::enableROIResize));
  }

  if (app) {
    CONNECT_CHECK(
          connect(app->prop_UpdateIntervalMsec(), &QcepIntProperty::valueChanged,
                  this, &QxrdDetectorControlWindow::onUpdateIntervalMsecChanged));

    CONNECT_CHECK(
          connect(&m_UpdateTimer, &QTimer::timeout,
                  this, &QxrdDetectorControlWindow::updateImageDisplay));

    m_UpdateTimer.start(app->get_UpdateIntervalMsec());
  }

  if (expt) {
//    int fs = expt->get_FontSize();
//    int sp = expt->get_Spacing();

//    if (fs > 0) {
//      setFontSize(fs);
//    }

//    if (sp >= 0) {
//      setSpacing(sp);
//    }

    expt  -> prop_ExperimentDirectory() -> linkTo(this -> m_ExperimentDirectory);
    expt  -> prop_LogFileName() -> linkTo(this -> m_LogFileName);
    expt  -> prop_DataDirectory() -> linkTo(this -> m_DataDirectory);
    expt  -> prop_ScanFileName() -> linkTo(this -> m_ScanFileName);

    CONNECT_CHECK(connect(m_LogFileName, &QLineEdit::editingFinished, expt.data(), &QxrdExperiment::openNewLogFile));

//    CONNECT_CHECK(connect(expt->prop_FontSize(), &QcepIntProperty::valueChanged, this, &QxrdDetectorControlWindow::setFontSize));
//    CONNECT_CHECK(connect(expt->prop_Spacing(), &QcepIntProperty::valueChanged, this, &QxrdDetectorControlWindow::setSpacing));
  }

  if (dt) {
    CONNECT_CHECK(connect(dt->prop_DetectorNumber(),   &QcepIntProperty::valueChanged,    this, &QxrdDetectorControlWindow::updateTitle));
    CONNECT_CHECK(connect(dt->prop_DetectorTypeName(), &QcepStringProperty::valueChanged, this, &QxrdDetectorControlWindow::updateTitle));
    CONNECT_CHECK(connect(dt->prop_DetectorName(),     &QcepStringProperty::valueChanged, this, &QxrdDetectorControlWindow::updateTitle));

    updateTitle();
  }

  if (acqp) {
    CONNECT_CHECK(connect(m_ActionAcquire,     &QAction::triggered, this, &QxrdDetectorControlWindow::doAcquire));
    CONNECT_CHECK(connect(m_ActionAcquireOnce, &QAction::triggered, this, &QxrdDetectorControlWindow::doAcquireOnce));
    CONNECT_CHECK(connect(m_ActionCancel,      &QAction::triggered, this, &QxrdDetectorControlWindow::doCancel));
    CONNECT_CHECK(connect(m_ActionAcquireDark, &QAction::triggered, this, &QxrdDetectorControlWindow::doAcquireDark));
    CONNECT_CHECK(connect(m_ActionTrigger,     &QAction::triggered, acqp.data(), &QxrdAcqCommon::trigger));

    CONNECT_CHECK(connect(m_BrowseLogFileButton, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::browseLogFile));
    CONNECT_CHECK(connect(m_BrowseScanFileButton, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::browseScanFile));

    CONNECT_CHECK(connect(m_AcquireButton, &QAbstractButton::clicked, m_ActionAcquire, &QAction::triggered));
    CONNECT_CHECK(connect(m_AcquireOnceButton, &QAbstractButton::clicked, m_ActionAcquireOnce, &QAction::triggered));
    CONNECT_CHECK(connect(m_CancelButton, &QAbstractButton::clicked, m_ActionCancel, &QAction::triggered));
    CONNECT_CHECK(connect(m_TriggerButton, &QAbstractButton::clicked, m_ActionTrigger, &QAction::triggered));
    CONNECT_CHECK(connect(m_DarkAcquireButton, &QAbstractButton::clicked, m_ActionAcquireDark, &QAction::triggered));

    CONNECT_CHECK(connect(m_ClearDroppedButton, &QAbstractButton::clicked, acqp.data(), &QxrdAcqCommon::clearDropped));


    acqp -> prop_ExposureTime() -> linkTo(this -> m_ExposureTime);
    acqp -> prop_SummedExposures() -> linkTo(this -> m_SummedExposures);
    acqp -> prop_SkippedExposures() -> linkTo(this -> m_SkippedExposures);
    acqp -> prop_SkippedExposuresAtStart() -> linkTo(this -> m_SkippedExposuresAtStart);
    acqp -> prop_DarkSummedExposures() -> linkTo(this -> m_DarkSummedExposures);
    acqp -> prop_FilePattern() -> linkTo(this -> m_FilePattern);
    acqp -> prop_FileIndex() -> linkTo(this -> m_FileIndex);
    acqp -> prop_PhasesInGroup() -> linkTo(this -> m_PhasesInGroup);
    acqp -> prop_PreTriggerFiles() -> linkTo(this -> m_PreTriggerFiles);
    acqp -> prop_PostTriggerFiles() -> linkTo(this -> m_PostTriggerFiles);
    acqp -> prop_DroppedFrames() -> linkTo(this -> m_DroppedFrames);

    acqp -> prop_UserComment1() -> linkTo(this -> m_UserComment1);
    acqp -> prop_UserComment2() -> linkTo(this -> m_UserComment2);
    acqp -> prop_UserComment3() -> linkTo(this -> m_UserComment3);
    acqp -> prop_UserComment4() -> linkTo(this -> m_UserComment4);

    acqp -> prop_LiveViewAtIdle() -> linkTo(this -> m_LiveViewAtIdle);
    acqp -> prop_AcquisitionCancelsLiveView() -> linkTo(this -> m_AcquisitionCancelsLiveView);
    acqp -> prop_RetryDropped() -> linkTo(this -> m_RetryDropped);
  }

  CONNECT_CHECK(connect(m_EventLogButton, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::openEventLog));
  CONNECT_CHECK(connect(m_DetectorInfoButton, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::detectorInfoWindow));
  CONNECT_CHECK(connect(m_RestartDetectorsButton, &QAbstractButton::clicked, this, &QxrdDetectorControlWindow::restartDetectors));

  CONNECT_CHECK(connect(m_ExposureOptionsButton, &QAbstractButton::clicked, this, &QxrdMainWindow::doEditExposurePreferences));
  CONNECT_CHECK(connect(m_DetectorOptionsButton, &QAbstractButton::clicked, this, &QxrdMainWindow::doEditDetectorPreferences));
  CONNECT_CHECK(connect(m_AcquireOptionsButton, &QAbstractButton::clicked, this, &QxrdMainWindow::doEditPreferences));

  QxrdDetectorControlWindowSettingsPtr set(detectorControlWindowSettings());

  if (set) {
    m_DetectorImage -> initialize(set->imagePlotWidgetSettings());

    if (dp) {
      QcepCenterFinderPtr     cf(dp->centerFinder());
      QcepMaskStackPtr        ms(dp->maskStack());
      QcepPowderRingsModelPtr pr(dp->powderRings());
      QcepROIModelWPtr        ro(dp->roiModel());

      m_DetectorImage -> setCenterFinder(cf);
      m_DetectorImage -> setMaskStack(ms);
      m_DetectorImage -> setPowderRings(pr);
      m_DetectorImage -> setROIModel(ro);
      m_DetectorImage -> addPlotCommandSpacer();
    }
  }
}

QxrdDetectorControlWindow::~QxrdDetectorControlWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting detector control window\n");
#endif
}

void QxrdDetectorControlWindow::setupMenus(QMenu *file, QMenu *edit, QMenu *window)
{
  inherited::setupMenus(file, edit, window);
}

QString QxrdDetectorControlWindow::windowName() const
{
  QString name;

  QxrdDetectorSettingsPtr dt(m_Detector);

  if (dt) {
    name = tr("Control %1 %2: \"%3\"")
        .arg(dt->get_DetectorTypeName())
        .arg(dt->get_DetectorNumber())
        .arg(dt->get_DetectorName());
  } else {
    name = inherited::windowName();
  }

  return name;
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

void QxrdDetectorControlWindow::captureSize()
{
  QxrdDetectorControlWindowSettingsPtr set(detectorControlWindowSettings());

  if (set) {
    set->set_DetectorWindowOpen(isVisible());
    set->set_DetectorWindowGeometry(saveGeometry());
    set->set_DetectorWindowState(saveState(QXRD_DETECTOR_WINDOW_STATE_VERSION));
  }
}

void QxrdDetectorControlWindow::resizeEvent(QResizeEvent *ev)
{
  captureSize();

  QxrdMainWindow::resizeEvent(ev);
}

void QxrdDetectorControlWindow::moveEvent(QMoveEvent *ev)
{
  captureSize();

  QxrdMainWindow::moveEvent(ev);
}

QxrdDetectorControlWindowSettingsWPtr QxrdDetectorControlWindow::detectorControlWindowSettings()
{
  QxrdDetectorControlWindowSettingsWPtr res;

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    res = det->detectorControlWindowSettings();
  }

  return res;
}

void QxrdDetectorControlWindow::printLine(QString line)
{
  displayMessage(line);
}

void QxrdDetectorControlWindow::printMessage(QString msg, QDateTime ts)
{
  QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+
      QThread::currentThread()->objectName()+": "+
      msg.trimmed();

  message = message.replace("\n", " : ");

  displayMessage(message);
}

void QxrdDetectorControlWindow::criticalMessage(QString /*msg*/, QDateTime /*ts*/)
{
}

void QxrdDetectorControlWindow::statusMessage(QString /*msg*/, QDateTime /*ts*/)
{
}

void QxrdDetectorControlWindow::displayMessage(QString msg)
{
  if (QThread::currentThread()==thread()) {
    m_Messages -> append(msg);
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "displayMessage",
                                           Qt::QueuedConnection,
                                           Q_ARG(QString, msg)));
  }
}

QVector<int> QxrdDetectorControlWindow::selectedROIs()
{
  QVector<int> res;
  QcepROIModelPtr roiModel(m_ROIModel);

  if (roiModel) {
    int roiCount = roiModel->rowCount(QModelIndex());

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
  QcepROIModelPtr roiModel(m_ROIModel);

  if (roiModel) {
    QMenu menu;

    for (int i=0; i<QcepROIShape::roiTypeCount(); i++) {
      for (int j=1; j<QcepROIShape::roiTypeCount(); j++) {
        menu.addAction(QcepROI::roiTypeName(i,j),
                       [=]() { appendROI(QcepROI::roiTypeID(i,j));});
      }
    }

    menu.exec(QCursor::pos());
  }
}

void QxrdDetectorControlWindow::appendROI(int roiType)
{
  QcepROIModelPtr roiModel(m_ROIModel);

  if (roiModel) {
    QcepROIPtr roi =
        QcepROI::newROICoordinates(roiType);

    roiModel->append(roi);
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
    QcepROIModelPtr roiModel(m_ROIModel);

    if (roiModel) {
      for (int i=rois.count()-1; i>=0; i--) {
        roiModel->removeROI(rois.value(i));
      }
    }
  }
}

void QxrdDetectorControlWindow::doMoveROIDown()
{
  QVector<int> rois = selectedROIs();

  if (rois.count() != 1) {
    QMessageBox::information(this, "Only Move One", "Must have a single ROI selected before moving it", QMessageBox::Ok);
  } else {
    QcepROIModelPtr roiModel(m_ROIModel);

    if (roiModel) {
      roiModel->moveROIDown(rois.first());
    }
  }
}

void QxrdDetectorControlWindow::doMoveROIUp()
{
  QVector<int> rois = selectedROIs();

  if (rois.count() != 1) {
    QMessageBox::information(this, "Only Move One", "Must have a single ROI selected before moving it", QMessageBox::Ok);
  } else {
    QcepROIModelPtr roiModel(m_ROIModel);

    if (roiModel) {
      roiModel->moveROIUp(rois.first());
    }
  }
}

void QxrdDetectorControlWindow::updateImageDisplay()
{
  //TODO: re-connect detector frame signals to widgets...

  if (m_NewDataAvailable.fetchAndStoreOrdered(0)) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_UpdateMutex);

      m_DisplayedImage = m_NewImage;
      m_NewImage       = QcepImageDataBasePtr();

      m_DisplayedOverflow = m_NewOverflow;
      m_NewOverflow = QcepMaskDataPtr();
    }

    QxrdProcessorPtr proc(m_Processor);

    if (proc) {
      if (proc->get_DetectorDisplayMode() == QxrdProcessor::ImageDisplayMode) {
//        m_DetectorImage->onProcessedImageAvailable(m_DisplayedImage, m_DisplayedOverflow);
      }
    }
  } else if (m_NewMaskAvailable.fetchAndStoreOrdered(0)) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_UpdateMutex);

      m_DisplayedMask = m_NewMask;
      m_NewMask       = QcepMaskDataPtr();
    }

    QxrdProcessorPtr proc(m_Processor);

    if (proc) {
      if (proc->get_DetectorDisplayMode() == QxrdProcessor::ImageDisplayMode) {
//        m_DetectorImage->onMaskedImageAvailable(m_DisplayedImage, m_DisplayedMask);
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

void QxrdDetectorControlWindow::updateROIDisplay(bool /*show*/)
{
  //TODO: re-enable ROI display
//  m_DetectorImage -> enableROIDisplay(show);
}

void QxrdDetectorControlWindow::doBrowseMask()
{
  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    QString newPath = QFileDialog::getOpenFileName(this, "Select Mask Image",
                                                   dp->get_MaskPath());

    if (newPath.length() > 0) {
      dp->loadMask(newPath);

      dp->set_MaskPath(newPath);
    }
  }
}

//void QxrdDetectorControlWindow::doClearMask()
//{
//  QxrdProcessorPtr dp(m_Processor);

//  if (dp) {
//    int res = QMessageBox::information(this, "Clear Mask?", "Do you really want to clear the mask?", QMessageBox::Ok, QMessageBox::Cancel);

//    if (res == QMessageBox::Ok) {
//      dp->set_MaskPath("");
//    }
//  }
//}

void QxrdDetectorControlWindow::doBrowseDark()
{
  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    QString newPath = QFileDialog::getOpenFileName(this, "Select Dark Image",
                                                   dp->get_DarkImagePath());

    if (newPath.length() > 0) {
      dp->set_DarkImagePath(newPath);
    }
  }
}

void QxrdDetectorControlWindow::doClearDark()
{
  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    int res = QMessageBox::information(this, "Clear Dark?", "Do you really want to clear the dark image?", QMessageBox::Ok, QMessageBox::Cancel);

    if (res == QMessageBox::Ok) {
      dp->set_DarkImagePath("");
    }
  }
}

void QxrdDetectorControlWindow::doBrowseBadPixels()
{
  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    QString newPath = QFileDialog::getOpenFileName(this, "Select Bad Pixels Image",
                                                   dp->get_BadPixelsPath());

    if (newPath.length() > 0) {
      dp->set_BadPixelsPath(newPath);
    }
  }
}

void QxrdDetectorControlWindow::doClearBadPixels()
{
  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    int res = QMessageBox::information(this, "Clear Bad Pixels?", "Do you really want to clear the bad pixels image?", QMessageBox::Ok, QMessageBox::Cancel);

    if (res == QMessageBox::Ok) {
      dp->set_BadPixelsPath("");
    }
  }
}

void QxrdDetectorControlWindow::doBrowseGainCorrection()
{
  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    QString newPath = QFileDialog::getOpenFileName(this, "Select Gain Correction Image",
                                                   dp->get_GainMapPath());

    if (newPath.length() > 0) {
      dp->set_GainMapPath(newPath);
    }
  }
}

void QxrdDetectorControlWindow::doClearGainCorrection()
{
  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    int res = QMessageBox::information(this, "Clear Gain Correction?", "Do you really want to clear the gain correction image?", QMessageBox::Ok, QMessageBox::Cancel);

    if (res == QMessageBox::Ok) {
      dp->set_GainMapPath("");
    }
  }
}

void QxrdDetectorControlWindow::doEditROI()
{
  QxrdDetectorSettingsPtr dt(m_Detector);
  QxrdProcessorPtr        dp(m_Processor);
  QcepROIModelPtr         roiModel(m_ROIModel);

  if (dt && dp) {
    QVector<int> rois = selectedROIs();

    if (rois.count() != 1) {
      QMessageBox::information(this, "Edit ROI", "Select one ROI to edit", QMessageBox::Ok);
    } else if (roiModel) {
      QcepROIPtr roi = roiModel->roi(rois.first());

      QcepROIEditorDialog editor(roi, this);

      editor.setWindowTitle(tr("Edit ROI %2 of Detector %1")
                            .arg(dt->get_DetectorNumber())
                            .arg(rois.first()));

      if (editor.exec() == QDialog::Accepted) {
        roiModel->setRoi(rois.first(), editor.roi());
      }
    }
  }
}

void QxrdDetectorControlWindow::doRecalculate()
{
  QxrdProcessorPtr dp(m_Processor);
  QcepROIModelPtr  roiModel(m_ROIModel);

  if (dp && roiModel) {
    roiModel->recalculate(dp->data(), dp->mask());
  }
}

void QxrdDetectorControlWindow::doVisualizeBackground()
{
  QxrdProcessorPtr dp(m_Processor);
  QcepROIModelPtr  roiModel(m_ROIModel);

  if (dp) {
    QVector<int> rois = selectedROIs();

    if (rois.count() != 1) {
      QMessageBox::information(this, "Visualize ROI", "Select one ROI to visualize", QMessageBox::Ok);
    } else if (roiModel) {
      roiModel->visualizeBackground(rois.first(), dp->data(), dp->mask());

      displayNewData(dp->data(), QcepMaskDataPtr());
    }
  }
}

void QxrdDetectorControlWindow::doVisualizePeak()
{
  QxrdProcessorPtr dp(m_Processor);
  QcepROIModelPtr  roiModel(m_ROIModel);

  if (dp) {
    QVector<int> rois = selectedROIs();

    if (rois.count() != 1) {
      QMessageBox::information(this, "Visualize ROI", "Select one ROI to visualize", QMessageBox::Ok);
    } else if (roiModel) {
      roiModel->visualizePeak(rois.first(), dp->data(), dp->mask());

      displayNewData(dp->data(), QcepMaskDataPtr());
    }
  }
}

void QxrdDetectorControlWindow::doAcquire()
{
  QxrdAcqCommonPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> acquire();
  }
}

void QxrdDetectorControlWindow::doAcquireOnce()
{
  QxrdAcqCommonPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> acquireOnce();
  }
}

void QxrdDetectorControlWindow::doCancel()
{
  QxrdAcqCommonPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> cancel();
  }
}

void QxrdDetectorControlWindow::doAcquireDark()
{
  QxrdAcqCommonPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> acquireDark();
  }
}

void QxrdDetectorControlWindow::acquireStarted()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    m_AcquireButton -> setEnabled(false);
    m_ActionAcquire -> setEnabled(false);

    m_AcquireOnceButton -> setEnabled(false);
    m_ActionAcquireOnce -> setEnabled(false);

    if (acq -> get_PreTriggerFiles() > 0) {
      m_TriggerButton -> setEnabled(true);
      m_ActionTrigger -> setEnabled(true);
    } else {
      m_TriggerButton -> setEnabled(false);
      m_ActionTrigger -> setEnabled(false);
    }

    m_CancelButton -> setEnabled(true);
    m_ActionCancel -> setEnabled(true);

    m_DarkAcquireButton -> setEnabled(false);
    m_ActionAcquireDark -> setEnabled(false);
  }

  inherited::acquireStarted();
}

void QxrdDetectorControlWindow::acquireComplete()
{
  m_AcquireButton -> setEnabled(true);
  m_ActionAcquire -> setEnabled(true);

  m_AcquireOnceButton -> setEnabled(true);
  m_ActionAcquireOnce -> setEnabled(true);

  m_TriggerButton -> setEnabled(false);
  m_ActionTrigger -> setEnabled(false);

  m_CancelButton -> setEnabled(false);
  m_ActionCancel -> setEnabled(false);

  m_DarkAcquireButton -> setEnabled(true);
  m_ActionAcquireDark -> setEnabled(true);

  inherited::acquireComplete();
}

void QxrdDetectorControlWindow::browseLogFile()
{
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_LogFileName->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial.absoluteFilePath());

  if (file != "") {
    m_LogFileName->setText(pwd.relativeFilePath(file));
  }
}

void QxrdDetectorControlWindow::browseScanFile()
{
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_ScanFileName->text());

  QString file = QFileDialog::getSaveFileName(this, "Scan File", initial.absoluteFilePath());

  if (file != "") {
    m_ScanFileName->setText(pwd.relativeFilePath(file));
  }
}

void QxrdDetectorControlWindow::onChangeROICreateType(int /*newType*/)
{
  //TODO: reimplement
//  switch (newType) {
//  case QxrdDetectorImagePlot::NewRectROI:
//    m_ROICreateButton->setIcon(QIcon(":/images/draw-rectangle.png"));
//    break;

//  case QxrdDetectorImagePlot::NewEllipseROI:
//    m_ROICreateButton->setIcon(QIcon(":/images/draw-ellipse.png"));
//    break;

//  case QxrdDetectorImagePlot::NewRectDonutROI:
//    m_ROICreateButton->setIcon(QIcon(":/images/draw-rectangle-donut.png"));
//    break;

//  case QxrdDetectorImagePlot::NewEllipseDonutROI:
//    m_ROICreateButton->setIcon(QIcon(":/images/draw-donut.png"));
//    break;
//  }
}

void QxrdDetectorControlWindow::newImage(QcepImageDataBasePtr img)
{
  printMessage("QxrdDetectorControlWindow::newImage");

  m_ImageProperties =
      QcepDataObjectPropertiesModelPtr(
        new QcepDataObjectPropertiesModel(img));

  m_ImageInfoTable -> setModel(m_ImageProperties.data());
}

void QxrdDetectorControlWindow::restartDetectors()
{
  int reply = QMessageBox::question(this, "Restart Detector Hardware", "Do you want to restart\nthe detector hardware?",
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);

  if (reply == QMessageBox::Yes) {
    QxrdAcqCommonPtr acq(m_Acquisition);

    if (acq) {
      INVOKE_CHECK(
            QMetaObject::invokeMethod(acq.data(), "restartDetectors"));
    }
  }
}

void QxrdDetectorControlWindow::detectorInfoWindow()
{
  if (m_InfoWindow == NULL) {
    m_InfoWindow =
        QxrdInfoWindowPtr(
          new QxrdInfoWindow("detectorInfo", m_Detector));

    m_InfoWindow -> initialize(m_Detector);
  }

  if (m_InfoWindow) {
    m_InfoWindow -> show();
    m_InfoWindow -> raise();
  }
}

void QxrdDetectorControlWindow::openEventLog()
{
  if (m_AcquisitionEventLog == NULL) {
    m_AcquisitionEventLog =
        QxrdAcquisitionEventLogWindowPtr(
          new QxrdAcquisitionEventLogWindow(m_Acquisition));
  }

  if (m_AcquisitionEventLog) {
    m_AcquisitionEventLog->show();
    m_AcquisitionEventLog->activateWindow();
    m_AcquisitionEventLog->raise();
  }
}
