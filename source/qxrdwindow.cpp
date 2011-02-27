#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquiredialog.h"
#include "qxrddisplaydialog.h"
#include "qxrdacquisitionthread.h"
#include "qxrdacquisition.h"
#include "qxrdsettings.h"
#include "qxrdimageplot.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddataprocessor.h"
#include "qxrdacquiredialog.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdmaskdialog.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"
#include "qxrdplotzoomer.h"
#include "qxrdscriptengine.h"
#include "qxrdfilebrowser.h"
#include "qxrdimagecalculator.h"
#include "qxrdmutexlocker.h"
#include "qxrdallocator.h"
#include "qxrdpowderfitdialog.h"
#include "qxrdimagedisplaywidget.h"

#include "qxrdtestdockwidget.h"

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
#include <QMenu>
#include <QDesktopWidget>

QxrdWindow::QxrdWindow(QxrdApplicationPtr app, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc, QxrdAllocatorPtr alloc, QWidget *parent)
  : QMainWindow(parent),
    m_Mutex(QMutex::Recursive),
    m_SettingsLoaded(false),
    m_Application(app),
    m_Acquisition(acq),
    m_DataProcessor(proc),
    m_Allocator(alloc),
    m_AcquireDialog(NULL),
    m_SynchronizedAcquisitionDialog(NULL),
    m_DisplayDialog(NULL),
    m_CenterFinderDialog(NULL),
    m_MaskDialog(NULL),
    m_CorrectionDialog(NULL),
    m_IntegratorDialog(NULL),
    m_Calculator(NULL),
    m_InputFileBrowser(NULL),
    m_OutputFileBrowser(NULL),
    m_SliceDialog(NULL),
    m_HistogramDialog(NULL),
    m_ImageInfoDialog(NULL),
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

//  m_AcquireDialog = m_Acquisition -> controlPanel(this);
  m_AcquireDialog      = new QxrdAcquireDialog(this, m_Acquisition, m_DataProcessor, this);
  m_SynchronizedAcquisitionDialog = new QxrdSynchronizedAcquisitionDialog(this, m_Acquisition);
  m_DisplayDialog      = new QxrdDisplayDialog(this);
  m_CenterFinderDialog = new QxrdCenterFinderDialog(m_DataProcessor -> centerFinder());
  m_MaskDialog         = new QxrdMaskDialog(this, m_DataProcessor);
  m_CorrectionDialog   = new QxrdCorrectionDialog(this, m_Acquisition, m_DataProcessor);
  m_IntegratorDialog   = new QxrdIntegratorDialog(m_DataProcessor -> integrator());
  m_InputFileBrowser   = new QxrdInputFileBrowser(m_DataProcessor, this);
  m_OutputFileBrowser  = new QxrdOutputFileBrowser(m_DataProcessor, this);
  m_SliceDialog        = new QxrdSliceDialog(this);
  m_HistogramDialog    = new QxrdHistogramDialog(this);
  m_ImageInfoDialog    = new QxrdInfoDialog(this);

  QDesktopWidget *dw = QApplication::desktop();
//  int screenNum = dw->screenNumber(this);
  QRect screenGeom = dw->screenGeometry(this);

//  printf("Screen number %d Geom: %d,%d-%d,%d\n", screenNum,
//         screenGeom.left(), screenGeom.top(),
//         screenGeom.right(), screenGeom.bottom());

  if (screenGeom.height() >= 1024) {
    addDockWidget(Qt::RightDockWidgetArea, m_AcquireDialog);

    splitDockWidget(m_AcquireDialog, m_CenterFinderDialog, Qt::Vertical);
    splitDockWidget(m_CenterFinderDialog, m_IntegratorDialog, Qt::Vertical);

    tabifyDockWidget(m_AcquireDialog, m_SynchronizedAcquisitionDialog);
    tabifyDockWidget(m_SynchronizedAcquisitionDialog, m_DisplayDialog);
    tabifyDockWidget(m_DisplayDialog, m_InputFileBrowser);
    tabifyDockWidget(m_InputFileBrowser, m_SliceDialog);
    tabifyDockWidget(m_SliceDialog, m_ImageInfoDialog);

    tabifyDockWidget(m_CenterFinderDialog, m_MaskDialog);
    tabifyDockWidget(m_MaskDialog, m_CorrectionDialog);
    tabifyDockWidget(m_CorrectionDialog, m_OutputFileBrowser);
    tabifyDockWidget(m_OutputFileBrowser, m_HistogramDialog);
} else {
    addDockWidget(Qt::RightDockWidgetArea, m_AcquireDialog);

    tabifyDockWidget(m_AcquireDialog, m_CenterFinderDialog);
    tabifyDockWidget(m_CenterFinderDialog, m_IntegratorDialog);

    tabifyDockWidget(m_IntegratorDialog, m_SynchronizedAcquisitionDialog);
    tabifyDockWidget(m_SynchronizedAcquisitionDialog, m_DisplayDialog);

    tabifyDockWidget(m_DisplayDialog, m_InputFileBrowser);

    tabifyDockWidget(m_InputFileBrowser, m_MaskDialog);
    tabifyDockWidget(m_MaskDialog, m_CorrectionDialog);
    tabifyDockWidget(m_CorrectionDialog, m_OutputFileBrowser);

    tabifyDockWidget(m_OutputFileBrowser, m_SliceDialog);
    tabifyDockWidget(m_SliceDialog, m_HistogramDialog);
    tabifyDockWidget(m_HistogramDialog, m_ImageInfoDialog);

    if (screenGeom.height() < 800) {
      shrinkDockWidget(m_AcquireDialog);
      shrinkDockWidget(m_CenterFinderDialog);
      shrinkDockWidget(m_IntegratorDialog);
      shrinkDockWidget(m_SynchronizedAcquisitionDialog);
      shrinkDockWidget(m_DisplayDialog);
      shrinkDockWidget(m_MaskDialog);
      shrinkDockWidget(m_InputFileBrowser);
      shrinkDockWidget(m_OutputFileBrowser);
      shrinkDockWidget(m_CorrectionDialog);
      shrinkDockWidget(m_SliceDialog);
      shrinkDockWidget(m_HistogramDialog);
      shrinkDockWidget(m_ImageInfoDialog);
    }
  }
//  tabifyDockWidget(m_IntegratorDialog, new QxrdTestDockWidget(this));

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

  connect(m_ActionAccumulateImages, SIGNAL(triggered()), this, SLOT(doAccumulateImages()));
  connect(m_ActionIntegrate, SIGNAL(triggered()), this, SLOT(doIntegrateSequence()));
  connect(m_ActionProcessData, SIGNAL(triggered()), this, SLOT(doProcessSequence()));

  connect(m_DisplayDialog -> m_AutoRange, SIGNAL(clicked()), m_ActionAutoRange, SIGNAL(triggered()));
  connect(m_DisplayDialog -> m_Display_5pct, SIGNAL(clicked()), m_Action005Range, SIGNAL(triggered()));
  connect(m_DisplayDialog -> m_Display_10pct, SIGNAL(clicked()), m_Action010Range, SIGNAL(triggered()));
  connect(m_DisplayDialog -> m_Display_100pct, SIGNAL(clicked()), m_Action100Range, SIGNAL(triggered()));

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

  connect(m_ActionAcquire, SIGNAL(triggered()), this, SLOT(doAcquire()));
  connect(m_ActionCancel, SIGNAL(triggered()), this, SLOT(doCancel()));
  connect(m_ActionAcquireDark, SIGNAL(triggered()), this, SLOT(doAcquireDark()));
  connect(m_ActionCancelDark, SIGNAL(triggered()), this, SLOT(doCancelDark()));
//  connect(m_ActionTrigger, SIGNAL(triggered()), m_Acquisition, SLOT(trigger()));

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
          this,              SLOT(acquireStarted(int)));
  connect(m_Acquisition,     SIGNAL(acquiredFrame(QString,int,int,int,int,int,int,int)),
          this,              SLOT(acquiredFrame(QString,int,int,int,int,int,int,int)));
  connect(m_Acquisition,     SIGNAL(acquireComplete(int)),
          this,              SLOT(acquireComplete(int)));


  m_Acquisition -> prop_OverflowLevel() -> linkTo(m_DisplayDialog->m_OverflowLevel);

  m_DataProcessor -> prop_PerformDarkSubtraction() -> linkTo(m_CorrectionDialog->m_PerformDark);
  m_DataProcessor -> prop_PerformDarkSubtractionTime() -> linkTo(m_CorrectionDialog->m_PerformDarkTime);
  m_DataProcessor -> prop_SaveRawImages() -> linkTo(m_CorrectionDialog->m_SaveRaw);
  m_DataProcessor -> prop_SaveDarkImages() -> linkTo(m_CorrectionDialog->m_SaveDark);
  m_Acquisition   -> prop_RawSaveTime() -> linkTo(m_CorrectionDialog->m_SaveRawTime);
  m_Acquisition   -> prop_DarkSaveTime() -> linkTo(m_CorrectionDialog->m_SaveDarkTime);
  m_DataProcessor -> prop_PerformBadPixels() -> linkTo(m_CorrectionDialog->m_PerformBadPixels);
  m_DataProcessor -> prop_PerformBadPixelsTime() -> linkTo(m_CorrectionDialog->m_PerformBadPixelsTime);
  m_DataProcessor -> prop_PerformGainCorrection() -> linkTo(m_CorrectionDialog->m_PerformGainCorrection);
  m_DataProcessor -> prop_PerformGainCorrectionTime() -> linkTo(m_CorrectionDialog->m_PerformGainCorrectionTime);
  m_DataProcessor -> prop_SaveSubtracted() -> linkTo(m_CorrectionDialog->m_SaveSubtracted);
  m_DataProcessor -> prop_SaveSubtractedTime() -> linkTo(m_CorrectionDialog->m_SaveSubtractedTime);
  m_DataProcessor -> prop_SaveAsText() -> linkTo(m_CorrectionDialog->m_SaveAsText);
  m_DataProcessor -> prop_SaveAsTextTime() -> linkTo(m_CorrectionDialog->m_SaveAsTextTime);
  m_DataProcessor -> prop_PerformIntegration() -> linkTo(m_CorrectionDialog->m_PerformIntegration);
  m_DataProcessor -> prop_PerformIntegrationTime() -> linkTo(m_CorrectionDialog->m_PerformIntegrationTime);
  m_DataProcessor -> prop_DisplayIntegratedData() -> linkTo(m_CorrectionDialog->m_DisplayIntegratedData);
  m_DataProcessor -> prop_DisplayIntegratedDataTime() -> linkTo(m_CorrectionDialog->m_DisplayIntegratedDataTime);
  m_DataProcessor -> prop_SaveIntegratedData() -> linkTo(m_CorrectionDialog->m_SaveIntegratedData);
  m_DataProcessor -> prop_SaveIntegratedDataTime() -> linkTo(m_CorrectionDialog->m_SaveIntegratedDataTime);
  m_DataProcessor -> prop_SaveIntegratedInSeparateFiles() -> linkTo(m_CorrectionDialog->m_SaveIntegratedInSeparateFiles);
  m_DataProcessor -> prop_EstimatedProcessingTime() -> linkTo(m_CorrectionDialog->m_EstimatedProcessingTime);

  m_Plot -> prop_DisplayMinimumPct() -> linkTo(m_DisplayDialog->m_DisplayMinimumPct);
  m_Plot -> prop_DisplayMaximumPct() -> linkTo(m_DisplayDialog->m_DisplayMaximumPct);
  m_Plot -> prop_DisplayMinimumVal() -> linkTo(m_DisplayDialog->m_DisplayMinimumVal);
  m_Plot -> prop_DisplayMaximumVal() -> linkTo(m_DisplayDialog->m_DisplayMaximumVal);
  m_Plot -> prop_DisplayMinimumPctle() -> linkTo(m_DisplayDialog->m_DisplayMinimumPctle);
  m_Plot -> prop_DisplayMaximumPctle() -> linkTo(m_DisplayDialog->m_DisplayMaximumPctle);

  m_Plot -> prop_DisplayScalingMode() -> linkTo(m_DisplayDialog->m_DisplayScalingMode);

  connect(m_Plot -> prop_DisplayScalingMode(), SIGNAL(changedValue(int)), m_DisplayDialog->m_DisplayParmsStack, SLOT(setCurrentIndex(int)));

  m_Plot -> prop_DisplayColorMap() -> linkTo(m_DisplayDialog->m_DisplayColorMap);

  m_Plot -> prop_ImageShown() -> linkTo(m_DisplayDialog->m_DisplayImage);
  m_Plot -> prop_MaskShown() -> linkTo(m_DisplayDialog->m_DisplayMask);
  m_Plot -> prop_OverflowShown() -> linkTo(m_DisplayDialog->m_DisplayOverflow);
  m_Plot -> prop_InterpolatePixels() -> linkTo(m_DisplayDialog->m_InterpolatePixels);
  m_Plot -> prop_MaintainAspectRatio() -> linkTo(m_DisplayDialog->m_MaintainAspectRatio);

  m_Plot -> setDataProcessor(m_DataProcessor);

  m_CenterFinderPlot -> setWindow(QxrdWindowPtr(this));
  m_IntegratorPlot -> setDataProcessor(m_DataProcessor);

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

  m_WindowsMenu -> addAction(m_AcquireDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_InputFileBrowser -> toggleViewAction());
  m_WindowsMenu -> addAction(m_OutputFileBrowser -> toggleViewAction());
  m_WindowsMenu -> addAction(m_SynchronizedAcquisitionDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_DisplayDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CenterFinderDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_MaskDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CorrectionDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_IntegratorDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_SliceDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_HistogramDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_ImageInfoDialog -> toggleViewAction());

  //  if (m_Acquisition->get_DetectorType() != 1) { // No file browser for PE detector...
  //    m_FileBrowser = new QxrdFileBrowser(m_DataProcessor);
  //    m_FileBrowserDockWidget -> setWidget(m_FileBrowser);

  //    m_WindowsMenu -> addAction(m_FileBrowserDockWidget -> toggleViewAction());
  //  }

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

void QxrdWindow::shrinkDockWidget(QDockWidget *dw)
{
  shrinkWidget(dw);
}

void QxrdWindow::shrinkWidget(QWidget *wid)
{
  QFont f = wid->font();
  if (f.pointSize() > 6) f.setPointSize(6);
  wid->setFont(f);
//  wid->setContentsMargins(1,1,1,1);

  QGridLayout *gl = qobject_cast<QGridLayout*>(wid);

  if (gl) {
    gl->setHorizontalSpacing(1);
    gl->setVerticalSpacing(1);
  }

  foreach(QObject* obj, wid->children()) {
    QWidget *wid = qobject_cast<QWidget*>(obj);

    if (wid) {
      shrinkWidget(wid);
    }
  }
}

void QxrdWindow::onAcquisitionInit()
{
  m_AcquireDialog->onAcquisitionInit();
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

//void QxrdWindow::acquisitionReady()
//{
//  //  readSettings();
//  THREAD_CHECK;

//  m_Acquisition -> acquisitionReady();

//  m_Progress -> reset();
//}

//void QxrdWindow::acquisitionStarted()
//{
//  THREAD_CHECK;

//  m_Acquisition -> acquisitionStarted();

//  m_Progress -> setValue(0);
//}

//void QxrdWindow::darkAcquisitionStarted()
//{
//  THREAD_CHECK;

//  m_Acquisition -> darkAcquisitionStarted();

//  m_Progress -> setValue(0);
//}

void QxrdWindow::selectOutputDirectory()
{
  THREAD_CHECK;

  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory",
                                                  m_DataProcessor -> get_OutputDirectory());
  if (dir.length()) {
    m_DataProcessor -> set_OutputDirectory(dir);
  }
}

void QxrdWindow::acquireStarted(int dark)
{
}

void QxrdWindow::acquireComplete(int dark)
{
  THREAD_CHECK;

  m_Progress -> reset();
}

void QxrdWindow::acquiredFrame(
  QString fileName, int /*fileIndex*/, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup)
{
  //   printf("QxrdWindow::acquiredFrame(\"%s\",%d,%d,%d,%d,%d)\n",
  // 	 qPrintable(fileName), fileIndex, isum, nsum, iframe, nframe);

  int totalframes = (nsum*nframe*ngroup <= 0 ? 1 : nsum*nframe*ngroup);
  int thisframe = igroup*nframe*nsum + iframe*nsum + isum + 1;

  //  printf("%d %% progress\n", thisframe*100/totalframes);

  statusMessage(QDateTime::currentDateTime(),
                tr("%1: Exposure %2 of %3, File %4 of %5")
                .arg(fileName)
                .arg(isum+1).arg(nsum).arg(iframe).arg(nframe));

  m_Progress -> setValue(thisframe*100/totalframes);
}

//void QxrdWindow::onAcquireComplete(int /*dark*/)
//{
//  acquisitionFinished();

//  m_Acquiring = false;
//}

void QxrdWindow::doAcquire()
{
  //  printMessage("QxrdWindow::doAcquire()");
//  acquisitionStarted();

  m_Acquiring = true;

  INVOKE_CHECK(QMetaObject::invokeMethod(m_Acquisition, "acquire"));
}

void QxrdWindow::doAcquireDark()
{
//  darkAcquisitionStarted();

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

void QxrdWindow::doSynchronizedAcquisition()
{
}

void QxrdWindow::readSettings(QxrdSettings &settings, QString section)
{
  m_Plot             -> readSettings(settings, section+"/plot");
  m_CenterFinderPlot -> readSettings(settings, section+"/centerFinderPlot");
  m_IntegratorPlot   -> readSettings(settings, section+"/integratorPlot");

  if (m_InputFileBrowser) {
    m_InputFileBrowser  -> readSettings(settings, section+"/inputFileBrowser");
  }

  if (m_OutputFileBrowser) {
    m_OutputFileBrowser  -> readSettings(settings, section+"/outputFileBrowser");
  }

  if (m_HistogramDialog) {
    m_HistogramDialog -> readSettings(settings, section+"/histogramDialog");
  }

  if (m_SliceDialog) {
    m_SliceDialog -> readSettings(settings, section+"/sliceDialog");
  }

  if (m_ImageInfoDialog) {
    m_ImageInfoDialog ->  readSettings(settings, section+"/imageInfoDialog");
  }

  m_SettingsLoaded = true;

  QByteArray geometry = settings.value(section+"-geometry").toByteArray();
  QByteArray winstate = settings.value(section+"-state").toByteArray();

  restoreGeometry(geometry);
  restoreState(winstate,1);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdWindow::writeSettings(QxrdSettings &settings, QString section)
{
  m_Plot             -> writeSettings(settings, section+"/plot");
  m_CenterFinderPlot -> writeSettings(settings, section+"/centerFinderPlot");
  m_IntegratorPlot   -> writeSettings(settings, section+"/integratorPlot");

  if (m_InputFileBrowser) {
    m_InputFileBrowser  -> writeSettings(settings, section+"/inputFileBrowser");
  }

  if (m_OutputFileBrowser) {
    m_OutputFileBrowser  -> writeSettings(settings, section+"/outputFileBrowser");
  }

  if (m_HistogramDialog) {
    m_HistogramDialog -> writeSettings(settings, section+"/histogramDialog");
  }

  if (m_SliceDialog) {
    m_SliceDialog -> writeSettings(settings, section+"/sliceDialog");
  }

  if (m_ImageInfoDialog) {
    m_ImageInfoDialog ->  writeSettings(settings, section+"/imageInfoDialog");
  }

  settings.setValue(section+"-geometry", saveGeometry());
  settings.setValue(section+"-state", saveState(1));

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdWindow::statusMessage(QDateTime /*ts*/, QString msg)
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

  //  mask -> copyMaskTo(m_NewMask);
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

    if (m_SliceDialog) {
      m_SliceDialog -> onProcessedImageAvailable(m_Data, m_Overflow);
    }

    if (m_HistogramDialog) {
      m_HistogramDialog -> onProcessedImageAvailable(m_Data, m_Overflow);
    }

    if (m_ImageInfoDialog) {
      m_ImageInfoDialog -> onProcessedImageAvailable(m_Data, m_Overflow);
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
        this, "Save Dark Data in", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->saveDark(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadDark()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Load Dark Data from...", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->loadDark(theFile);
  }
}

void QxrdWindow::doSaveMask()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Save Mask in", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->saveMask(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadMask()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Load Mask from...", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->loadMask(theFile);
  }
}

void QxrdWindow::doSaveBadPixels()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Save Bad Pixels in", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->saveBadPixels(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadBadPixels()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Load Bad Pixel Map from...", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->loadBadPixels(theFile);
  }
}

void QxrdWindow::doSaveGainMap()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Save Gain Map in", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->saveGainMap(theFile, QxrdDataProcessor::CanOverwrite);
  }
}

void QxrdWindow::doLoadGainMap()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Load Pixel Gain Map from...", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->loadGainMap(theFile);
  }
}

void QxrdWindow::selectLogFile()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Save Log File in", m_DataProcessor -> get_DataPath());

  if (theFile.length()) {
    m_DataProcessor->newLogFile(theFile);
  }
}

//void QxrdWindow::doProcessorOptionsDialog()
//{
//  m_DataProcessor->processorOptionsDialog();
//}

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

void QxrdWindow::doAccumulateImages()
{
  QStringList files = QFileDialog::getOpenFileNames(this,
                                                    "Select data files to accumulate...",
                                                    m_DataProcessor -> get_DataPath());

  m_DataProcessor->accumulateImages(files);
}

void QxrdWindow::doIntegrateSequence()
{
  QStringList files = QFileDialog::getOpenFileNames(this,
                                                    "Select data files to integrate...",
                                                    m_DataProcessor -> get_DataPath());

  foreach (QString file, files) {
    m_DataProcessor->integrateData(file);
  }
}

void QxrdWindow::doProcessSequence()
{
  QStringList files = QFileDialog::getOpenFileNames(this,
                                                    "Select data files to process...",
                                                    m_DataProcessor -> get_DataPath());

  foreach (QString file, files) {
    m_DataProcessor->processData(file);
  }
}

