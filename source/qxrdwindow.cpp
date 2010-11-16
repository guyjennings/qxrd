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
#include "qxrdimagedisplaywidget.h"

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
    m_ImageDisplay(NULL)
{
  setupUi(this);

  if (sizeof(void*) == 4) {
    setWindowTitle(windowTitle()+" - 32 bit - v"+QXRD_VERSION);
  } else {
    setWindowTitle(windowTitle()+" - 64 bit - v"+QXRD_VERSION);
  }

  m_AcquireDialog = m_Acquisition -> controlPanel(this);
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

  connect(m_ActionAcquire, SIGNAL(triggered()), this, SLOT(doAcquire()));
  connect(m_ActionCancel, SIGNAL(triggered()), this, SLOT(doCancel()));
  connect(m_ActionAcquireDark, SIGNAL(triggered()), this, SLOT(doAcquireDark()));
  connect(m_ActionCancelDark, SIGNAL(triggered()), this, SLOT(doCancelDark()));
  connect(m_ActionTrigger, SIGNAL(triggered()), m_Acquisition, SLOT(trigger()));

  connect(m_ActionShowImage, SIGNAL(triggered()), m_Plot, SLOT(toggleShowImage()));
  connect(m_ActionShowMask, SIGNAL(triggered()), m_Plot, SLOT(toggleShowMask()));
  connect(m_ActionShowOverflow, SIGNAL(triggered()), m_Plot, SLOT(toggleShowOverflow()));
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

//  connect(m_SaveDarkOptions, SIGNAL(clicked()), this, SLOT(doProcessorOptionsDialog()));

  m_Plot->prop_XMouse()->linkTo(m_XMouse);
  m_Plot->prop_YMouse()->linkTo(m_YMouse);
  m_Plot->prop_ValMouse()->linkTo(m_ValMouse);
  m_Plot->prop_TTHMouse()->linkTo(m_TTHMouse);

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
    emit criticalMessage(QDateTime::currentDateTime(), "Oh no, QxrdWindow::m_Acquisition == NULL");
  }

  connect(m_Acquisition,     SIGNAL(acquireStarted(int)),
          this,              SLOT(onAcquireStarted(int)));
  connect(m_Acquisition,     SIGNAL(acquiredFrame(QString,int,int,int,int,int)),
          this,              SLOT(onAcquiredFrame(QString,int,int,int,int,int)));
  connect(m_Acquisition,     SIGNAL(acquireComplete(int)),
          this,              SLOT(onAcquireComplete(int)));


  m_Acquisition -> prop_OverflowLevel() -> linkTo(m_OverflowLevel);

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

//  m_DataProcessor -> prop_DarkImagePath() -> linkTo(m_DarkImagePath);
//  m_DataProcessor -> prop_BadPixelsPath() -> linkTo(m_BadPixelsPath);
//  m_DataProcessor -> prop_GainMapPath() -> linkTo(m_GainMapPath);

  m_DataProcessor -> prop_MaskMinimumValue() -> linkTo(Ui::QxrdWindow::m_MaskMinimum);
  m_DataProcessor -> prop_MaskMaximumValue() -> linkTo(Ui::QxrdWindow::m_MaskMaximum);
  m_DataProcessor -> prop_MaskCircleRadius() -> linkTo(Ui::QxrdWindow::m_MaskCircleRadius);
  m_DataProcessor -> prop_MaskSetPixels() -> linkTo(Ui::QxrdWindow::m_MaskSetPixels);

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
  m_Plot -> prop_OverflowShown() -> linkTo(Ui::QxrdWindow::m_DisplayOverflow);
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

  connect(m_Plot, SIGNAL(printMessage(QDateTime,QString)), this, SLOT(printMessage(QDateTime,QString)));
  connect(m_CenterFinderPlot, SIGNAL(printMessage(QDateTime,QString)), this, SLOT(printMessage(QDateTime,QString)));
  connect(m_IntegratorPlot, SIGNAL(printMessage(QDateTime,QString)), this, SLOT(printMessage(QDateTime,QString)));
  connect(m_Plot, SIGNAL(statusMessage(QDateTime,QString)), this, SLOT(statusMessage(QDateTime,QString)));
  connect(m_CenterFinderPlot, SIGNAL(statusMessage(QDateTime,QString)), this, SLOT(statusMessage(QDateTime,QString)));
  connect(m_IntegratorPlot, SIGNAL(statusMessage(QDateTime,QString)), this, SLOT(statusMessage(QDateTime,QString)));
  connect(m_Plot, SIGNAL(criticalMessage(QDateTime,QString)), this, SLOT(criticalMessage(QDateTime,QString)));
  connect(m_CenterFinderPlot, SIGNAL(criticalMessage(QDateTime,QString)), this, SLOT(criticalMessage(QDateTime,QString)));
  connect(m_IntegratorPlot, SIGNAL(criticalMessage(QDateTime,QString)), this, SLOT(criticalMessage(QDateTime,QString)));

//  connect(m_DataProcessor, SIGNAL(printMessage(QDateTime,QString)), this, SLOT(printMessage(QDateTime,QString)));

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

//#ifndef QT_NO_DEBUG
//  m_ImageDisplay = QxrdImageDisplayWidget::insertNew(app, m_XRDTabWidget);
//#endif

  m_UpdateTimer.start(100);
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

void QxrdWindow::printMessage(QDateTime ts, QString msg)
{
//  printf("print message %s\n", qPrintable(msg));

  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "printMessage", Qt::QueuedConnection,
                                           Q_ARG(QDateTime, ts), Q_ARG(QString, msg)));
  } else {
    QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+msg.trimmed();

    //int msgSize = m_Messages->document()->characterCount();
    //int blkCount = m_Messages->document()->blockCount();

    //printf("msgSize = %d, blkCount=%d\n", msgSize, blkCount);

    m_Messages -> append(message);
    m_DataProcessor -> logMessage(message);
  }
}

void QxrdWindow::criticalMessage(QDateTime ts, QString msg)
{
  static int dialogCount = 0;

  printf("critical message %s, count = %d\n", qPrintable(msg), dialogCount);

  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "criticalMessage", Qt::QueuedConnection,
                                           Q_ARG(QDateTime, ts), Q_ARG(QString, msg)));
  } else {
    statusMessage(ts, msg);

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

  m_Acquisition -> acquisitionReady();

  m_Progress -> reset();
}

void QxrdWindow::acquisitionStarted()
{
  THREAD_CHECK;

  m_Acquisition -> acquisitionStarted();

  m_Progress -> setValue(0);
}

void QxrdWindow::darkAcquisitionStarted()
{
  THREAD_CHECK;

  m_Acquisition -> darkAcquisitionStarted();

  m_Progress -> setValue(0);
}

void QxrdWindow::acquisitionFinished()
{
  THREAD_CHECK;

  m_Acquisition -> acquisitionFinished();

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
    QString fileName, int fileIndex, int isum, int nsum, int iframe, int nframe)
{
//   printf("QxrdWindow::acquiredFrame(\"%s\",%d,%d,%d,%d,%d)\n",
// 	 qPrintable(fileName), fileIndex, isum, nsum, iframe, nframe);

  int totalframes = (nsum*nframe <= 0 ? 1 : nsum*nframe);
  int thisframe = iframe*nsum+isum+1;

  //  printf("%d %% progress\n", thisframe*100/totalframes);

  statusMessage(QDateTime::currentDateTime(),
                tr("%1: Exposure %2 of %3, File %4 of %5")
                .arg(fileName)
                .arg(isum+1).arg(nsum).arg(iframe).arg(nframe));

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

void QxrdWindow::statusMessage(QDateTime ts, QString msg)
{
  m_StatusMsg -> setText(msg);

//  printMessage(msg);

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

    if (m_ImageDisplay) {
      m_ImageDisplay -> updateImage(m_Data, m_Overflow);
    }
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

    if (m_ImageDisplay) {
      m_ImageDisplay->updateImage(QxrdDoubleImageDataPtr(), QxrdMaskDataPtr(), m_Mask);
    }
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

void QxrdWindow::doProcessorOptionsDialog()
{
  m_DataProcessor->processorOptionsDialog();
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
  QString about = "QXRD Data Acquisition for PE Area Detectors\nVersion " QXRD_VERSION;

  if (sizeof(void*) == 4) {
    about += " - 32 Bit";
  } else {
    about += " - 64 Bit";
  }

#ifdef Q_CC_MSVC
  about += " MSVC";
#endif

#ifdef Q_CC_GNU
  about += " gcc";
#endif

#ifdef QT_NO_DEBUG
  about += " Release\n";
#else
  about += " Debug\n";
#endif

  about += tr("Qt Version %1").arg(qVersion());

  QMessageBox::about(this, "QXRD", about);
}

void QxrdWindow::doOpenQXRDWebPage()
{
  QDesktopServices::openUrl(QUrl("http://qxrd.sourceforge.net/"));
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
