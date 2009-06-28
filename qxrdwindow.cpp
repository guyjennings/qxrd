/******************************************************************
*
*  $Id: qxrdwindow.cpp,v 1.60 2009/06/28 04:04:52 jennings Exp $
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

#include "tiffio.h"

QxrdWindow::QxrdWindow(QxrdApplication *app, QxrdAcquisitionThread *acq, QWidget *parent)
  : QMainWindow(parent),
    m_SettingsLoaded(false),
    m_Application(app),
    m_AcquisitionThread(acq),
    m_Acquisition(NULL),
    m_DataProcessor(NULL),
    m_CenterFinderDialog(NULL),
    m_CenterFinder(NULL),
    m_IntegratorDialog(NULL),
    m_Integrator(NULL),
    m_Progress(NULL),
    m_Acquiring(false),
    m_AcquiringDark(false),
    m_Data(new QxrdImageData(2048,2048)),
    m_DarkFrame(NULL),
    m_BadPixels(NULL),
    m_GainFrame(NULL),
    SOURCE_IDENT("$Id: qxrdwindow.cpp,v 1.60 2009/06/28 04:04:52 jennings Exp $")
{
  setupUi(this);

  m_CenterFinderDialog = new QxrdCenterFinderDialog();

  QLayout *l = m_CenteringPage -> layout();

  if (l) {
    l -> addWidget(m_CenterFinderDialog);
    l -> addItem(new QSpacerItem(20, 372, QSizePolicy::Minimum, QSizePolicy::Expanding));
  }

  m_CenterFinder = new QxrdCenterFinder(this, m_Plot, m_CenterFinderPlot, m_CenterFinderDialog, this);
  m_CenterFinder -> setEnabled(false, true);

  m_IntegratorDialog = new QxrdIntegratorDialog();

  QLayout *ll = m_IntegratorPage -> layout();

  if (ll) {
    ll -> addWidget(m_IntegratorDialog);
    ll -> addItem(new QSpacerItem(20, 372, QSizePolicy::Minimum, QSizePolicy::Expanding));
  }

  m_Integrator = new QxrdIntegrator(this);

  connect(m_ControlToolBox, SIGNAL(currentChanged(int)), this, SLOT(onToolBoxPageChanged(int)));
  connect(m_XRDTabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabWidgetPageChanged(int)));

  connect(m_ExecuteScriptButton, SIGNAL(clicked()), m_ActionExecuteScript, SIGNAL(triggered()));
  connect(m_ActionExecuteScript, SIGNAL(triggered()), this, SLOT(executeScript()));
  connect(m_ActionAutoScale, SIGNAL(triggered()), m_Plot, SLOT(autoScale()));
  connect(m_ActionQuit, SIGNAL(triggered()), m_Application, SLOT(possiblyQuit()));
  connect(m_ActionLoadData, SIGNAL(triggered()), this, SLOT(doLoadData()));
//  connect(m_ActionImportData, SIGNAL(triggered()), this, SLOT(doImportData()));
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

  connect(m_DisplayMinimum, SIGNAL(valueChanged(double)), m_Plot, SLOT(on_minimum_changed(double)));
  connect(m_DisplayMaximum, SIGNAL(valueChanged(double)), m_Plot, SLOT(on_maximum_changed(double)));

  connect(m_InterpolatePixels, SIGNAL(stateChanged(int)), m_Plot, SLOT(on_interpolate_changed(int)));
  connect(m_MaintainAspectRatio, SIGNAL(stateChanged(int)), m_Plot, SLOT(on_maintain_aspect_changed(int)));

  connect(m_Plot, SIGNAL(minimum_changed(double)), m_DisplayMinimum, SLOT(setValue(double)));
  connect(m_Plot, SIGNAL(maximum_changed(double)), m_DisplayMaximum, SLOT(setValue(double)));

  connect(m_AutoRange, SIGNAL(clicked()), m_ActionAutoRange, SIGNAL(triggered()));
  connect(m_Display_5pct, SIGNAL(clicked()), m_Action005Range, SIGNAL(triggered()));
  connect(m_Display_10pct, SIGNAL(clicked()), m_Action010Range, SIGNAL(triggered()));
  connect(m_Display_100pct, SIGNAL(clicked()), m_Action100Range, SIGNAL(triggered()));

  connect(m_Action005Range, SIGNAL(triggered()), m_Plot, SLOT(set005Range()));
  connect(m_Action010Range, SIGNAL(triggered()), m_Plot, SLOT(set010Range()));
  connect(m_Action100Range, SIGNAL(triggered()), m_Plot, SLOT(set100Range()));
  connect(m_ActionAutoRange, SIGNAL(triggered()), m_Plot, SLOT(setAutoRange()));

  connect(m_GrayscaleButton, SIGNAL(clicked()), m_ActionGrayscale, SIGNAL(triggered()));
  connect(m_InvGrayscaleButton, SIGNAL(clicked()), m_ActionInverseGrayscale, SIGNAL(triggered()));
  connect(m_EarthTonesButton, SIGNAL(clicked()), m_ActionEarthTones, SIGNAL(triggered()));
  connect(m_SpectrumButton, SIGNAL(clicked()), m_ActionSpectrum, SIGNAL(triggered()));
  connect(m_FireButton, SIGNAL(clicked()), m_ActionFire, SIGNAL(triggered()));
  connect(m_IceButton, SIGNAL(clicked()), m_ActionIce, SIGNAL(triggered()));

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

  onAcquisitionRunning();
}

QxrdWindow::~QxrdWindow()
{
//  printf("QxrdWindow::~QxrdWindow()\n");

  if (m_SettingsLoaded) {
    writeSettings();
  }
}

void QxrdWindow::onAcquisitionRunning()
{
  m_Acquisition = m_AcquisitionThread -> acquisition();

  if (m_Acquisition == NULL) {
    printf("Oh no...\n");
  }

  connect(m_AcquisitionThread, SIGNAL(printMessage(QString)),
          this,              SLOT(printMessage(QString)));

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

  m_Acquisition -> prop_ReadoutMode() -> linkTo(m_ReadoutMode);
  m_Acquisition -> prop_ExposureTime() -> linkTo(m_ExposureTime);
  m_Acquisition -> prop_SummedExposures() -> linkTo(m_SummedExposures);
  m_Acquisition -> prop_DarkSummedExposures() -> linkTo(m_DarkSummedExposures);
  m_Acquisition -> prop_FilesInSequence() -> linkTo(m_FilesInSequence);
  m_Acquisition -> prop_OutputDirectory() -> linkTo(m_OutputDirectory);
  m_Acquisition -> prop_FilePattern() -> linkTo(m_FilePattern);
  m_Acquisition -> prop_FileIndex() -> linkTo(m_FileIndex);

  m_DataProcessor = new QxrdDataProcessor(this, m_Acquisition, this);

  connect(m_DataProcessor, SIGNAL(processedImageAvailable()), this, SLOT(onProcessedImageAvailable()));
  connect(m_DataProcessor, SIGNAL(darkImageAvailable()), this, SLOT(onDarkImageAvailable()));
  connect(m_DataProcessor, SIGNAL(printMessage(QString)), this, SLOT(printMessage(QString)));

  readSettings();

  connect(m_Acquisition, SIGNAL(oneReadoutModeChanged(int,double)),
          this,          SLOT(setReadoutTime(int, double)));
//
//  QMetaObject::invokeMethod(m_Acquisition, "initialize", Qt::QueuedConnection);
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
  readSettings();

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
                                                  m_Acquisition -> get_OutputDirectory());
  if (dir.length()) {
    m_Acquisition -> set_OutputDirectory(dir);
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

  statusMessage(tr("Exposure %1 of %2, File %3 of %4")
                .arg(isum+1).arg(nsum).arg(iframe+1).arg(nframe));

  m_Progress -> setValue(thisframe*100/totalframes);
}

void QxrdWindow::onAcquireComplete(int dark)
{
  acquisitionFinished();

  m_Acquiring = false;
}

void QxrdWindow::darkImageAcquired(QxrdImageData *image)
{
  newDarkImage(image);

  saveImageData(m_DarkFrame);
}

void QxrdWindow::doAcquire()
{
//  printMessage("QxrdWindow::doAcquire()\n");

  acquisitionStarted();

  m_Acquiring = true;

  m_AcquisitionThread -> doAcquire();
}

void QxrdWindow::doAcquireDark()
{
  darkAcquisitionStarted();

  m_AcquiringDark = true;

  m_AcquisitionThread -> doAcquireDark();
}

void QxrdWindow::doCancel()
{
  if (m_Acquiring) {
    m_AcquisitionThread -> cancel();
  }
}

void QxrdWindow::doCancelDark()
{
  if (m_AcquiringDark) {
    m_AcquisitionThread -> cancelDark();
  }
}

void QxrdWindow::readSettings()
{
  QxrdSettings settings;

  m_Acquisition -> readSettings(&settings, "acquire");

  setPerformDarkSubtraction(settings.value("acq/subtractdark",1).toInt());
  setSaveRawImages(settings.value("acq/saveraw",0).toInt());
  setDarkImagePath(settings.value("acq/darkpath","").toString());

  setPerformBadPixels(settings.value("acq/performbadpixels",0).toInt());
  setBadPixelsPath(settings.value("acq/badpixelspath","").toString());

  setPerformGainCorrection(settings.value("acq/gaincorrect",0).toInt());
  setGainMapPath(settings.value("acq/gaincorrectpath","").toString());

  setDisplayMinimumPct(settings.value("disp/minimumpct",0).toDouble());
  setDisplayMaximumPct(settings.value("disp/maximumpct",100).toDouble());
  setInterpolatePixels(settings.value("disp/interpolate",1).toInt());
  setMaintainAspectRatio(settings.value("disp/maintainaspect",1).toInt());

  m_SettingsLoaded = true;
}

void QxrdWindow::writeSettings()
{
  QxrdSettings settings;

  m_Acquisition -> writeSettings(&settings, "acquire");

  settings.setValue("acq/subtractdark", performDarkSubtraction());
  settings.setValue("acq/saveraw", saveRawImages());
  settings.setValue("acq/darkpath", darkImagePath());

  settings.setValue("acq/performbadpixels", performBadPixels());
  settings.setValue("acq/badpixelspath", badPixelsPath());

  settings.setValue("acq/gaincorrect", performGainCorrection());
  settings.setValue("acq/gaincorrectpath", gainMapPath());

  settings.setValue("disp/minimumpct", displayMinimumPct());
  settings.setValue("disp/maximumpct", displayMaximumPct());
  settings.setValue("disp/interpolate", interpolatePixels());
  settings.setValue("disp/maintainaspect", maintainAspectRatio());
}

void QxrdWindow::statusMessage(QString msg)
{
  m_StatusMsg -> setText(msg);

  m_StatusTimer.start(5000);
}

void QxrdWindow::clearStatusMessage()
{
//  if (!m_StatusMessages.isEmpty()) {
//    QString msg = m_StatusMessages.takeFirst();
//
//
//    if (m_StatusMessages.count() > 10) {
//      QTimer::singleShot(50, this, SLOT(displayStatusMessage()));
//    } else if (m_StatusMessages.count() > 2) {
//      QTimer::singleShot(1000, this, SLOT(displayStatusMessage()));
//    } else {
//      QTimer::singleShot(5000, this, SLOT(displayStatusMessage()));
//    }
//  } else {
  m_StatusMsg -> setText("");
//  }
}

void QxrdWindow::doSaveData()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Data in", m_Acquisition -> get_OutputDirectory());

  if (theFile.length()) {
    saveData(theFile);
  }
}

void QxrdWindow::doLoadData()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Image from...", m_Acquisition -> get_OutputDirectory());

  if (theFile.length()) {
    loadData(theFile);
  }
}

void QxrdWindow::loadData(QString name)
{
    QxrdImageData* res = m_Acquisition -> takeNextFreeImage();

    res -> readImage(name);

    newData(res);
}

void QxrdWindow::saveData(QString name)
{
  m_Data -> set_FileName(name);

  saveImageData(m_Data);
}

void QxrdWindow::saveImageData(QxrdImageData *image)
{
  saveNamedImageData(image->get_FileName(), image);
}

void QxrdWindow::saveRawData(QxrdImageData *image)
{
  saveNamedImageData(image->rawFileName(), image);
}

void QxrdWindow::saveNamedImageData(QString name, QxrdImageData *image)
{
//  emit printMessage(tr("Saved \"%1\")").arg(name));

  QReadLocker lock(image->rwLock());

  int nrows = image -> get_Height();
  int ncols = image -> get_Width();

  TIFF* tif = TIFFOpen(qPrintable(name),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
  TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);

  QVector<float> buffvec(4096);
  float* buffer = buffvec.data();

  for (int y=0; y<nrows; y++) {
    for (int x=0; x<ncols; x++) {
      buffer[x] = image->value(x,y);
    }

    TIFFWriteScanline(tif, buffer, y, 0);
  }

  TIFFClose(tif);
}


int QxrdWindow::performDarkSubtraction()
{
  return m_PerformDark->checkState();
}

void QxrdWindow::setPerformDarkSubtraction(int subt)
{
  m_PerformDark->setChecked(subt);
}

int QxrdWindow::saveRawImages()
{
  return m_SaveRaw->checkState();
}

void QxrdWindow::setSaveRawImages(int sav)
{
  m_SaveRaw->setChecked(sav);
}

void QxrdWindow::doLoadDarkImage()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Dark Image from...", darkImagePath());

  if (theFile.length()) {
    loadDarkImage(theFile);
  }
}

void QxrdWindow::loadDarkImage(QString name)
{
  QxrdImageData* img = m_Acquisition -> takeNextFreeImage();

  img -> readImage(name);

  newDarkImage(img);
}

QString QxrdWindow::darkImagePath()
{
  return m_DarkImageName->text();
}

void QxrdWindow::setDarkImagePath(QString path)
{
  m_DarkImageName -> setText(path);
}

int QxrdWindow::performBadPixels()
{
  return m_PerformBadPixels -> checkState();
}

void QxrdWindow::setPerformBadPixels(int corr)
{
  m_PerformBadPixels->setChecked(corr);
}

void QxrdWindow::doLoadBadPixels()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Bad Pixel Map from...", badPixelsPath());

  if (theFile.length()) {
    loadBadPixels(theFile);
  }
}

void QxrdWindow::loadBadPixels(QString name)
{
  QxrdImageData* res = m_Acquisition -> takeNextFreeImage();

  res -> readImage(name);

  newBadPixelsImage(res);
}

QString QxrdWindow::badPixelsPath()
{
  return m_BadPixelsFileName->text();
}

void QxrdWindow::setBadPixelsPath(QString path)
{
  m_BadPixelsFileName->setText(path);
}

int QxrdWindow::performGainCorrection()
{
  return m_PerformGainCorrection->checkState();
}

void QxrdWindow::setPerformGainCorrection(int corr)
{
  m_PerformGainCorrection->setChecked(corr);
}

void QxrdWindow::doLoadGainMap()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Pixel Gain Map from...", gainMapPath());

  if (theFile.length()) {
    loadGainMap(theFile);
  }
}

void QxrdWindow::loadGainMap(QString name)
{
  QxrdImageData* res = m_Acquisition -> takeNextFreeImage();

  res -> readImage(name);

  newGainMapImage(res);
}

QString QxrdWindow::gainMapPath()
{
  return m_GainCorrectionFileName -> text();
}

void QxrdWindow::setGainMapPath(QString path)
{
  m_GainCorrectionFileName -> setText(path);
}

void QxrdWindow::newData(QxrdImageData *image)
{
  if (m_Data != image) {
    image -> copyMask(m_Data);

    if (m_Data) {
      m_Acquisition -> returnImageToPool(m_Data);
    }

    m_Data = image;
  }

  QxrdRasterData     data(m_Data, interpolatePixels());
  QxrdMaskRasterData mask(m_Data, interpolatePixels());

  m_Plot -> setImage(data);
  m_Plot -> setMask(mask);
  m_Plot -> setTitle(m_Data -> get_Title());
  m_Plot -> replot();
}

void QxrdWindow::newDarkImage(QxrdImageData *image)
{
  if (m_DarkFrame != image) {
    if (m_DarkFrame) {
      m_Acquisition -> returnImageToPool(m_DarkFrame);
    }

    m_DarkFrame = image;
  }

  setDarkImagePath(image->get_FileName());
}

QxrdImageData *QxrdWindow::data()
{
  return m_Data;
}
QxrdImageData *QxrdWindow::darkImage()
{
  return m_DarkFrame;
}

void QxrdWindow::newBadPixelsImage(QxrdImageData *image)
{
  if (m_BadPixels != image) {
    if (m_BadPixels) {
      m_Acquisition -> returnImageToPool(m_BadPixels);
    }

    m_BadPixels = image;
  }

  setBadPixelsPath(image->get_FileName());
}

void QxrdWindow::newGainMapImage(QxrdImageData *image)
{
  if (m_GainFrame != image) {
    if (m_GainFrame) {
      m_Acquisition -> returnImageToPool(m_GainFrame);
    }

    m_GainFrame = image;
  }

  setGainMapPath(image->get_FileName());
}

double QxrdWindow::displayMinimumPct()
{
  return m_DisplayMinimum -> value();
}

void QxrdWindow::setDisplayMinimumPct(double pct)
{
  m_DisplayMinimum -> setValue(pct);
}

double QxrdWindow::displayMaximumPct()
{
  return m_DisplayMaximum -> value();
}

void QxrdWindow::setDisplayMaximumPct(double pct)
{
  m_DisplayMaximum -> setValue(pct);
}

int QxrdWindow::interpolatePixels()
{
  return m_InterpolatePixels -> checkState();
}

void QxrdWindow::setInterpolatePixels(int interp)
{
  m_InterpolatePixels -> setChecked(interp);
}

int QxrdWindow::maintainAspectRatio()
{
  return m_MaintainAspectRatio -> checkState();
}

void QxrdWindow::setMaintainAspectRatio(int prsrv)
{
  m_MaintainAspectRatio -> setChecked(prsrv);
}

void QxrdWindow::onProcessedImageAvailable()
{
//  printf("onProcessedImageAvailable()\n");

  QxrdImageData* img = m_DataProcessor -> takeLatestProcessedImage();

  if (img) {
    newData(img);
  }
}

void QxrdWindow::onDarkImageAvailable()
{
//  printf("onDarkImageAvailable()\n");

  QxrdImageData* img = m_DataProcessor -> takeNextDarkImage();

  if (img) {
    newDarkImage(img);
  }
}

void QxrdWindow::doTest()
{

  m_Data -> setCircularMask();
}

void QxrdWindow::showMaskRange()
{
  double min = m_MaskMinimum -> value();
  double max = m_MaskMaximum -> value();

  if (m_Data) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Data -> showMaskRange(min, max);

    newData(m_Data);
  }
}

void QxrdWindow::hideMaskAll()
{
  if (m_Data) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Data -> hideMaskAll();

    newData(m_Data);
  }
}

void QxrdWindow::showMaskAll()
{
  if (m_Data) {
//    printf ("clearMaskRange(%g,%g)\n", min, max);

    m_Data -> showMaskAll();

    newData(m_Data);
  }
}

void QxrdWindow::hideMaskRange()
{
  double min = m_MaskMinimum -> value();
  double max = m_MaskMaximum -> value();

  if (m_Data) {
//    printf ("setMaskRange(%g,%g)\n", min, max);

    m_Data -> hideMaskRange(min, max);

    newData(m_Data);
  }
}

void QxrdWindow::onToolBoxPageChanged(int page)
{
//  printf("QxrdWindow::onToolBoxPageChanged(%d)\n", page);
}

void QxrdWindow::onTabWidgetPageChanged(int page)
{
//  printf("QxrdWindow::onTabWidgetPageChanged(%d)\n", page);
}

void QxrdWindow::executeScript()
{
  m_Application -> executeScript(m_ScriptEdit -> toPlainText());
}

/******************************************************************
*
*  $Log: qxrdwindow.cpp,v $
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

