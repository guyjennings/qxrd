/******************************************************************
*
*  $Id: qxrdwindow.cpp,v 1.97 2009/09/21 19:51:11 jennings Exp $
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
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"
#include "qxrdplotzoomer.h"
#include "qxrdscriptengine.h"

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

QxrdWindow::QxrdWindow(QxrdApplication *app, QxrdAcquisition *acq, QxrdDataProcessor *proc, QWidget *parent)
  : QMainWindow(parent),
    m_SettingsLoaded(false),
    m_Application(app),
    m_Acquisition(acq),
    m_DataProcessor(proc),
    m_CenterFinderDialog(NULL),
    m_IntegratorDialog(NULL),
    m_Progress(NULL),
    m_Acquiring(false),
    m_AcquiringDark(false),
    m_Data(new QxrdDoubleImageData(2048,2048)),
    m_SpareData(new QxrdDoubleImageData(2048,2048)),
    SOURCE_IDENT("$Id: qxrdwindow.cpp,v 1.97 2009/09/21 19:51:11 jennings Exp $")
{
  setupUi(this);

  setWindowTitle(windowTitle()+" - v"+VERSION);

  m_CenterFinderDialog = new QxrdCenterFinderDialog(m_DataProcessor -> centerFinder());
  m_CenteringDockWidget -> setWidget(m_CenterFinderDialog);

  m_IntegratorDialog = new QxrdIntegratorDialog(m_DataProcessor -> integrator());
  m_IntegratorDockWidget -> setWidget(m_IntegratorDialog);

  connect(m_ExecuteScriptButton, SIGNAL(clicked()), m_ActionExecuteScript, SIGNAL(triggered()));
  connect(m_ActionExecuteScript, SIGNAL(triggered()), this, SLOT(executeScript()));
  connect(m_CancelScriptButton, SIGNAL(clicked()), m_ActionCancelScript, SIGNAL(triggered()));
  connect(m_ActionCancelScript, SIGNAL(triggered()), this, SLOT(cancelScript()));

  connect(m_ActionAutoScale, SIGNAL(triggered()), m_Plot, SLOT(autoScale()));
  connect(m_ActionQuit, SIGNAL(triggered()), m_Application, SLOT(possiblyQuit()));
  connect(m_ActionLoadData, SIGNAL(triggered()), this, SLOT(doLoadData()));
  connect(m_ActionSaveData, SIGNAL(triggered()), this, SLOT(doSaveData()));
  connect(m_ActionLoadDark, SIGNAL(triggered()), this, SLOT(doLoadDark()));
  connect(m_ActionSaveDark, SIGNAL(triggered()), this, SLOT(doSaveDark()));
  connect(m_ActionLoadMask, SIGNAL(triggered()), this, SLOT(doLoadMask()));
  connect(m_ActionSaveMask, SIGNAL(triggered()), this, SLOT(doSaveMask()));
  connect(m_ActionSelectLogFile, SIGNAL(triggered()), this, SLOT(selectLogFile()));
  connect(m_SelectLogFileButton, SIGNAL(clicked()), this, SLOT(selectLogFile()));
  connect(m_SelectDirectoryButton, SIGNAL(clicked()), this, SLOT(selectOutputDirectory()));

//  connect(m_LoadDarkButton, SIGNAL(clicked()), this, SLOT(doLoadDarkImage()));
//  connect(m_LoadBadPixelsButton, SIGNAL(clicked()), this, SLOT(doLoadBadPixels()));
//  connect(m_LoadGainCorrection, SIGNAL(clicked()), this, SLOT(doLoadGainMap()));

  connect(m_AcquireButton, SIGNAL(clicked()), m_ActionAcquire, SIGNAL(triggered()));
  connect(m_CancelButton, SIGNAL(clicked()), m_ActionCancel, SIGNAL(triggered()));
  connect(m_DarkAcquireButton, SIGNAL(clicked()), m_ActionAcquireDark, SIGNAL(triggered()));
  connect(m_DarkCancelButton, SIGNAL(clicked()), m_ActionCancelDark, SIGNAL(triggered()));
  connect(m_TriggerButton, SIGNAL(clicked()), m_ActionTrigger, SIGNAL(triggered()));

  connect(m_ActionAcquire, SIGNAL(triggered()), this, SLOT(doAcquire()));
  connect(m_ActionCancel, SIGNAL(triggered()), this, SLOT(doCancel()));
  connect(m_ActionAcquireDark, SIGNAL(triggered()), this, SLOT(doAcquireDark()));
  connect(m_ActionCancelDark, SIGNAL(triggered()), this, SLOT(doCancelDark()));
  connect(m_ActionTrigger, SIGNAL(triggered()), m_Acquisition, SLOT(trigger()));

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

  connect(&m_StatusTimer, SIGNAL(timeout()), this, SLOT(clearStatusMessage()));

  for (int i=0; i<8; i++) {
    m_ReadoutMode -> addItem(tr("Item %1").arg(i));
    m_Exposures.append(0);
  }

  m_StatusMsg = new QLabel(NULL);
  m_StatusMsg -> setMinimumWidth(200);
  m_StatusMsg -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  statusBar() -> addPermanentWidget(m_StatusMsg);

  m_Progress = new QProgressBar(NULL);
  m_Progress -> setMinimumWidth(150);
  m_Progress -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  statusBar() -> addPermanentWidget(m_Progress);

  if (m_Acquisition == NULL) {
    emit criticalMessage("Oh no, QxrdWindow::m_Acquisition == NULL");
  }

  connect(m_Acquisition,     SIGNAL(statusMessage(QString)),
          this,              SLOT(statusMessage(QString)));
  connect(m_Acquisition,     SIGNAL(printMessage(QString)),
          this,              SLOT(printMessage(QString)));
  connect(m_Acquisition,     SIGNAL(criticalMessage(QString)),
          this,              SLOT(criticalMessage(QString)));

  connect(m_Acquisition,     SIGNAL(acquireStarted(int)),
          this,              SLOT(onAcquireStarted(int)));
  connect(m_Acquisition,     SIGNAL(acquiredFrame(QString,int,int,int,int,int)),
          this,              SLOT(onAcquiredFrame(QString,int,int,int,int,int)));
  connect(m_Acquisition,     SIGNAL(acquireComplete(int)),
          this,              SLOT(onAcquireComplete(int)));

//  show();

  m_Acquisition -> setupCameraGainMenu(m_CameraGain);
  m_Acquisition -> setupCameraBinningModeMenu(m_BinningMode);

  m_Acquisition -> prop_ReadoutMode() -> linkTo(m_ReadoutMode);
  m_Acquisition -> prop_ExposureTime() -> linkTo(m_ExposureTime);
  m_Acquisition -> prop_SummedExposures() -> linkTo(m_SummedExposures);
  m_Acquisition -> prop_DarkSummedExposures() -> linkTo(m_DarkSummedExposures);
  m_Acquisition -> prop_FilePattern() -> linkTo(m_FilePattern);
  m_Acquisition -> prop_FileIndex() -> linkTo(m_FileIndex);
  m_Acquisition -> prop_PreTriggerFiles() -> linkTo(m_PreTriggerFiles);
  m_Acquisition -> prop_PostTriggerFiles() -> linkTo(m_PostTriggerFiles);
  m_Acquisition -> prop_CameraGain() -> linkTo(m_CameraGain);
  m_Acquisition -> prop_BinningMode() -> linkTo(m_BinningMode);

  m_DataProcessor -> prop_OutputDirectory() -> linkTo(m_OutputDirectory);
  m_DataProcessor -> prop_PerformDarkSubtraction() -> linkTo(m_PerformDark);
  m_DataProcessor -> prop_SaveRawImages() -> linkTo(m_SaveRaw);
  m_DataProcessor -> prop_PerformBadPixels() -> linkTo(m_PerformBadPixels);
  m_DataProcessor -> prop_PerformGainCorrection() -> linkTo(m_PerformGainCorrection);
  m_DataProcessor -> prop_SaveSubtracted() -> linkTo(m_SaveSubtracted);
  m_DataProcessor -> prop_PerformIntegration() -> linkTo(m_PerformIntegration);
  m_DataProcessor -> prop_DisplayIntegratedData() -> linkTo(m_DisplayIntegratedData);
  m_DataProcessor -> prop_SaveIntegratedData() -> linkTo(m_SaveIntegratedData);
  m_DataProcessor -> prop_EstimatedProcessingTime() -> linkTo(m_EstimatedProcessingTime);
  m_DataProcessor -> prop_LogFilePath() -> linkTo(m_LogFilePath);

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

  m_Plot -> prop_DisplayScalingMode() -> linkTo(Ui::QxrdWindow::m_DisplayScalingMode);
  m_Plot -> prop_DisplayColorMap() -> linkTo(Ui::QxrdWindow::m_DisplayColorMap);

  m_Plot -> prop_ImageShown() -> linkTo(Ui::QxrdWindow::m_DisplayImage);
  m_Plot -> prop_MaskShown() -> linkTo(Ui::QxrdWindow::m_DisplayMask);
  m_Plot -> prop_InterpolatePixels() -> linkTo(Ui::QxrdWindow::m_InterpolatePixels);
  m_Plot -> prop_MaintainAspectRatio() -> linkTo(Ui::QxrdWindow::m_MaintainAspectRatio);

  m_Plot -> setDataProcessor(m_DataProcessor);
  m_CenterFinderPlot -> setDataProcessor(m_DataProcessor);
  m_IntegratorPlot -> setDataProcessor(m_DataProcessor);

//  connect(m_DataProcessor, SIGNAL(newDataAvailable(QxrdDoubleImageData *)),
//          m_Plot, SLOT(onProcessedImageAvailable(QxrdDoubleImageData *)));
  connect(m_DataProcessor, SIGNAL(newMaskAvailable(QxrdDoubleImageData *, QxrdMaskData *)),
          m_Plot, SLOT(onMaskedImageAvailable(QxrdDoubleImageData *, QxrdMaskData *)));

  connect(m_DataProcessor, SIGNAL(newDarkImageAvailable(QxrdDoubleImageData *)),
          m_Plot, SLOT(onDarkImageAvailable(QxrdDoubleImageData *)));

//  connect(m_DataProcessor, SIGNAL(newDataAvailable(QxrdDoubleImageData *)),
//          m_CenterFinderPlot, SLOT(onProcessedImageAvailable(QxrdDoubleImageData *)));

  connect(m_DataProcessor, SIGNAL(newMaskAvailable(QxrdDoubleImageData *, QxrdMaskData *)),
          m_CenterFinderPlot, SLOT(onMaskedImageAvailable(QxrdDoubleImageData *, QxrdMaskData *)));

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

  connect(m_Acquisition, SIGNAL(oneReadoutModeChanged(int,double)),
          this,          SLOT(setReadoutTime(int, double)));

  connect(m_DataProcessor -> centerFinder() -> prop_CenterX(), SIGNAL(changedValue(double)),
          m_Plot, SLOT(onCenterXChanged(double)));

  connect(m_DataProcessor -> centerFinder() -> prop_CenterY(), SIGNAL(changedValue(double)),
          m_Plot, SLOT(onCenterYChanged(double)));

  connect(m_DataProcessor -> centerFinder() -> prop_CenterX(), SIGNAL(changedValue(double)),
          m_CenterFinderPlot, SLOT(onCenterXChanged(double)));

  connect(m_DataProcessor -> centerFinder() -> prop_CenterY(), SIGNAL(changedValue(double)),
          m_CenterFinderPlot, SLOT(onCenterYChanged(double)));

  connect(m_DataProcessor -> integrator(), SIGNAL(newIntegrationAvailable(QVector<double>, QVector<double>)),
          m_IntegratorPlot, SLOT(onNewIntegrationAvailable(QVector<double>,QVector<double>)));

  m_WindowsMenu -> addAction(m_AcquireDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CorrectionDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_MaskingDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_DisplayDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_CenteringDockWidget -> toggleViewAction());
  m_WindowsMenu -> addAction(m_IntegratorDockWidget -> toggleViewAction());
}

QxrdWindow::~QxrdWindow()
{
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
    QMetaObject::invokeMethod(this, "printMessage", Qt::QueuedConnection, Q_ARG(QString, msg));
  } else {
    m_Messages -> append(timeStamp()+msg.trimmed());

    m_DataProcessor -> logMessage(msg);
  }
}

void QxrdWindow::criticalMessage(QString msg)
{
//  printf("critical message %s\n", qPrintable(msg));

  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "criticalMessage", Qt::QueuedConnection, Q_ARG(QString, msg));
  } else {
    statusMessage(msg);
    QMessageBox::critical(this, "Error", msg);
  }
}

void QxrdWindow::acquisitionReady()
{
//  readSettings();
  THREAD_CHECK;

  m_AcquireButton -> setEnabled(true);
  m_TriggerButton -> setEnabled(false);
  m_CancelButton -> setEnabled(false);
  m_ActionAcquire -> setEnabled(true);
  m_ActionCancel -> setEnabled(false);
  m_DarkAcquireButton -> setEnabled(true);
  m_DarkCancelButton -> setEnabled(false);
  m_ActionAcquireDark -> setEnabled(true);
  m_ActionCancelDark -> setEnabled(false);
  m_Progress -> reset();
}

void QxrdWindow::acquisitionStarted()
{
  THREAD_CHECK;

  if (m_Acquisition -> get_PreTriggerFiles() > 0) {
    m_TriggerButton -> setEnabled(true);
  } else {
    m_TriggerButton -> setEnabled(false);
  }

  m_AcquireButton -> setEnabled(false);
  m_CancelButton -> setEnabled(true);
  m_ActionAcquire -> setEnabled(false);
  m_ActionCancel -> setEnabled(true);
  m_DarkAcquireButton -> setEnabled(false);
  m_DarkCancelButton -> setEnabled(false);
  m_ActionAcquireDark -> setEnabled(false);
  m_ActionCancelDark -> setEnabled(false);
  m_Progress -> setValue(0);
}

void QxrdWindow::darkAcquisitionStarted()
{
  THREAD_CHECK;

  m_AcquireButton -> setEnabled(false);
  m_TriggerButton -> setEnabled(false);
  m_CancelButton -> setEnabled(false);
  m_ActionAcquire -> setEnabled(false);
  m_ActionCancel -> setEnabled(false);
  m_DarkAcquireButton -> setEnabled(false);
  m_DarkCancelButton -> setEnabled(true);
  m_ActionAcquireDark -> setEnabled(false);
  m_ActionCancelDark -> setEnabled(true);
  m_Progress -> setValue(0);
}

void QxrdWindow::acquisitionFinished()
{
  THREAD_CHECK;

  m_AcquireButton -> setEnabled(true);
  m_TriggerButton -> setEnabled(false);
  m_CancelButton -> setEnabled(false);
  m_ActionAcquire -> setEnabled(true);
  m_ActionCancel -> setEnabled(false);
  m_DarkAcquireButton -> setEnabled(true);
  m_DarkCancelButton -> setEnabled(false);
  m_ActionAcquireDark -> setEnabled(true);
  m_ActionCancelDark -> setEnabled(false);
  m_Progress -> reset();
}

void QxrdWindow::setReadoutTime(int n, double t)
{
  THREAD_CHECK;

  while (n >= m_ReadoutMode->count()) {
    m_ReadoutMode -> addItem("");
    m_Exposures.append(0);
  }

  m_ReadoutMode -> setItemText(n, tr("%1 msec").arg(t/1e3,0,'f',0));
  m_Exposures[n] = t;
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

  QMetaObject::invokeMethod(m_Acquisition, "acquire");
}

void QxrdWindow::doAcquireDark()
{
  darkAcquisitionStarted();

  m_AcquiringDark = true;

  QMetaObject::invokeMethod(m_Acquisition, "acquireDark");
}

void QxrdWindow::doCancel()
{
  if (m_Acquiring) {
    QMetaObject::invokeMethod(m_Acquisition, "cancel");
  }
}

void QxrdWindow::doCancelDark()
{
  if (m_AcquiringDark) {
    QMetaObject::invokeMethod(m_Acquisition, "cancelDark");
  }
}

void QxrdWindow::readSettings(QxrdSettings *settings, QString section)
{
  m_Plot         -> readSettings(settings, section+"/plot");

  m_SettingsLoaded = true;

  QByteArray geometry = settings -> value(section+"-geometry").toByteArray();
  QByteArray winstate = settings -> value(section+"-state").toByteArray();

  restoreGeometry(geometry);
  restoreState(winstate,1);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdWindow::writeSettings(QxrdSettings *settings, QString section)
{
  m_Plot         -> writeSettings(settings, section+"/plot");

  settings -> setValue(section+"-geometry", saveGeometry());
  settings -> setValue(section+"-state", saveState(1));

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

QxrdDoubleImageData *QxrdWindow::newDataAvailable(QxrdDoubleImageData *image)
{
  QMutexLocker lock(&m_NewDataMutex);
  QxrdDoubleImageData *res;

  if (m_Plotting) {
    emit printMessage("Already plotting...");

    res = m_SpareData;
    m_SpareData = image;

    if (!m_SpareDataAvailable) {
      QMetaObject::invokeMethod(this, "spareData", Qt::QueuedConnection);
    }

    m_SpareDataAvailable = true;
  } else {
    emit printMessage("Not already plotting...");

    res = m_Data;
    m_Data = image;
    QMetaObject::invokeMethod(this, "newData", Qt::QueuedConnection);
  }

  return res;
}

void QxrdWindow::newData()
{
  if (m_Plotting.testAndSetOrdered(0,1)) {
    emit printMessage("QxrdWindow::newData called, not already plotting");
    m_Plot -> onProcessedImageAvailable(m_Data);
    m_CenterFinderPlot -> onProcessedImageAvailable(m_Data);
    m_Plotting = 0;
    emit printMessage("plotting completed");
  } else {
    emit printMessage("QxrdWindow::newData called, but already plotting");
  }
}

void QxrdWindow::spareData()
{
  bool canDo;

  {
    QMutexLocker lock(&m_NewDataMutex);

    canDo = m_Plotting == 0;

    if (canDo) {
      emit printMessage("QxrdWindow swap data & spare");

      QxrdDoubleImageData *tmp = m_Data;
      m_Data = m_SpareData;
      m_SpareData = tmp;
    }
  }

  if (canDo) {
    newData();
  }

  emit printMessage("QxrdWindow::spareData");
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
  } else {
    m_Messages -> append(result.toString());
  }

  m_CancelScriptButton  -> setEnabled(false);
  m_ActionCancelScript  -> setEnabled(false);
  m_ExecuteScriptButton -> setEnabled(true);
  m_ActionExecuteScript -> setEnabled(true);
}

QxrdScriptEngine *QxrdWindow::scriptEngine() const
{
  return m_ScriptEngine;
}

void QxrdWindow::setScriptEngine(QxrdScriptEngine *engine)
{
  m_ScriptEngine = engine;
}

  /******************************************************************
*
*  $Log: qxrdwindow.cpp,v $
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

