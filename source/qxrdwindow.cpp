/******************************************************************
*
*  $Id: qxrdwindow.cpp,v 1.5 2010/10/22 21:44:26 jennings Exp $
*
*******************************************************************/

#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"
#include "qxrdacquisition.h"
#include "qxrdsettings.h"
#include "qxrdimageplot.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddataprocessor.h"
#include "qxrdacquiredialog.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"
#include "qxrdplotzoomer.h"
#include "qxrdscriptengine.h"
//#include "qxrdfilebrowser.h"
#include "qxrdimagecalculator.h"
#include "qxrdmutexlocker.h"
#include "qxrdallocator.h"
#include "qxrdpowderfitdialog.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_symbol.h>
#include <QPen>
#include <QFile>
#include <QCheckBox>
#include <QSignalMapper>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTime>
#include <QDirModel>
#include <QString>
#include <QMetaObject>
#include <QMetaMethod>
#include <QDesktopServices>
#include <QUrl>

QxrdWindow::QxrdWindow(QxrdApplicationPtr app, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc, QxrdAllocatorPtr alloc, QWidget *parent)
  : QMainWindow(parent),
    m_Mutex(QMutex::Recursive),
    m_SettingsLoaded(false),
    m_Application(app),
    m_Acquisition(acq),
    m_DataProcessor(proc),
    m_Allocator(alloc),
    m_CenterFinderDialog(NULL),
    m_IntegratorDialog(NULL),
//    m_FileBrowser(NULL),
    m_Calculator(NULL),
    m_Progress(NULL),
    m_AllocationStatus(NULL),
    m_Acquiring(false),
    m_AcquiringDark(false),
//    m_NewDataMutex(QMutex::Recursive),
    m_Data(NULL/*new QxrdDoubleImageData(NULL,2048,2048)*/),
    m_Overflow(NULL),
    m_NewData(NULL/*new QxrdDoubleImageData(NULL,2048,2048)*/),
    m_NewOverflow(NULL),
    m_NewDataAvailable(false),
//    m_NewMaskMutex(QMutex::Recursive),
    m_Mask(NULL/*new QxrdMaskData(NULL,2048,2048)*/),
    m_NewMask(NULL/*new QxrdMaskData(2048,2048)*/),
    m_NewMaskAvailable(false),
    SOURCE_IDENT("$Id: qxrdwindow.cpp,v 1.5 2010/10/22 21:44:26 jennings Exp $")
{
  setupUi(this);

  setWindowTitle(windowTitle()+" - v"+QXRD_VERSION);

  m_AcquireDialog = QxrdAcquireDialogPtr(new QxrdAcquireDialog(this));
  m_AcquireDockWidget -> setWidget(m_AcquireDialog);

  m_CenterFinderDialog = QxrdCenterFinderDialogPtr(new QxrdCenterFinderDialog(m_DataProcessor -> centerFinder()));
  m_CenteringDockWidget -> setWidget(m_CenterFinderDialog);

  m_IntegratorDialog = QxrdIntegratorDialogPtr(new QxrdIntegratorDialog(m_DataProcessor -> integrator()));
  m_IntegratorDockWidget -> setWidget(m_IntegratorDialog);

//  m_FileBrowser = new QxrdFileBrowser(m_DataProcessor);
//  m_FileBrowserDockWidget -> setWidget(m_FileBrowser);

//  m_Calculator = new QxrdImageCalculator(m_DataProcessor);
//  addDockWidget(Qt::RightDockWidgetArea, m_Calculator);

  connect(m_ExecuteScriptButton, SIGNAL(clicked()), m_ActionExecuteScript, SIGNAL(triggered()));
  connect(m_ActionExecuteScript, SIGNAL(triggered()), this, SLOT(executeScript()));
  connect(m_CancelScriptButton, SIGNAL(clicked()), m_ActionCancelScript, SIGNAL(triggered()));
  connect(m_ActionCancelScript, SIGNAL(triggered()), this, SLOT(cancelScript()));

  connect(m_ActionAutoScale, SIGNAL(triggered()), m_Plot, SLOT(autoScale()));
  connect(m_ActionQuit, SIGNAL(triggered()), m_Application, SLOT(possiblyQuit()));
  connect(m_ActionPreferences, SIGNAL(triggered()), m_Application, SLOT(editPreferences()));
  connect(m_ActionLoadData, SIGNAL(triggered()), this, SLOT(doLoadData()));
  connect(m_ActionSaveData, SIGNAL(triggered()), this, SLOT(doSaveData()));
  connect(m_ActionLoadDark, SIGNAL(triggered()), this, SLOT(doLoadDark()));
  connect(m_ActionSaveDark, SIGNAL(triggered()), this, SLOT(doSaveDark()));
  connect(m_ActionLoadMask, SIGNAL(triggered()), this, SLOT(doLoadMask()));
  connect(m_ActionSaveMask, SIGNAL(triggered()), this, SLOT(doSaveMask()));
  connect(m_ActionSelectLogFile, SIGNAL(triggered()), this, SLOT(selectLogFile()));

  connect(m_ActionAcquire, SIGNAL(triggered()), this, SLOT(doAcquire()));
  connect(m_ActionCancel, SIGNAL(triggered()), this, SLOT(doCancel()));
  connect(m_ActionAcquireDark, SIGNAL(triggered()), this, SLOT(doAcquireDark()));
  connect(m_ActionCancelDark, SIGNAL(triggered()), this, SLOT(doCancelDark()));
  connect(m_ActionTrigger, SIGNAL(triggered()), m_Acquisition, SLOT(trigger()));

  connect(m_AcquireDialog -> m_SelectLogFileButton, SIGNAL(clicked()), this, SLOT(selectLogFile()));
  connect(m_AcquireDialog -> m_SelectDirectoryButton, SIGNAL(clicked()), this, SLOT(selectOutputDirectory()));

  connect(m_AcquireDialog -> m_AcquireButton, SIGNAL(clicked()), m_ActionAcquire, SIGNAL(triggered()));
  connect(m_AcquireDialog -> m_CancelButton, SIGNAL(clicked()), m_ActionCancel, SIGNAL(triggered()));
  connect(m_AcquireDialog -> m_DarkAcquireButton, SIGNAL(clicked()), m_ActionAcquireDark, SIGNAL(triggered()));
//  connect(m_AcquireDialog -> m_DarkCancelButton, SIGNAL(clicked()), m_ActionCancelDark, SIGNAL(triggered()));
  connect(m_AcquireDialog -> m_TriggerButton, SIGNAL(clicked()), m_ActionTrigger, SIGNAL(triggered()));

  connect(m_AcquireDialog -> m_ClearDroppedButton, SIGNAL(clicked()), m_Acquisition, SLOT(clearDropped()));

  connect(m_AutoRange, SIGNAL(clicked()), m_ActionAutoRange, SIGNAL(triggered()));
  connect(m_Display_5pct, SIGNAL(clicked()), m_Action005Range, SIGNAL(triggered()));
  connect(m_Display_10pct, SIGNAL(clicked()), m_Action010Range, SIGNAL(triggered()));
  connect(m_Display_100pct, SIGNAL(clicked()), m_Action100Range, SIGNAL(triggered()));

  connect(m_Action005Range, SIGNAL(triggered()), m_Plot, SLOT(set005Range()));
  connect(m_Action010Range, SIGNAL(triggered()), m_Plot, SLOT(set010Range()));
  connect(m_Action100Range, SIGNAL(triggered()), m_Plot, SLOT(set100Range()));
  connect(m_ActionAutoRange, SIGNAL(triggered()), m_Plot, SLOT(setAutoRange()));

  connect(m_ActionGrayscale, SIGNAL(triggered()), m_Plot, SLOT(setGrayscale()));
  connect(m_ActionInverseGrayscale, SIGNAL(triggered()), m_Plot, SLOT(setInverseGrayscale()));
  connect(m_ActionEarthTones, SIGNAL(triggered()), m_Plot, SLOT(setEarthTones()));
  connect(m_ActionSpectrum, SIGNAL(triggered()), m_Plot, SLOT(setSpectrum()));
  connect(m_ActionFire, SIGNAL(triggered()), m_Plot, SLOT(setFire()));
  connect(m_ActionIce, SIGNAL(triggered()), m_Plot, SLOT(setIce()));

  connect(m_ActionRefineCenterTilt, SIGNAL(triggered()), this, SLOT(doRefineCenterTilt()));
  connect(m_HideMaskRange, SIGNAL(clicked()), m_ActionHideMaskRange, SIGNAL(triggered()));
  connect(m_ShowMaskRange, SIGNAL(clicked()), m_ActionShowMaskRange, SIGNAL(triggered()));

  connect(m_HideMaskAll, SIGNAL(clicked()), m_ActionHideMaskAll, SIGNAL(triggered()));
  connect(m_ShowMaskAll, SIGNAL(clicked()), m_ActionShowMaskAll, SIGNAL(triggered()));
  connect(m_InvertMask, SIGNAL(clicked()), m_ActionInvertMask, SIGNAL(triggered()));

  connect(m_MaskCirclesRadio, SIGNAL(clicked()), m_ImageMaskCirclesButton, SLOT(click()));
  connect(m_MaskPolygonsRadio, SIGNAL(clicked()), m_ImageMaskPolygonsButton, SLOT(click()));

  connect(m_ActionShowImage, SIGNAL(triggered()), m_Plot, SLOT(toggleShowImage()));
  connect(m_ActionShowMask, SIGNAL(triggered()), m_Plot, SLOT(toggleShowMask()));
  connect(m_ActionShowMaskRange, SIGNAL(triggered()), m_DataProcessor, SLOT(showMaskRange()));
  connect(m_ActionHideMaskRange, SIGNAL(triggered()), m_DataProcessor, SLOT(hideMaskRange()));
  connect(m_ActionShowMaskAll, SIGNAL(triggered()), m_DataProcessor, SLOT(showMaskAll()));
  connect(m_ActionHideMaskAll, SIGNAL(triggered()), m_DataProcessor, SLOT(hideMaskAll()));
  connect(m_ActionInvertMask, SIGNAL(triggered()), m_DataProcessor, SLOT(invertMask()));
  connect(m_ActionMaskCircles, SIGNAL(triggered()), m_ImageMaskCirclesButton, SLOT(click()));
  connect(m_ActionMaskPolygons, SIGNAL(triggered()), m_ImageMaskPolygonsButton, SLOT(click()));

  connect(m_ActionROICalculate, SIGNAL(triggered()), m_DataProcessor, SLOT(calculateROI()));
  connect(m_ActionHistogramCalculate, SIGNAL(triggered()), m_DataProcessor, SLOT(calculateHistogram()));
  connect(m_ActionTest, SIGNAL(triggered()), this, SLOT(doTest()));

  connect(m_ImageZoomInButton, SIGNAL(clicked()), m_Plot, SLOT(enableZooming()));
  connect(m_ImageZoomOutButton, SIGNAL(clicked()), m_Plot, SLOT(zoomOut()));
  connect(m_ImageZoomAllButton, SIGNAL(clicked()), m_Plot, SLOT(autoScale()));
  connect(m_ImageSetCenterButton, SIGNAL(clicked()), m_Plot, SLOT(enableCentering()));
  connect(m_ImageSliceButton, SIGNAL(clicked()), m_Plot, SLOT(enableSlicing()));
  connect(m_ImageMeasureButton, SIGNAL(clicked()), m_Plot, SLOT(enableMeasuring()));
  connect(m_ImageMaskCirclesButton, SIGNAL(clicked()), m_Plot, SLOT(enableMaskCircles()));
  connect(m_ImageMaskPolygonsButton, SIGNAL(clicked()), m_Plot, SLOT(enableMaskPolygons()));

  connect(m_CenteringZoomInButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(enableZooming()));
  connect(m_CenteringZoomOutButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(zoomOut()));
  connect(m_CenteringZoomAllButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(autoScale()));
  connect(m_CenteringMeasureButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(enableMeasuring()));

  connect(m_IntegratorZoomInButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(enableZooming()));
  connect(m_IntegratorZoomOutButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(zoomOut()));
  connect(m_IntegratorZoomAllButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(autoScale()));
  connect(m_IntegratorMeasureButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(enableMeasuring()));

  connect(m_IntegratorDialog -> m_ClearGraphButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(clearGraph()));

  connect(m_ActionAboutQXRD, SIGNAL(triggered()), this, SLOT(doAboutQxrd()));
  connect(m_ActionOpenQXRDWebPage, SIGNAL(triggered()), this, SLOT(doOpenQXRDWebPage()));

  connect(m_HelpHomeButton, SIGNAL(clicked()), m_HelpBrowser, SLOT(home()));
  connect(m_HelpForwardButton, SIGNAL(clicked()), m_HelpBrowser, SLOT(forward()));
  connect(m_HelpBackButton, SIGNAL(clicked()), m_HelpBrowser, SLOT(backward()));

  connect(m_HelpBrowser, SIGNAL(forwardAvailable(bool)), m_HelpForwardButton, SLOT(setEnabled(bool)));
  connect(m_HelpBrowser, SIGNAL(backwardAvailable(bool)), m_HelpBackButton, SLOT(setEnabled(bool)));

  connect(&m_StatusTimer, SIGNAL(timeout()), this, SLOT(clearStatusMessage()));
  connect(&m_UpdateTimer, SIGNAL(timeout()), this, SLOT(newData()));

  m_StatusMsg = QLabelPtr(new QLabel(NULL));
  m_StatusMsg -> setMinimumWidth(200);
  m_StatusMsg -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_StatusMsg -> setToolTip(tr("Status Messages"));

  statusBar() -> addPermanentWidget(m_StatusMsg);

  m_Progress = QProgressBarPtr(new QProgressBar(NULL));
  m_Progress -> setMinimumWidth(150);
  m_Progress -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_Progress -> setToolTip(tr("Acquisition progress"));

  statusBar() -> addPermanentWidget(m_Progress);

//  m_Alloc = QLabelPtr(new QLabel(NULL));
//  m_Alloc -> setMinimumWidth(80);
//  m_Alloc -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//
//  statusBar() -> addPermanentWidget(m_Alloc);
//
//  m_MaxAlloc = QLabelPtr(new QLabel(NULL));
//  m_MaxAlloc -> setMinimumWidth(80);
//  m_MaxAlloc -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//
//  statusBar() -> addPermanentWidget(m_MaxAlloc);

  m_AllocationStatus = QProgressBarPtr(new QProgressBar(NULL));
  m_AllocationStatus -> setMinimumWidth(100);
  m_AllocationStatus -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_AllocationStatus -> setFormat("%v/%m");
  m_AllocationStatus -> setTextVisible(true);
  m_AllocationStatus -> setToolTip(tr("Memory usage"));

  statusBar() -> addPermanentWidget(m_AllocationStatus);

  if (m_Acquisition == NULL) {
    emit criticalMessage("Oh no, QxrdWindow::m_Acquisition == NULL");
  }

  connect(m_Acquisition,     SIGNAL(acquireStarted(int)),
          this,              SLOT(onAcquireStarted(int)));
  connect(m_Acquisition,     SIGNAL(acquiredFrame(QString,int,int,int,int,int)),
          this,              SLOT(onAcquiredFrame(QString,int,int,int,int,int)));
  connect(m_Acquisition,     SIGNAL(acquireComplete(int)),
          this,              SLOT(onAcquireComplete(int)));


  m_Acquisition -> setupExposureMenu(m_AcquireDialog -> m_ExposureTime);
  m_Acquisition -> setupCameraGainMenu(m_AcquireDialog -> m_CameraGain);
  m_Acquisition -> setupCameraBinningModeMenu(m_AcquireDialog -> m_BinningMode);

  m_Acquisition -> prop_DetectorTypeName() -> linkTo(m_AcquireDialog -> m_DetectorTypeNameLabel);
  m_Acquisition -> prop_ExposureTime() -> linkTo(m_AcquireDialog -> m_ExposureTime);
  m_Acquisition -> prop_SummedExposures() -> linkTo(m_AcquireDialog -> m_SummedExposures);
  m_Acquisition -> prop_SkippedExposures() -> linkTo(m_AcquireDialog -> m_SkippedExposures);
  m_Acquisition -> prop_DarkSummedExposures() -> linkTo(m_AcquireDialog -> m_DarkSummedExposures);
  m_Acquisition -> prop_FilePattern() -> linkTo(m_AcquireDialog -> m_FilePattern);
  m_Acquisition -> prop_FileIndex() -> linkTo(m_AcquireDialog -> m_FileIndex);
  m_Acquisition -> prop_PreTriggerFiles() -> linkTo(m_AcquireDialog -> m_PreTriggerFiles);
  m_Acquisition -> prop_PostTriggerFiles() -> linkTo(m_AcquireDialog -> m_PostTriggerFiles);
  m_Acquisition -> prop_CameraGain() -> linkTo(m_AcquireDialog -> m_CameraGain);
  m_Acquisition -> prop_BinningMode() -> linkTo(m_AcquireDialog -> m_BinningMode);
  m_Acquisition -> prop_DroppedFrames() -> linkTo(m_AcquireDialog -> m_DroppedFrames);

  m_DataProcessor -> prop_OutputDirectory() -> linkTo(m_AcquireDialog -> m_OutputDirectory);
  m_DataProcessor -> prop_PerformDarkSubtraction() -> linkTo(m_PerformDark);
  m_DataProcessor -> prop_PerformDarkSubtractionTime() -> linkTo(m_PerformDarkTime);
  m_DataProcessor -> prop_SaveRawImages() -> linkTo(m_SaveRaw);
  m_DataProcessor -> prop_SaveDarkImages() -> linkTo(m_SaveDark);
  m_Acquisition   -> prop_RawSaveTime() -> linkTo(m_SaveRawTime);
  m_Acquisition   -> prop_DarkSaveTime() -> linkTo(m_SaveDarkTime);
  m_DataProcessor -> prop_PerformBadPixels() -> linkTo(m_PerformBadPixels);
  m_DataProcessor -> prop_PerformBadPixelsTime() -> linkTo(m_PerformBadPixelsTime);
  m_DataProcessor -> prop_PerformGainCorrection() -> linkTo(m_PerformGainCorrection);
  m_DataProcessor -> prop_PerformGainCorrectionTime() -> linkTo(m_PerformGainCorrectionTime);
  m_DataProcessor -> prop_SaveSubtracted() -> linkTo(m_SaveSubtracted);
  m_DataProcessor -> prop_SaveSubtractedTime() -> linkTo(m_SaveSubtractedTime);
  m_DataProcessor -> prop_SaveAsText() -> linkTo(m_SaveAsText);
  m_DataProcessor -> prop_SaveAsTextTime() -> linkTo(m_SaveAsTextTime);
  m_DataProcessor -> prop_PerformIntegration() -> linkTo(m_PerformIntegration);
  m_DataProcessor -> prop_PerformIntegrationTime() -> linkTo(m_PerformIntegrationTime);
  m_DataProcessor -> prop_DisplayIntegratedData() -> linkTo(m_DisplayIntegratedData);
  m_DataProcessor -> prop_DisplayIntegratedDataTime() -> linkTo(m_DisplayIntegratedDataTime);
  m_DataProcessor -> prop_SaveIntegratedData() -> linkTo(m_SaveIntegratedData);
  m_DataProcessor -> prop_SaveIntegratedDataTime() -> linkTo(m_SaveIntegratedDataTime);
//  m_DataProcessor -> prop_CorrectedTime() -> linkTo(m_CorrectedTime);
//  m_DataProcessor -> prop_IntegratedTime() -> linkTo(m_IntegratedTime);
  m_DataProcessor -> prop_EstimatedProcessingTime() -> linkTo(m_EstimatedProcessingTime);
  m_DataProcessor -> prop_LogFilePath() -> linkTo(m_AcquireDialog -> m_LogFilePath);

//  m_DataProcessor -> prop_DarkImagePath() -> linkTo(m_DarkImagePath);
//  m_DataProcessor -> prop_BadPixelsPath() -> linkTo(m_BadPixelsPath);
//  m_DataProcessor -> prop_GainMapPath() -> linkTo(m_GainMapPath);

  m_DataProcessor -> prop_MaskMinimumValue() -> linkTo(Ui::QxrdWindow::m_MaskMinimum);
  m_DataProcessor -> prop_MaskMaximumValue() -> linkTo(Ui::QxrdWindow::m_MaskMaximum);
  m_DataProcessor -> prop_MaskCircleRadius() -> linkTo(Ui::QxrdWindow::m_MaskCircleRadius);
  m_DataProcessor -> prop_MaskSetPixels() -> linkTo(Ui::QxrdWindow::m_MaskSetPixels);

  m_DataProcessor -> prop_Average() -> linkTo(m_AcquireDialog -> m_AverageDisplay);

  m_Plot -> prop_DisplayMinimumPct() -> linkTo(Ui::QxrdWindow::m_DisplayMinimumPct);
  m_Plot -> prop_DisplayMaximumPct() -> linkTo(Ui::QxrdWindow::m_DisplayMaximumPct);
  m_Plot -> prop_DisplayMinimumVal() -> linkTo(Ui::QxrdWindow::m_DisplayMinimumVal);
  m_Plot -> prop_DisplayMaximumVal() -> linkTo(Ui::QxrdWindow::m_DisplayMaximumVal);
  m_Plot -> prop_DisplayMinimumPctle() -> linkTo(Ui::QxrdWindow::m_DisplayMinimumPctle);
  m_Plot -> prop_DisplayMaximumPctle() -> linkTo(Ui::QxrdWindow::m_DisplayMaximumPctle);

  m_Plot -> prop_DisplayScalingMode() -> linkTo(Ui::QxrdWindow::m_DisplayScalingMode);

  connect(m_Plot -> prop_DisplayScalingMode(), SIGNAL(changedValue(int)), Ui::QxrdWindow::m_DisplayParmsStack, SLOT(setCurrentIndex(int)));

  m_Plot -> prop_DisplayColorMap() -> linkTo(Ui::QxrdWindow::m_DisplayColorMap);

  m_Plot -> prop_ImageShown() -> linkTo(Ui::QxrdWindow::m_DisplayImage);
  m_Plot -> prop_MaskShown() -> linkTo(Ui::QxrdWindow::m_DisplayMask);
  m_Plot -> prop_InterpolatePixels() -> linkTo(Ui::QxrdWindow::m_InterpolatePixels);
  m_Plot -> prop_MaintainAspectRatio() -> linkTo(Ui::QxrdWindow::m_MaintainAspectRatio);

  m_Plot -> setDataProcessor(m_DataProcessor);
  m_CenterFinderPlot -> setWindow(QxrdWindowPtr(this));
  m_IntegratorPlot -> setDataProcessor(m_DataProcessor);

//  connect(m_DataProcessor, SIGNAL(newDataAvailable(QxrdDoubleImageData *)),
//          m_Plot, SLOT(onProcessedImageAvailable(QxrdDoubleImageData *)));
//  connect(m_DataProcessor, SIGNAL(newMaskAvailable(QxrdDoubleImageData *, QxrdMaskData *)),
//          m_Plot, SLOT(onMaskedImageAvailable(QxrdDoubleImageData *, QxrdMaskData *)));
//
//  connect(m_DataProcessor, SIGNAL(newDarkImageAvailable(QxrdDoubleImageData *)),
//          m_Plot, SLOT(onDarkImageAvailable(QxrdDoubleImageData *)));

//  connect(m_DataProcessor, SIGNAL(newDataAvailable(QxrdDoubleImageData *)),
//          m_CenterFinderPlot, SLOT(onProcessedImageAvailable(QxrdDoubleImageData *)));

//  connect(m_DataProcessor, SIGNAL(newMaskAvailable(QxrdDoubleImageData *, QxrdMaskData *)),
//          m_CenterFinderPlot, SLOT(onMaskedImageAvailable(QxrdDoubleImageData *, QxrdMaskData *)));

  connect(m_Plot, SIGNAL(printMessage(QString)), this, SLOT(printMessage(QString)));
  connect(m_CenterFinderPlot, SIGNAL(printMessage(QString)), this, SLOT(printMessage(QString)));
  connect(m_IntegratorPlot, SIGNAL(printMessage(QString)), this, SLOT(printMessage(QString)));
  connect(m_Plot, SIGNAL(statusMessage(QString)), this, SLOT(statusMessage(QString)));
  connect(m_CenterFinderPlot, SIGNAL(statusMessage(QString)), this, SLOT(statusMessage(QString)));
  connect(m_IntegratorPlot, SIGNAL(statusMessage(QString)), this, SLOT(statusMessage(QString)));
  connect(m_Plot, SIGNAL(criticalMessage(QString)), this, SLOT(criticalMessage(QString)));
  connect(m_CenterFinderPlot, SIGNAL(criticalMessage(QString)), this, SLOT(criticalMessage(QString)));
  connect(m_IntegratorPlot, SIGNAL(criticalMessage(QString)), this, SLOT(criticalMessage(QString)));

//  connect(m_DataProcessor, SIGNAL(printMessage(QString)), this, SLOT(printMessage(QString)));

//  readSettings();

  connect(m_DataProcessor -> centerFinder() -> prop_CenterX(), SIGNAL(changedValue(double)),
          m_Plot, SLOT(onCenterXChanged(double)));

  connect(m_DataProcessor -> centerFinder() -> prop_CenterY(), SIGNAL(changedValue(double)),
          m_Plot, SLOT(onCenterYChanged(double)));

  connect(m_DataProcessor -> centerFinder() -> prop_CenterX(), SIGNAL(changedValue(double)),
          m_CenterFinderPlot, SLOT(onCenterXChanged(double)));

  connect(m_DataProcessor -> centerFinder() -> prop_CenterY(), SIGNAL(changedValue(double)),
          m_CenterFinderPlot, SLOT(onCenterYChanged(double)));

  connect(m_DataProcessor, SIGNAL(newIntegrationAvailable(QxrdIntegratedDataPtr)),
          m_IntegratorPlot, SLOT(onNewIntegrationAvailable(QxrdIntegratedDataPtr)));

  connect(m_Allocator -> prop_Allocated(), SIGNAL(changedValue(int)), this, SLOT(allocatedMemoryChanged()));
  connect(m_Allocator -> prop_Max(), SIGNAL(changedValue(int)), this, SLOT(allocatedMemoryChanged()));

  m_WindowsMenu -> addAction(m_AcquireDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CorrectionDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_MaskingDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_DisplayDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CenteringDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_IntegratorDockWidget -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_FileBrowserDockWidget -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_PowderFitWidget -> toggleViewAction());

  m_Messages -> document() -> setMaximumBlockCount(20000);

#ifdef QT_NO_DEBUG
  m_ActionRefineCenterTilt->setEnabled(false);
#endif

  m_UpdateTimer.start(1500);
}

QxrdWindow::~QxrdWindow()
{
  QCEP_DEBUG(DEBUG_WINDOW,
             printf("QxrdWindow::~QxrdWindow\n");
  );
//  delete m_Data;
//  delete m_NewData;
//  delete m_Mask;
//  delete m_NewMask;
}

void QxrdWindow::enableTiltRefinement(bool enable)
{
  m_ActionRefineCenterTilt->setEnabled(enable);
}

void QxrdWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
}

void QxrdWindow::possiblyClose()
{
//   printf("QxrdWindow::possiblyClose()\n");
  if (wantToClose()) {
    close();
  }
}

bool QxrdWindow::wantToClose()
{
  THREAD_CHECK;

  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window?"),
                                  QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

QString QxrdWindow::timeStamp()
{
  return QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ");
}

void QxrdWindow::printMessage(QString msg)
{
//  printf("print message %s\n", qPrintable(msg));

  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "printMessage", Qt::QueuedConnection, Q_ARG(QString, msg)));
  } else {
    QString message = timeStamp()+msg.trimmed();

    //int msgSize = m_Messages->document()->characterCount();
    //int blkCount = m_Messages->document()->blockCount();

    //printf("msgSize = %d, blkCount=%d\n", msgSize, blkCount);

    m_Messages -> append(message);
    m_DataProcessor -> logMessage(message);
  }
}

void QxrdWindow::criticalMessage(QString msg)
{
  static int dialogCount = 0;

  printf("critical message %s, count = %d\n", qPrintable(msg), dialogCount);

  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "criticalMessage", Qt::QueuedConnection, Q_ARG(QString, msg)));
  } else {
    statusMessage(msg);

    dialogCount++;
    if (dialogCount <= 1) {
      QMessageBox::critical(this, "Error", msg);
    }
    dialogCount--;
  }
}

void QxrdWindow::acquisitionReady()
{
//  readSettings();
  THREAD_CHECK;

  m_AcquireDialog -> m_AcquireButton -> setEnabled(true);
  m_AcquireDialog -> m_TriggerButton -> setEnabled(false);
  m_AcquireDialog -> m_CancelButton -> setEnabled(false);
  m_ActionAcquire -> setEnabled(true);
  m_ActionCancel -> setEnabled(false);
  m_AcquireDialog -> m_DarkAcquireButton -> setEnabled(true);
//  m_AcquireDialog -> m_DarkCancelButton -> setEnabled(false);
  m_ActionAcquireDark -> setEnabled(true);
  m_ActionCancelDark -> setEnabled(false);
  m_Progress -> reset();
}

void QxrdWindow::acquisitionStarted()
{
  THREAD_CHECK;

  if (m_Acquisition -> get_PreTriggerFiles() > 0) {
    m_AcquireDialog -> m_TriggerButton -> setEnabled(true);
  } else {
    m_AcquireDialog -> m_TriggerButton -> setEnabled(false);
  }

  m_AcquireDialog -> m_AcquireButton -> setEnabled(false);
  m_AcquireDialog -> m_CancelButton -> setEnabled(true);
  m_ActionAcquire -> setEnabled(false);
  m_ActionCancel -> setEnabled(true);
  m_AcquireDialog -> m_DarkAcquireButton -> setEnabled(false);
//  m_AcquireDialog -> m_DarkCancelButton -> setEnabled(false);
  m_ActionAcquireDark -> setEnabled(false);
  m_ActionCancelDark -> setEnabled(false);
  m_Progress -> setValue(0);
}

void QxrdWindow::darkAcquisitionStarted()
{
  THREAD_CHECK;

  m_AcquireDialog -> m_AcquireButton -> setEnabled(false);
  m_AcquireDialog -> m_TriggerButton -> setEnabled(false);
  m_AcquireDialog -> m_CancelButton -> setEnabled(/*false*/ true);
  m_ActionAcquire -> setEnabled(false);
  m_ActionCancel -> setEnabled(false);
  m_AcquireDialog -> m_DarkAcquireButton -> setEnabled(false);
//  m_AcquireDialog -> m_DarkCancelButton -> setEnabled(true);
  m_ActionAcquireDark -> setEnabled(false);
  m_ActionCancelDark -> setEnabled(true);
  m_Progress -> setValue(0);
}

void QxrdWindow::acquisitionFinished()
{
  THREAD_CHECK;

  m_AcquireDialog -> m_AcquireButton -> setEnabled(true);
  m_AcquireDialog -> m_TriggerButton -> setEnabled(false);
  m_AcquireDialog -> m_CancelButton -> setEnabled(false);
  m_ActionAcquire -> setEnabled(true);
  m_ActionCancel -> setEnabled(false);
  m_AcquireDialog -> m_DarkAcquireButton -> setEnabled(true);
//  m_AcquireDialog -> m_DarkCancelButton -> setEnabled(false);
  m_ActionAcquireDark -> setEnabled(true);
  m_ActionCancelDark -> setEnabled(false);
  m_Progress -> reset();
}

void QxrdWindow::selectOutputDirectory()
{
  THREAD_CHECK;

  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory",
                                                  m_DataProcessor -> get_OutputDirectory());
  if (dir.length()) {
    m_DataProcessor -> set_OutputDirectory(dir);
  }
}

void QxrdWindow::onAcquireStarted(int dark)
{
  if (dark) {
    darkAcquisitionStarted();
  } else {
    acquisitionStarted();
  }

  m_Acquiring = true;
}

void QxrdWindow::onAcquiredFrame(
    QString /*fileName*/, int /*fileIndex*/, int isum, int nsum, int iframe, int nframe)
{
//   printf("QxrdWindow::acquiredFrame(\"%s\",%d,%d,%d,%d,%d)\n",
// 	 qPrintable(fileName), fileIndex, isum, nsum, iframe, nframe);

  int totalframes = (nsum*nframe <= 0 ? 1 : nsum*nframe);
  int thisframe = iframe*nsum+isum+1;

  //  printf("%d %% progress\n", thisframe*100/totalframes);

  statusMessage(tr("Exposure %1 of %2, File %3 of %4")
                .arg(isum+1).arg(nsum).arg(iframe+1).arg(nframe));

  m_Progress -> setValue(thisframe*100/totalframes);
}

void QxrdWindow::onAcquireComplete(int /*dark*/)
{
  acquisitionFinished();

  m_Acquiring = false;
}

void QxrdWindow::doAcquire()
{
//  printMessage("QxrdWindow::doAcquire()");

  acquisitionStarted();

  m_Acquiring = true;

  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "acquire"));
}

void QxrdWindow::doAcquireDark()
{
  darkAcquisitionStarted();

  m_AcquiringDark = true;

  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "acquireDark"));
}

void QxrdWindow::doCancel()
{
  if (m_Acquiring) {
    m_Acquisition -> cancel();
//    INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "cancel"));
  }
}

void QxrdWindow::doCancelDark()
{
  if (m_AcquiringDark) {
    m_Acquisition -> cancelDark();
//    INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "cancelDark"));
  }
}

void QxrdWindow::readSettings(QxrdSettings &settings, QString section)
{
  m_Plot         -> readSettings(settings, section+"/plot");
//  m_FileBrowser  -> readSettings(settings, section+"/fileBrowser");

  m_SettingsLoaded = true;

  QByteArray geometry = settings.value(section+"-geometry").toByteArray();
  QByteArray winstate = settings.value(section+"-state").toByteArray();

  restoreGeometry(geometry);
  restoreState(winstate,1);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdWindow::writeSettings(QxrdSettings &settings, QString section)
{
  m_Plot         -> writeSettings(settings, section+"/plot");
//  m_FileBrowser  -> writeSettings(settings, section+"/fileBrowser");

  settings.setValue(section+"-geometry", saveGeometry());
  settings.setValue(section+"-state", saveState(1));

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdWindow::statusMessage(QString msg)
{
  m_StatusMsg -> setText(msg);

  printMessage(msg);

  m_StatusTimer.start(5000);
}

void QxrdWindow::clearStatusMessage()
{
  m_StatusMsg -> setText("");
}

void QxrdWindow::newDataAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_NewDataMutex);

//  image -> copyImage(m_NewData);
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_NewData = image;
  m_NewOverflow = overflow;

  m_NewDataAvailable.fetchAndAddOrdered(1);

//  INVOKE_CHECK(QMetaObject::invokeMethod(this, "newData", Qt::QueuedConnection));
}

void QxrdWindow::newMaskAvailable(QxrdMaskDataPtr mask)
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_NewMaskMutex);

//  mask -> copyMask(m_NewMask);
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_NewMask = mask;
  m_NewMaskAvailable.fetchAndAddOrdered(1);

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "newMask", Qt::QueuedConnection));
}

void QxrdWindow::newData()
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_NewDataMutex);

  if (m_NewDataAvailable.fetchAndStoreOrdered(0)) {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//    QxrdDoubleImageDataPtr tmp = m_Data;
    m_Data = m_NewData;
    m_NewData = QxrdDoubleImageDataPtr(NULL);

    m_Overflow = m_NewOverflow;
    m_NewOverflow = QxrdMaskDataPtr(NULL);

    m_Plot             -> onProcessedImageAvailable(m_Data, m_Overflow);
    m_CenterFinderPlot -> onProcessedImageAvailable(m_Data);
  }
}

void QxrdWindow::newMask()
{
//  QxrdMutexLocker lock(__FILE__, __LINE__, &m_NewMaskMutex);

  if (m_NewMaskAvailable.fetchAndAddOrdered(0)) {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//    QxrdMaskDataPtr tmp = m_Mask;
    m_Mask = m_NewMask;
    m_NewMask = QxrdMaskDataPtr(NULL);
    m_NewMaskAvailable.fetchAndStoreOrdered(0);

    m_Plot             -> onMaskedImageAvailable(m_Data, m_Mask);
    m_CenterFinderPlot -> onMaskedImageAvailable(m_Data, m_Mask);
  }
}

//void QxrdWindow::spareData()
//{
//  bool canDo;
//
//  {
//    QxrdMutexLocker lock(__FILE__, __LINE__, &m_NewDataMutex);
//
//    canDo = m_Plotting == 0;
//
//    if (canDo) {
//      emit printMessage("QxrdWindow swap data & spare");
//
//      QxrdDoubleImageData *tmp = m_Data;
//      m_Data = m_SpareData;
//      m_SpareData = tmp;
//    }
//  }
//
//  if (canDo) {
//    newData();
//  }
//
//  emit printMessage("QxrdWindow::spareData");
//}
//
void QxrdWindow::doSaveData()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Data in", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->saveData(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadData()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Data from...", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->loadData(theFile);
  }
}

void QxrdWindow::doSaveDark()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Dark Data in", m_DataProcessor -> get_DarkImagePath());

  if (theFile.length()) {
    m_DataProcessor->saveDark(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadDark()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Dark Data from...", m_DataProcessor -> get_DarkImagePath());

  if (theFile.length()) {
    m_DataProcessor->loadDark(theFile);
  }
}

void QxrdWindow::doSaveMask()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Mask in", m_DataProcessor -> get_MaskPath());

  if (theFile.length()) {
    m_DataProcessor->saveMask(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadMask()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Mask from...", m_DataProcessor -> get_MaskPath());

  if (theFile.length()) {
    m_DataProcessor->loadMask(theFile);
  }
}

void QxrdWindow::doSaveBadPixels()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Bad Pixels in", m_DataProcessor -> get_BadPixelsPath());

  if (theFile.length()) {
    m_DataProcessor->saveBadPixels(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadBadPixels()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Bad Pixel Map from...", m_DataProcessor -> get_BadPixelsPath());

  if (theFile.length()) {
    m_DataProcessor->loadBadPixels(theFile);
  }
}

void QxrdWindow::doSaveGainMap()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Gain Map in", m_DataProcessor -> get_GainMapPath());

  if (theFile.length()) {
    m_DataProcessor->saveGainMap(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadGainMap()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Pixel Gain Map from...", m_DataProcessor -> get_GainMapPath());

  if (theFile.length()) {
    m_DataProcessor->loadGainMap(theFile);
  }
}

void QxrdWindow::selectLogFile()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Log File in", m_DataProcessor -> get_LogFilePath());

  if (theFile.length()) {
    m_DataProcessor->newLogFile(theFile);
  }
}

void QxrdWindow::doTest()
{

//  m_Data -> setCircularMask();
}

void QxrdWindow::executeScript()
{
  emit executeCommand(m_ScriptEdit -> toPlainText());

  m_CancelScriptButton  -> setEnabled(true);
  m_ActionCancelScript  -> setEnabled(true);
  m_ExecuteScriptButton -> setEnabled(false);
  m_ActionExecuteScript -> setEnabled(false);
}

void QxrdWindow::cancelScript()
{
  m_ScriptEngine -> cancelCommand();
}

void QxrdWindow::finishedCommand(QScriptValue result)
{
  if (result.isError()) {
    int line = result.property("lineNumber").toInteger();

    m_Messages -> append(tr("Error in line %1").arg(line));
    m_Messages -> append(result.property("message").toString());
  } else if (!result.isUndefined()){
    m_Messages -> append(result.toString());
  }

  m_CancelScriptButton  -> setEnabled(false);
  m_ActionCancelScript  -> setEnabled(false);
  m_ExecuteScriptButton -> setEnabled(true);
  m_ActionExecuteScript -> setEnabled(true);
}

QxrdScriptEnginePtr QxrdWindow::scriptEngine() const
{
  return m_ScriptEngine;
}

void QxrdWindow::setScriptEngine(QxrdScriptEnginePtr engine)
{
  m_ScriptEngine = engine;
}

QxrdDataProcessorPtr QxrdWindow::dataProcessor() const
{
  return m_DataProcessor;
}

QxrdAcquisitionPtr QxrdWindow::acquisition() const
{
  return m_Acquisition;
}

QxrdDoubleImageDataPtr QxrdWindow::data()
{
  return m_Data;
}

QxrdMaskDataPtr QxrdWindow::mask()
{
  return m_Mask;
}

void QxrdWindow::doAboutQxrd()
{
  QMessageBox::about(this, "QXRD", "QXRD Data Acquisition for PE Area Detectors\nVersion " QXRD_VERSION);
}

void QxrdWindow::doOpenQXRDWebPage()
{
  QDesktopServices::openUrl(QUrl("http://cep.xor.aps.anl.gov/software/qxrd/index.html"));
}

void QxrdWindow::allocatedMemoryChanged()
{
  int alloc = m_Allocator -> get_Allocated();
  int maxalloc = m_Allocator -> get_Max();

  m_AllocationStatus -> setMaximum(maxalloc);
  m_AllocationStatus -> setValue(alloc);
}

void QxrdWindow::doRefineCenterTilt()
{
  m_PowderFitDialog = QxrdPowderFitDialogPtr(new QxrdPowderFitDialog(m_DataProcessor, this));
  m_PowderFitDialog -> exec();
}

/******************************************************************
*
*  $Log: qxrdwindow.cpp,v $
*  Revision 1.5  2010/10/22 21:44:26  jennings
*  *** empty log message ***
*
*  Revision 1.4  2010/10/21 19:44:03  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.3  2010/10/15 20:56:31  jennings
*  Added extra locking when updating image display
*
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.12  2010/09/11 21:44:04  jennings
*  Work on powder ring fitting
*
*  Revision 1.1.2.11  2010/09/10 21:09:52  jennings
*  Tilt and powder fitting modifications
*
*  Revision 1.1.2.10  2010/09/09 19:57:31  jennings
*  Moved acquisition dialog into separate class
*  Reorganised acquisition dialog widgets to be more compact
*
*  Revision 1.1.2.9  2010/09/09 16:45:00  jennings
*  Added separate parameters for percentile display mode, changed display control
*  to only show relevent parameters
*
*  Revision 1.1.2.8  2010/09/08 19:40:57  jennings
*  Added tilt controls to center finder
*  Disabled fit refine command (for now)
*  Added percentile display mode
*
*  Revision 1.1.2.7  2010/08/17 19:20:51  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.6  2010/08/11 19:27:44  jennings
*  Rearranged source tree
*
*  Revision 1.1.2.5  2010/08/10 20:41:56  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*  Revision 1.1.2.4  2010/08/09 21:53:58  jennings
*  Made center and fit refiner into a modal dialog, partial implementation
*
*  Revision 1.1.2.3  2010/08/06 21:09:35  jennings
*  Initial partial implementation of powder ring fitting widget
*
*  Revision 1.1.2.2  2010/07/28 19:29:45  jennings
*  Modified script output for void function results - no output in window
*  Added 'ellipse' command to processor
*  Implemented QxrdGenerateTestImage.generateImage
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.115.2.31  2010/07/21 21:32:18  jennings
*  Improved error message display from file saving routines
*
*  Revision 1.115.2.30  2010/07/21 20:13:15  jennings
*  Improved calculation and display of estimated save times
*
*  Revision 1.115.2.29  2010/07/21 19:40:23  jennings
*  Removed extra processing time displays
*
*  Revision 1.115.2.28  2010/07/20 20:59:58  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.115.2.27  2010/07/20 20:30:25  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.115.2.26  2010/06/23 21:39:34  jennings
*  Removed some unnecessary locking. Reduced frequency of display updates.
*
*  Revision 1.115.2.25  2010/06/17 19:19:59  jennings
*  Added 'save dark' and 'skip at start' options
*
*  Revision 1.115.2.24  2010/06/17 18:37:13  jennings
*  Added dropped frames counter to display window
*
*  Revision 1.115.2.23  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.115.2.22  2010/05/25 22:03:41  jennings
*  Moved all printMessage etc. signal connection into QxrdApplication
*
*  Revision 1.115.2.21  2010/05/24 21:02:39  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.115.2.20  2010/05/21 20:01:25  jennings
*  Added processor type label to UI
*
*  Revision 1.115.2.19  2010/05/19 21:15:35  jennings
*  PE acquisition debugging
*
*  Revision 1.115.2.18  2010/05/19 20:30:20  jennings
*  *** empty log message ***
*
*  Revision 1.115.2.17  2010/05/18 22:47:01  jennings
*  More perkin elmer readout fixes
*
*  Revision 1.115.2.16  2010/05/17 15:59:53  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.115.2.15  2010/05/06 18:52:02  jennings
*  Initialize m_NewData and m_NewMask to NULL
*
*  Revision 1.115.2.14  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.115.2.13  2010/05/02 08:12:07  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.115.2.12  2010/04/27 19:37:51  jennings
*  Explicitly delete some data no longer protected by QSharedPointer s
*
*  Revision 1.115.2.11  2010/04/26 23:46:14  jennings
*  *** empty log message ***
*
*  Revision 1.115.2.10  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.115.2.9  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.115.2.8  2010/04/21 21:45:03  jennings
*  Changed mask values to shorts instead of bools.  Added ROI and histogram calculation tests.
*
*  Revision 1.115.2.7  2010/04/21 20:49:47  jennings
*  Added detector type label
*
*  Revision 1.115.2.6  2010/04/21 19:51:40  jennings
*  Added preferences command, removed benchmarks menu and commands
*
*  Revision 1.115.2.5  2010/04/20 21:20:36  jennings
*  Added script support for skippedExposures parameter
*
*  Revision 1.115.2.4  2010/04/19 21:48:27  jennings
*  More rearrangement
*
*  Revision 1.115.2.3  2010/04/19 19:20:16  jennings
*  More reorganization of acquisition code
*
*  Revision 1.115.2.2  2010/04/18 17:20:48  jennings
*  Further updates
*
*  Revision 1.115.2.1  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.115  2010/04/09 22:29:38  jennings
*  Removed file browser, added CMake support, build under VC
*
*  Revision 1.114  2010/03/05 22:32:03  jennings
*  Version 0.3.9 adds text file output and conversion
*
*  Revision 1.113  2010/03/02 22:15:31  jennings
*  Removed some debug output, files for windows installer
*
*  Revision 1.112  2009/12/11 17:50:24  jennings
*  Disable calculator window
*  Set messages window limit to 20000 lines
*
*  Revision 1.111  2009/12/03 22:31:51  jennings
*  Set upper limit on number of lines in 'messages' box - set to 100 lines for testing
*
*  Revision 1.110  2009/11/23 23:37:27  jennings
*  Corrected URL to qxrd website
*
*  Revision 1.109  2009/11/17 20:43:00  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.108  2009/11/13 20:15:58  jennings
*  *** empty log message ***
*
*  Revision 1.107  2009/11/03 19:57:56  jennings
*  Added help menu with an about dialog and a link to the documentation web site
*
*  Revision 1.106  2009/10/23 19:42:30  jennings
*  Add file browser entry to windows menu
*
*  Revision 1.105  2009/10/22 21:50:58  jennings
*  More code for file browser, basic operation now works
*
*  Revision 1.104  2009/10/22 19:22:28  jennings
*  Initial file browser
*
*  Revision 1.103  2009/10/02 18:51:43  jennings
*  Display estimated times of individual processing steps
*
*  Revision 1.102  2009/10/01 21:43:09  jennings
*  Added time stamp to log file output
*
*  Revision 1.101  2009/09/30 16:50:29  jennings
*  Make acquisition interruptable when it was started from the command line
*
*  Revision 1.100  2009/09/25 22:42:48  jennings
*  Masking changes
*
*  Revision 1.99  2009/09/25 14:22:16  jennings
*  Simplified double-buffering for plotted data - there is now a separate copy of data and mask
*  in QxrdWindow
*
*  Revision 1.98  2009/09/22 18:19:00  jennings
*  Added slicing routines
*  Set title for traces in avg data graph
*
*  Revision 1.97  2009/09/21 19:51:11  jennings
*  Added call to statusMessage to criticalMessage and call printMessage from statusMessage
*
*  Revision 1.96  2009/09/21 19:40:46  jennings
*  Added version number to window title, added more measurement output
*
*  Revision 1.95  2009/09/21 18:49:49  jennings
*  Display msecs in log file timestamps
*
*  Revision 1.94  2009/09/21 16:27:58  jennings
*  Added user interface to log file path
*
*  Revision 1.93  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.92  2009/09/14 19:07:55  jennings
*  Added menu commands to show/hide toolbox widgets
*
*  Revision 1.91  2009/09/13 13:59:48  jennings
*  Added 'canOverwrite' argument to data saving routines and arrange
*  that saves via file dialogs can overwrite, programmatic saves use
*  unique file names
*
*  Revision 1.90  2009/09/10 21:33:30  jennings
*  Added TIFF error handling
*
*  Revision 1.89  2009/09/07 22:10:14  jennings
*  Allow NULL mask
*
*  Revision 1.88  2009/09/04 20:04:31  jennings
*  Debugging pre-trigger acquisition
*
*  Revision 1.87  2009/09/04 15:15:43  jennings
*  Added log file routines
*  Removed newlines from any printMessage calls.
*
*  Revision 1.86  2009/09/04 12:46:35  jennings
*  Added binning mode parameter
*  Added camera gain and binning mode user interfaces
*
*  Revision 1.85  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.84  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.83  2009/08/27 17:04:16  jennings
*  Added load/save commands for dark and mask
*
*  Revision 1.82  2009/08/25 18:49:19  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.81  2009/08/09 18:00:00  jennings
*  Added graph clearing button to integrator dialog
*
*  Revision 1.80  2009/08/09 15:40:32  jennings
*  Added measurer tool to all graphs
*
*  Revision 1.79  2009/08/04 20:42:53  jennings
*  Simple, initial, implementation of integration
*
*  Revision 1.78  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.77  2009/08/03 13:26:25  jennings
*  Added option to set/clear mask pixels
*
*  Revision 1.76  2009/08/02 18:02:42  jennings
*  Added a number of masking operations to the UI - no actual implementation yet
*
*  Revision 1.75  2009/07/25 15:18:19  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*
*  Revision 1.74  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.73  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.72  2009/07/20 00:35:23  jennings
*  Trying to optimise screen redraws
*
*  Revision 1.71  2009/07/17 20:41:20  jennings
*  Modifications related to mask display
*
*  Revision 1.70  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.69  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.68  2009/07/16 21:26:25  jennings
*  Made various image display variables into properties
*
*  Revision 1.67  2009/07/16 20:10:43  jennings
*  Made various image display variables into properties
*
*  Revision 1.66  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.65  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.64  2009/07/07 22:04:55  jennings
*  Save window size and position in settings
*
*  Revision 1.63  2009/06/30 21:36:17  jennings
*  Modified user interface to use tool box widgets
*
*  Revision 1.62  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.61  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.60  2009/06/28 04:04:52  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.59  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.58  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

