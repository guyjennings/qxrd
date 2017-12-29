#include "qxrddebug.h"
#include "qxrdwindow.h"
#include "qxrdwindowsettings.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrddisplaydialog.h"
#include "qxrdacquisition.h"
#include "qxrdimageplot.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qxrddataprocessor.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder.h"
#include "qxrdmaskdialog.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"
#include "qcepplotzoomer.h"
#include "qxrdscriptengine.h"
#include "qxrdjsengine.h"
#include "qxrdfilebrowser.h"
#include "qxrdimagecalculator.h"
#include "qcepmutexlocker.h"
#include "qcepallocator.h"
#include "qxrdimagedisplaywidget.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdsynchronizedacquisitiondialog.h"
#include "qxrdcorrectiondialog.h"
#include "qxrdslicedialog.h"
#include "qxrdhistogramdialog.h"
#include "qxrdinfodialog.h"
#include "qxrdscriptdialog.h"
#include "qxrdhighlighter.h"
#include "qxrdacquisitionextrainputsdialog.h"
#include "qxrdacquisitionscalerdialog.h"
#include "qxrdtodolist.h"
#include "qxrdpolartransformdialog.h"
#include "qxrdpolarnormalizationdialog.h"
#include "qxrdapplicationsettings.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdcenterfinderplotsettings.h"
#include "qxrddistortioncorrectionplotsettings.h"
#include "qxrdintegratorplotsettings.h"

#include "qxrdtestimagegeneratorwindow.h"
#include "qxrdtestscangeneratorwindow.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_symbol.h>
#include <QPen>
#include <QFile>
#include <QCheckBox>
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
#include <QSortFilterProxyModel>

QxrdWindow::QxrdWindow(QxrdWindowSettingsWPtr settings,
                       QxrdApplicationWPtr appl,
                       QxrdExperimentWPtr docw,
                       QxrdAcquisitionWPtr acqw,
                       QxrdDataProcessorWPtr procw,
                       QWidget * /*parent*/)  //TODO: needed?
  : QxrdMainWindow("window", appl, docw),
    m_ObjectNamer(this, "window"),
    m_Mutex(QMutex::Recursive),
    m_WindowSettings(settings),
    m_Application(appl),
    m_Experiment(docw),
    m_Acquisition(acqw),
    m_DataProcessor(procw),
//    m_AcquisitionScalerDialog(NULL),
//    m_AcquisitionExtraInputsDialog(NULL),
//    m_SynchronizedAcquisitionDialog(NULL),
//    m_DisplayDialog(NULL),
//    m_CenterFinderDialog(NULL),
//    m_MaskDialog(NULL),
//    m_CorrectionDialog(NULL),
//    m_IntegratorDialog(NULL),
//    m_DatasetBrowserDialog(NULL),
//    m_Calculator(NULL),
//    m_InputFileBrowser(NULL),
//    m_OutputFileBrowser(NULL),
//    m_SliceDialog(NULL),
//    m_HistogramDialog(NULL),
//    m_ImageInfoDialog(NULL),
//    m_ScriptDialog(NULL),
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
//    m_ImageDisplay(NULL),
    m_Highlighter(NULL)
{
#ifndef QT_NO_DEBUG
  printf("Constructing main window\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdWindow::QxrdWindow(%p)\n", this);
  }

  QxrdApplicationPtr app(m_Application);

  if (app && qcepDebug(DEBUG_APP)) {
    app->printMessage("QxrdWindow::QxrdWindow");
  }

  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);
  m_Splitter->setStretchFactor(2, 1);

  m_DatasetBrowserView -> setExperiment(m_Experiment);

  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdDataProcessorPtr proc(exp->dataProcessor());

    QxrdWindowSettingsPtr settings(m_WindowSettings);

    if (settings) {
      m_FileBrowserWidget -> init(settings->fileBrowserSettings(), exp, proc);
    }
  }

  setAttribute(Qt::WA_DeleteOnClose, false);
}

void QxrdWindow::initialize()
{
  QxrdApplicationPtr app(m_Application);
  QxrdExperimentPtr expt(m_Experiment);
  QxrdWindowSettingsPtr set(m_WindowSettings);

  updateTitle();

  setWindowIcon(QIcon(":/images/qxrd-icon-64x64.png"));

  QxrdAcquisitionPtr acq(m_Acquisition);
  QxrdDataProcessorPtr proc(m_DataProcessor);

//  m_AcquisitionDialog = new QxrdAcquisitionDialog(m_Experiment,
//                                                  sharedFromThis(),
//                                                  m_Acquisition,
//                                                  m_DataProcessor,
//                                                  this);

//  m_AcquisitionScalerDialog = new QxrdAcquisitionScalerDialog(m_Acquisition, this);
//  m_SynchronizedAcquisitionDialog = new QxrdSynchronizedAcquisitionDialog(set->synchronizedAcquisitionDialogSettings(), this, m_Acquisition);
//  m_AcquisitionExtraInputsDialog = new QxrdAcquisitionExtraInputsDialog(set->acquisitionExtraInputsDialogSettings(), this, m_Acquisition);

//  m_DisplayDialog      = new QxrdDisplayDialog(this);

//  if (proc) {
//    m_CenterFinderDialog = new QxrdCenterFinderDialog(proc -> centerFinder());
//  } else {
//    m_CenterFinderDialog = new QxrdCenterFinderDialog(QxrdCenterFinderWPtr());
//  }

//  m_MaskDialog       = new QxrdMaskDialog(m_DataProcessor, this);

//  m_CorrectionDialog   = new QxrdCorrectionDialog(this, m_Acquisition, m_DataProcessor);

//  if (proc) {
//    m_IntegratorDialog   = new QxrdIntegratorDialog(proc -> integrator());
//  } else {
//    m_IntegratorDialog   = new QxrdIntegratorDialog(QxrdIntegratorWPtr());
//  }

//  if (expt) {
//    QcepDatasetModelPtr ds = expt->dataset();

//    if (ds) {
//      m_DatasetBrowserDialog = new QcepDatasetBrowserDialog(expt, ds, this);
//    } else {
//      m_DatasetBrowserDialog = new QcepDatasetBrowserDialog(expt, QcepDatasetModelWPtr(), this);
//    }
//  }

//  if (expt && set) {
//    m_InputFileBrowser   = new QxrdInputFileBrowser(set->inputFileBrowserSettings(), m_Experiment, m_DataProcessor, this);
//    m_OutputFileBrowser  = new QxrdOutputFileBrowser(set->outputFileBrowserSettings(), m_Experiment, m_DataProcessor, this);
//    m_ScriptDialog       = new QxrdScriptDialog(set->scriptDialogSettings(), m_Experiment, this);
//  }

//  if (set) {
//    m_SliceDialog        = new QxrdSliceDialog(set->sliceDialogSettings(), this);
//    m_HistogramDialog    = new QxrdHistogramDialog(set->histogramDialogSettings(), m_Experiment, this);
//    m_ImageInfoDialog    = new QxrdInfoDialog(set->infoDialogSettings(), this);
//  }

  if (set) {
    //TODO: re-init
//    m_ImagePlot        -> init(set->imagePlotSettings(), expt);
//    m_CenterFinderPlot -> init(set->centerFinderPlotSettings());
//    m_IntegratorPlot   -> init(set->integratorPlotSettings());
//    m_DistortionCorrectionPlot -> init(set->distortionCorrectionPlotSettings());
  }

  QDesktopWidget *dw = QApplication::desktop();
  //  int screenNum = dw->screenNumber(this);
  QRect screenGeom = dw->screenGeometry(this);

  //  printf("Screen number %d Geom: %d,%d-%d,%d\n", screenNum,
  //         screenGeom.left(), screenGeom.top(),
  //         screenGeom.right(), screenGeom.bottom());

//  if (m_AcquisitionDialog) {
//    addDockWidget(Qt::RightDockWidgetArea, m_AcquisitionDialog);
//  }

//  if (m_InputFileBrowser) {
//    addDockWidget(Qt::LeftDockWidgetArea, m_InputFileBrowser);
//  }

  if (screenGeom.height() >= 1280) {
//    splitDockWidget(m_AcquisitionDialog, m_CenterFinderDialog, Qt::Vertical);
//    splitDockWidget(m_CenterFinderDialog, m_IntegratorDialog, Qt::Vertical);

//    tabifyDockWidget(m_InputFileBrowser, /*m_DatasetBrowserDialog);

//    tabifyDockWidget(m_AcquisitionDialog, *//*m_AcquisitionScalerDialog);
//    tabifyDockWidget(m_AcquisitionScalerDialog, *//*m_AcquisitionExtraInputsDialog);
//    tabifyDockWidget(m_AcquisitionExtraInputsDialog, *//*m_SynchronizedAcquisitionDialog);
//    tabifyDockWidget(m_SynchronizedAcquisitionDialog, */m_DisplayDialog);
//    tabifyDockWidget(m_DisplayDialog, /*m_SliceDialog);
//    tabifyDockWidget(m_SliceDialog, *//*m_ImageInfoDialog);
//    tabifyDockWidget(m_ImageInfoDialog,*/ m_ScriptDialog);

//    tabifyDockWidget(m_CenterFinderDialog, /*m_MaskDialog);
//    tabifyDockWidget(m_MaskDialog, */m_CorrectionDialog);
//    tabifyDockWidget(m_OutputFileBrowser, */m_HistogramDialog);
  } else if (screenGeom.height() >= 1000) {
//    splitDockWidget(m_AcquisitionDialog, m_CenterFinderDialog, Qt::Vertical);

//    tabifyDockWidget(m_InputFileBrowser, /*m_DatasetBrowserDialog);

//    tabifyDockWidget(m_AcquisitionDialog, *//*m_AcquisitionScalerDialog);
//    tabifyDockWidget(m_AcquisitionScalerDialog, *//*m_AcquisitionExtraInputsDialog);
//    tabifyDockWidget(m_AcquisitionExtraInputsDialog, *//*m_SynchronizedAcquisitionDialog);
//    tabifyDockWidget(m_SynchronizedAcquisitionDialog, */m_DisplayDialog);
//    tabifyDockWidget(m_DisplayDialog, /*m_SliceDialog);
//    tabifyDockWidget(m_SliceDialog, *//*m_ImageInfoDialog);
//    tabifyDockWidget(m_ImageInfoDialog,*/ m_ScriptDialog);

//    tabifyDockWidget(m_CenterFinderDialog, /*m_MaskDialog);
//    tabifyDockWidget(m_MaskDialog, */m_CorrectionDialog);
//    tabifyDockWidget(m_OutputFileBrowser, */m_HistogramDialog);
//    tabifyDockWidget(m_HistogramDialog, m_IntegratorDialog);
  } else {
//    tabifyDockWidget(m_AcquisitionDialog, /*m_AcquisitionScalerDialog);
//    tabifyDockWidget(m_AcquisitionScalerDialog, *//*m_AcquisitionExtraInputsDialog);
//    tabifyDockWidget(m_AcquisitionExtraInputsDialog, *//*m_SynchronizedAcquisitionDialog);
//    tabifyDockWidget(m_CenterFinderDialog, *//*m_IntegratorDialog);

//    tabifyDockWidget(m_IntegratorDialog,*/ /*m_SynchronizedAcquisitionDialog);
//    tabifyDockWidget(m_SynchronizedAcquisitionDialog, */m_DisplayDialog);

//    tabifyDockWidget(m_InputFileBrowser, m_DatasetBrowserDialog);

//    tabifyDockWidget(m_DisplayDialog, /*m_MaskDialog);
//    tabifyDockWidget(m_MaskDialog, */m_CorrectionDialog);

//    tabifyDockWidget(m_OutputFileBrowser, *//*m_SliceDialog);
//    tabifyDockWidget(m_SliceDialog, *//*m_HistogramDialog);
//    tabifyDockWidget(m_HistogramDialog, *//*m_ImageInfoDialog);
//    tabifyDockWidget(m_ImageInfoDialog,*/ m_ScriptDialog);

//    if (screenGeom.height() < 1000) {
//      //      shrinkObject(this);
//      shrinkPanels(6,1);
//    }
  }

//  if (expt) {
//    int fs = expt->get_FontSize();
//    int sp = expt->get_Spacing();

//    if (fs > 0) {
//      setFontSize(fs);
//    }

//    if (sp >= 0) {
//      setSpacing(sp);
//    }

//    connect(expt->prop_FontSize(), &QcepIntProperty::valueChanged, this, &QxrdWindow::setFontSize);
//    connect(expt->prop_Spacing(), &QcepIntProperty::valueChanged, this, &QxrdWindow::setSpacing);
//  }

  //  m_Calculator = new QxrdImageCalculator(m_DataProcessor);
  //  addDockWidget(Qt::RightDockWidgetArea, m_Calculator);

  connect(m_ExecuteScriptJSButton, &QAbstractButton::clicked, m_ActionExecuteScriptJS, &QAction::triggered);
  connect(m_ExecuteScriptButton, &QAbstractButton::clicked, m_ActionExecuteScript, &QAction::triggered);
  connect(m_ActionExecuteScript, &QAction::triggered, this, &QxrdWindow::executeScript);
  connect(m_ActionExecuteScriptJS, &QAction::triggered, this, &QxrdWindow::executeScriptJS);
  connect(m_CancelScriptButton, &QAbstractButton::clicked, m_ActionCancelScript, &QAction::triggered);
  connect(m_ActionCancelScript, &QAction::triggered, this, &QxrdWindow::cancelScript);
  connect(m_LoadScriptButton, &QAbstractButton::clicked, m_ActionLoadScript, &QAction::triggered);
  connect(m_ActionLoadScript, &QAction::triggered, this, &QxrdWindow::doLoadScript);

//  connect(m_ActionAutoScale, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::autoScale);

  if (app) {
    connect(m_ActionLoadPreferences, &QAction::triggered, app.data(), &QxrdApplication::doLoadPreferences);
    connect(m_ActionSavePreferences, &QAction::triggered, app.data(), &QxrdApplication::doSavePreferences);
  }

  connect(m_ActionSaveExperimentAsText, &QAction::triggered, this, &QxrdWindow::doSaveExperimentAsText);

  connect(m_ActionReadObjectTreeFromText, &QAction::triggered, this, &QxrdWindow::doReadObjectTreeFromText);

//  m_ExperimentsMenu->menuAction()->setMenuRole(QAction::NoRole);

  connect(m_ConfigureDetectorMenu, &QMenu::aboutToShow, this, &QxrdWindow::populateConfigureDetectorMenu);
  m_ConfigureDetectorMenu->menuAction()->setMenuRole(QAction::NoRole);

  connect(m_DetectorControlWindowsMenu, &QMenu::aboutToShow, this, &QxrdWindow::populateDetectorControlWindowsMenu);

  connect(m_ActionSetupDetectors, &QAction::triggered, this, &QxrdWindow::doEditDetectorPreferences);

  connect(m_ActionLoadData, &QAction::triggered, this, &QxrdWindow::doLoadData);
  connect(m_ActionSaveData, &QAction::triggered, this, &QxrdWindow::doSaveData);
  connect(m_ActionLoadDark, &QAction::triggered, this, &QxrdWindow::doLoadDark);
  connect(m_ActionSaveDark, &QAction::triggered, this, &QxrdWindow::doSaveDark);
  connect(m_ActionClearDark, &QAction::triggered, this, &QxrdWindow::doClearDark);
  connect(m_ActionLoadMask, &QAction::triggered, this, &QxrdWindow::doLoadMask);
  connect(m_ActionSaveMask, &QAction::triggered, this, &QxrdWindow::doSaveMask);
  connect(m_ActionClearMask, &QAction::triggered, this, &QxrdWindow::doClearMask);
  connect(m_ActionLoadGainMap, &QAction::triggered, this, &QxrdWindow::doLoadGainMap);
  connect(m_ActionSaveGainMap, &QAction::triggered, this, &QxrdWindow::doSaveGainMap);
  connect(m_ActionClearGainMap, &QAction::triggered, this, &QxrdWindow::doClearGainMap);

//  connect(m_ActionPrintImage, &QAction::triggered, m_ImagePlot, &QcepPlot::printGraph);

  connect(m_ActionAccumulateImages, &QAction::triggered, this, &QxrdWindow::doAccumulateImages);
  connect(m_ActionAddImage, &QAction::triggered, this, &QxrdWindow::doAddImages);
  connect(m_ActionSubtractImage, &QAction::triggered, this, &QxrdWindow::doSubtractImages);
  connect(m_ActionProjectImagesX, &QAction::triggered, this, &QxrdWindow::doProjectAlongX);
  connect(m_ActionProjectImagesY, &QAction::triggered, this, &QxrdWindow::doProjectAlongY);
  connect(m_ActionProjectImagesZ, &QAction::triggered, this, &QxrdWindow::doProjectAlongZ);
  connect(m_ActionCorrelateImage, &QAction::triggered, this, &QxrdWindow::doCorrelate);
  connect(m_ActionProcessData, &QAction::triggered, this, &QxrdWindow::doProcessSequence);

  connect(m_ActionReflectHorizontally, &QAction::triggered, this, &QxrdWindow::doReflectHorizontally);
  connect(m_ActionReflectVertically, &QAction::triggered, this, &QxrdWindow::doReflectVertically);

//  connect(m_DisplayDialog -> m_AutoRange, &QAbstractButton::clicked, m_ActionAutoRange, &QAction::triggered);
//  connect(m_DisplayDialog -> m_Display_5pct, &QAbstractButton::clicked, m_Action005Range, &QAction::triggered);
//  connect(m_DisplayDialog -> m_Display_10pct, &QAbstractButton::clicked, m_Action010Range, &QAction::triggered);
//  connect(m_DisplayDialog -> m_Display_100pct, &QAbstractButton::clicked, m_Action100Range, &QAction::triggered);

//  connect(m_Action005Range, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::set005Range);
//  connect(m_Action010Range, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::set010Range);
//  connect(m_Action100Range, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::set100Range);
//  connect(m_ActionAutoRange, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::setAutoRange);

//  connect(m_ActionGrayscale, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::setGrayscale);
//  connect(m_ActionInverseGrayscale, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::setInverseGrayscale);
//  connect(m_ActionEarthTones, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::setEarthTones);
//  connect(m_ActionSpectrum, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::setSpectrum);
//  connect(m_ActionFire, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::setFire);
//  connect(m_ActionIce, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::setIce);

  connect(m_ActionRefineCenterTilt, &QAction::triggered, this, &QxrdWindow::doRefineCenterTilt);
//  connect(m_ActionMoveCenterUp, &QAction::triggered, m_CenterFinderDialog, &QxrdCenterFinderDialog::centerMoveUp);
//  connect(m_ActionMoveCenterDown, &QAction::triggered, m_CenterFinderDialog, &QxrdCenterFinderDialog::centerMoveDown);
//  connect(m_ActionMoveCenterLeft, &QAction::triggered, m_CenterFinderDialog, &QxrdCenterFinderDialog::centerMoveLeft);
//  connect(m_ActionMoveCenterRight, &QAction::triggered, m_CenterFinderDialog, &QxrdCenterFinderDialog::centerMoveRight);

  connect(m_ActionNewTestImageGenerator, &QAction::triggered, this, &QxrdWindow::doNewTestImageGenerator);
  connect(m_ActionNewTestScanGenerator, &QAction::triggered, this, &QxrdWindow::doNewTestScanGenerator);

  if (proc) {
    QxrdCenterFinderPtr cf(proc->centerFinder());

    if (cf) {
      connect(m_ActionFindBeamCenter, &QAction::triggered, cf.data(), &QxrdCenterFinder::fitPowderCircle, Qt::DirectConnection);
      connect(m_ActionClearMarkers, &QAction::triggered, cf.data(), &QxrdCenterFinder::deletePowderPoints, Qt::DirectConnection);
      connect(m_ActionCalculateCalibrationPowder, &QAction::triggered, cf.data(), &QxrdCenterFinder::calculateCalibration);
    }
  }

  connect(m_ActionPlotPowderRingPoints, &QAction::triggered, this, &QxrdWindow::plotPowderRingRadii);
  connect(m_ActionPlotPowderRingTwoTheta, &QAction::triggered, this, &QxrdWindow::plotPowderRingTwoTheta);
  connect(m_ActionPlotPowderRingCenters, &QAction::triggered, this, &QxrdWindow::plotPowderRingCenters);

//  m_AcquisitionDialog->setupDetectorsMenu(m_DetectorsMenu);

//  m_AcquisitionDialog->acquisitionReady();

  //TODO: re-implement
//  connect(m_ActionShowImage, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::toggleShowImage);
//  connect(m_ActionShowMask, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::toggleShowMask);
//  connect(m_ActionShowOverflow, &QAction::triggered, m_ImagePlot, &QxrdImagePlot::toggleShowOverflow);

  if (proc) {
    connect(m_ActionShowMaskRange, &QAction::triggered, proc.data(), (void (QxrdDataProcessor::*)()) &QxrdDataProcessor::showMaskRange);
    connect(m_ActionHideMaskRange, &QAction::triggered, proc.data(), (void (QxrdDataProcessor::*)()) &QxrdDataProcessor::hideMaskRange);
    connect(m_ActionShowMaskAll, &QAction::triggered, proc.data(), &QxrdDataProcessor::showMaskAll);
    connect(m_ActionHideMaskAll, &QAction::triggered, proc.data(), &QxrdDataProcessor::hideMaskAll);
    connect(m_ActionInvertMask, &QAction::triggered, proc.data(), &QxrdDataProcessor::invertMask);
//    connect(m_ActionMaskCircles, &QAction::triggered, m_ImageMaskCirclesButton, &QAbstractButton::click);
//    connect(m_ActionMaskPolygons, &QAction::triggered, m_ImageMaskPolygonsButton, &QAbstractButton::click);

//    connect(m_ActionROICalculate, &QAction::triggered, proc.data(), &QxrdDataProcessor::doCalculateROI);
//    connect(m_ActionHistogramCalculate, &QAction::triggered, proc.data(), &QxrdDataProcessor::doCalculateHistogram);
  }

//  connect(m_ImageZoomInButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::enableZooming);
//  connect(m_ImageZoomOutButton, &QAbstractButton::clicked, m_ImagePlot, &QcepPlot::zoomOut);
//  connect(m_ImageZoomAllButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::autoScale);
//  connect(m_ImageSetCenterButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::enableCentering);
//  connect(m_ImageSliceButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::enableSlicing);
//  connect(m_ImageMeasureButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::enableMeasuring);
//  connect(m_ImageHistogramButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::enableHistograms);
//  connect(m_ImageMaskCirclesButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::enableMaskCircles);
//  connect(m_ImageMaskPolygonsButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::enableMaskPolygons);
//  connect(m_ImagePowderPointsButton, &QAbstractButton::clicked, m_ImagePlot, &QxrdImagePlot::enablePowderPoints);

//  connect(m_CenteringZoomInButton, &QAbstractButton::clicked, m_CenterFinderPlot, &QcepPlot::enableZooming);
//  connect(m_CenteringZoomOutButton, &QAbstractButton::clicked, m_CenterFinderPlot, &QcepPlot::zoomOut);
//  connect(m_CenteringZoomAllButton, &QAbstractButton::clicked, m_CenterFinderPlot, &QcepPlot::autoScale);
//  connect(m_CenteringMeasureButton, &QAbstractButton::clicked, m_CenterFinderPlot, &QcepPlot::enableMeasuring);

//  connect(m_IntegratorZoomInButton, &QAbstractButton::clicked, m_IntegratorPlot, &QcepPlot::enableZooming);
//  connect(m_IntegratorZoomOutButton, &QAbstractButton::clicked, m_IntegratorPlot, &QcepPlot::zoomOut);
//  connect(m_IntegratorZoomAllButton, &QAbstractButton::clicked, m_IntegratorPlot, &QcepPlot::autoScale);
//  connect(m_IntegratorMeasureButton, &QAbstractButton::clicked, m_IntegratorPlot, &QcepPlot::enableMeasuring);

//  connect(m_DistortionCorrectionZoomInButton, &QAbstractButton::clicked, m_DistortionCorrectionPlot, &QcepPlot::enableZooming);
//  connect(m_DistortionCorrectionZoomOutButton, &QAbstractButton::clicked, m_DistortionCorrectionPlot, &QcepPlot::zoomOut);
//  connect(m_DistortionCorrectionZoomAllButton, &QAbstractButton::clicked, m_DistortionCorrectionPlot, &QcepPlot::autoScale);
//  connect(m_DistortionCorrectionMeasureButton, &QAbstractButton::clicked, m_DistortionCorrectionPlot, &QcepPlot::enableMeasuring);

//  connect(m_DisplayDialog -> m_DisplayOptionsButton, &QAbstractButton::clicked, this, &QxrdWindow::doEditPreferences);
//  connect(m_CorrectionDialog -> m_CorrectionOptionsButton, &QAbstractButton::clicked, this, &QxrdWindow::doEditPreferences);

  if (app) {
    connect(m_ActionAboutQXRD, &QAction::triggered, app.data(), &QxrdApplication::doAboutQxrd);
    connect(m_ActionOpenQXRDWebPage, &QAction::triggered, app.data(), &QxrdApplication::doOpenQXRDWebPage);
  }

  connect(m_HelpHomeButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::home);
  connect(m_HelpForwardButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::forward);
  connect(m_HelpBackButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::backward);

  connect(m_HelpBrowser, &QTextBrowser::forwardAvailable, m_HelpForwardButton, &QWidget::setEnabled);
  connect(m_HelpBrowser, &QTextBrowser::backwardAvailable, m_HelpBackButton, &QWidget::setEnabled);

  m_HelpBrowser->init(m_Experiment);

  connect(&m_UpdateTimer, &QTimer::timeout, this, &QxrdWindow::doTimerUpdate);

  connect(m_ActionIntegrate, &QAction::triggered, this, &QxrdWindow::doIntegrateSequence);

  if (proc) {
    connect(m_ActionIntegrateCurrent, &QAction::triggered,
            proc.data(), &QxrdDataProcessor::integrateSaveAndDisplay);
  }

//  connect(m_ActionIntegrateInputImages, &QAction::triggered,
//          m_InputFileBrowser, &QxrdFileBrowser::doIntegrate);

//  connect(m_ActionPolarTransform, &QAction::triggered, this, &QxrdWindow::doPolarTransform);
//  connect(m_ActionPolarNormalization, &QAction::triggered, this, &QxrdWindow::doPolarNormalization);

//  connect(m_IntegratorDialog -> m_ClearGraphButton, &QAbstractButton::clicked, m_IntegratorPlot, &QxrdIntegratorPlot::clearGraph);
//  connect(m_IntegratorDialog -> m_ClearSelectedGraphButton, &QAbstractButton::clicked, m_IntegratorPlot, &QxrdIntegratorPlot::clearSelectedCurves);
  //TODO: replace?
  //  connect(m_ActionClearIntegratedData, &QAction::triggered, m_IntegratorPlot, &QxrdIntegratorPlot::clearGraph);
//  connect(m_ActionClearSelectedIntegratedData, &QAction::triggered, m_IntegratorPlot, &QxrdIntegratorPlot::clearSelectedCurves);

  connect(m_ActionSaveCachedGeometry, &QAction::triggered, this, &QxrdWindow::doSaveCachedGeometry);
  connect(m_ActionSaveCachedIntensity, &QAction::triggered, this, &QxrdWindow::doSaveCachedIntensity);

//  connect(m_IntegratorDialog -> m_IntegrateOptionsButton, &QAbstractButton::clicked, this, &QxrdWindow::doEditPreferences);

  if (proc) {
    connect(proc->integrator()->prop_IntegrationXUnits(), &QcepIntProperty::valueChanged,
            this, &QxrdWindow::integrationXUnitsChanged);
    integrationXUnitsChanged(proc->integrator()->get_IntegrationXUnits());

    connect(m_ActionIntegrateVsR,   &QAction::triggered, proc->integrator().data(), &QxrdIntegrator::integrateVsR);
    connect(m_ActionIntegrateVsQ,   &QAction::triggered, proc->integrator().data(), &QxrdIntegrator::integrateVsQ);
    connect(m_ActionIntegrateVsTTH, &QAction::triggered, proc->integrator().data(), &QxrdIntegrator::integrateVsTTH);
  }

  //TODO: reimplement
//  if (set) {
//    QxrdImagePlotSettingsPtr ps(set->imagePlotSettings());

//    if (ps) {
//      ps->prop_XMouse()->linkTo(m_XMouse);
//      ps->prop_YMouse()->linkTo(m_YMouse);
//      ps->prop_ValMouse()->linkTo(m_ValMouse);
//      ps->prop_TTHMouse()->linkTo(m_TTHMouse);
//      ps->prop_QMouse()->linkTo(m_QMouse);
//      ps->prop_RMouse()->linkTo(m_RMouse);
//    }
//  }

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

//  if (app && m_Acquisition == NULL) {
//    app->criticalMessage("Oh no, QxrdWindow::m_Acquisition == NULL");
//  }

  if (acq) {
    connect(acq.data(), &QxrdAcquisition::acquireStarted,
            this,       &QxrdWindow::acquireStarted);
    connect(acq.data(), SIGNAL(acquiredFrame(QString,int,int,int,int,int,int)),
            this,       SLOT(acquiredFrame(QString,int,int,int,int,int,int)));
    connect(acq.data(), &QxrdAcquisition::acquireComplete,
            this,       &QxrdWindow::acquireComplete);

//    acq -> prop_OverflowLevel() -> linkTo(m_DisplayDialog->m_OverflowLevel);
//    acq -> prop_RawSaveTime() -> linkTo(m_CorrectionDialog->m_SaveRawTime);
//    acq -> prop_DarkSaveTime() -> linkTo(m_CorrectionDialog->m_SaveDarkTime);

//    connect(acq->prop_OverflowLevel(), &QcepIntProperty::valueChanged,
//            m_HistogramDialog, &QxrdHistogramDialog::updateHistogramNeeded);
  }

  if (expt) {
    expt -> prop_CompletionPercentage() -> linkTo(m_Progress);
  }

//  if (proc) {
//    proc -> prop_PerformDarkSubtraction() -> linkTo(m_CorrectionDialog->m_PerformDark);
//    proc -> prop_PerformDarkSubtractionTime() -> linkTo(m_CorrectionDialog->m_PerformDarkTime);
//    proc -> prop_SaveRawImages() -> linkTo(m_CorrectionDialog->m_SaveRaw);
//    proc -> prop_SaveDarkImages() -> linkTo(m_CorrectionDialog->m_SaveDark);
//    proc -> prop_PerformBadPixels() -> linkTo(m_CorrectionDialog->m_PerformBadPixels);
//    proc -> prop_PerformBadPixelsTime() -> linkTo(m_CorrectionDialog->m_PerformBadPixelsTime);
//    proc -> prop_PerformGainCorrection() -> linkTo(m_CorrectionDialog->m_PerformGainCorrection);
//    proc -> prop_PerformGainCorrectionTime() -> linkTo(m_CorrectionDialog->m_PerformGainCorrectionTime);
//    proc -> prop_SaveSubtracted() -> linkTo(m_CorrectionDialog->m_SaveSubtracted);
//    proc -> prop_SaveSubtractedTime() -> linkTo(m_CorrectionDialog->m_SaveSubtractedTime);
//    proc -> prop_SaveAsText() -> linkTo(m_CorrectionDialog->m_SaveAsText);
//    proc -> prop_SaveAsTextTime() -> linkTo(m_CorrectionDialog->m_SaveAsTextTime);
//    proc -> prop_PerformIntegration() -> linkTo(m_CorrectionDialog->m_PerformIntegration);
//    proc -> prop_PerformIntegrationTime() -> linkTo(m_CorrectionDialog->m_PerformIntegrationTime);
//    proc -> prop_DisplayIntegratedData() -> linkTo(m_CorrectionDialog->m_DisplayIntegratedData);
//    proc -> prop_DisplayIntegratedDataTime() -> linkTo(m_CorrectionDialog->m_DisplayIntegratedDataTime);
//    proc -> prop_SaveIntegratedData() -> linkTo(m_CorrectionDialog->m_SaveIntegratedData);
//    proc -> prop_SaveIntegratedDataTime() -> linkTo(m_CorrectionDialog->m_SaveIntegratedDataTime);
//    proc -> prop_SaveIntegratedInSeparateFiles() -> linkTo(m_CorrectionDialog->m_SaveIntegratedInSeparateFiles);
//    proc -> prop_AccumulateIntegrated2D() ->linkTo(m_CorrectionDialog->m_AccumulateIntegrated2D);
//    proc -> prop_AccumulateIntegratedName() -> linkTo(m_CorrectionDialog->m_AccumulateIntegratedName);
//    proc -> prop_EstimatedProcessingTime() -> linkTo(m_CorrectionDialog->m_EstimatedProcessingTime);
//  }

//  if (set) {
//    QxrdImagePlotSettingsPtr ps(set->imagePlotSettings());

//    if (ps) {
//      ps -> prop_DisplayMinimumPct() -> linkTo(m_DisplayDialog->m_DisplayMinimumPct);
//      ps -> prop_DisplayMaximumPct() -> linkTo(m_DisplayDialog->m_DisplayMaximumPct);
//      ps -> prop_DisplayMinimumVal() -> linkTo(m_DisplayDialog->m_DisplayMinimumVal);
//      ps -> prop_DisplayMaximumVal() -> linkTo(m_DisplayDialog->m_DisplayMaximumVal);
//      ps -> prop_DisplayMinimumPctle() -> linkTo(m_DisplayDialog->m_DisplayMinimumPctle);
//      ps -> prop_DisplayMaximumPctle() -> linkTo(m_DisplayDialog->m_DisplayMaximumPctle);

//      ps -> prop_DisplayLog() -> linkTo(m_DisplayDialog->m_DisplayImageLog);
//      ps -> prop_DisplayScalingMode() -> linkTo(m_DisplayDialog->m_DisplayScalingMode);

//      connect(ps -> prop_DisplayScalingMode(), &QcepIntProperty::valueChanged, m_DisplayDialog->m_DisplayParmsStack, &QStackedWidget::setCurrentIndex);
//      m_DisplayDialog->m_DisplayParmsStack->setCurrentIndex(ps->get_DisplayScalingMode());

//      ps -> prop_DisplayColorMap() -> linkTo(m_DisplayDialog->m_DisplayColorMap);

//      ps -> prop_ImageShown() -> linkTo(m_DisplayDialog->m_DisplayImage);
//      ps -> prop_MaskShown() -> linkTo(m_DisplayDialog->m_DisplayMask);
//      ps -> prop_OverflowShown() -> linkTo(m_DisplayDialog->m_DisplayOverflow);
//      ps -> prop_InterpolatePixels() -> linkTo(m_DisplayDialog->m_InterpolatePixels);
//      ps -> prop_MaintainAspectRatio() -> linkTo(m_DisplayDialog->m_MaintainAspectRatio);

//    }
//  }

  //TODO: re-init
//  m_ImagePlot -> setProcessor(m_DataProcessor);
//  m_DistortionCorrectionPlot -> setWindow(this);
//  m_CenterFinderPlot -> setWindow(this);
//  m_IntegratorPlot -> setDataProcessor(m_DataProcessor);

  //TODO: re-connect data flows
//  if (proc) {
//    connect(proc -> centerFinder() -> prop_CenterX(), &QcepDoubleProperty::valueChanged,
//            m_ImagePlot, &QxrdImagePlot::onCenterXChanged);

//    connect(proc -> centerFinder() -> prop_CenterY(), &QcepDoubleProperty::valueChanged,
//            m_ImagePlot, &QxrdImagePlot::onCenterYChanged);

//    connect(proc -> centerFinder() -> prop_CenterX(), &QcepDoubleProperty::valueChanged,
//            m_CenterFinderPlot, &QxrdCenterFinderPlot::onCenterXChanged);

//    connect(proc -> centerFinder() -> prop_CenterY(), &QcepDoubleProperty::valueChanged,
//            m_CenterFinderPlot, &QxrdCenterFinderPlot::onCenterYChanged);

//    connect(proc.data(), &QxrdDataProcessor::newIntegrationAvailable,
//            m_IntegratorPlot, &QxrdIntegratorPlot::onNewIntegrationAvailable);
//  }

//  m_WindowsMenu -> addAction(m_AcquisitionDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_AcquisitionScalerDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_AcquisitionExtraInputsDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_InputFileBrowser -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_OutputFileBrowser -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_SynchronizedAcquisitionDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_DisplayDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_CenterFinderDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_DatasetBrowserDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_MaskDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_CorrectionDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_IntegratorDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_SliceDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_HistogramDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_ImageInfoDialog -> toggleViewAction());
//  m_WindowsMenu -> addAction(m_ScriptDialog -> toggleViewAction());

  m_Highlighter = new QxrdHighlighter(m_ScriptEdit->document());

//  if (expt) {
//    expt->prop_DefaultScript()->linkTo(m_ScriptEdit);
//  }

  QxrdToDoList *toDoList = new QxrdToDoList(this);
  QSortFilterProxyModel *sorted = new QSortFilterProxyModel(this);

  sorted->setSourceModel(toDoList);

  sorted->sort(0, Qt::DescendingOrder);

  m_ToDoList->setModel(sorted);
  m_ToDoList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_ToDoList->horizontalHeader()->setStretchLastSection(true);
  m_ToDoList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_ToDoList->setSelectionBehavior(QAbstractItemView::SelectRows);

//  connect(m_ImagePlot, &QxrdImagePlot::slicePolygon,
//          m_SliceDialog, &QxrdSliceDialog::slicePolygon);

//  connect(m_ImagePlot, &QxrdImagePlot::selectHistogram,
//          m_HistogramDialog, &QxrdHistogramDialog::histogramSelectionChanged);

  if (app) {
    QxrdApplicationSettingsPtr appset(app->settings());

    if (appset) {
      m_Messages -> document() -> setMaximumBlockCount(appset->get_MessageWindowLines());

      connect(appset->prop_MessageWindowLines(), &QcepIntProperty::valueChanged, this, &QxrdWindow::onMessageWindowLinesChanged);
      connect(appset->prop_UpdateIntervalMsec(), &QcepIntProperty::valueChanged, this, &QxrdWindow::onUpdateIntervalMsecChanged);

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

          if (!restoreState(winstate, QXRD_WINDOW_STATE_VERSION)) {
            printf("Restore state failed\n");
          }
        } else{
          expt->set_DefaultLayout(0);
        }
      }

      m_UpdateTimer.start(appset->get_UpdateIntervalMsec());
    }
  }

  captureSize();
}

QxrdWindow::~QxrdWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting main window from thread %s\n", qPrintable(QThread::currentThread()->objectName()));
#endif  

  QxrdApplicationPtr app(m_Application);

  if (app && qcepDebug(DEBUG_APP)) {
    app->printMessage("QxrdWindow::~QxrdWindow");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdWindow::~QxrdWindow(%p)\n", this);
  }
}

//void QxrdWindow::setupMenus()
//{
//  QxrdMainWindow::setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);
//}

void QxrdWindow::updateTitle()
{
  QString title;

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    title = expt->experimentFilePath()+" - QXRD";
  } else {
    title = "QXRD";
  }

  if (sizeof(void*) == 4) {
    title.append(" - 32 bit - v");
  } else {
    title.append(" - 64 bit - v");
  }

  title.append(STR(QXRD_VERSION));

  if (expt && expt->isChanged()) {
    title.append(tr(" [%1]").arg(expt->isChanged()));
  }

  setWindowTitle(title);
}

void QxrdWindow::onAcquisitionInit()
{
//  m_AcquisitionDialog->onAcquisitionInit();
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

//void QxrdWindow::populateWindowMenu()
//{
//  printMessage("populate window menu");
//}

void QxrdWindow::populateConfigureDetectorMenu()
{
  m_ConfigureDetectorMenu->clear();

  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    int nDets = acq->get_DetectorCount();

    for (int i=0; i<nDets; i++) {
      QxrdDetectorSettingsPtr det = acq->detector(i);
      QString detType = det->get_DetectorTypeName();
      QString detName = det->get_DetectorName();
      bool    enabled = det->get_Enabled();

      QString str = tr("(%1) Configure %2 detector \"%3\"...").arg(i).arg(detType).arg(detName);

      QAction *action = new QAction(str, m_ConfigureDetectorMenu);

      action->setCheckable(true);
      action->setChecked(enabled);

      connect(action, &QAction::triggered, [=] {acq->configureDetector(i);});

      m_ConfigureDetectorMenu->addAction(action);
    }
  }
}

void QxrdWindow::populateDetectorControlWindowsMenu()
{
  m_DetectorControlWindowsMenu->clear();

  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    int nDets = acq->get_DetectorCount();

    for (int i=0; i<nDets; i++) {
      QxrdDetectorSettingsPtr det = acq->detector(i);
      QString detType = det->get_DetectorTypeName();
      QString detName = det->get_DetectorName();
      bool    enabled = det->get_Enabled();

      QString str = tr("(%1) Open %2 detector \"%3\" Control...").arg(i).arg(detType).arg(detName);

      QAction *action = new QAction(str, m_DetectorControlWindowsMenu);

      action->setCheckable(true);
      action->setChecked(enabled);

      connect(action, &QAction::triggered, [=] {acq->openDetectorControlWindow(i);});

      m_DetectorControlWindowsMenu->addAction(action);
    }
  }
}

QString QxrdWindow::timeStamp() const
{
  return QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss.zzz ");
}

void QxrdWindow::printLine(QString line)
{
  displayMessage(line);
}

void QxrdWindow::criticalMessage(QString msg, QDateTime /*ts*/)
{
  displayCriticalMessage(msg);
}

void QxrdWindow::statusMessage(QString /*msg*/, QDateTime /*ts*/)
{
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
    m_Messages -> append("<font color=#ff0000>"+aline+"</font>");
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
    QString fileName, int iphase, int nphases, int isum, int nsum, int igroup, int ngroup)
{
  printMessage(tr("QxrdWindow::acquiredFrame(\"%1\",%2,%3,%4,%5,%6,%7)")
               .arg(fileName)
               .arg(iphase).arg(nphases)
               .arg(isum).arg(nsum)
               .arg(igroup).arg(ngroup));
  //   printf("\n",
  // 	 qPrintable(fileName), fileIndex, isum, nsum, iframe, nframe);

  int totalFrames = (nphases*nsum*ngroup <= 0 ? 1 : nphases*nsum*ngroup);
  int thisFrame   = igroup*nphases*nsum + isum*nphases + iphase + 1;

  printMessage(tr("Frame %1 of %2").arg(thisFrame).arg(totalFrames));

  //  printf("%d %% progress\n", thisFrame*100/totalFrames);

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    if (nphases <= 1) {
      expt->statusMessage(tr("%1: Exposure %2 of %3, File %4 of %5")
                          .arg(fileName)
                          .arg(isum+1).arg(nsum)
                          .arg(igroup+1).arg(ngroup));
    } else {
      expt->statusMessage(tr("%1: Phase %2 of %3, Sum %4 of %5, Group %6 of %7")
                          .arg(fileName)
                          .arg(iphase+1).arg(nphases)
                          .arg(isum+1).arg(nsum)
                          .arg(igroup+1).arg(ngroup));
    }
  }

  m_Progress -> setValue(thisFrame*100/totalFrames);
}

void QxrdWindow::captureSize()
{
  QxrdWindowSettingsPtr set(m_WindowSettings);

  if (set) {
    set->set_WindowGeometry(saveGeometry());
    set->set_WindowState(saveState(QXRD_WINDOW_STATE_VERSION));
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

void QxrdWindow::newDataAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow)
{
  //  QcepMutexLocker lock(__FILE__, __LINE__, &m_NewDataMutex);

  //  image -> copyImage(m_NewData);
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_NewData = image;
  m_NewOverflow = overflow;

  m_NewDataAvailable.fetchAndAddOrdered(1);

  //  INVOKE_CHECK(QMetaObject::invokeMethod(this, "newData", Qt::QueuedConnection));
}

void QxrdWindow::newMaskAvailable(QcepMaskDataPtr mask)
{
  //  QcepMutexLocker lock(__FILE__, __LINE__, &m_NewMaskMutex);

  //  mask -> copyMaskTo(m_NewMask);
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_NewMask = mask;
  m_NewMaskAvailable.fetchAndAddOrdered(1);

  INVOKE_CHECK(QMetaObject::invokeMethod(this, "newMask", Qt::QueuedConnection));
}

void QxrdWindow::doTimerUpdate()
{
  updateTitle();

  allocatedMemoryChanged();

  QTime t;
  t.start();

  captureSize();

  //  QcepMutexLocker lock(__FILE__, __LINE__, &m_NewDataMutex);

  if (m_NewDataAvailable.fetchAndStoreOrdered(0)) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    //    QcepDoubleImageDataPtr tmp = m_Data;
    m_Data = m_NewData;
    m_NewData = QcepDoubleImageDataPtr(NULL);

    m_Overflow = m_NewOverflow;
    m_NewOverflow = QcepMaskDataPtr(NULL);

    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
      g_Application->printMessage(tr("QxrdWindow::newData new data after %1 msec").arg(t.elapsed()));
    }

    //TODO: re-connect data flow
//    m_ImagePlot        -> onProcessedImageAvailable(m_Data, m_Overflow);

//    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
//      g_Application->printMessage(tr("QxrdWindow::newData imagePlot after %1 msec").arg(t.elapsed()));
//    }

//    m_CenterFinderPlot -> onProcessedImageAvailable(m_Data);

//    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
//      g_Application->printMessage(tr("QxrdWindow::newData centerFinder after %1 msec").arg(t.elapsed()));
//    }

//    if (m_ImageDisplay) {
//      m_ImageDisplay -> updateImage(m_Data, m_Overflow);

//      if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
//        g_Application->printMessage(tr("QxrdWindow::newData imageDisplay after %1 msec").arg(t.elapsed()));
//      }
//    }

//    if (m_SliceDialog) {
//      m_SliceDialog -> onProcessedImageAvailable(m_Data, m_Overflow);

//      if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
//        g_Application->printMessage(tr("QxrdWindow::newData sliceDialog after %1 msec").arg(t.elapsed()));
//      }
//    }

//    if (m_HistogramDialog) {
//      m_HistogramDialog -> onProcessedImageAvailable(m_Data, m_Overflow);

//      if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
//        g_Application->printMessage(tr("QxrdWindow::newData histogramDialog after %1 msec").arg(t.elapsed()));
//      }
//    }

//    if (m_ImageInfoDialog) {
//      m_ImageInfoDialog -> onProcessedImageAvailable(m_Data, m_Overflow);

//      if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
//        g_Application->printMessage(tr("QxrdWindow::newData imageInfoDialog after %1 msec").arg(t.elapsed()));
//      }
//    }


    if (g_Application && qcepDebug(DEBUG_DISPLAY)) {
      g_Application->printMessage(tr("QxrdWindow::newData took %1 msec").arg(t.elapsed()));
    }
  }
}

void QxrdWindow::newMask()
{
  //  QcepMutexLocker lock(__FILE__, __LINE__, &m_NewMaskMutex);

  if (m_NewMaskAvailable.fetchAndAddOrdered(0)) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    //    QcepMaskDataPtr tmp = m_Mask;
    m_Mask = m_NewMask;
    m_NewMask = QcepMaskDataPtr(NULL);
    m_NewMaskAvailable.fetchAndStoreOrdered(0);

    //TODO: reconnect data flow
//    m_ImagePlot        -> onMaskedImageAvailable(m_Data, m_Mask);
//    m_CenterFinderPlot -> onMaskedImageAvailable(m_Data, m_Mask);

//    if (m_ImageDisplay) {
//      m_ImageDisplay->updateImage(QcepDoubleImageDataPtr(), QcepMaskDataPtr(), m_Mask);
//    }
  }
}

void QxrdWindow::doSaveExperimentAsText()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(m_Experiment);
  QxrdApplicationPtr app(m_Application);

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
    QString dirp = expt->defaultExperimentDirectory(path);

    QDir dir(expt->get_ExperimentDirectory());

    QString newPath = dir.filePath(name+".txt");

    QString newChoice = QFileDialog::getSaveFileName(NULL,
                                                     "Save Experiment As Text",
                                                     newPath);

    if (newChoice.length() > 0) {
      expt->saveExperimentAsText(newChoice);
    }
  }
}

void QxrdWindow::doReadObjectTreeFromText()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(m_Experiment);
  QxrdApplicationPtr app(m_Application);

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
    QString dirp = expt->defaultExperimentDirectory(path);

    QDir dir(expt->get_ExperimentDirectory());

    QString newPath = dir.filePath(name+".txt");

    QString theFile = QFileDialog::getOpenFileName(
          this, "Load Object Tree from...", newPath);

    if (theFile.length()) {
      expt->readObjectTreeFromText(theFile);
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

void QxrdWindow::executeScript()
{
  emit executeCommand(m_ScriptEdit -> toPlainText());

  m_CancelScriptButton  -> setEnabled(true);
  m_ActionCancelScript  -> setEnabled(true);
  m_ExecuteScriptButton -> setEnabled(false);
  m_ExecuteScriptJSButton -> setEnabled(false);
  m_ActionExecuteScript -> setEnabled(false);
}

void QxrdWindow::executeScriptJS()
{
  emit executeCommandJS(m_ScriptEdit -> toPlainText());

  m_CancelScriptButton  -> setEnabled(true);
  m_ActionCancelScript  -> setEnabled(true);
  m_ExecuteScriptButton -> setEnabled(false);
  m_ExecuteScriptJSButton -> setEnabled(false);
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
  m_ExecuteScriptJSButton -> setEnabled(true);
  m_ActionExecuteScript -> setEnabled(true);
}

void QxrdWindow::finishedCommandJS(QJSValue result)
{
  if (result.isError()) {
    int line = result.property("lineNumber").toInt();

    m_Messages -> append(tr("Error in line %1").arg(line));
    m_Messages -> append(result.property("message").toString());
  } else {
    m_Messages -> append(QxrdJSEngine::convertToString(result));
  }

  m_CancelScriptButton  -> setEnabled(false);
  m_ActionCancelScript  -> setEnabled(false);
  m_ExecuteScriptButton -> setEnabled(true);
  m_ExecuteScriptJSButton -> setEnabled(true);
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

QcepDoubleImageDataPtr QxrdWindow::data()
{
  return m_Data;
}

QcepMaskDataPtr QxrdWindow::mask()
{
  return m_Mask;
}

void QxrdWindow::allocatedMemoryChanged()
{
  int alloc = QcepAllocator::allocatedMemoryMB();
  int avail = QcepAllocator::availableMemoryMB();

  m_AllocationStatus -> setMaximum(avail);
  m_AllocationStatus -> setValue(alloc);
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
  m_ActionIntegrateVsR   -> setChecked(newXUnits == QxrdIntegrator::RadialIntegrateR);
  m_ActionIntegrateVsQ   -> setChecked(newXUnits == QxrdIntegrator::RadialIntegrateQ);
  m_ActionIntegrateVsTTH -> setChecked(newXUnits == QxrdIntegrator::RadialIntegrateTTH);
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
  //TODO: re-implement
//  QxrdExperimentPtr   expt(m_Experiment);

//  if (expt) {
//    QxrdCenterFinderPtr cf(expt->centerFinder());

//    if (cf) {
//      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
//      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotMarker);

//      int nrgs = cf->countPowderRings();
//      int npts = cf->countPowderRingPoints();

//      for (int r=0; r<nrgs; r++) {
//        QVector<double> x, y;

//        for (int i=0; i<npts; i++) {
//          QxrdPowderPoint pt = cf->powderRingPoint(i);

//          if (pt.n1() == r && pt.n2() == 0) {
//            x.append(cf->getChi(pt.x(), pt.y()));
//            y.append(cf->getR  (pt.x(), pt.y()));
//          }
//        }

//        if (cf->get_SubtractRingAverages()) {
//          double sum = 0;
//          int n = y.count();
//          for (int i=0; i<n; i++) {
//            sum += y[i];
//          }

//          double avg = sum/(double)n - cf->get_RingAverageDisplacement()*r;
//          for (int i=0; i<n; i++) {
//            y[i] -= avg;
//          }
////        } else {
////          double d = cf->get_RingAverageDisplacement()*r;
////          int n=y.count();
////          for (int i=0; i<n; i++) {
////            y[i] += d;
////          }
//        }

//        if (x.count() > 0) {
//          QwtPlotCurve* pc = new QwtPlotCurve(tr("Ring %1").arg(r));

//          m_DistortionCorrectionPlot->setPlotCurveStyle(r, pc);

//          pc -> setSamples(x, y);

//          pc -> setStyle(QwtPlotCurve::NoCurve);
//          pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);

//          pc -> attach(m_DistortionCorrectionPlot);
//        }
//      }

////      m_DistortionCorrectionPlot->autoScale();
//      m_DistortionCorrectionPlot->replot();
//    }
//  }
}

void QxrdWindow::plotPowderRingTwoTheta()
{
  //TODO: reimplement
//  QxrdExperimentPtr   expt(m_Experiment);

//  if (expt) {
//    QxrdCenterFinderPtr cf(expt->centerFinder());

//    if (cf) {
////      cf->updateCalibrantDSpacings();

//      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
//      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotMarker);

//      int nrgs = cf->countPowderRings();
//      int npts = cf->countPowderRingPoints();

//      for (int r=0; r<nrgs; r++) {
//        QVector<double> x, y;

//        for (int i=0; i<npts; i++) {
//          QxrdPowderPoint pt = cf->powderRingPoint(i);

//          if (pt.n1() == r && pt.n2() == 0) {
//            x.append(cf->getChi(pt.x(), pt.y()));
//            y.append(cf->getTTH(pt.x(), pt.y()));
//          }
//        }

//        double avg=0;

//        double calTTH = cf->calibrantTTH(r);

//        if (cf->get_SubtractRingAverages()) {
//          int n = y.count();
//          //          double sum = 0;
////          for (int i=0; i<n; i++) {
////            sum += y[i];
////          }

//          avg = calTTH - cf->get_RingAverageDisplacement()*r;
//          for (int i=0; i<n; i++) {
//            y[i] -= avg;
//          }
////        } else {
////          double d = cf->get_RingAverageDisplacement()*r;
////          int n=y.count();
////          for (int i=0; i<n; i++) {
////            y[i] += d;
////          }
//        }

//        if (x.count() >= 0) {
//          QwtPlotCurve* pc = new QwtPlotCurve(tr("Ring %1").arg(r));

//          m_DistortionCorrectionPlot->setPlotCurveStyle(r, pc);

//          pc -> setSamples(x, y);

//          pc -> setStyle(QwtPlotCurve::NoCurve);
//          pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);

//          pc -> attach(m_DistortionCorrectionPlot);

//          if (calTTH > 0) {
//            QwtPlotCurve* tth = new QwtPlotCurve(tr("Cal %1").arg(r));
//            QVector<double> x1,y1;
//            x1.append(0); y1.append(calTTH - avg);
//            x1.append(360); y1.append(calTTH - avg);
//            m_DistortionCorrectionPlot->setPlotCurveStyle(r, tth);
//            tth -> setSamples(x1,y1);
//            tth -> setSymbol(NULL);
//            tth -> attach(m_DistortionCorrectionPlot);
//          }
//        }
//      }

////      m_DistortionCorrectionPlot->autoScale();
//      m_DistortionCorrectionPlot->replot();
//    }
//  }
}

void QxrdWindow::plotPowderRingCenters()
{
  //TODO: reimplement
//  QxrdExperimentPtr   expt(m_Experiment);

//  if (expt) {
//    QxrdCenterFinderPtr cf(expt->centerFinder());

//    if (cf) {
//      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
//      m_DistortionCorrectionPlot->detachItems(QwtPlotItem::Rtti_PlotMarker);

//      QxrdPowderPointVector pts = cf->get_FittedRings();
//      int npts = pts.count();

//      QVector<double> x, y;

//      for (int i=0; i<npts; i++) {
//        QxrdPowderPoint &pt = pts[i];
//        x.append(pt.x());
//        y.append(pt.y());
//      }

//      QwtPlotCurve* pc = new QwtPlotCurve(tr("Ring Centers"));

//      m_DistortionCorrectionPlot->setPlotCurveStyle(0, pc);

//      pc -> setSamples(x, y);

//      pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);
//      pc -> attach(m_DistortionCorrectionPlot);

//      m_DistortionCorrectionPlot->replot();
//    }
//  }
}

static int s_TestImageCount = 0;
static int s_TestScanCount = 0;

void QxrdWindow::doNewTestImageGenerator()
{
  auto win = new QxrdTestImageGeneratorWindow(m_Experiment);

  win -> setWindowTitle(tr("Test Image Generator %1").arg(s_TestImageCount++));
  win -> show();
}

void QxrdWindow::doNewTestScanGenerator()
{
  auto win = new QxrdTestScanGeneratorWindow(m_Experiment);

  win -> setWindowTitle(tr("Test Scan Generator %1").arg(s_TestScanCount++));
  win -> show();
}
