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

//void QxrdWindow::saveData()
//{
//}
//
//void QxrdWindow::loadData()
//{
//}

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
  int totalframes = nsum*nframe;
  int thisframe = iframe*nsum+isum+1;

  //  printf("%d %% progress\n", thisframe*100/totalframes);

  m_Progress -> setValue(thisframe*100/totalframes);
}

void QxrdWindow::summedFrameCompleted(QString fileName, int iframe)
{
  QxrdImageData *latest = dequeue();
  QxrdImageData *current = m_Data;

  m_AcquisitionThread -> enqueue(current);

  m_Data = latest;
  QxrdRasterData data(m_Data);

  QFileInfo fileInfo(fileName);

  m_Plot -> setImage(data);
  m_Plot -> setTitle(fileInfo.fileName());
//  m_Plot -> autoScale();
}

void QxrdWindow::enqueue(QxrdImageData *image)
{
  m_AcquiredImages.enqueue(image);
}

QxrdImageData* QxrdWindow::dequeue()
{
  return m_AcquiredImages.dequeue();
}

int QxrdWindow::acquire()
{
  acquisitionStarted();

  QString outDir   = outputDirectory();
  QString filePatt = filePattern();
  int    index     = fileIndex();
  int    integmode = integrationMode();
  int    nsum      = nSummed();
  int    nframes   = nFrames();

  m_AcquisitionThread -> acquire(outDir, filePatt, index, integmode, nsum, nframes);

  m_Acquiring = true;

  return 0;
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

  m_AcquisitionThread -> acquire(outDir, filePatt, index, integmode, nsum, nframes);

  m_Acquiring = true;
}

void QxrdWindow::doAcquireDark()
{
  darkAcquisitionStarted();

  QString outDir   = outputDirectory();
  QString filePatt = filePattern();
  int    index     = fileIndex();
  int    integmode = integrationMode();
  int     nsum     = darkNSummed();

  m_AcquisitionThread -> acquireDark(outDir, filePatt, index, integmode, nsum);

  m_AcquiringDark = true;
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
  setNFrames(settings.value("acq/nframes",1).toInt());
  setFilePattern(settings.value("acq/filepattern","saveddata").toString());
  setOutputDirectory(settings.value("acq/directory","").toString());
  setFileIndex(settings.value("acq/fileindex",1).toInt());

  m_SettingsLoaded = true;
}

void QxrdWindow::saveSettings()
{
  QxrdSettings settings;

  settings.setValue("acq/integ",integrationMode());
  settings.setValue("acq/nsums",nSummed());
  settings.setValue("acq/nframes",nFrames());
  settings.setValue("acq/filepattern",filePattern());
  settings.setValue("acq/directory",outputDirectory());
  settings.setValue("acq/fileindex",fileIndex());
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

      m_Data -> resize(imageWidth, imageHeight);
      m_Data -> clear();

      void* buffer = malloc(TIFFScanlineSize(tif));

      for (int y=0; y<imageHeight; y++) {
        if (TIFFReadScanline(tif, buffer, y)==1) {
          for (int x=0; x<imageWidth; x++) {
            switch (sampleFormat) {
            case SAMPLEFORMAT_INT:
              switch (bitsPerSample) {
              case 8:
                m_Data -> setValue(x,y, ((qint8*) buffer)[x]);
                break;
              case 16:
                m_Data -> setValue(x,y, ((qint16*) buffer)[x]);
                break;
              case 32:
                m_Data -> setValue(x,y, ((qint32*) buffer)[x]);
                break;
              }
              break;
            case SAMPLEFORMAT_UINT:
              switch (bitsPerSample) {
              case 8:
                m_Data -> setValue(x,y, ((quint8*) buffer)[x]);
                break;
              case 16:
                m_Data -> setValue(x,y, ((quint16*) buffer)[x]);
                break;
              case 32:
                m_Data -> setValue(x,y, ((quint32*) buffer)[x]);
                break;
              }
              break;
            case SAMPLEFORMAT_IEEEFP:
              switch (bitsPerSample) {
              case 32:
                m_Data -> setValue(x,y, ((float*) buffer)[x]);
                break;
              case 64:
                m_Data -> setValue(x,y, ((double*) buffer)[x]);
                break;
              }
              break;
            }
          }
        }
      }

      emit summedFrameCompleted(name,0);
    } else {
      emit statusMessage("Couldn't open file\n");
    }
  } else {
    emit statusMessage("Bad TIFF File\n");
  }

  if (tif) {
    TIFFClose(tif);
  }
}

void QxrdWindow::saveData(QString name)
{
  int nrows = m_Data -> height();
  int ncols = m_Data -> width();

  TIFF* tif = TIFFOpen(qPrintable(name+".tif"),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
  TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);

  float buffer[4096];

  for (int y=0; y<nrows; y++) {
    for (int x=0; x<ncols; x++) {
      buffer[x] = m_Data->value(x,y);
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
