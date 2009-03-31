#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"
#include "qxrdsettings.h"
#include "qxrdimageplot.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"

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

#include "tiffio.h"

QxrdWindow::QxrdWindow(QxrdApplication *app, QxrdAcquisitionThread *acq, QWidget *parent)
  : QMainWindow(parent),
    m_SettingsLoaded(false),
    m_Application(app),
    m_AcquisitionThread(acq),
    m_DataProcessor(new QxrdDataProcessor(this, acq, this)),
    m_Progress(NULL),
    m_Acquiring(false),
    m_AcquiringDark(false),
    m_Data(new QxrdImageData(2048,2048)),
    m_DarkFrame(NULL),
    m_BadPixels(NULL),
    m_GainFrame(NULL),
    m_FileBrowserModel(NULL),
    m_FileBrowserTimer()
{
  setupUi(this);

  m_FileBrowserModel = new QDirModel();
  m_FileBrowser -> setModel(m_FileBrowserModel);
  m_FileBrowser -> setRootIndex(m_FileBrowserModel->index(QDir::currentPath()));

  connect(&m_FileBrowserTimer, SIGNAL(timeout()), this, SLOT(refreshFileBrowser()));
  m_FileBrowserTimer.start(5000);
  connect(m_OutputDirectory, SIGNAL(textChanged(QString)), this, SLOT(setFileBrowserDirectory(QString)));

  connect(m_ActionAutoScale, SIGNAL(triggered()), m_Plot, SLOT(autoScale()));
  connect(m_ActionQuit, SIGNAL(triggered()), m_Application, SLOT(possiblyQuit()));
  connect(m_ActionLoadData, SIGNAL(triggered()), this, SLOT(doLoadData()));
  connect(m_ActionImportData, SIGNAL(triggered()), this, SLOT(doImportData()));
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

  connect(m_AcquisitionThread, SIGNAL(acquiredFrame(QString,int,int,int,int,int)),
          this, SLOT(acquiredFrame(QString,int,int,int,int,int)));
  connect(m_AcquisitionThread, SIGNAL(acquireComplete()),
          this, SLOT(acquireComplete()));

  connect(m_AcquisitionThread, SIGNAL(fileIndexChanged(int)),
          this, SLOT(setFileIndex(int)));
  connect(m_AcquisitionThread, SIGNAL(statusMessage(QString)),
          this, SLOT(statusMessage(QString)));

  connect(m_DataProcessor, SIGNAL(processedImageAvailable()), this, SLOT(onProcessedImageAvailable()));
  connect(m_DataProcessor, SIGNAL(darkImageAvailable()), this, SLOT(onDarkImageAvailable()));

  connect(m_ActionTest, SIGNAL(triggered()), this, SLOT(doTest()));

  for (int i=0; i<8; i++) {
    m_ExposureTime -> addItem(tr("Item %1").arg(i));
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
}

QxrdWindow::~QxrdWindow()
{
  printf("QxrdWindow::~QxrdWindow()\n");

  if (m_SettingsLoaded) {
    saveSettings();
  }
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

void QxrdWindow::printMessage(QString msg)
{
  m_Messages -> append(msg.trimmed());
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

void QxrdWindow::setIntegrationTime(int n, double t)
{
  while (n >= m_ExposureTime->count()) {
    m_ExposureTime -> addItem("");
    m_Exposures.append(0);
  }

  m_ExposureTime -> setItemText(n, tr("%1 msec").arg(t/1e3,0,'f',0));
  m_Exposures[n] = t;
}

void QxrdWindow::setExposureTime(double t)
{
  int best=0;
  double t_usec = t*1e6;

  for (int i=1; i<8; i++) {
    if (fabs(t_usec-m_Exposures.value(i)) < fabs(t_usec-m_Exposures.value(best))) {
      best = i;
    }
  }

  setIntegrationMode(best);
}

void QxrdWindow::setIntegrationMode(int mode)
{
  m_ExposureTime->setCurrentIndex(mode);
}

void QxrdWindow::setNSummed(int nsummed)
{
  m_SummedFrames->setValue(nsummed);
}

void QxrdWindow::setDarkNSummed(int nsummed)
{
  m_DarkSummedFrames->setValue(nsummed);
}

void QxrdWindow::setNFrames(int nframes)
{
  m_SequenceFrames->setValue(nframes);
}

void QxrdWindow::setFileIndex(int index)
{
  m_FileIndexNumber->setValue(index);
}

void QxrdWindow::setFilePattern(QString pattern)
{
  m_SaveFilePattern->setText(pattern);
}

void QxrdWindow::setOutputDirectory(QString pattern)
{
  m_OutputDirectory->setText(pattern);
}

double  QxrdWindow::exposureTime()
{
  int choice = m_ExposureTime->currentIndex();

  return m_Exposures.value(choice)/1e6;
}

int     QxrdWindow::integrationMode()
{
  return m_ExposureTime->currentIndex();
}

int     QxrdWindow::nSummed()
{
  return m_SummedFrames->value();
}

int     QxrdWindow::darkNSummed()
{
  return m_DarkSummedFrames->value();
}

int     QxrdWindow::nFrames()
{
  return m_SequenceFrames->value();
}

int     QxrdWindow::fileIndex()
{
  return m_FileIndexNumber->value();
}

QString QxrdWindow::filePattern()
{
  return m_SaveFilePattern->text();
}

QString QxrdWindow::outputDirectory()
{
  return m_OutputDirectory->text();
}

void QxrdWindow::selectOutputDirectory()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory", outputDirectory());
  if (dir.length()) {
    setOutputDirectory(dir);
  }
}

void QxrdWindow::acquiredFrame(QString fileName, int fileIndex, int isum, int nsum, int iframe, int nframe)
{
//   printf("QxrdWindow::acquiredFrame(\"%s\",%d,%d,%d,%d,%d)\n",
// 	 qPrintable(fileName), fileIndex, isum, nsum, iframe, nframe);

  int totalframes = nsum*nframe;
  int thisframe = iframe*nsum+isum+1;

  //  printf("%d %% progress\n", thisframe*100/totalframes);

  m_Progress -> setValue(thisframe*100/totalframes);
}

void QxrdWindow::darkImageAcquired(QxrdImageData *image)
{
  newDarkImage(image);

  saveImageData(m_DarkFrame);
}

int QxrdWindow::acquisitionStatus(double time)
{
  return m_AcquisitionThread -> acquisitionStatus(time);
}

void QxrdWindow::doAcquire()
{
//  printMessage("QxrdWindow::doAcquire()\n");

  acquisitionStarted();

  QString outDir   = outputDirectory();
  QString filePatt = filePattern();
  int    index     = fileIndex();
  int    integmode = integrationMode();
  int    nsum      = nSummed();
  int    nframes   = nFrames();

  m_Acquiring = true;

  m_AcquisitionThread -> acquire(outDir, filePatt, index, integmode, nsum, nframes);
}

void QxrdWindow::doAcquireDark()
{
  darkAcquisitionStarted();

  QString outDir   = outputDirectory();
  QString filePatt = filePattern();
  int    index     = fileIndex();
  int    integmode = integrationMode();
  int     nsum     = darkNSummed();

  m_AcquiringDark = true;

  m_AcquisitionThread -> acquireDark(outDir, filePatt, index, integmode, nsum);
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

void QxrdWindow::acquireComplete()
{
  acquisitionFinished();

  m_Acquiring = false;
}

void QxrdWindow::acquireDarkComplete()
{
  acquisitionFinished();

  m_AcquiringDark = false;
}

void QxrdWindow::readSettings()
{
  QxrdSettings settings;

  setIntegrationMode(settings.value("acq/integ",7).toInt());
  setNSummed(settings.value("acq/nsums",1).toInt());
  setDarkNSummed(settings.value("acq/dknsums",1).toInt());
  setNFrames(settings.value("acq/nframes",1).toInt());
  setFilePattern(settings.value("acq/filepattern","saveddata").toString());
  setOutputDirectory(settings.value("acq/directory","").toString());
  setFileIndex(settings.value("acq/fileindex",1).toInt());

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

void QxrdWindow::saveSettings()
{
  QxrdSettings settings;

  settings.setValue("acq/integ",integrationMode());
  settings.setValue("acq/nsums",nSummed());
  settings.setValue("acq/dknsums",darkNSummed());
  settings.setValue("acq/nframes",nFrames());
  settings.setValue("acq/filepattern",filePattern());
  settings.setValue("acq/directory",outputDirectory());
  settings.setValue("acq/fileindex",fileIndex());

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
}

void QxrdWindow::doSaveData()
{
  QString theFile = QFileDialog::getSaveFileName(
      this, "Save Data in", outputDirectory());

  if (theFile.length()) {
    saveData(theFile);
  }
}

void QxrdWindow::doImportData()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Import Image from...", outputDirectory());

  if (theFile.length()) {
    importData(theFile);
  }
}

void QxrdWindow::importData(QString name)
{
    QxrdImageData* res = m_AcquisitionThread -> takeNextFreeImage();

    res -> readImage(name);

    newData(res);
}

void QxrdWindow::doLoadData()
{
  QString theFile = QFileDialog::getOpenFileName(
      this, "Load Image from...", outputDirectory());

  if (theFile.length()) {
    loadData(theFile);
  }
}

void QxrdWindow::loadData(QString name)
{
  QxrdImageData* res = loadNewImage(name);

  newData(res);
}

QxrdImageData* QxrdWindow::loadNewImage(QString name)
{
  QxrdImageData* res = m_AcquisitionThread -> takeNextFreeImage();

  QWriteLocker lock(res->rwLock());

  quint32 imageWidth = 0;
  quint32 imageHeight = 0;
  quint16 sampleFormat = 0;
  quint16 samplesPerPixel = 0;
  quint16 bitsPerSample = 0;

  TIFF* tif = TIFFOpen(qPrintable(name),"r");

  if (tif) {
    if ((TIFFGetFieldDefaulted(tif, TIFFTAG_IMAGEWIDTH, &imageWidth)==1) &&
        (TIFFGetFieldDefaulted(tif, TIFFTAG_IMAGELENGTH, &imageHeight)==1) &&
        (TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel)==1) &&
        (TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample)==1) &&
        (TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLEFORMAT, &sampleFormat)==1)) {

      emit printMessage(tr("Image file W(%1),H(%2),SPP(%3),BPS(%4),SF(%5)\n")
                        .arg(imageWidth).arg(imageHeight).arg(samplesPerPixel).arg(bitsPerSample).arg(sampleFormat));

      res -> resize(imageWidth, imageHeight);
      res -> clear();

      void* buffer = malloc(TIFFScanlineSize(tif));

      for (quint32 y=0; y<imageHeight; y++) {
        if (TIFFReadScanline(tif, buffer, y)==1) {
          for (quint32 x=0; x<imageWidth; x++) {
            switch (sampleFormat) {
            case SAMPLEFORMAT_INT:
              switch (bitsPerSample) {
              case 8:
                res -> setValue(x,y, ((qint8*) buffer)[x]);
                break;
              case 16:
                res -> setValue(x,y, ((qint16*) buffer)[x]);
                break;
              case 32:
                res -> setValue(x,y, ((qint32*) buffer)[x]);
                break;
              }
              break;
            case SAMPLEFORMAT_UINT:
              switch (bitsPerSample) {
              case 8:
                res -> setValue(x,y, ((quint8*) buffer)[x]);
                break;
              case 16:
                res -> setValue(x,y, ((quint16*) buffer)[x]);
                break;
              case 32:
                res -> setValue(x,y, ((quint32*) buffer)[x]);
                break;
              }
              break;
            case SAMPLEFORMAT_IEEEFP:
              switch (bitsPerSample) {
              case 32:
                res -> setValue(x,y, ((float*) buffer)[x]);
                break;
              case 64:
                res -> setValue(x,y, ((double*) buffer)[x]);
                break;
              }
              break;
            }
          }
        }
      }

      free(buffer);
    } else {
      emit statusMessage("Couldn't open file\n");
    }
  } else {
    emit statusMessage("Bad TIFF File\n");
  }

  if (tif) {
    TIFFClose(tif);
  }

  QFileInfo info(name);

  res -> setFilename(name);
  res -> setTitle(info.fileName());

  return res;
}

void QxrdWindow::saveData(QString name)
{
  m_Data -> setFilename(name);

  saveImageData(m_Data);
}

void QxrdWindow::saveImageData(QxrdImageData *image)
{
  saveNamedImageData(image->filename(), image);
}

void QxrdWindow::saveRawData(QxrdImageData *image)
{
  QFileInfo info(image->filename());

  QString name = info.dir().filePath(
      info.completeBaseName()+".raw.tif");

  saveNamedImageData(name, image);
}

void QxrdWindow::saveNamedImageData(QString name, QxrdImageData *image)
{
  emit printMessage(tr("Saved \"%1\")").arg(name));

  QReadLocker lock(image->rwLock());

  int nrows = image -> height();
  int ncols = image -> width();

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
  QxrdImageData* img = loadNewImage(name);

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
  QxrdImageData* img = loadNewImage(name);

  newBadPixelsImage(img);
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
  QxrdImageData* img = loadNewImage(name);

  newGainMapImage(img);
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
    if (m_Data) {
      m_AcquisitionThread -> returnImageToPool(m_Data);
    }

    m_Data = image;
  }

  QxrdRasterData data(m_Data, interpolatePixels());

  m_Plot -> setImage(data);
  m_Plot -> setTitle(m_Data -> title());
  m_Plot -> replot();
}

void QxrdWindow::newDarkImage(QxrdImageData *image)
{
  if (m_DarkFrame != image) {
    if (m_DarkFrame) {
      m_AcquisitionThread -> returnImageToPool(m_DarkFrame);
    }

    m_DarkFrame = image;
  }

  setDarkImagePath(image->filename());
}

QxrdImageData *QxrdWindow::darkImage()
{
  return m_DarkFrame;
}

void QxrdWindow::newBadPixelsImage(QxrdImageData *image)
{
  if (m_BadPixels != image) {
    if (m_BadPixels) {
      m_AcquisitionThread -> returnImageToPool(m_BadPixels);
    }

    m_BadPixels = image;
  }

  setBadPixelsPath(image->filename());
}

void QxrdWindow::newGainMapImage(QxrdImageData *image)
{
  if (m_GainFrame != image) {
    if (m_GainFrame) {
      m_AcquisitionThread -> returnImageToPool(m_GainFrame);
    }

    m_GainFrame = image;
  }

  setGainMapPath(image->filename());
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

void QxrdWindow::setFileBrowserDirectory(QString dir)
{
  m_FileBrowser -> setRootIndex(m_FileBrowserModel->index(dir));
}

void QxrdWindow::refreshFileBrowser()
{
//   printf("Refresh file browser\n");

  m_FileBrowserModel -> refresh();
}

void QxrdWindow::onProcessedImageAvailable()
{
  printf("onProcessedImageAvailable()\n");

  QxrdImageData* img = m_DataProcessor -> takeLatestProcessedImage();

  if (img) {
    newData(img);
  }
}

void QxrdWindow::onDarkImageAvailable()
{
  printf("onDarkImageAvailable()\n");

  QxrdImageData* img = m_DataProcessor -> takeNextDarkImage();

  if (img) {
    newDarkImage(img);
  }
}

void QxrdWindow::doTest()
{
  int status = m_AcquisitionThread -> acquisitionStatus(1.0);

  printf("QxrdWindow::doTest : m_AcquisitionThread -> acquisitionStatus(1.0) = %d\n", status);

  emit printMessage(tr("QxrdWindow::doTest : m_AcquisitionThread -> acquisitionStatus(1.0) = %1").arg(status));
}
