#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"
#include "qxrdsettings.h"
#include "qxrdimageplot.h"
#include "qxrdimagedata.h"

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

#include "tiffio.h"

QxrdWindow::QxrdWindow(QxrdApplication *app, QxrdAcquisitionThread *acq, QWidget *parent)
  : QMainWindow(parent),
    m_SettingsLoaded(false),
    m_Application(app),
    m_AcquisitionThread(acq),
    m_Progress(NULL),
    m_Acquiring(false),
    m_AcquiringDark(false),
    m_AcquiredImages(QString("QxrdWindow acquired images")),
    m_Data(new QxrdImageData(2048,2048)),
    m_DarkFrame(NULL),
    m_BadPixels(NULL),
    m_GainFrame(NULL)
{
  setupUi(this);

  setupConnections();
}

void QxrdWindow::setupConnections()
{
  connect(m_ActionAutoScale, SIGNAL(triggered()), m_Plot, SLOT(autoScale()));
  connect(m_ActionQuit, SIGNAL(triggered()), m_Application, SLOT(possiblyQuit()));
  connect(m_ActionLoadData, SIGNAL(triggered()), this, SLOT(doLoadData()));
  connect(m_ActionSaveData, SIGNAL(triggered()), this, SLOT(doSaveData()));

  connect(m_AcquireButton, SIGNAL(clicked()), this, SLOT(doAcquire()));
  connect(m_CancelButton, SIGNAL(clicked()), this, SLOT(doCancel()));
  connect(m_SelectDirectoryButton, SIGNAL(clicked()), this, SLOT(selectOutputDirectory()));

  connect(m_LoadDarkButton, SIGNAL(clicked()), this, SLOT(doLoadDarkImage()));
  connect(m_LoadBadPixelsButton, SIGNAL(clicked()), this, SLOT(doLoadBadPixels()));
  connect(m_LoadGainCorrection, SIGNAL(clicked()), this, SLOT(doLoadGainMap()));

  connect(m_DarkAcquireButton, SIGNAL(clicked()), m_ActionAcquireDark, SIGNAL(triggered()));
  connect(m_DarkCancelButton, SIGNAL(clicked()), m_ActionCancelDark, SIGNAL(triggered()));

  connect(m_ActionAcquireDark, SIGNAL(triggered()), this, SLOT(doAcquireDark()));
  connect(m_ActionCancelDark, SIGNAL(triggered()), this, SLOT(doCancelDark()));

  connect(m_Action005Range, SIGNAL(triggered()), m_Plot, SLOT(set005Range()));
  connect(m_Action010Range, SIGNAL(triggered()), m_Plot, SLOT(set010Range()));
  connect(m_Action100Range, SIGNAL(triggered()), m_Plot, SLOT(set100Range()));

  connect(m_ActionGrayscale, SIGNAL(triggered()), m_Plot, SLOT(setGrayscale()));
  connect(m_ActionInverseGrayscale, SIGNAL(triggered()), m_Plot, SLOT(setInverseGrayscale()));
  connect(m_ActionEarthTones, SIGNAL(triggered()), m_Plot, SLOT(setEarthTones()));
  connect(m_ActionSpectrum, SIGNAL(triggered()), m_Plot, SLOT(setSpectrum()));
  connect(m_ActionFire, SIGNAL(triggered()), m_Plot, SLOT(setFire()), Qt::DirectConnection);

  connect(m_AcquisitionThread, SIGNAL(acquiredFrame(QString,int,int,int,int,int)),
          this, SLOT(acquiredFrame(QString,int,int,int,int,int)));
  connect(m_AcquisitionThread, SIGNAL(summedFrameCompleted(QString,int)),
          this, SLOT(summedFrameCompleted(QString,int)));
  connect(m_AcquisitionThread, SIGNAL(acquireComplete()),
          this, SLOT(acquireComplete()));

  connect(m_AcquisitionThread, SIGNAL(fileIndexChanged(int)),
          this, SLOT(setFileIndex(int)));
  connect(m_AcquisitionThread, SIGNAL(statusMessage(QString)),
          this, SLOT(statusMessage(QString)));

  connect(m_ActionAcquire, SIGNAL(triggered()), this, SLOT(doAcquire()));
  connect(m_ActionCancel, SIGNAL(triggered()), this, SLOT(doCancel()));
  connect(m_ActionAcquireDark, SIGNAL(triggered()), this, SLOT(doAcquireDark()));
  connect(m_ActionCancelDark, SIGNAL(triggered()), this, SLOT(doCancelDark()));

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

void QxrdWindow::summedFrameCompleted(QString fileName, int iframe)
{
  printf("QxrdWindow::summedFrameCompleted(\"%s\",%d)\n", qPrintable(fileName), iframe);

  if (m_Acquiring) {
    QxrdImageData *latest = dequeue();
    QxrdImageData *current = m_Data;

    m_AcquisitionThread -> enqueue(current);

    m_Data = latest;

    performImageCorrections(m_Data);

    QxrdRasterData data(m_Data);

    QFileInfo fileInfo(fileName);

    m_Plot -> setImage(data);
    m_Plot -> setTitle(fileInfo.fileName());
    //  m_Plot -> autoScale();

    saveData(fileName);

    //  QtConcurrent::run insert processing step here...
  }
}

void QxrdWindow::darkImageAcquired(QxrdImageData *image)
{
  newDarkImage(image);

  saveImageData(m_DarkFrame);
}

void QxrdWindow::enqueue(QxrdImageData *image)
{
  m_AcquiredImages.enqueue(image);
}

QxrdImageData* QxrdWindow::dequeue()
{
  return m_AcquiredImages.dequeue();
}

int QxrdWindow::acquisitionStatus(double time)
{
  return m_AcquisitionThread -> acquisitionStatus(time);
}

void QxrdWindow::doAcquire()
{
  printMessage("QxrdWindow::doAcquire()\n");

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
}

void QxrdWindow::statusMessage(QString msg)
{
  m_StatusMsg -> setText(msg);
}

void QxrdWindow::doSaveData()
{
  QString theFile = QFileDialog::getSaveFileName(this, "Save Data in");

  if (theFile.length()) {
    saveData(theFile);
  }
}

void QxrdWindow::doLoadData()
{
  QString theFile = QFileDialog::getOpenFileName(this, "Load Image from...");

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
  QxrdImageData* res = dequeue();

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

      for (int y=0; y<imageHeight; y++) {
        if (TIFFReadScanline(tif, buffer, y)==1) {
          for (int x=0; x<imageWidth; x++) {
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

  QString name = info.completeBaseName()+".raw.tif";

  saveNamedImageData(name, image);
}

void QxrdWindow::saveNamedImageData(QString name, QxrdImageData *image)
{
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

void QxrdWindow::saveTestTIFF(QString name, int nbits, int isfloat)
{
  TIFF* tif = TIFFOpen(qPrintable(name),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 2048);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 2048);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, nbits);

  if (isfloat) {
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
  } else {
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
  }

  switch (nbits+isfloat) {
  case 8+0:
    {
      signed char b[2048];
      for (int y=0; y<2048; y++) {
        for (int x=0; x<2048; x++) {
          b[x] = 100.0*sin(y/100.0)*sin(x/100.0);
        }
        TIFFWriteScanline(tif, b, y, 0);
      }
    }
    break;

  case 16+0:
    {
      short b[2048];
      for (int y=0; y<2048; y++) {
        for (int x=0; x<2048; x++) {
          b[x] = 100.0*sin(y/100.0)*sin(x/100.0);
        }
        TIFFWriteScanline(tif, b, y, 0);
      }
    }
    break;

  case 32+0:
    {
      int b[2048];
      for (int y=0; y<2048; y++) {
        for (int x=0; x<2048; x++) {
          b[x] = 100.0*sin(y/100.0)*sin(x/100.0);
        }
        TIFFWriteScanline(tif, b, y, 0);
      }
    }
    break;

  case 32+1:
    {
      float b[2048];
      for (int y=0; y<2048; y++) {
        for (int x=0; x<2048; x++) {
          b[x] = 100.0*sin(y/100.0)*sin(x/100.0);
        }
        TIFFWriteScanline(tif, b, y, 0);
      }
    }
    break;

  case 64+1:
    {
      double b[2048];
      for (int y=0; y<2048; y++) {
        for (int x=0; x<2048; x++) {
          b[x] = 100.0*sin(y/100.0)*sin(x/100.0);
        }
        TIFFWriteScanline(tif, b, y, 0);
      }
    }
    break;
  }

  TIFFClose(tif);
}

void QxrdWindow::performImageCorrections(QxrdImageData *image)
{
  correctBadPixels(image);
  subtractDarkImage(image);
  correctImageGains(image);
}

void QxrdWindow::subtractDarkImage(QxrdImageData *image)
{
  if (performDarkSubtraction()) {
    if (saveRawImages()) {
      saveRawData(image);
    }

    if (m_DarkFrame && image) {
      if (m_DarkFrame->integrationMode() != image->integrationMode()) {
        if (QMessageBox::question(this, "Integration times differ",
                                  "Integration times of acquired data and dark image are different\nSubtract anyway?",
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
          return;
        }
      }

      if (m_DarkFrame->width() != image->width() ||
          m_DarkFrame->height() != image->height()) {
        if (QMessageBox::question(this, "Image dimensions differ",
                                  "Dimensions of acquired data and dark image are different\nSkip subtraction",
                                  QMessageBox::Yes) == QMessageBox::Yes) {
          return;
        }
      }

      QReadLocker lock1(m_DarkFrame->rwLock());
      QWriteLocker lock2(image->rwLock());

      int height = image->height();
      int width  = image->width();
      int nres = image->nSummed();
      int ndrk = m_DarkFrame -> nSummed();
      int npixels = width*height;

      double ratio = ((double) nres)/((double) ndrk);

      printf("Dark subtraction nres=%d, ndrk=%d, npixels=%d, ratio=%g\n",
             nres, ndrk, npixels, ratio);

      QTime tic;
      tic.start();

      double *result = image->data();
      double *dark   = m_DarkFrame->data();

      for (int i=0; i<npixels; i++) {
        result[i] = result[i]-ratio*dark[i];
      }

      printf("Dark subtraction took %d msec\n", tic.elapsed());
    }
  }
}

void QxrdWindow::correctBadPixels(QxrdImageData *image)
{
  if (m_BadPixels) {
  }
}

void QxrdWindow::correctImageGains(QxrdImageData *image)
{
  if (m_GainFrame) {
  }
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
  QString theFile = QFileDialog::getOpenFileName(this, "Load Dark Image from...");

  if (theFile.length()) {
    loadDarkImage(theFile);
  }
}

void QxrdWindow::loadDarkImage(QString name)
{
  QxrdImageData* img = loadNewImage(name);

  setDarkImagePath(name);

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
  QString theFile = QFileDialog::getOpenFileName(this, "Load Bad Pixel Map from...");

  if (theFile.length()) {
    loadBadPixels(theFile);
  }
}

void QxrdWindow::loadBadPixels(QString name)
{
  QxrdImageData* img = loadNewImage(name);

  setBadPixelsPath(name);

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
  QString theFile = QFileDialog::getOpenFileName(this, "Load Pixel Gain Map from...");

  if (theFile.length()) {
    loadGainMap(theFile);
  }
}

void QxrdWindow::loadGainMap(QString name)
{
  QxrdImageData* img = loadNewImage(name);

  setGainMapPath(name);

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
      enqueue(m_Data);
    }

    m_Data = image;
  }
}

void QxrdWindow::newDarkImage(QxrdImageData *image)
{
  if (m_DarkFrame != image) {
    if (m_DarkFrame) {
      enqueue(m_DarkFrame);
    }

    m_DarkFrame = image;
  }
}

void QxrdWindow::newBadPixelsImage(QxrdImageData *image)
{
  if (m_BadPixels != image) {
    if (m_BadPixels) {
      enqueue(m_BadPixels);
    }

    m_BadPixels = image;
  }
}

void QxrdWindow::newGainMapImage(QxrdImageData *image)
{
  if (m_GainFrame != image) {
    if (m_GainFrame) {
      enqueue(m_GainFrame);
    }

    m_GainFrame = image;
  }
}
