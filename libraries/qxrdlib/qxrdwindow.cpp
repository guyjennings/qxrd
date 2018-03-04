#include "qxrddebug.h"
#include "qxrdwindow.h"
#include "qxrdwindowsettings.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdplotwidgetdialog.h"
#include "qxrdacqcommon.h"
#include "qxrdimageplot.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qxrdprocessor.h"
#include "qxrdcenterfinder.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrator.h"
#include "qcepplotzoomer.h"
#include "qxrdscriptengine.h"
#include "qxrdjsengine.h"
#include "qcepmutexlocker.h"
#include "qcepallocator.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdhistogramdialog.h"
#include "qxrdhighlighter.h"
#include "qxrdtodolist.h"
#include "qxrdpolartransformdialog.h"
#include "qxrdpolarnormalizationdialog.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdintegratorplotsettings.h"

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

QxrdWindow::QxrdWindow(QString name)
  : inherited(name),
    m_ObjectNamer(this, "window"),
    m_Mutex(QMutex::Recursive),
    m_Data(NULL),
    m_Overflow(NULL),
    m_NewData(NULL),
    m_NewOverflow(NULL),
    m_NewDataAvailable(false),
    m_Mask(NULL),
    m_NewMask(NULL),
    m_NewMaskAvailable(false),
    m_Highlighter(NULL)
{
#ifndef QT_NO_DEBUG
  printf("Constructing main window\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdWindow::QxrdWindow(%p)\n", this);
  }
}

void QxrdWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  inherited::initialize(parent);

  m_WindowSettings =
      qSharedPointerDynamicCast<QxrdWindowSettings>(m_Parent);

  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);
  m_Splitter->setStretchFactor(2, 1);

  m_DatasetBrowserView -> setExperiment(QxrdExperiment::findExperiment(m_Parent));

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_Parent));

  if (exp) {
    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdProcessorPtr     proc(exp->processor());
    QxrdCenterFinderPtr  cf(proc?proc->centerFinder():QxrdCenterFinderWPtr());

    QxrdWindowSettingsPtr settings(m_WindowSettings);

    if (settings) {
      m_FileBrowserWidget -> initialize(settings->fileBrowserSettings(), exp, proc);
      m_ImagePlot         -> initialize(settings->imagePlotWidgetSettings(), proc);
      m_CenterFinderPlot  -> initialize(settings->centeringPlotWidgetSettings(), cf);
      m_IntegratorPlot    -> initialize(settings->integratedPlotWidgetSettings());
      m_DistortionCorrectionPlot -> initialize(settings->distortionPlotWidgetSettings());
    }
  }

  setAttribute(Qt::WA_DeleteOnClose, false);

  QxrdApplicationPtr    app(
        qSharedPointerDynamicCast<QxrdApplication>(QxrdAppCommon::findApplication(m_Parent)));
  QxrdExperimentPtr     expt(QxrdExperiment::findExperiment(m_Parent));
  QxrdWindowSettingsPtr set(m_WindowSettings);


  QxrdAcqCommonPtr   acq(QxrdAcqCommon::findAcquisition(m_Parent));
  QxrdProcessorPtr   proc(processor());

  connect(m_ExecuteScriptJSButton, &QAbstractButton::clicked, m_ActionExecuteScriptJS, &QAction::triggered);
  connect(m_ExecuteScriptButton, &QAbstractButton::clicked, m_ActionExecuteScript, &QAction::triggered);
  connect(m_ActionExecuteScript, &QAction::triggered, this, &QxrdWindow::executeScript);
  connect(m_ActionExecuteScriptJS, &QAction::triggered, this, &QxrdWindow::executeScriptJS);
  connect(m_CancelScriptButton, &QAbstractButton::clicked, m_ActionCancelScript, &QAction::triggered);
  connect(m_ActionCancelScript, &QAction::triggered, this, &QxrdWindow::cancelScript);
  connect(m_LoadScriptButton, &QAbstractButton::clicked, m_ActionLoadScript, &QAction::triggered);
  connect(m_ActionLoadScript, &QAction::triggered, this, &QxrdWindow::doLoadScript);

  if (app) {
    connect(m_ActionLoadPreferences, &QAction::triggered, app.data(), &QxrdApplication::doLoadPreferences);
    connect(m_ActionSavePreferences, &QAction::triggered, app.data(), &QxrdApplication::doSavePreferences);
  }

  connect(m_ActionSaveExperimentAsText, &QAction::triggered, this, &QxrdWindow::doSaveExperimentAsText);

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

  connect(m_ActionRefineCenterTilt, &QAction::triggered, this, &QxrdWindow::doRefineCenterTilt);

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

  if (proc) {
    connect(m_ActionShowMaskRange, &QAction::triggered, proc.data(), (void (QxrdProcessor::*)()) &QxrdProcessor::showMaskRange);
    connect(m_ActionHideMaskRange, &QAction::triggered, proc.data(), (void (QxrdProcessor::*)()) &QxrdProcessor::hideMaskRange);
    connect(m_ActionShowMaskAll, &QAction::triggered, proc.data(), &QxrdProcessor::showMaskAll);
    connect(m_ActionHideMaskAll, &QAction::triggered, proc.data(), &QxrdProcessor::hideMaskAll);
    connect(m_ActionInvertMask, &QAction::triggered, proc.data(), &QxrdProcessor::invertMask);
//    connect(m_ActionMaskCircles, &QAction::triggered, m_ImageMaskCirclesButton, &QAbstractButton::click);
//    connect(m_ActionMaskPolygons, &QAction::triggered, m_ImageMaskPolygonsButton, &QAbstractButton::click);

//    connect(m_ActionROICalculate, &QAction::triggered, proc.data(), &QxrdProcessor::doCalculateROI);
//    connect(m_ActionHistogramCalculate, &QAction::triggered, proc.data(), &QxrdProcessor::doCalculateHistogram);
  }

  if (app) {
    connect(m_ActionAboutQXRD, &QAction::triggered, app.data(), &QxrdApplication::doAboutQxrd);
    connect(m_ActionOpenQXRDWebPage, &QAction::triggered, app.data(), &QxrdApplication::doOpenQXRDWebPage);
  }

  connect(m_HelpHomeButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::home);
  connect(m_HelpForwardButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::forward);
  connect(m_HelpBackButton, &QAbstractButton::clicked, m_HelpBrowser, &QTextBrowser::backward);

  connect(m_HelpBrowser, &QTextBrowser::forwardAvailable, m_HelpForwardButton, &QWidget::setEnabled);
  connect(m_HelpBrowser, &QTextBrowser::backwardAvailable, m_HelpBackButton, &QWidget::setEnabled);

  m_HelpBrowser->initialize(QxrdExperiment::findExperiment(m_Parent));


  connect(m_ActionIntegrate, &QAction::triggered, this, &QxrdWindow::doIntegrateSequence);

  if (proc) {
    connect(m_ActionIntegrateCurrent, &QAction::triggered,
            proc.data(), &QxrdProcessor::integrateSaveAndDisplay);
  }

  connect(m_ActionSaveCachedGeometry, &QAction::triggered, this, &QxrdWindow::doSaveCachedGeometry);
  connect(m_ActionSaveCachedIntensity, &QAction::triggered, this, &QxrdWindow::doSaveCachedIntensity);

//  connect(m_IntegratorDialog -> m_IntegrateOptionsButton, &QAbstractButton::clicked, this, &QxrdWindow::doEditPreferences);

  if (proc) {
    connect(proc.data(), &QxrdProcessor::dataAvailable,
            m_ImagePlot, &QxrdImagePlotWidget::newImage);

    connect(proc.data(), &QxrdProcessor::maskAvailable,
            m_ImagePlot, &QxrdImagePlotWidget::newMask);

    connect(proc->integrator()->prop_IntegrationXUnits(), &QcepIntProperty::valueChanged,
            this, &QxrdWindow::integrationXUnitsChanged);
    integrationXUnitsChanged(proc->integrator()->get_IntegrationXUnits());

    connect(m_ActionIntegrateVsR,   &QAction::triggered, proc->integrator().data(), &QxrdIntegrator::integrateVsR);
    connect(m_ActionIntegrateVsQ,   &QAction::triggered, proc->integrator().data(), &QxrdIntegrator::integrateVsQ);
    connect(m_ActionIntegrateVsTTH, &QAction::triggered, proc->integrator().data(), &QxrdIntegrator::integrateVsTTH);
  }

  m_Highlighter = new QxrdHighlighter(m_ScriptEdit->document());

  QxrdToDoList *toDoList = new QxrdToDoList(this);
  QSortFilterProxyModel *sorted = new QSortFilterProxyModel(this);

  sorted->setSourceModel(toDoList);

  sorted->sort(0, Qt::DescendingOrder);

  m_ToDoList->setModel(sorted);
  m_ToDoList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_ToDoList->horizontalHeader()->setStretchLastSection(true);
  m_ToDoList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_ToDoList->setSelectionBehavior(QAbstractItemView::SelectRows);

  if (app) {
    m_Messages -> document() -> setMaximumBlockCount(app->get_MessageWindowLines());

    connect(app->prop_MessageWindowLines(), &QcepIntProperty::valueChanged, this, &QxrdWindow::onMessageWindowLinesChanged);

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
  }

  captureSize();
}

QxrdWindow::~QxrdWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting main window from thread %s\n", qPrintable(QThread::currentThread()->objectName()));
#endif  

  QxrdAppCommonPtr app(QxrdAppCommon::findApplication(m_Parent));

  if (app && qcepDebug(DEBUG_APP)) {
    app->printMessage("QxrdWindow::~QxrdWindow");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdWindow::~QxrdWindow(%p)\n", this);
  }
}

void QxrdWindow::onAcquisitionInit()
{
//  m_AcquisitionDialog->onAcquisitionInit();
}

void QxrdWindow::enableTiltRefinement(bool enable)
{
  m_ActionRefineCenterTilt->setEnabled(enable);
}
//TODO: rework
void QxrdWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    QxrdAppCommonPtr app(QxrdAppCommon::findApplication(m_Parent));

    if (app) {
      app->closeExperiment(QxrdExperiment::findExperiment(m_Parent));
    }
    event -> accept();
  } else {
    event -> ignore();
  }
}

//TODO: move to QcepApplication
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

//TODO: move to QcepApplication
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

//TODO: move to QxrdStartupWindow
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

//TODO: move to QxrdStartupWindow
void QxrdWindow::displayCriticalMessage(QString msg)
{
  if (QThread::currentThread()==thread()) {
    static int dialogCount = 0;

    QxrdExperimentPtr expt(QxrdExperiment::findExperiment(m_Parent));

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
//  updateTitle();

//  allocatedMemoryChanged();

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
  }
}

void QxrdWindow::doSaveExperimentAsText()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(QxrdExperiment::findExperiment(m_Parent));
  QxrdAppCommonPtr  app(QxrdAppCommon::findApplication(m_Parent));

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
//    QString dirp = expt->defaultExperimentDirectory(path);

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

void QxrdWindow::doSaveData()
{
  GUI_THREAD_CHECK;

  QxrdProcessorPtr proc(processor());

  if (proc) {
    if (proc->data() == NULL) {
      warningMessage("No data available to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Data in", proc -> get_DataPath());

      if (theFile.length()) {
        QxrdProcessor *p = proc.data();

#       if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
          INVOKE_CHECK(
              QMetaObject::invokeMethod(p,
                                        [=]() { p->saveData(theFile, QxrdProcessor::CanOverwrite);} ));
#       else
          INVOKE_CHECK(
                QMetaObject::invokeMethod(proc.data(),
                                          "saveData",
                                          Qt::BlockingQueuedConnection,
                                          Q_ARG(QString, theFile),
                                          Q_ARG(int, QxrdProcessor::CanOverwrite)));
#       endif
      }
    }
  }
}

void QxrdWindow::doLoadData()
{
  GUI_THREAD_CHECK;

  QxrdProcessorPtr proc(processor());

  if (proc) {
    QString theFile = QFileDialog::getOpenFileName(
          this, "Load Data from...", proc -> get_DataPath());

    if (theFile.length()) {
      QMetaObject::invokeMethod(proc.data(),
                                "loadData",
                                Qt::BlockingQueuedConnection,
                                Q_ARG(QString, theFile));
    }
  }
}

void QxrdWindow::doSaveDark()
{
  GUI_THREAD_CHECK;

  QxrdProcessorPtr proc(processor());

  if (proc) {
    if (proc->dark() == NULL) {
      warningMessage("No dark image available to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Dark Data in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveDark(theFile, QxrdProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadDark()
{
  GUI_THREAD_CHECK;

  QxrdProcessorPtr proc(processor());

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

  QxrdProcessorPtr proc(processor());

  if (proc) {
    if (proc->dark() == NULL) {
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

  QxrdProcessorPtr proc(processor());

  if (proc) {
    if (proc->mask() == NULL) {
      warningMessage("No mask image to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Mask in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveMask(theFile, QxrdProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadMask()
{
  GUI_THREAD_CHECK;

  QxrdProcessorPtr proc(processor());

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

  QxrdProcessorPtr proc(processor());

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

  QxrdProcessorPtr proc(processor());

  if (proc) {
    if (proc->badPixels() == NULL) {
      warningMessage("No Bad Pixel data to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Bad Pixels in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveBadPixels(theFile, QxrdProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadBadPixels()
{
  GUI_THREAD_CHECK;

  QxrdProcessorPtr proc(processor());

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

  QxrdProcessorPtr proc(processor());

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

  QxrdProcessorPtr proc(processor());

  if (proc) {
    if (proc->gainMap() == NULL) {
      warningMessage("No Gain Map available to save");
    } else {
      QString theFile = QFileDialog::getSaveFileName(
            this, "Save Gain Map in", proc -> get_DataPath());

      if (theFile.length()) {
        proc->saveGainMap(theFile, QxrdProcessor::CanOverwrite);
      }
    }
  }
}

void QxrdWindow::doLoadGainMap()
{
  GUI_THREAD_CHECK;

  QxrdProcessorPtr proc(processor());

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

  QxrdProcessorPtr proc(processor());

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

  QxrdProcessorPtr proc(processor());

  if (proc) {
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

  QxrdProcessorPtr proc(processor());

  if (proc) {
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
  QxrdExperimentPtr expt(QxrdExperiment::findExperiment(m_Parent));

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
  QxrdProcessorPtr proc(processor());

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
  QxrdExperimentPtr expt(QxrdExperiment::findExperiment(m_Parent));

  if (expt) {
    QxrdScriptEnginePtr eng(expt -> scriptEngine());

    if (eng) {
      expt -> printMessage(tr("Loading script file from %1").arg(path));
      eng -> loadScript(path);
    }
  }
}

QxrdProcessorWPtr QxrdWindow::processor() const
{
  return QxrdProcessor::findProcessor(m_Parent);
}

QcepDoubleImageDataPtr QxrdWindow::data()
{
  return m_Data;
}

QcepMaskDataPtr QxrdWindow::mask()
{
  return m_Mask;
}

void QxrdWindow::doRefineCenterTilt()
{
}

void QxrdWindow::doAccumulateImages()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to accumulate...",
                                                      proc -> get_DataPath());

    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "accumulateImages", Q_ARG(QStringList, files)));
  }
}

void QxrdWindow::doAddImages()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data file(s) to add to current...",
                                                      proc -> get_DataPath());

    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "addImages", Q_ARG(QStringList, files)));
  }
}

void QxrdWindow::doSubtractImages()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data file(s) to subtract from current...",
                                                      proc -> get_DataPath());

    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "subtractImages", Q_ARG(QStringList, files)));
  }
}

void QxrdWindow::doReflectHorizontally()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "reflectHorizontally"));
  }
}

void QxrdWindow::doReflectVertically()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "reflectVertically"));
  }
}

void QxrdWindow::doProjectAlongX()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to project along X...",
                                                      proc -> get_DataPath());

    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, files),
                              Q_ARG(int, 1), Q_ARG(int, 0), Q_ARG(int, 0)));
  }
}

void QxrdWindow::doProjectAlongY()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to project along Y...",
                                                      proc -> get_DataPath());

    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, files),
                                    Q_ARG(int, 0), Q_ARG(int, 1), Q_ARG(int, 0)));
  }
}

void QxrdWindow::doProjectAlongZ()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to project along Z...",
                                                      proc -> get_DataPath());

    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "projectImages", Q_ARG(QStringList, files),
                                    Q_ARG(int, 0), Q_ARG(int, 0), Q_ARG(int, 1)));
  }
}

void QxrdWindow::doCorrelate()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select data files to correlate with current image...",
                                                      proc -> get_DataPath());

    INVOKE_CHECK(
          QMetaObject::invokeMethod(proc.data(), "correlateImages", Q_ARG(QStringList, files)));
  }
}

void QxrdWindow::doIntegrateSequence()
{
  QxrdProcessorPtr proc(processor());

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
  QxrdProcessorPtr proc(processor());

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
