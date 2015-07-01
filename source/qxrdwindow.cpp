#include "qxrddebug.h"
#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdacquisitiondialog.h"
#include "qxrddisplaydialog.h"
#include "qxrdacquisitionthread.h"
#include "qxrdacquisition.h"
#include "qxrdimageplot.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddataprocessor.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder.h"
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
#include "qxrdimagedisplaywidget.h"
#include "qxrdsynchronizedacquisitiondialog.h"
#include "qxrdcorrectiondialog.h"
#include "qxrdslicedialog.h"
#include "qxrdhistogramdialog.h"
#include "qxrdinfodialog.h"
#include "qxrdscriptdialog.h"
#include "qxrdhighlighter.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdacquisitionextrainputsdialog.h"
#include "qxrddataset.h"

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
#include <QMenu>
#include <QDesktopWidget>

QxrdWindow::QxrdWindow(QxrdWindowSettingsWPtr settings,
                       QxrdApplicationWPtr appl,
                       QxrdExperimentWPtr docw,
                       QxrdAcquisitionWPtr acqw,
                       QxrdDataProcessorWPtr procw,
                       QxrdAllocatorWPtr allocw,
                       QWidget *parent)
  : QMainWindow(parent),
    m_ObjectNamer(this, "window"),
    m_Mutex(QMutex::Recursive),
    m_WindowSettings(settings),
    m_Application(appl),
    m_Experiment(docw),
    m_Acquisition(acqw),
    m_DataProcessor(procw),
    m_Allocator(allocw),
    m_AcquisitionDialog(NULL),
    m_AcquisitionExtraInputsDialog(NULL),
    m_SynchronizedAcquisitionDialog(NULL),
    m_DisplayDialog(NULL),
    m_CenterFinderDialog(NULL),
    m_MaskDialog(NULL),
    m_CorrectionDialog(NULL),
    m_IntegratorDialog(NULL),
    m_CalibrantDialog(NULL),
    m_DatasetBrowserDialog(NULL),
    m_Calculator(NULL),
    m_InputFileBrowser(NULL),
    m_OutputFileBrowser(NULL),
    m_SliceDialog(NULL),
    m_HistogramDialog(NULL),
    m_ImageInfoDialog(NULL),
    m_ScriptDialog(NULL),
    m_DistortionCorrectionDialog(NULL),
    m_Progress(NULL),
    m_AllocationStatus(NULL),
    m_Data(NULL),
    m_Overflow(NULL),
    m_NewData(NULL),
    m_NewOverflow(NULL),
    m_NewDataAvailable(false),
    m_Mask(NULL),
    m_NewMask(NULL),
    m_NewMaskAvailable(false),
    m_ImageDisplay(NULL),
    m_Highlighter(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdWindow::QxrdWindow(%p)\n", this);
  }

  QxrdApplicationPtr app(m_Application);

  if (app && qcepDebug(DEBUG_APP)) {
    app->printMessage("QxrdWindow::QxrdWindow");
  }
}

void QxrdWindow::initialize(QxrdWindowWPtr win)
{
  m_Window = win;

  setupUi(this);

  QxrdApplicationPtr app(m_Application);
  QxrdExperimentPtr expt(m_Experiment);
  QxrdWindowSettingsPtr set(m_WindowSettings);

  updateTitle();

  setWindowIcon(QIcon(":/images/qxrd-icon-64x64.png"));

  QxrdAcquisitionPtr acq(m_Acquisition);
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (acq) {
    m_AcquisitionDialog = acq -> controlPanel(m_Window);
    m_SynchronizedAcquisitionDialog = new QxrdSynchronizedAcquisitionDialog(set->synchronizedAcquisitionDialogSettings(), this, m_Acquisition);
    m_AcquisitionExtraInputsDialog = new QxrdAcquisitionExtraInputsDialog(set->acquisitionExtraInputsDialogSettings(), this, m_Acquisition);
  }

  m_DisplayDialog      = new QxrdDisplayDialog(this);

  if (proc) {
    m_CenterFinderDialog = new QxrdCenterFinderDialog(proc -> centerFinder());
    m_DistortionCorrectionDialog = new QxrdDistortionCorrectionDialog(proc->distortionCorrection(), m_Window);
  }

  if (proc) {
    m_MaskDialog       = new QxrdMaskDialog(m_DataProcessor, this);
  }

  if (acq) {
    m_CorrectionDialog   = new QxrdCorrectionDialog(this, m_Acquisition, m_DataProcessor);
  }

  if (proc) {
    m_IntegratorDialog   = new QxrdIntegratorDialog(proc -> integrator());
  }

  if (expt && proc) {
    QxrdCalibrantLibraryPtr cal(expt->calibrantLibrary());

    if (cal) {
      m_CalibrantDialog = new QxrdCalibrantDialog(cal, proc -> centerFinder(), this);
    }
  }

  if (expt) {
    QxrdDatasetPtr ds = expt->dataset();

    if (ds) {
      m_DatasetBrowserDialog = new QcepDatasetBrowserDialog(ds, this);
    }
  }

  if (expt && set) {
    m_InputFileBrowser   = new QxrdInputFileBrowser(set->inputFileBrowserSettings(), m_Experiment, m_DataProcessor, this);
    m_OutputFileBrowser  = new QxrdOutputFileBrowser(set->outputFileBrowserSettings(), m_Experiment, m_DataProcessor, this);
    m_ScriptDialog       = new QxrdScriptDialog(set->scriptDialogSettings(), m_Experiment, this);
  }

  if (set) {
    m_SliceDialog        = new QxrdSliceDialog(set->sliceDialogSettings(), this);
    m_HistogramDialog    = new QxrdHistogramDialog(set->histogramDialogSettings(), m_Experiment, this);
    m_ImageInfoDialog    = new QxrdInfoDialog(set->infoDialogSettings(), this);
  }

  if (set) {
    m_ImagePlot        -> init(set->imagePlotSettings());
    m_CenterFinderPlot -> init(set->centerFinderPlotSettings());
    m_IntegratorPlot   -> init(set->integratorPlotSettings());
    m_DistortionCorrectionDialog -> init(set->distortionCorrectionDialogSettings());
    m_DistortionCorrectionPlot -> init(set->distortionCorrectionPlotSettings());
  }

  QDesktopWidget *dw = QApplication::desktop();
  //  int screenNum = dw->screenNumber(this);
  QRect screenGeom = dw->screenGeometry(this);

  //  printf("Screen number %d Geom: %d,%d-%d,%d\n", screenNum,
  //         screenGeom.left(), screenGeom.top(),
  //         screenGeom.right(), screenGeom.bottom());

  if (m_AcquisitionDialog) {
    addDockWidget(Qt::RightDockWidgetArea, m_AcquisitionDialog);
  }

  if (m_InputFileBrowser) {
    addDockWidget(Qt::LeftDockWidgetArea, m_InputFileBrowser);
  }

  if (screenGeom.height() >= 1280) {
    splitDockWidget(m_AcquisitionDialog, m_CenterFinderDialog, Qt::Vertical);
    splitDockWidget(m_CenterFinderDialog, m_IntegratorDialog, Qt::Vertical);

    tabifyDockWidget(m_InputFileBrowser, m_DatasetBrowserDialog);

    tabifyDockWidget(m_AcquisitionDialog, m_AcquisitionExtraInputsDialog);
    tabifyDockWidget(m_AcquisitionExtraInputsDialog, m_SynchronizedAcquisitionDialog);
    tabifyDockWidget(m_SynchronizedAcquisitionDialog, m_DisplayDialog);
    tabifyDockWidget(m_DisplayDialog, m_SliceDialog);
    tabifyDockWidget(m_SliceDialog, m_ImageInfoDialog);
    tabifyDockWidget(m_ImageInfoDialog, m_ScriptDialog);

    tabifyDockWidget(m_CenterFinderDialog, m_MaskDialog);
    tabifyDockWidget(m_MaskDialog, m_CorrectionDialog);
    tabifyDockWidget(m_CorrectionDialog, m_CalibrantDialog);
    tabifyDockWidget(m_CalibrantDialog, m_OutputFileBrowser);
    tabifyDockWidget(m_OutputFileBrowser, m_HistogramDialog);
    tabifyDockWidget(m_HistogramDialog, m_DistortionCorrectionDialog);
  } else if (screenGeom.height() >= 1000) {
    splitDockWidget(m_AcquisitionDialog, m_CenterFinderDialog, Qt::Vertical);

    tabifyDockWidget(m_InputFileBrowser, m_DatasetBrowserDialog);

    tabifyDockWidget(m_AcquisitionDialog, m_AcquisitionExtraInputsDialog);
    tabifyDockWidget(m_AcquisitionExtraInputsDialog, m_SynchronizedAcquisitionDialog);
    tabifyDockWidget(m_SynchronizedAcquisitionDialog, m_DisplayDialog);
    tabifyDockWidget(m_DisplayDialog, m_SliceDialog);
    tabifyDockWidget(m_SliceDialog, m_ImageInfoDialog);
    tabifyDockWidget(m_ImageInfoDialog, m_ScriptDialog);

    tabifyDockWidget(m_CenterFinderDialog, m_MaskDialog);
    tabifyDockWidget(m_MaskDialog, m_CorrectionDialog);
    tabifyDockWidget(m_CorrectionDialog, m_CalibrantDialog);
    tabifyDockWidget(m_CalibrantDialog, m_OutputFileBrowser);
    tabifyDockWidget(m_OutputFileBrowser, m_HistogramDialog);
    tabifyDockWidget(m_HistogramDialog, m_IntegratorDialog);
    tabifyDockWidget(m_IntegratorDialog, m_DistortionCorrectionDialog);
  } else {
    tabifyDockWidget(m_AcquisitionDialog, m_AcquisitionExtraInputsDialog);
    tabifyDockWidget(m_AcquisitionExtraInputsDialog, m_SynchronizedAcquisitionDialog);
    tabifyDockWidget(m_CenterFinderDialog, m_IntegratorDialog);

    tabifyDockWidget(m_InputFileBrowser, m_DatasetBrowserDialog);

    tabifyDockWidget(m_IntegratorDialog, m_SynchronizedAcquisitionDialog);
    tabifyDockWidget(m_SynchronizedAcquisitionDialog, m_DisplayDialog);

    tabifyDockWidget(m_DisplayDialog, m_MaskDialog);
    tabifyDockWidget(m_MaskDialog, m_CorrectionDialog);
    tabifyDockWidget(m_CorrectionDialog, m_CalibrantDialog);
    tabifyDockWidget(m_CalibrantDialog, m_OutputFileBrowser);

    tabifyDockWidget(m_OutputFileBrowser, m_SliceDialog);
    tabifyDockWidget(m_SliceDialog, m_HistogramDialog);
    tabifyDockWidget(m_HistogramDialog, m_ImageInfoDialog);
    tabifyDockWidget(m_ImageInfoDialog, m_ScriptDialog);
    tabifyDockWidget(m_ScriptDialog, m_DistortionCorrectionDialog);

    if (screenGeom.height() < 1000) {
      //      shrinkObject(this);
      shrinkPanels(6,1);
    }
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

    connect(expt->prop_FontSize(), SIGNAL(valueChanged(int,int)), this, SLOT(setFontSize(int)));
    connect(expt->prop_Spacing(), SIGNAL(valueChanged(int,int)), this, SLOT(setSpacing(int)));
  }

  //  m_Calculator = new QxrdImageCalculator(m_DataProcessor);
  //  addDockWidget(Qt::RightDockWidgetArea, m_Calculator);

  connect(m_ExecuteScriptButton, SIGNAL(clicked()), m_ActionExecuteScript, SIGNAL(triggered()));
  connect(m_ActionExecuteScript, SIGNAL(triggered()), this, SLOT(executeScript()));
  connect(m_CancelScriptButton, SIGNAL(clicked()), m_ActionCancelScript, SIGNAL(triggered()));
  connect(m_ActionCancelScript, SIGNAL(triggered()), this, SLOT(cancelScript()));
  connect(m_LoadScriptButton, SIGNAL(clicked()), m_ActionLoadScript, SIGNAL(triggered()));
  connect(m_ActionLoadScript, SIGNAL(triggered()), this, SLOT(doLoadScript()));

  if (app) {
    connect(m_ActionAutoScale, SIGNAL(triggered()), m_ImagePlot, SLOT(autoScale()));
    connect(m_ActionQuit, SIGNAL(triggered()), app.data(), SLOT(possiblyQuit()));
    connect(m_ActionGlobalPreferences, SIGNAL(triggered()), app.data(), SLOT(editGlobalPreferences()));
    connect(m_ActionExperimentPreferences, SIGNAL(triggered()), this, SLOT(doEditPreferences()));
    connect(m_ActionLoadPreferences, SIGNAL(triggered()), app.data(), SLOT(doLoadPreferences()));
    connect(m_ActionSavePreferences, SIGNAL(triggered()), app.data(), SLOT(doSavePreferences()));

    connect(m_ActionNewExperiment, SIGNAL(triggered()), app.data(), SLOT(chooseNewExperiment()));
    connect(m_ActionOpenExperiment, SIGNAL(triggered()), app.data(), SLOT(chooseExistingExperiment()));
    connect(m_ActionCloseExperiment, SIGNAL(triggered()), this, SLOT(close()));
  }

  if (expt) {
    connect(m_ActionSaveExperiment, SIGNAL(triggered()), expt.data(), SLOT(saveExperiment()));
  }

  connect(m_ActionSaveExperimentAs, SIGNAL(triggered()), this, SLOT(saveExperimentAs()));
  connect(m_ActionSaveExperimentCopy, SIGNAL(triggered()), this, SLOT(saveExperimentCopy()));

  connect(m_ExperimentsMenu, SIGNAL(aboutToShow()), this, SLOT(populateExperimentsMenu()));
  setupRecentExperimentsMenu(m_ActionRecentExperiments);

  connect(m_ActionLoadData, SIGNAL(triggered()), this, SLOT(doLoadData()));
  connect(m_ActionSaveData, SIGNAL(triggered()), this, SLOT(doSaveData()));
  connect(m_ActionLoadDark, SIGNAL(triggered()), this, SLOT(doLoadDark()));
  connect(m_ActionSaveDark, SIGNAL(triggered()), this, SLOT(doSaveDark()));
  connect(m_ActionClearDark, SIGNAL(triggered()), this, SLOT(doClearDark()));
  connect(m_ActionLoadMask, SIGNAL(triggered()), this, SLOT(doLoadMask()));
  connect(m_ActionSaveMask, SIGNAL(triggered()), this, SLOT(doSaveMask()));
  connect(m_ActionClearMask, SIGNAL(triggered()), this, SLOT(doClearMask()));
  connect(m_ActionLoadGainMap, SIGNAL(triggered()), this, SLOT(doLoadGainMap()));
  connect(m_ActionSaveGainMap, SIGNAL(triggered()), this, SLOT(doSaveGainMap()));
  connect(m_ActionClearGainMap, SIGNAL(triggered()), this, SLOT(doClearGainMap()));

  connect(m_ActionPrintImage, SIGNAL(triggered()), m_ImagePlot, SLOT(printGraph()));

  //  connect(m_ActionSelectLogFile, SIGNAL(triggered()), this, SLOT(selectLogFile()));
  //  connect(m_ActionSetAcquireDirectory, SIGNAL(triggered()), this, SLOT(selectOutputDirectory()));

  connect(m_ActionAccumulateImages, SIGNAL(triggered()), this, SLOT(doAccumulateImages()));
  connect(m_ActionAddImage, SIGNAL(triggered()), this, SLOT(doAddImages()));
  connect(m_ActionSubtractImage, SIGNAL(triggered()), this, SLOT(doSubtractImages()));
  connect(m_ActionProjectImagesX, SIGNAL(triggered()), this, SLOT(doProjectAlongX()));
  connect(m_ActionProjectImagesY, SIGNAL(triggered()), this, SLOT(doProjectAlongY()));
  connect(m_ActionProjectImagesZ, SIGNAL(triggered()), this, SLOT(doProjectAlongZ()));
  connect(m_ActionCorrelateImage, SIGNAL(triggered()), this, SLOT(doCorrelate()));
  connect(m_ActionProcessData, SIGNAL(triggered()), this, SLOT(doProcessSequence()));

  connect(m_ActionReflectHorizontally, SIGNAL(triggered()), this, SLOT(doReflectHorizontally()));
  connect(m_ActionReflectVertically, SIGNAL(triggered()), this, SLOT(doReflectVertically()));

  connect(m_DisplayDialog -> m_AutoRange, SIGNAL(clicked()), m_ActionAutoRange, SIGNAL(triggered()));
  connect(m_DisplayDialog -> m_Display_5pct, SIGNAL(clicked()), m_Action005Range, SIGNAL(triggered()));
  connect(m_DisplayDialog -> m_Display_10pct, SIGNAL(clicked()), m_Action010Range, SIGNAL(triggered()));
  connect(m_DisplayDialog -> m_Display_100pct, SIGNAL(clicked()), m_Action100Range, SIGNAL(triggered()));

  connect(m_Action005Range, SIGNAL(triggered()), m_ImagePlot, SLOT(set005Range()));
  connect(m_Action010Range, SIGNAL(triggered()), m_ImagePlot, SLOT(set010Range()));
  connect(m_Action100Range, SIGNAL(triggered()), m_ImagePlot, SLOT(set100Range()));
  connect(m_ActionAutoRange, SIGNAL(triggered()), m_ImagePlot, SLOT(setAutoRange()));

  connect(m_ActionGrayscale, SIGNAL(triggered()), m_ImagePlot, SLOT(setGrayscale()));
  connect(m_ActionInverseGrayscale, SIGNAL(triggered()), m_ImagePlot, SLOT(setInverseGrayscale()));
  connect(m_ActionEarthTones, SIGNAL(triggered()), m_ImagePlot, SLOT(setEarthTones()));
  connect(m_ActionSpectrum, SIGNAL(triggered()), m_ImagePlot, SLOT(setSpectrum()));
  connect(m_ActionFire, SIGNAL(triggered()), m_ImagePlot, SLOT(setFire()));
  connect(m_ActionIce, SIGNAL(triggered()), m_ImagePlot, SLOT(setIce()));

  connect(m_ActionRefineCenterTilt, SIGNAL(triggered()), this, SLOT(doRefineCenterTilt()));
  connect(m_ActionMoveCenterUp, SIGNAL(triggered()), m_CenterFinderDialog, SLOT(centerMoveUp()));
  connect(m_ActionMoveCenterDown, SIGNAL(triggered()), m_CenterFinderDialog, SLOT(centerMoveDown()));
  connect(m_ActionMoveCenterLeft, SIGNAL(triggered()), m_CenterFinderDialog, SLOT(centerMoveLeft()));
  connect(m_ActionMoveCenterRight, SIGNAL(triggered()), m_CenterFinderDialog, SLOT(centerMoveRight()));

  if (proc) {
    QxrdCenterFinderPtr cf(proc->centerFinder());

    if (cf) {
      connect(m_ActionFindBeamCenter, SIGNAL(triggered()), cf.data(), SLOT(fitPowderCircle()), Qt::DirectConnection);
      connect(m_ActionClearMarkers, SIGNAL(triggered()), cf.data(), SLOT(deletePowderPoints()), Qt::DirectConnection);
      connect(m_ActionCalculateCalibrationPowder, SIGNAL(triggered()), cf.data(), SLOT(calculateCalibration()));
    }
  }

  connect(m_ActionPlotPowderRingPoints, SIGNAL(triggered()), this, SLOT(plotPowderRingRadii()));
  connect(m_ActionPlotPowderRingTwoTheta, SIGNAL(triggered()), this, SLOT(plotPowderRingTwoTheta()));
  connect(m_ActionPlotPowderRingCenters, SIGNAL(triggered()), this, SLOT(plotPowderRingCenters()));

  m_AcquisitionDialog->setupAcquireMenu(m_AcquireMenu);

  m_AcquisitionDialog->acquisitionReady();

  connect(m_ActionShowImage, SIGNAL(triggered()), m_ImagePlot, SLOT(toggleShowImage()));
  connect(m_ActionShowMask, SIGNAL(triggered()), m_ImagePlot, SLOT(toggleShowMask()));
  connect(m_ActionShowOverflow, SIGNAL(triggered()), m_ImagePlot, SLOT(toggleShowOverflow()));

  if (proc) {
    connect(m_ActionShowMaskRange, SIGNAL(triggered()), proc.data(), SLOT(showMaskRange()));
    connect(m_ActionHideMaskRange, SIGNAL(triggered()), proc.data(), SLOT(hideMaskRange()));
    connect(m_ActionShowMaskAll, SIGNAL(triggered()), proc.data(), SLOT(showMaskAll()));
    connect(m_ActionHideMaskAll, SIGNAL(triggered()), proc.data(), SLOT(hideMaskAll()));
    connect(m_ActionInvertMask, SIGNAL(triggered()), proc.data(), SLOT(invertMask()));
    connect(m_ActionMaskCircles, SIGNAL(triggered()), m_ImageMaskCirclesButton, SLOT(click()));
    connect(m_ActionMaskPolygons, SIGNAL(triggered()), m_ImageMaskPolygonsButton, SLOT(click()));

    connect(m_ActionROICalculate, SIGNAL(triggered()), proc.data(), SLOT(calculateROI()));
    connect(m_ActionHistogramCalculate, SIGNAL(triggered()), proc.data(), SLOT(calculateHistogram()));
  }

  connect(m_ActionTest, SIGNAL(triggered()), this, SLOT(doTest()));
  connect(m_ActionCrashProgram, SIGNAL(triggered()), this, SLOT(crashProgram()));

  connect(m_ImageZoomInButton, SIGNAL(clicked()), m_ImagePlot, SLOT(enableZooming()));
  connect(m_ImageZoomOutButton, SIGNAL(clicked()), m_ImagePlot, SLOT(zoomOut()));
  connect(m_ImageZoomAllButton, SIGNAL(clicked()), m_ImagePlot, SLOT(autoScale()));
  connect(m_ImageSetCenterButton, SIGNAL(clicked()), m_ImagePlot, SLOT(enableCentering()));
  connect(m_ImageSliceButton, SIGNAL(clicked()), m_ImagePlot, SLOT(enableSlicing()));
  connect(m_ImageMeasureButton, SIGNAL(clicked()), m_ImagePlot, SLOT(enableMeasuring()));
  connect(m_ImageHistogramButton, SIGNAL(clicked()), m_ImagePlot, SLOT(enableHistograms()));
  connect(m_ImageMaskCirclesButton, SIGNAL(clicked()), m_ImagePlot, SLOT(enableMaskCircles()));
  connect(m_ImageMaskPolygonsButton, SIGNAL(clicked()), m_ImagePlot, SLOT(enableMaskPolygons()));
  connect(m_ImagePowderPointsButton, SIGNAL(clicked()), m_ImagePlot, SLOT(enablePowderPoints()));

  connect(m_CenteringZoomInButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(enableZooming()));
  connect(m_CenteringZoomOutButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(zoomOut()));
  connect(m_CenteringZoomAllButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(autoScale()));
  connect(m_CenteringMeasureButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(enableMeasuring()));

  connect(m_IntegratorZoomInButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(enableZooming()));
  connect(m_IntegratorZoomOutButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(zoomOut()));
  connect(m_IntegratorZoomAllButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(autoScale()));
  connect(m_IntegratorMeasureButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(enableMeasuring()));

  connect(m_DistortionCorrectionZoomInButton, SIGNAL(clicked()), m_DistortionCorrectionPlot, SLOT(enableZooming()));
  connect(m_DistortionCorrectionZoomOutButton, SIGNAL(clicked()), m_DistortionCorrectionPlot, SLOT(zoomOut()));
  connect(m_DistortionCorrectionZoomAllButton, SIGNAL(clicked()), m_DistortionCorrectionPlot, SLOT(autoScale()));
  connect(m_DistortionCorrectionMeasureButton, SIGNAL(clicked()), m_DistortionCorrectionPlot, SLOT(enableMeasuring()));

  connect(m_DisplayDialog -> m_DisplayOptionsButton, SIGNAL(clicked()), this, SLOT(doEditPreferences()));
  connect(m_CorrectionDialog -> m_CorrectionOptionsButton, SIGNAL(clicked()), this, SLOT(doEditPreferences()));

  if (app) {
    connect(m_ActionAboutQXRD, SIGNAL(triggered()), app.data(), SLOT(doAboutQxrd()));
    connect(m_ActionOpenQXRDWebPage, SIGNAL(triggered()), app.data(), SLOT(doOpenQXRDWebPage()));
  }

  connect(m_HelpHomeButton, SIGNAL(clicked()), m_HelpBrowser, SLOT(home()));
  connect(m_HelpForwardButton, SIGNAL(clicked()), m_HelpBrowser, SLOT(forward()));
  connect(m_HelpBackButton, SIGNAL(clicked()), m_HelpBrowser, SLOT(backward()));

  connect(m_HelpBrowser, SIGNAL(forwardAvailable(bool)), m_HelpForwardButton, SLOT(setEnabled(bool)));
  connect(m_HelpBrowser, SIGNAL(backwardAvailable(bool)), m_HelpBackButton, SLOT(setEnabled(bool)));

  m_HelpBrowser->init(m_Experiment);

  connect(&m_UpdateTimer, SIGNAL(timeout()), this, SLOT(newData()));

  connect(m_ActionIntegrate, SIGNAL(triggered()), this, SLOT(doIntegrateSequence()));
  connect(m_ActionIntegrateCurrent, SIGNAL(triggered()),
          m_DataProcessor.data(), SLOT(integrateSaveAndDisplay()));
  connect(m_ActionIntegrateInputImages, SIGNAL(triggered()),
          m_InputFileBrowser, SLOT(doIntegrate()));

  connect(m_IntegratorDialog -> m_ClearGraphButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(clearGraph()));
  connect(m_IntegratorDialog -> m_ClearSelectedGraphButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(clearSelectedCurves()));
  connect(m_ActionClearIntegratedData, SIGNAL(triggered()), m_IntegratorPlot, SLOT(clearGraph()));
  connect(m_ActionClearSelectedIntegratedData, SIGNAL(triggered()), m_IntegratorPlot, SLOT(clearSelectedCurves()));

  connect(m_ActionSaveCachedGeometry, SIGNAL(triggered()), this, SLOT(doSaveCachedGeometry()));
  connect(m_ActionSaveCachedIntensity, SIGNAL(triggered()), this, SLOT(doSaveCachedIntensity()));

  connect(m_IntegratorDialog -> m_IntegrateOptionsButton, SIGNAL(clicked()), this, SLOT(doEditPreferences()));

  if (proc) {
    connect(proc->integrator()->prop_IntegrationXUnits(), SIGNAL(valueChanged(int,int)),
            this, SLOT(integrationXUnitsChanged(int)));
    integrationXUnitsChanged(proc->integrator()->get_IntegrationXUnits());

    connect(m_ActionIntegrateVsR,   SIGNAL(triggered()), proc->integrator().data(), SLOT(integrateVsR()));
    connect(m_ActionIntegrateVsQ,   SIGNAL(triggered()), proc->integrator().data(), SLOT(integrateVsQ()));
    connect(m_ActionIntegrateVsTTH, SIGNAL(triggered()), proc->integrator().data(), SLOT(integrateVsTTH()));
  }

  //  connect(m_SaveDarkOptions, SIGNAL(clicked()), this, SLOT(doProcessorOptionsDialog()));

  if (set) {
    QxrdImagePlotSettingsPtr ps(set->imagePlotSettings());

    if (ps) {
      ps->prop_XMouse()->linkTo(m_XMouse);
      ps->prop_YMouse()->linkTo(m_YMouse);
      ps->prop_ValMouse()->linkTo(m_ValMouse);
      ps->prop_TTHMouse()->linkTo(m_TTHMouse);
      ps->prop_QMouse()->linkTo(m_QMouse);
      ps->prop_RMouse()->linkTo(m_RMouse);
    }
  }

  m_StatusMsg = new QLabel(NULL);
  m_StatusMsg -> setMinimumWidth(200);
  m_StatusMsg -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_StatusMsg -> setToolTip(tr("Status Messages"));

  statusBar() -> addPermanentWidget(m_StatusMsg);

  m_Progress = new QProgressBar(NULL);
  m_Progress -> setMinimumWidth(150);
  m_Progress -> setMinimum(0);
  m_Progress -> setMaximum(100);
  m_Progress -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_Progress -> setToolTip(tr("Acquisition progress"));

  statusBar() -> addPermanentWidget(m_Progress);

  m_AllocationStatus = new QProgressBar(NULL);
  m_AllocationStatus -> setMinimumWidth(100);
  m_AllocationStatus -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_AllocationStatus -> setFormat("%v/%m");
  m_AllocationStatus -> setTextVisible(true);
  m_AllocationStatus -> setToolTip(tr("Memory usage"));

  statusBar() -> addPermanentWidget(m_AllocationStatus);

  if (app && m_Acquisition == NULL) {
    app->criticalMessage("Oh no, QxrdWindow::m_Acquisition == NULL");
  }

  connect(m_Acquisition.data(), SIGNAL(acquireStarted()),
          this,                 SLOT(acquireStarted()));
  connect(m_Acquisition.data(), SIGNAL(acquiredFrame(QString,int,int,int,int,int,int,int)),
          this,                 SLOT(acquiredFrame(QString,int,int,int,int,int,int,int)));
  connect(m_Acquisition.data(), SIGNAL(acquireComplete()),
          this,                 SLOT(acquireComplete()));

  if (acq) {
    acq -> prop_OverflowLevel() -> linkTo(m_DisplayDialog->m_OverflowLevel);
    acq -> prop_RawSaveTime() -> linkTo(m_CorrectionDialog->m_SaveRawTime);
    acq -> prop_DarkSaveTime() -> linkTo(m_CorrectionDialog->m_SaveDarkTime);

    connect(acq->prop_OverflowLevel(), SIGNAL(valueChanged(int,int)),
            m_HistogramDialog, SLOT(updateHistogramNeeded()));
  }

  if (expt) {
    expt -> prop_CompletionPercentage() -> linkTo(m_Progress);
  }

  if (proc) {
    proc -> prop_PerformDarkSubtraction() -> linkTo(m_CorrectionDialog->m_PerformDark);
    proc -> prop_PerformDarkSubtractionTime() -> linkTo(m_CorrectionDialog->m_PerformDarkTime);
    proc -> prop_SaveRawImages() -> linkTo(m_CorrectionDialog->m_SaveRaw);
    proc -> prop_SaveDarkImages() -> linkTo(m_CorrectionDialog->m_SaveDark);
    proc -> prop_PerformBadPixels() -> linkTo(m_CorrectionDialog->m_PerformBadPixels);
    proc -> prop_PerformBadPixelsTime() -> linkTo(m_CorrectionDialog->m_PerformBadPixelsTime);
    proc -> prop_PerformGainCorrection() -> linkTo(m_CorrectionDialog->m_PerformGainCorrection);
    proc -> prop_PerformGainCorrectionTime() -> linkTo(m_CorrectionDialog->m_PerformGainCorrectionTime);
    proc -> prop_SaveSubtracted() -> linkTo(m_CorrectionDialog->m_SaveSubtracted);
    proc -> prop_SaveSubtractedTime() -> linkTo(m_CorrectionDialog->m_SaveSubtractedTime);
    proc -> prop_SaveAsText() -> linkTo(m_CorrectionDialog->m_SaveAsText);
    proc -> prop_SaveAsTextTime() -> linkTo(m_CorrectionDialog->m_SaveAsTextTime);
    proc -> prop_PerformIntegration() -> linkTo(m_CorrectionDialog->m_PerformIntegration);
    proc -> prop_PerformIntegrationTime() -> linkTo(m_CorrectionDialog->m_PerformIntegrationTime);
    proc -> prop_DisplayIntegratedData() -> linkTo(m_CorrectionDialog->m_DisplayIntegratedData);
    proc -> prop_DisplayIntegratedDataTime() -> linkTo(m_CorrectionDialog->m_DisplayIntegratedDataTime);
    proc -> prop_SaveIntegratedData() -> linkTo(m_CorrectionDialog->m_SaveIntegratedData);
    proc -> prop_SaveIntegratedDataTime() -> linkTo(m_CorrectionDialog->m_SaveIntegratedDataTime);
    proc -> prop_SaveIntegratedInSeparateFiles() -> linkTo(m_CorrectionDialog->m_SaveIntegratedInSeparateFiles);
    proc -> prop_EstimatedProcessingTime() -> linkTo(m_CorrectionDialog->m_EstimatedProcessingTime);
  }

  if (set) {
    QxrdImagePlotSettingsPtr ps(set->imagePlotSettings());

    if (ps) {
      ps -> prop_DisplayMinimumPct() -> linkTo(m_DisplayDialog->m_DisplayMinimumPct);
      ps -> prop_DisplayMaximumPct() -> linkTo(m_DisplayDialog->m_DisplayMaximumPct);
      ps -> prop_DisplayMinimumVal() -> linkTo(m_DisplayDialog->m_DisplayMinimumVal);
      ps -> prop_DisplayMaximumVal() -> linkTo(m_DisplayDialog->m_DisplayMaximumVal);
      ps -> prop_DisplayMinimumPctle() -> linkTo(m_DisplayDialog->m_DisplayMinimumPctle);
      ps -> prop_DisplayMaximumPctle() -> linkTo(m_DisplayDialog->m_DisplayMaximumPctle);

      ps -> prop_DisplayLog() -> linkTo(m_DisplayDialog->m_DisplayImageLog);
      ps -> prop_DisplayScalingMode() -> linkTo(m_DisplayDialog->m_DisplayScalingMode);

      connect(ps -> prop_DisplayScalingMode(), SIGNAL(valueChanged(int,int)), m_DisplayDialog->m_DisplayParmsStack, SLOT(setCurrentIndex(int)));
      m_DisplayDialog->m_DisplayParmsStack->setCurrentIndex(ps->get_DisplayScalingMode());

      ps -> prop_DisplayColorMap() -> linkTo(m_DisplayDialog->m_DisplayColorMap);

      ps -> prop_ImageShown() -> linkTo(m_DisplayDialog->m_DisplayImage);
      ps -> prop_MaskShown() -> linkTo(m_DisplayDialog->m_DisplayMask);
      ps -> prop_OverflowShown() -> linkTo(m_DisplayDialog->m_DisplayOverflow);
      ps -> prop_InterpolatePixels() -> linkTo(m_DisplayDialog->m_InterpolatePixels);
      ps -> prop_MaintainAspectRatio() -> linkTo(m_DisplayDialog->m_MaintainAspectRatio);

    }
  }

  m_ImagePlot -> setProcessor(m_DataProcessor);
  m_DistortionCorrectionPlot -> setWindow(this);
  m_CenterFinderPlot -> setWindow(this);
  m_IntegratorPlot -> setDataProcessor(m_DataProcessor);

  if (proc) {
    connect(proc -> centerFinder() -> prop_CenterX(), SIGNAL(valueChanged(double,int)),
            m_ImagePlot, SLOT(onCenterXChanged(double)));

    connect(proc -> centerFinder() -> prop_CenterY(), SIGNAL(valueChanged(double,int)),
            m_ImagePlot, SLOT(onCenterYChanged(double)));

    connect(proc -> centerFinder() -> prop_CenterX(), SIGNAL(valueChanged(double,int)),
            m_CenterFinderPlot, SLOT(onCenterXChanged(double)));

    connect(proc -> centerFinder() -> prop_CenterY(), SIGNAL(valueChanged(double,int)),
            m_CenterFinderPlot, SLOT(onCenterYChanged(double)));

    connect(proc.data(), SIGNAL(newIntegrationAvailable(QxrdIntegratedDataPtr)),
            m_IntegratorPlot, SLOT(onNewIntegrationAvailable(QxrdIntegratedDataPtr)));
  }

  QxrdAllocatorPtr alloc(m_Allocator);

  if (alloc) {
    connect(alloc -> prop_Allocated(), SIGNAL(valueChanged(int,int)), this, SLOT(allocatedMemoryChanged()));
    connect(alloc -> prop_Max(), SIGNAL(valueChanged(int,int)), this, SLOT(allocatedMemoryChanged()));
  }

  m_WindowsMenu -> addAction(m_AcquisitionDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_AcquisitionExtraInputsDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_InputFileBrowser -> toggleViewAction());
  m_WindowsMenu -> addAction(m_OutputFileBrowser -> toggleViewAction());
  m_WindowsMenu -> addAction(m_SynchronizedAcquisitionDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_DisplayDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CenterFinderDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_DistortionCorrectionDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_MaskDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CalibrantDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_DatasetBrowserDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CorrectionDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_IntegratorDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_SliceDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_HistogramDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_ImageInfoDialog -> toggleViewAction());
  m_WindowsMenu -> addAction(m_ScriptDialog -> toggleViewAction());

  m_Highlighter = new QxrdHighlighter(m_ScriptEdit->document());

//  if (expt) {
//    expt->prop_DefaultScript()->linkTo(m_ScriptEdit);
//  }

  connect(m_ImagePlot, SIGNAL(slicePolygon(QVector<QPointF>)),
          m_SliceDialog, SLOT(slicePolygon(QVector<QPointF>)));

  connect(m_ImagePlot, SIGNAL(selectHistogram(QRectF)),
          m_HistogramDialog, SLOT(histogramSelectionChanged(QRectF)));

  if (app) {
    m_Messages -> document() -> setMaximumBlockCount(app->get_MessageWindowLines());

    connect(app->prop_MessageWindowLines(), SIGNAL(valueChanged(int,int)), this, SLOT(onMessageWindowLinesChanged(int)));
    connect(app->prop_UpdateIntervalMsec(), SIGNAL(valueChanged(int,int)), this, SLOT(onUpdateIntervalMsecChanged(int)));
  }

#ifdef QT_NO_DEBUG
  m_ActionRefineCenterTilt->setEnabled(false);
#endif

  if (expt && set) {
    if (!expt->get_DefaultLayout()) {
      QByteArray geometry = set->get_WindowGeometry();
      QByteArray winstate = set->get_WindowState();

      if (!restoreGeometry(geometry)) {
        printf("Restore geometry failed\n");
      }

      if (!restoreState(winstate,2)) {
        printf("Restore state failed\n");
      }
    } else{
      expt->set_DefaultLayout(0);
    }
  }

  if (app) {
    m_UpdateTimer.start(app->get_UpdateIntervalMsec());
  }

  captureSize();
}

QxrdWindow::~QxrdWindow()
{
  QxrdApplicationPtr app(m_Application);

  if (app && qcepDebug(DEBUG_APP)) {
    app->printMessage("QxrdWindow::~QxrdWindow");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdWindow::~QxrdWindow(%p)\n", this);
  }
}

void QxrdWindow::updateTitle()
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    setWindowTitle(expt->experimentFilePath()+" - QXRD");
  } else {
    setWindowTitle("QXRD");
  }

  if (sizeof(void*) == 4) {
    setWindowTitle(windowTitle()+" - 32 bit - v"+STR(QXRD_VERSION));
  } else {
    setWindowTitle(windowTitle()+" - 64 bit - v"+STR(QXRD_VERSION));
  }
}

void QxrdWindow::setFontSize(int fontSize)
{
  if (QThread::currentThread()==thread()) {
    if (fontSize > 0) {
      QFont f = font();

      f.setPointSize(fontSize);

      setFont(f);
    } else {
      QFont f = QApplication::font(this);
      setFont(f);
    }
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "setFontSize",
                                           Qt::QueuedConnection,
                                           Q_ARG(int, fontSize)));
  }
}

void QxrdWindow::setSpacing(int spacing)
{
  if (QThread::currentThread()==thread()) {
    setObjectSpacing(this, spacing);
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "setSpacing",
                                           Qt::QueuedConnection,
                                           Q_ARG(int, spacing)));
  }
}

void QxrdWindow::setObjectSpacing(QObject *obj, int spacing)
{
  QLayout *ly = qobject_cast<QLayout*>(obj);

  if (ly) {
    ly->setContentsMargins(spacing, spacing, spacing, spacing);

    QGridLayout *gl = qobject_cast<QGridLayout*>(ly);

    if (gl) {
      gl->setHorizontalSpacing(spacing);
      gl->setVerticalSpacing(spacing);
    }
  }

//  QTableView *tv = qobject_cast<QTableView*>(obj);

//  if (tv) {
//    tv->setStyleSheet(tr("QTableWidget::item { padding: %1px }").arg(spacing));
//  }

  foreach(QObject* chobj, obj->children()) {
    if (chobj) {
      setObjectSpacing(chobj, spacing);
    }
  }
}

void QxrdWindow::shrinkPanels(int fontSize, int spacing)
{
//  if (QThread::currentThread()==thread()) {
//    shrinkObject(this, fontSize, spacing);
//  } else {
//    INVOKE_CHECK(QMetaObject::invokeMethod(this,
//                                           "shrinkPanels",
//                                           Qt::QueuedConnection,
//                                           Q_ARG(int, fontSize),
//                                           Q_ARG(int, spacing)));
//  }
  setFontSize(fontSize);
  setSpacing(spacing);
}

void QxrdWindow::shrinkDockWidget(QDockWidget *dw, int fontSize, int spacing)
{
  shrinkObject(dw, fontSize, spacing);
}

void QxrdWindow::shrinkObject(QObject *obj, int fontSize, int spacing)
{
  if (obj) {
    //    printf("shrinkObject %p[%s]\n", obj, qPrintable(obj->objectName()));

    QWidget *wid = qobject_cast<QWidget*>(obj);

    QVariant fsz = obj->property("defaultFontSize");
    QVariant mrga = obj->property("defaultAMargins");
    QVariant mrgb = obj->property("defaultBMargins");

    if (qcepDebug(DEBUG_WINDOW)) {
      printMessage(tr("QxrdWindow::shrinkObject(\"%1\",%2)").arg(obj->objectName()).HEXARG(obj));
      printMessage(tr("Default Font Size %1, Margins A [%2,%3], B [%4,%5]").arg(fsz.toInt())
        .arg(mrga.toPoint().x()).arg(mrga.toPoint().y())
        .arg(mrgb.toPoint().x()).arg(mrgb.toPoint().y()));
    }

    if (wid) {
      int sz = wid->font().pointSize();

      int left, top, right, bottom;
      wid->getContentsMargins(&left, &top, &right, &bottom);

      if (qcepDebug(DEBUG_WINDOW)) {
        printMessage(tr("Existing Font Size %1, Margins A [%2,%3], B [%4,%5]").arg(sz).arg(left).arg(top).arg(right).arg(bottom));
      }

      if (!fsz.isValid()) {
        wid->setProperty("defaultFontSize", sz);
        fsz = wid->property("defaultFontSize");
      }

      if (!mrga.isValid()) {
        wid->setProperty("defaultAMargins", QPoint(left, top));
        mrga = wid->property("defaultAMargins");
      }

      if (!mrgb.isValid()) {
        wid->setProperty("defaultBMargins", QPoint(right, bottom));
        mrgb = wid->property("defaultBMargins");
      }

      if (fontSize >= 0) {
        QFont f = wid->font();
        f.setPointSize(fontSize);
        wid->setFont(f);
      } else {
        QFont f = wid->font();
        f.setPointSize(fsz.toInt());
        wid->setFont(f);
      }

      if (spacing >= 0) {
        wid->setContentsMargins(spacing, spacing, spacing, spacing);
      } else {
        QPoint ma = mrga.toPoint();
        QPoint mb = mrgb.toPoint();

        if (!ma.isNull() && !mb.isNull()) {
          wid->setContentsMargins(ma.x(), ma.y(), mb.x(), mb.y());
        }
      }
    }

    if (qcepDebug(DEBUG_WINDOW)) {
      printMessage(tr("Default Font Size %1, Margins A [%2,%3], B [%4,%5]").arg(fsz.toInt())
        .arg(mrga.toPoint().x()).arg(mrga.toPoint().y())
        .arg(mrgb.toPoint().x()).arg(mrgb.toPoint().y()));
    }

    QLayout *ly = qobject_cast<QLayout*>(obj);

    if (ly) {
      if (spacing >= 0) {
        ly->setContentsMargins(spacing, spacing, spacing, spacing);
      } else {
        QPoint ma = mrga.toPoint();
        QPoint mb = mrgb.toPoint();

        if (!ma.isNull() && !mb.isNull()) {
          ly->setContentsMargins(ma.x(), ma.y(), mb.x(), mb.y());
        }
      }

      QGridLayout *gl = qobject_cast<QGridLayout*>(ly);

      if (gl) {
//        gl->setContentsMargins(spacing, spacing, spacing, spacing);
        QVariant spc = gl->property("defaultSpacing");

        if (qcepDebug(DEBUG_WINDOW)) {
          printMessage(tr("Default spacing [%1,%2]").arg(spc.toPoint().x()).arg(spc.toPoint().y()));
        }

        if (!spc.isValid()) {
          int hs = gl->horizontalSpacing();
          int vs = gl->verticalSpacing();

          if (qcepDebug(DEBUG_WINDOW)) {
            printMessage(tr("Existing spacing [%1,%2]").arg(hs).arg(vs));
          }

          gl->setProperty("defaultSpacing", QPoint(hs,vs));
          spc = gl->property("defaultSpacing");
        }

        if (spacing >= 0) {
          gl->setHorizontalSpacing(spacing);
          gl->setVerticalSpacing(spacing);
        } else {
          QPoint sp = spc.toPoint();
          if (!sp.isNull()) {
            gl->setHorizontalSpacing(sp.x());
            gl->setVerticalSpacing(sp.y());
          }
        }

        if (qcepDebug(DEBUG_WINDOW)) {
          printMessage(tr("Default spacing [%1,%2]").arg(spc.toPoint().x()).arg(spc.toPoint().y()));
        }
      }
    }

    foreach(QObject* chobj, obj->children()) {
      if (chobj) {
        shrinkObject(chobj, fontSize, spacing);
      }
    }
  }
}

void QxrdWindow::onAcquisitionInit()
{
  m_AcquisitionDialog->onAcquisitionInit();
}

void QxrdWindow::enableTiltRefinement(bool enable)
{
  m_ActionRefineCenterTilt->setEnabled(enable);
}

void QxrdWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    QxrdApplicationPtr app(m_Application);

    if (app) {
      app->closeExperiment(m_Experiment);
    }
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

void QxrdWindow::setupRecentExperimentsMenu(QAction *action)
{
  m_RecentExperimentsMenu = new QMenu(this);

  action->setMenu(m_RecentExperimentsMenu);

  connect(m_RecentExperimentsMenu, SIGNAL(aboutToShow()), this, SLOT(populateRecentExperimentsMenu()));
}

void QxrdWindow::populateExperimentsMenu()
{
  //  printMessage("Populating recent experiments menu");

  m_ExperimentsMenu->clear();

  QxrdApplicationPtr app(m_Application);

  if (app) {
    QList<QxrdExperimentPtr> exps = app->experiments();

    foreach (QxrdExperimentPtr expw, exps) {
      QxrdExperimentPtr exp(expw);

      if (exp) {
        QString path = exp->experimentFilePath();

        QAction *action = new QAction(path, m_ExperimentsMenu);
        QSignalMapper *mapper = new QSignalMapper(action);
        connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
        mapper->setMapping(action, path);

        connect(mapper, SIGNAL(mapped(const QString &)), app.data(), SLOT(activateExperiment(QString)));

        m_ExperimentsMenu -> addAction(action);
      }
    }
  }
}

void QxrdWindow::populateRecentExperimentsMenu()
{
  //  printMessage("Populating recent experiments menu");

  m_RecentExperimentsMenu->clear();

  QxrdApplicationPtr app(m_Application);

  if (app) {
    QStringList recent = app->get_RecentExperiments();

    foreach (QString exp, recent) {
      QAction *action = new QAction(exp, m_RecentExperimentsMenu);
      QSignalMapper *mapper = new QSignalMapper(action);
      connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
      mapper->setMapping(action, exp);

      connect(mapper, SIGNAL(mapped(const QString &)), app.data(), SLOT(openRecentExperiment(QString)));

      m_RecentExperimentsMenu -> addAction(action);
    }
  }
}

QString QxrdWindow::timeStamp() const
{
  return QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ");
}

void QxrdWindow::printMessage(QString msg, QDateTime ts)
{
  QString message = ts.toString("yyyy.MM.dd : hh:mm:ss.zzz ")+
      QThread::currentThread()->objectName()+": "+
      msg.trimmed();

  message = message.replace("\n", " : ");

  displayMessage(message);
}

void QxrdWindow::warningMessage(QString msg)
{
  if (QThread::currentThread()==thread()) {
    QMessageBox::warning(this, tr("Warning"), msg);
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "warningMessage",
                                           Qt::QueuedConnection,
                                           Q_ARG(QString, msg)));
  }
}

void QxrdWindow::displayMessage(QString msg)
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

void QxrdWindow::initialLogEntry(QString aline)
{
  if (QThread::currentThread()==thread()) {
    m_Messages -> append("<font color=#cccccc>"+aline+"</font>");
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "initialLogEntry",
                                           Qt::QueuedConnection,
                                           Q_ARG(QString, aline)));
  }
}

void QxrdWindow::displayCriticalMessage(QString msg)
{
  if (QThread::currentThread()==thread()) {
    static int dialogCount = 0;

    QxrdExperimentPtr expt(m_Experiment);

    if (expt) {
      expt->printMessage(tr("critical message %1, count = %2").arg(msg).arg(dialogCount));
    }

    dialogCount++;
    if (dialogCount <= 1) {
      QMessageBox::critical(this, "Error", msg);
    }
    dialogCount--;
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "displayCriticalMessage",
                                           Qt::QueuedConnection,
                                           Q_ARG(QString, msg)));
  }
}

void QxrdWindow::acquireStarted()
{
}

void QxrdWindow::acquireComplete()
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

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    if (nsum == 1) {
      expt->statusMessage(tr("%1: Exposure %2 of %3, File %4 of %5")
                          .arg(fileName)
                          .arg(iframe+1).arg(nframe).arg(igroup+1).arg(ngroup));
    } else {
      expt->statusMessage(tr("%1: Phase %2 of %3, Sum %4 of %5, Group %6 of %7")
                          .arg(fileName)
                          .arg(isum+1).arg(nsum).arg(iframe+1).arg(nframe).arg(igroup+1).arg(ngroup));
    }
  }

  m_Progress -> setValue(thisframe*100/totalframes);
}

void QxrdWindow::captureSize()
{
  QxrdWindowSettingsPtr set(m_WindowSettings);

  if (set) {
    set->set_WindowGeometry(saveGeometry());
    set->set_WindowState(saveState(2));
  }
}

void QxrdWindow::resizeEvent(QResizeEvent *ev)
{
  captureSize();

  QMainWindow::resizeEvent(ev);
}

void QxrdWindow::moveEvent(QMoveEvent *ev)
{
  captureSize();

  QMainWindow::moveEvent(ev);
}

void QxrdWindow::displayStatusMessage(QString msg)
{
  if (QThread::currentThread()==thread()) {
    m_StatusMsg -> setText(msg);

    //  printMessage(msg);

    m_StatusTimer.start(5000);
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "displayStatusMessage",
                                           Qt::QueuedConnection,
                                           Q_ARG(QString, msg)));
  }
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
  captureSize();

  //  QxrdMutexLocker lock(__FILE__, __LINE__, &m_NewDataMutex);

  if (m_NewDataAvailable.fetchAndStoreOrdered(0)) {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    //    QxrdDoubleImageDataPtr tmp = m_Data;
    m_Data = m_NewData;
    m_NewData = QxrdDoubleImageDataPtr(NULL);

    m_Overflow = m_NewOverflow;
    m_NewOverflow = QxrdMaskDataPtr(NULL);

    m_ImagePlot        -> onProcessedImageAvailable(m_Data, m_Overflow);
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

    m_ImagePlot        -> onMaskedImageAvailable(m_Data, m_Mask);
    m_CenterFinderPlot -> onMaskedImageAvailable(m_Data, m_Mask);

    if (m_ImageDisplay) {
      m_ImageDisplay->updateImage(QxrdDoubleImageDataPtr(), QxrdMaskDataPtr(), m_Mask);
    }
  }
}

void QxrdWindow::doEditPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPreferencesDialog prefs(m_Experiment, this, 1);

  prefs.exec();
}

void QxrdWindow::doEditDetectorPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPreferencesDialog prefs(m_Experiment, this, 0);

  prefs.exec();
}

void QxrdWindow::saveExperimentAs()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(m_Experiment);
  QxrdApplicationPtr app(m_Application);

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
    QString dirp = expt->defaultExperimentDirectory(path);

    QDir dir(expt->get_ExperimentDirectory());

    QString newPath = dir.filePath(name+"-copy.qxrdp");

    QString newChoice = QFileDialog::getSaveFileName(NULL,
                                                     "Save Experiment As",
                                                     newPath,
                                                     "QXRD Experiments (*.qxrdp)");

    if (newChoice.length()>0) {
      expt->saveExperimentAs(newChoice);
      app->appendRecentExperiment(newChoice);
    }
  }
}

void QxrdWindow::saveExperimentCopy()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(m_Experiment);
  QxrdApplicationPtr app(m_Application);

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
    QString dirp = expt->defaultExperimentDirectory(path);

    QDir dir(expt->get_ExperimentDirectory());

    QString newPath = dir.filePath(name+"-copy.qxrdp");

    QString newChoice = QFileDialog::getSaveFileName(NULL,
                                                     "Save Experiment Copy",
                                                     newPath,
                                                     "QXRD Experiments (*.qxrdp)");

    if (newChoice.length()>0) {
      expt->saveExperimentCopyAs(newChoice);
      app->appendRecentExperiment(newChoice);
    }
  }
}

void QxrdWindow::doSaveData()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->data() == NULL) {
      warningMessage("No data available to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Data in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveData(theFile, QxrdDataProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadData()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QString theFile = QFileDialog::getOpenFileName(
          this, "Load Data from...", proc -> get_DataPath());

    if (theFile.length()) {
      proc->loadData(theFile);
    }
  }
}

void QxrdWindow::doSaveDark()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->darkImage() == NULL) {
      warningMessage("No dark image available to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Dark Data in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveDark(theFile, QxrdDataProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadDark()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QString theFile = QFileDialog::getOpenFileName(
          this, "Load Dark Data from...", proc -> get_DataPath());

    if (theFile.length()) {
      proc->loadDark(theFile);
    }
  }
}

void QxrdWindow::doClearDark()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->darkImage() == NULL) {
      warningMessage("No dark image available to clear");
    } else {
      if (QMessageBox::question(this, "Clear Dark Image?", "Do you really want to clear the dark image?",
                                QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
        proc->clearDark();
      }
    }
  }
}

void QxrdWindow::doSaveMask()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->mask() == NULL) {
      warningMessage("No mask image to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Mask in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveMask(theFile, QxrdDataProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadMask()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QString theFile = QFileDialog::getOpenFileName(
          this, "Load Mask from...", proc -> get_DataPath());

    if (theFile.length()) {
      proc->loadMask(theFile);
    }
  }
}

void QxrdWindow::doClearMask()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->mask() == NULL) {
      warningMessage("No mask image available to clear");
    } else {
      if (QMessageBox::question(this, "Clear Mask?", "Do you really want to clear the mask?",
                                QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
        proc->clearMask();
      }
    }
  }
}

void QxrdWindow::doSaveBadPixels()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->badPixels() == NULL) {
      warningMessage("No Bad Pixel data to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Bad Pixels in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveBadPixels(theFile, QxrdDataProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadBadPixels()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QString theFile = QFileDialog::getOpenFileName(
          this, "Load Bad Pixel Map from...", proc -> get_DataPath());

    if (theFile.length()) {
      proc->loadBadPixels(theFile);
    }
  }
}

void QxrdWindow::doClearBadPixels()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->badPixels() == NULL) {
      warningMessage("No Bad Pixel data to clear");
    } else {
      if (QMessageBox::question(this, "Clear Bad Pixels", "Do you really want to clear the bad pixel map?",
                                QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
        proc->clearBadPixels();
      }
    }
  }
}

void QxrdWindow::doSaveGainMap()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->gainMap() == NULL) {
      warningMessage("No Gain Map available to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Gain Map in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveGainMap(theFile, QxrdDataProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadGainMap()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QString theFile = QFileDialog::getOpenFileName(
          this, "Load Pixel Gain Map from...", proc -> get_DataPath());

    if (theFile.length()) {
      proc->loadGainMap(theFile);
    }
  }
}

void QxrdWindow::doClearGainMap()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    if (proc->gainMap() == NULL) {
      warningMessage("No Gain Map available to clear");
    } else {
      if (QMessageBox::question(this, "Clear Gain Map", "Do you really want to clear the gain map?",
                                QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
        proc->clearGainMap();
      }
    }
  }
}

void QxrdWindow::doSaveCachedGeometry()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QxrdIntegratorPtr integ(proc->integrator());

    QString theFile = QFileDialog::getSaveFileName(
          this, "Save Cached Geometry in", m_CachedGeometryPath);

    if (theFile.length()) {
      m_CachedGeometryPath = theFile;

      proc -> saveCachedGeometry(theFile);
    }
  }
}

void QxrdWindow::doSaveCachedIntensity()
{
  GUI_THREAD_CHECK;

  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QxrdIntegratorPtr integ(proc->integrator());

    QString theFile = QFileDialog::getSaveFileName(
          this, "Save Cached Intensity in", m_CachedIntensityPath);

    if (theFile.length()) {
      m_CachedIntensityPath = theFile;

      proc -> saveCachedIntensity(theFile);
    }
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
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdScriptEnginePtr eng(expt -> scriptEngine());

    if (eng) {
      eng -> cancelCommand();
    }
  }
}

void QxrdWindow::finishedCommand(QScriptValue result)
{
  if (result.isError()) {
    int line = result.property("lineNumber").toInteger();

    m_Messages -> append(tr("Error in line %1").arg(line));
    m_Messages -> append(result.property("message").toString());
  } else {
    m_Messages -> append(QxrdScriptEngine::convertToString(result));
  }

  m_CancelScriptButton  -> setEnabled(false);
  m_ActionCancelScript  -> setEnabled(false);
  m_ExecuteScriptButton -> setEnabled(true);
  m_ActionExecuteScript -> setEnabled(true);
}

void QxrdWindow::doLoadScript()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QString theFile = QFileDialog::getOpenFileName(
          this, "Load Script from...", proc->get_ScriptPath());

    if (theFile.length()) {
      proc->set_ScriptPath(theFile);

      loadScript(theFile);
    }
  }
}

void QxrdWindow::loadScript(QString path)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdScriptEnginePtr eng(expt -> scriptEngine());

    if (eng) {
      expt -> printMessage(tr("Loading script file from %1").arg(path));
      eng -> loadScript(path);
    }
  }
}

QxrdDataProcessorWPtr QxrdWindow::dataProcessor() const
{
  return m_DataProcessor;
}

QxrdAcquisitionWPtr QxrdWindow::acquisition() const
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

void QxrdWindow::allocatedMemoryChanged()
{
  QxrdAllocatorPtr allocator(m_Allocator);

  if (allocator) {
    int alloc = allocator -> get_Allocated();
    int maxalloc = allocator -> get_Max();

    m_AllocationStatus -> setMaximum(maxalloc);
    m_AllocationStatus -> setValue(alloc);
  }
}

void QxrdWindow::doRefineCenterTilt()
{
}

void QxrdWindow::doAccumulateImages()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to accumulate...",
                                                      proc -> get_DataPath());

    QMetaObject::invokeMethod(proc.data(), "accumulateImages", Q_ARG(QStringList, files));
  }
}

void QxrdWindow::doAddImages()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data file(s) to add to current...",
                                                      proc -> get_DataPath());

    QMetaObject::invokeMethod(proc.data(), "addImages", Q_ARG(QStringList, files));
  }
}

void QxrdWindow::doSubtractImages()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data file(s) to subtract from current...",
                                                      proc -> get_DataPath());

    QMetaObject::invokeMethod(proc.data(), "subtractImages", Q_ARG(QStringList, files));
  }
}

void QxrdWindow::doReflectHorizontally()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QMetaObject::invokeMethod(proc.data(), "reflectHorizontally");
  }
}

void QxrdWindow::doReflectVertically()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QMetaObject::invokeMethod(proc.data(), "reflectVertically");
  }
}

void QxrdWindow::doProjectAlongX()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to project along X...",
                                                      proc -> get_DataPath());

    QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, files),
                              Q_ARG(int, 1), Q_ARG(int, 0), Q_ARG(int, 0));
  }
}

void QxrdWindow::doProjectAlongY()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to project along Y...",
                                                      proc -> get_DataPath());

    QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, files),
                              Q_ARG(int, 0), Q_ARG(int, 1), Q_ARG(int, 0));
  }
}

void QxrdWindow::doProjectAlongZ()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to project along Z...",
                                                      proc -> get_DataPath());

    QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, files),
                              Q_ARG(int, 0), Q_ARG(int, 0), Q_ARG(int, 1));
  }
}

void QxrdWindow::doCorrelate()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to correlate with current image...",
                                                      proc -> get_DataPath());

    QMetaObject::invokeMethod(proc.data(), "correlateImages", Q_ARG(QStringList, files));
  }
}

void QxrdWindow::doIntegrateSequence()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to integrate...",
                                                      proc -> get_DataPath());

    foreach (QString file, files) {
      proc->integrateData(file);
    }
  }
}

void QxrdWindow::doProcessSequence()
{
  QxrdDataProcessorPtr proc(m_DataProcessor);

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to process...",
                                                      proc -> get_DataPath());

    foreach (QString file, files) {
      proc->processData(file);
    }
  }
}

void QxrdWindow::integrationXUnitsChanged(int newXUnits)
{
  m_ActionIntegrateVsR   -> setChecked(newXUnits == QxrdIntegrator::IntegrateR);
  m_ActionIntegrateVsQ   -> setChecked(newXUnits == QxrdIntegrator::IntegrateQ);
  m_ActionIntegrateVsTTH -> setChecked(newXUnits == QxrdIntegrator::IntegrateTTH);
}

void QxrdWindow::crashProgram()
{
  if (QMessageBox::question(this, tr("Really Crash?"),
                            tr("Do you really want to crash the program?"),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
    int *j = NULL;

    *j= 42;
  }
}

void QxrdWindow::testWidget()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "testWidget", Qt::BlockingQueuedConnection));
  } else {
    QDockWidget *dockWidget = new QDockWidget("Test", this);
    QWidget     *contents   = new QMainWindow(dockWidget);
    QMenuBar    *menuBar    = new QMenuBar(contents);

    QMenu *test1Menu = menuBar->addMenu("Test1");
    QMenu *test2Menu = menuBar->addMenu("Test2");

    test1Menu->addAction("Cmd1");
    test1Menu->addAction("Cmd2");
    test2Menu->addAction("Cmd3");
    test2Menu->addAction("Cmd4");

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    contents->show();
  }
}

void QxrdWindow::onMessageWindowLinesChanged(int newVal)
{
  m_Messages -> document() -> setMaximumBlockCount(newVal);
}

void QxrdWindow::onUpdateIntervalMsecChanged(int newVal)
{
  m_UpdateTimer.setInterval(newVal);
}

void QxrdWindow::plotPowderRingRadii()
{
  QxrdExperimentPtr   expt(m_Experiment);

  if (expt) {
    QxrdCenterFinderPtr cf(expt->centerFinder());

    if (cf) {
      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotMarker);

      int nrgs = cf->countPowderRings();
      int npts = cf->countPowderRingPoints();

      for (int r=0; r<nrgs; r++) {
        QVector<double> x, y;

        for (int i=0; i<npts; i++) {
          QxrdPowderPoint pt = cf->powderRingPoint(i);

          if (pt.n1() == r && pt.n2() == 0) {
            x.append(cf->getChi(pt.x(), pt.y()));
            y.append(cf->getR  (pt.x(), pt.y()));
          }
        }

        if (cf->get_SubtractRingAverages()) {
          double sum = 0;
          int n = y.count();
          for (int i=0; i<n; i++) {
            sum += y[i];
          }

          double avg = sum/(double)n - cf->get_RingAverageDisplacement()*r;
          for (int i=0; i<n; i++) {
            y[i] -= avg;
          }
//        } else {
//          double d = cf->get_RingAverageDisplacement()*r;
//          int n=y.count();
//          for (int i=0; i<n; i++) {
//            y[i] += d;
//          }
        }

        if (x.count() > 0) {
          QwtPlotCurve* pc = new QwtPlotCurve(tr("Ring %1").arg(r));

          m_DistortionCorrectionPlot->setPlotCurveStyle(r, pc);

          pc -> setSamples(x, y);

          pc -> setStyle(QwtPlotCurve::NoCurve);
          pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);

          pc -> attach(m_DistortionCorrectionPlot);
        }
      }

//      m_DistortionCorrectionPlot->autoScale();
      m_DistortionCorrectionPlot->replot();
    }
  }
}

void QxrdWindow::plotPowderRingTwoTheta()
{
  QxrdExperimentPtr   expt(m_Experiment);

  if (expt) {
    QxrdCenterFinderPtr cf(expt->centerFinder());

    if (cf) {
      cf->updateCalibrantDSpacings();

      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotMarker);

      int nrgs = cf->countPowderRings();
      int npts = cf->countPowderRingPoints();

      for (int r=0; r<nrgs; r++) {
        QVector<double> x, y;

        for (int i=0; i<npts; i++) {
          QxrdPowderPoint pt = cf->powderRingPoint(i);

          if (pt.n1() == r && pt.n2() == 0) {
            x.append(cf->getChi(pt.x(), pt.y()));
            y.append(cf->getTTH(pt.x(), pt.y()));
          }
        }

        double avg=0;

        double calTTH = cf->calibrantTTH(r);

        if (cf->get_SubtractRingAverages()) {
          int n = y.count();
          //          double sum = 0;
//          for (int i=0; i<n; i++) {
//            sum += y[i];
//          }

          avg = calTTH - cf->get_RingAverageDisplacement()*r;
          for (int i=0; i<n; i++) {
            y[i] -= avg;
          }
//        } else {
//          double d = cf->get_RingAverageDisplacement()*r;
//          int n=y.count();
//          for (int i=0; i<n; i++) {
//            y[i] += d;
//          }
        }

        if (x.count() >= 0) {
          QwtPlotCurve* pc = new QwtPlotCurve(tr("Ring %1").arg(r));

          m_DistortionCorrectionPlot->setPlotCurveStyle(r, pc);

          pc -> setSamples(x, y);

          pc -> setStyle(QwtPlotCurve::NoCurve);
          pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);

          pc -> attach(m_DistortionCorrectionPlot);

          if (calTTH > 0) {
            QwtPlotCurve* tth = new QwtPlotCurve(tr("Cal %1").arg(r));
            QVector<double> x1,y1;
            x1.append(0); y1.append(calTTH - avg);
            x1.append(360); y1.append(calTTH - avg);
            m_DistortionCorrectionPlot->setPlotCurveStyle(r, tth);
            tth -> setSamples(x1,y1);
            tth -> setSymbol(NULL);
            tth -> attach(m_DistortionCorrectionPlot);
          }
        }
      }

//      m_DistortionCorrectionPlot->autoScale();
      m_DistortionCorrectionPlot->replot();
    }
  }
}

void QxrdWindow::plotPowderRingCenters()
{
  QxrdExperimentPtr   expt(m_Experiment);

  if (expt) {
    QxrdCenterFinderPtr cf(expt->centerFinder());

    if (cf) {
      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotMarker);

      QxrdPowderPointVector pts = cf->get_FittedRings();
      int npts = pts.count();

      QVector<double> x, y;

      for (int i=0; i<npts; i++) {
        QxrdPowderPoint &pt = pts[i];
        x.append(pt.x());
        y.append(pt.y());
      }

      QwtPlotCurve* pc = new QwtPlotCurve(tr("Ring Centers"));

      m_DistortionCorrectionPlot->setPlotCurveStyle(0, pc);

      pc -> setSamples(x, y);

      pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);
      pc -> attach(m_DistortionCorrectionPlot);

      m_DistortionCorrectionPlot->replot();
    }
  }
}
