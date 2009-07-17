/******************************************************************
*
*  $Id: qxrdwindow.cpp,v 1.69 2009/07/17 12:41:33 jennings Exp $
*
*******************************************************************/

#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"
#include "qxrdacquisition.h"
#include "qxrdsettings.h"
#include "qxrdimageplot.h"
#include "qxrdimagedata.h"
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
//    m_ScriptEngine(NULL),
    m_CenterFinderDialog(NULL),
    m_CenterFinder(NULL),
    m_IntegratorDialog(NULL),
    m_Integrator(NULL),
    m_Progress(NULL),
    m_Acquiring(false),
    m_AcquiringDark(false),
    SOURCE_IDENT("$Id: qxrdwindow.cpp,v 1.69 2009/07/17 12:41:33 jennings Exp $")
{
  setupUi(this);

  m_CenterFinderDialog = new QxrdCenterFinderDialog();
  m_CenteringDockWidget -> setWidget(m_CenterFinderDialog);

  m_CenterFinder = new QxrdCenterFinder(this, m_Plot, m_CenterFinderPlot, m_CenterFinderDialog, this);
  m_CenterFinder -> setEnabled(false, true);

  m_IntegratorDialog = new QxrdIntegratorDialog();
  m_IntegratorDockWidget -> setWidget(m_IntegratorDialog);

  m_Integrator = new QxrdIntegrator(this);

  connect(m_ExecuteScriptButton, SIGNAL(clicked()), m_ActionExecuteScript, SIGNAL(triggered()));
  connect(m_ActionExecuteScript, SIGNAL(triggered()), this, SLOT(executeScript()));
  connect(m_CancelScriptButton, SIGNAL(clicked()), m_ActionCancelScript, SIGNAL(triggered()));
  connect(m_ActionCancelScript, SIGNAL(triggered()), this, SLOT(cancelScript()));

  connect(m_ActionAutoScale, SIGNAL(triggered()), m_Plot, SLOT(autoScale()));
  connect(m_ActionQuit, SIGNAL(triggered()), m_Application, SLOT(possiblyQuit()));
  connect(m_ActionLoadData, SIGNAL(triggered()), this, SLOT(doLoadData()));
  connect(m_ActionSaveData, SIGNAL(triggered()), this, SLOT(doSaveData()));

  connect(m_SelectDirectoryButton, SIGNAL(clicked()), this, SLOT(selectOutputDirectory()));

  connect(m_LoadDarkButton, SIGNAL(clicked()), this, SLOT(doLoadDarkImage()));
  connect(m_LoadBadPixelsButton, SIGNAL(clicked()), this, SLOT(doLoadBadPixels()));
  connect(m_LoadGainCorrection, SIGNAL(clicked()), this, SLOT(doLoadGainMap()));

  connect(m_AcquireButton, SIGNAL(clicked()), m_ActionAcquire, SIGNAL(triggered()));
  connect(m_CancelButton, SIGNAL(clicked()), m_ActionCancel, SIGNAL(triggered()));
  connect(m_DarkAcquireButton, SIGNAL(clicked()), m_ActionAcquireDark, SIGNAL(triggered()));
  connect(m_DarkCancelButton, SIGNAL(clicked()), m_ActionCancelDark, SIGNAL(triggered()));

  connect(m_ActionAcquire, SIGNAL(triggered()), this, SLOT(doAcquire()));
  connect(m_ActionCancel, SIGNAL(triggered()), this, SLOT(doCancel()));
  connect(m_ActionAcquireDark, SIGNAL(triggered()), this, SLOT(doAcquireDark()));
  connect(m_ActionCancelDark, SIGNAL(triggered()), this, SLOT(doCancelDark()));

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

  connect(m_ActionShowImage, SIGNAL(triggered()), m_Plot, SLOT(toggleShowImage()));
  connect(m_ActionShowMask, SIGNAL(triggered()), m_Plot, SLOT(toggleShowMask()));
  connect(m_ActionShowMaskRange, SIGNAL(triggered()), this, SLOT(showMaskRange()));
  connect(m_ActionHideMaskRange, SIGNAL(triggered()), this, SLOT(hideMaskRange()));
  connect(m_ActionShowMaskAll, SIGNAL(triggered()), this, SLOT(showMaskAll()));
  connect(m_ActionHideMaskAll, SIGNAL(triggered()), this, SLOT(hideMaskAll()));

  connect(m_ActionTest, SIGNAL(triggered()), this, SLOT(doTest()));

  connect(m_ImageZoomInButton, SIGNAL(clicked()), m_Plot, SLOT(doZoomIn()));
  connect(m_ImageZoomOutButton, SIGNAL(clicked()), m_Plot, SLOT(doZoomOut()));
  connect(m_ImageZoomAllButton, SIGNAL(clicked()), m_Plot, SLOT(doZoomAll()));
  connect(m_ImageSetCenterButton, SIGNAL(clicked()), m_Plot, SLOT(doSetCenter()));
  connect(m_ImageSliceButton, SIGNAL(clicked()), m_Plot, SLOT(doSlice()));
  connect(m_ImageMeasureButton, SIGNAL(clicked()), m_Plot, SLOT(doMeasure()));

  connect(m_CenteringZoomInButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(doZoomIn()));
  connect(m_CenteringZoomOutButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(doZoomOut()));
  connect(m_CenteringZoomAllButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(doZoomAll()));

  connect(m_IntegratorZoomInButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(doZoomIn()));
  connect(m_IntegratorZoomOutButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(doZoomOut()));
  connect(m_IntegratorZoomAllButton, SIGNAL(clicked()), m_IntegratorPlot, SLOT(doZoomAll()));

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
    printf("Oh no...\n");
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

  m_Acquisition -> prop_ReadoutMode() -> linkTo(m_ReadoutMode);
  m_Acquisition -> prop_ExposureTime() -> linkTo(m_ExposureTime);
  m_Acquisition -> prop_SummedExposures() -> linkTo(m_SummedExposures);
  m_Acquisition -> prop_DarkSummedExposures() -> linkTo(m_DarkSummedExposures);
  m_Acquisition -> prop_FilesInSequence() -> linkTo(m_FilesInSequence);
  m_Acquisition -> prop_FilePattern() -> linkTo(m_FilePattern);
  m_Acquisition -> prop_FileIndex() -> linkTo(m_FileIndex);

  m_DataProcessor -> prop_OutputDirectory() -> linkTo(m_OutputDirectory);
  m_DataProcessor -> prop_PerformDarkSubtraction() -> linkTo(m_PerformDark);
  m_DataProcessor -> prop_SaveRawImages() -> linkTo(m_SaveRaw);
  m_DataProcessor -> prop_PerformBadPixels() -> linkTo(m_PerformBadPixels);
  m_DataProcessor -> prop_PerformGainCorrection() -> linkTo(m_PerformGainCorrection);

  m_DataProcessor -> prop_DarkImagePath() -> linkTo(m_DarkImagePath);
  m_DataProcessor -> prop_BadPixelsPath() -> linkTo(m_BadPixelsPath);
  m_DataProcessor -> prop_GainMapPath() -> linkTo(m_GainMapPath);

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

//  connect(m_DataProcessor, SIGNAL(processedImageAvailable()), this, SLOT(onProcessedImageAvailable()));
//  connect(m_DataProcessor, SIGNAL(darkImageAvailable()), this, SLOT(onDarkImageAvailable()));
//  connect(m_DataProcessor, SIGNAL(printMessage(QString)), this, SLOT(printMessage(QString)));

//  readSettings();

  connect(m_Acquisition, SIGNAL(oneReadoutModeChanged(int,double)),
          this,          SLOT(setReadoutTime(int, double)));
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
  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window?"),
                                  QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

QString QxrdWindow::timeStamp()
{
  return QDateTime::currentDateTime().toString("yyyy.MM.dd : hh:mm:ss ");
}

void QxrdWindow::printMessage(QString msg)
{
//  printf("%s\n", qPrintable(msg));
//
  m_Messages -> append(timeStamp()+msg.trimmed());
}

void QxrdWindow::criticalMessage(QString msg)
{
  QMessageBox::critical(this, "Error", msg);
}

void QxrdWindow::acquisitionReady()
{
//  readSettings();

  m_AcquireButton -> setEnabled(true);
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
  m_AcquireButton -> setEnabled(false);
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
  m_AcquireButton -> setEnabled(true);
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
  while (n >= m_ReadoutMode->count()) {
    m_ReadoutMode -> addItem("");
    m_Exposures.append(0);
  }

  m_ReadoutMode -> setItemText(n, tr("%1 msec").arg(t/1e3,0,'f',0));
  m_Exposures[n] = t;
}

void QxrdWindow::selectOutputDirectory()
{
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
//  printMessage("QxrdWindow::doAcquire()\n");

  acquisitionStarted();

  m_Acquiring = true;

  QMetaObject::invokeMethod(m_Acquisition, "doAcquire");
}

void QxrdWindow::doAcquireDark()
{
  darkAcquisitionStarted();

  m_AcquiringDark = true;

  QMetaObject::invokeMethod(m_Acquisition, "doAcquireDark");
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
  m_CenterFinder -> readSettings(settings, section+"/centerfinder");
  m_Integrator   -> readSettings(settings, section+"/integrator");
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
  m_CenterFinder -> writeSettings(settings, section+"/centerfinder");
  m_Integrator   -> writeSettings(settings, section+"/integrator");
  m_Plot         -> writeSettings(settings, section+"/plot");

  settings -> setValue(section+"-geometry", saveGeometry());
  settings -> setValue(section+"-state", saveState(1));

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdWindow::statusMessage(QString msg)
{
  m_StatusMsg -> setText(msg);

  m_StatusTimer.start(5000);
}

void QxrdWindow::clearStatusMessage()
{
  m_StatusMsg -> setText("");
}

void QxrdWindow::doSaveData()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Data in", m_DataProcessor -> get_OutputDirectory());

  if (theFile.length()) {
    QMetaObject::invokeMethod(m_DataProcessor, "saveData", Q_ARG(QString, theFile));
  }
}

void QxrdWindow::doLoadData()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Image from...", m_DataProcessor -> get_OutputDirectory());

  if (theFile.length()) {
    QMetaObject::invokeMethod(m_DataProcessor, "loadData", Q_ARG(QString, theFile));
  }
}

void QxrdWindow::doLoadDarkImage()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Dark Image from...", m_DataProcessor -> get_DarkImagePath());

  if (theFile.length()) {
    QMetaObject::invokeMethod(m_DataProcessor, "loadDarkImage", Q_ARG(QString, theFile));
  }
}

void QxrdWindow::doLoadBadPixels()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Bad Pixel Map from...", m_DataProcessor -> get_BadPixelsPath());

  if (theFile.length()) {
    QMetaObject::invokeMethod(m_DataProcessor, "loadBadPixels", Q_ARG(QString, theFile));
  }
}

void QxrdWindow::doLoadGainMap()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Pixel Gain Map from...", m_DataProcessor -> get_GainMapPath());

  if (theFile.length()) {
    QMetaObject::invokeMethod(m_DataProcessor, "loadGainMap", Q_ARG(QString, theFile));
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

QxrdCenterFinder  *QxrdWindow::centerFinder() const
{
  QMutexLocker  lock(&m_Mutex);

  return m_CenterFinder;
}

QxrdIntegrator    *QxrdWindow::integrator() const
{
  QMutexLocker  lock(&m_Mutex);

  return m_Integrator;
}

  /******************************************************************
*
*  $Log: qxrdwindow.cpp,v $
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

