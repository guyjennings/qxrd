#include "qxrdacquisition.h"
#include "qxrdacquisitionthread.h"
#include "qxrdapplication.h"

#ifdef Q_OS_UNIX
#include "AcqLinuxTypes.h"
#endif

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include <math.h>

#include "Acq.h"
#include <QThread>
#include <QDir>
#include <QFile>
#include <QtConcurrentRun>

#include "tiffio.h"

static QxrdAcquisition *g_Acquisition = NULL;

QxrdAcquisition::QxrdAcquisition(QxrdApplication *app, QxrdAcquisitionThread *thread)
  : QObject(),
    m_Thread(thread),
    m_Mutex(QMutex::Recursive),
    m_Application(app),
    //m_Cancel(false),
    m_NRows(0),
    m_NCols(0),
    m_IntegMode(0),
    m_NSums(0),
    m_NFrames(0),
    m_NBufferFrames(0),
    m_CurrentFrame(0),
    m_NIntTimes(0)
{
  emit printMessage("Enter QxrdAcquisition::QxrdAcquisition\n");

  g_Acquisition = this;

  connect(this, SIGNAL(haltAcquire()), this, SLOT(_haltAcquire()), Qt::QueuedConnection);
}

QxrdAcquisition::~QxrdAcquisition()
{
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);

static HACQDESC m_AcqDesc = NULL;

void QxrdAcquisition::initialize()
{
  emit printMessage("QxrdAcquisition::initialize()\n");

  int nRet = HIS_ALL_OK;
  UINT nSensors;
  BOOL bEnableIRQ = true;
  ACQDESCPOS Pos = 0;
  UINT nChannelType;
  int nChannelNr;
  UINT dwSortFlags, dwDataType, dwRows, dwColumns, dwFrames;
  DWORD 
    dwAcqType, dwSystemID, dwSyncMode, dwHwAccess,
    dwNumSensors;
  char strBuffer[1000];

  nRet = Acquisition_EnumSensors(&nSensors, bEnableIRQ, FALSE);

  emit printMessage(tr("Acquisition_EnumSensors = %1\n").arg(nRet));

  if (nRet != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  emit printMessage(tr("Number of sensors = %1\n").arg(nSensors));

  if ((nRet = Acquisition_GetNextSensor(&Pos, &m_AcqDesc))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_GetCommChannel(m_AcqDesc, &nChannelType, &nChannelNr))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  if ((nRet=Acquisition_GetConfiguration(m_AcqDesc, &dwFrames, &dwRows, &dwColumns, &dwDataType, 
					 &dwSortFlags, &bEnableIRQ, &dwAcqType, 
					 &dwSystemID, &dwSyncMode, &dwHwAccess))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  m_NRows = dwRows;
  m_NCols = dwColumns;

  m_NIntTimes = 8;
  if ((nRet=Acquisition_GetIntTimes(m_AcqDesc, m_IntTimes, &m_NIntTimes)) != HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }

  emit printMessage(tr("channel type: %1, ChannelNr: %2\n").arg(nChannelType).arg(nChannelNr));
  emit printMessage(tr("frames: %1\n").arg(dwFrames));
  emit printMessage(tr("rows: %1\ncolumns: %2\n").arg(dwRows).arg(dwColumns));
  
  if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
						0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
    acquisitionError(nRet);
    return;
  }
}

void QxrdAcquisition::acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes)
{
  {
//     QMutexLocker lock(&m_Mutex);

    if (nsum <= 0) nsum = 1;
    if (nframes <= 0) nframes = 1;

    emit printMessage(tr("QxrdAcquisition::acquire(\"%1\",\"%2\",%3,%4,%5,%6)\n")
                      .arg(outDir).arg(filePattern).arg(fileIndex).arg(integmode).arg(nsum).arg(nframes));
    emit statusMessage("Starting acquisition");

    int nRet = HIS_ALL_OK;

    m_OutputDir   = outDir;
    m_FilePattern = filePattern;
    m_FileIndex = fileIndex;
    m_IntegMode = integmode;
    m_NSums = nsum;
    m_NFrames = nframes;
    m_NBufferFrames = 10;
    m_BufferFrame = 0;

    m_AcquiredImage.resize(m_NRows*m_NCols*m_NFrames);
    m_AcquiredImage.fill(0);
    m_Buffer.resize(m_NRows*m_NCols*m_NBufferFrames);
    m_Buffer.fill(0);
    m_Saved.resize(m_NFrames);
    m_Saved.fill(QFuture<int>());

    if ((nRet=Acquisition_SetCallbacksAndMessages(m_AcqDesc, NULL, 0,
						  0, OnEndFrameCallback, OnEndAcqCallback))!=HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    if ((nRet=Acquisition_SetCameraMode(m_AcqDesc, integmode)) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    if ((nRet=Acquisition_DefineDestBuffers(m_AcqDesc, m_Buffer.data(), m_NBufferFrames, m_NRows, m_NCols)) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }

    m_CurrentSum = 0;
    m_CurrentFrame = 0;

    if ((nRet=Acquisition_Acquire_Image(m_AcqDesc, m_NBufferFrames, 0, HIS_SEQ_CONTINUOUS, NULL, NULL, NULL)) != HIS_ALL_OK) {
      acquisitionError(nRet);
      return;
    }
  }
}

void QxrdAcquisition::acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum)
{
}

void QxrdAcquisition::onEndFrame()
{
  QString fileName = QDir(m_OutputDir).filePath(m_FilePattern+tr("%1").arg(m_FileIndex,5,10,QChar('0')));

  emit acquiredFrame(fileName, m_FileIndex, m_CurrentSum,m_NSums, m_CurrentFrame, m_NFrames);
  // sum current frame

  long npixels = m_NRows*m_NCols;
  double* current = m_AcquiredImage.data() + m_CurrentFrame*npixels;
  unsigned short* frame = m_Buffer.data() + m_BufferFrame*npixels;
  unsigned short max=0;

//   printf("m_AcquiredImage.data() = %p\n", current);

  for (long i=0; i<npixels; i++) {
    *current += *frame;
    current++; frame++;
  }

  m_CurrentSum++;
  m_BufferFrame++;

  if (m_BufferFrame >= m_NBufferFrames) {
    m_BufferFrame = 0;
  }

  if (m_CurrentSum >= m_NSums) {
    m_CurrentSum = 0;

    m_Saved[m_CurrentFrame] =
        QtConcurrent::run(this, &QxrdAcquisition::saveAcquiredFrame, fileName, m_CurrentFrame);
    emit statusMessage("Saving "+fileName);
    emit printMessage("Saving """+fileName+"""");
    emit summedFrameCompleted(fileName, m_CurrentFrame);

    m_FileIndex++;
    emit fileIndexChanged(m_FileIndex);
    m_CurrentFrame++;

    if (m_CurrentFrame >= m_NFrames) {
      emit printMessage("Acquisition ended\n");
      emit haltAcquire();
      emit printMessage("Aborted acquisition\n");
    }
  }
}

void QxrdAcquisition::onEndAcquisition()
{
  emit printMessage("(CB) Acquisition ended\n");

  QFuture<int> f;

  emit statusMessage("Waiting for saves");

  foreach(f, m_Saved) {
    f.waitForFinished();
  }

  emit statusMessage("Acquire Complete");
  emit acquireComplete();
}

int QxrdAcquisition::acquisitionStatus()
{
  return m_CurrentFrame - 1;
}

void QxrdAcquisition::resultsAvailable(int chan)
{
  emit resultsChanged();
}

void QxrdAcquisition::savingComplete(int chan)
{
}

void QxrdAcquisition::acquisitionError(int n)
{
  emit printMessage(tr("Acquisition Error %1\n").arg(n));
}

void QxrdAcquisition::_haltAcquire()
{
    Acquisition_Abort(m_AcqDesc);

    emit acquireComplete();
}

void QxrdAcquisition::cancel()
{
  _haltAcquire();

  //m_Cancel = true;
}

void QxrdAcquisition::cancelDark()
{
  _haltAcquire();

  //m_Cancel = true;
}

static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
  g_Acquisition -> onEndFrame();
}

static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc)
{
  g_Acquisition -> onEndAcquisition();
}

QVector<double> QxrdAcquisition::integrationTimes()
{
  QVector<double> res;

  for (int i=0; i<m_NIntTimes; i++) {
    res.append(m_IntTimes[i]);
  }

  return res;
}

void QxrdAcquisition::saveData(QString name)
{
  for (int i=0; i<m_NFrames; i++) {
    saveAcquiredFrame(QString(name+"-%1").arg(m_FileIndex,5,10,QChar('0')), i);
    m_FileIndex++;
  }

  emit fileIndexChanged(m_FileIndex);
}

void QxrdAcquisition::loadData(QString name)
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

      m_AcquiredImage.resize(imageWidth*imageHeight);
      m_AcquiredImage.fill(0);
      m_NCols = imageWidth;
      m_NRows = imageHeight;
      m_NFrames = 1;

      void* buffer = malloc(TIFFScanlineSize(tif));

      for (int y=0; y<m_NRows; y++) {
        if (TIFFReadScanline(tif, buffer, y)==1) {
          for (int x=0; x<m_NCols; x++) {
            switch (sampleFormat) {
            case SAMPLEFORMAT_INT:
              switch (bitsPerSample) {
              case 8:
                m_AcquiredImage[y*m_NCols+x] = ((qint8*) buffer)[x];
                break;
              case 16:
                m_AcquiredImage[y*m_NCols+x] = ((qint16*) buffer)[x];
                break;
              case 32:
                m_AcquiredImage[y*m_NCols+x] = ((qint32*) buffer)[x];
                break;
              }
              break;
            case SAMPLEFORMAT_UINT:
              switch (bitsPerSample) {
              case 8:
                m_AcquiredImage[y*m_NCols+x] = ((quint8*) buffer)[x];
                break;
              case 16:
                m_AcquiredImage[y*m_NCols+x] = ((quint16*) buffer)[x];
                break;
              case 32:
                m_AcquiredImage[y*m_NCols+x] = ((quint32*) buffer)[x];
                break;
              }
              break;
            case SAMPLEFORMAT_IEEEFP:
              switch (bitsPerSample) {
              case 32:
                m_AcquiredImage[y*m_NCols+x] = ((float*) buffer)[x];
                break;
              case 64:
                m_AcquiredImage[y*m_NCols+x] = ((double*) buffer)[x];
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

int QxrdAcquisition::saveAcquiredFrame(QString name, int frame)
{
  long npixels = m_NRows*m_NCols;
  double* current = m_AcquiredImage.data()+frame*npixels;

//  QFile outfile(name);
//  outfile.open(QIODevice::ReadWrite);
//  outfile.write((const char*) current, npixels*sizeof(double));

  TIFF* tif = TIFFOpen(qPrintable(name+".tif"),"w");

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, m_NCols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, m_NRows);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
  TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);

  float buffer[4096];

  for (int y=0; y<m_NRows; y++) {
    for (int x=0; x<m_NCols; x++) {
      buffer[x] = current[y*m_NCols+x];
    }

    TIFFWriteScanline(tif, buffer, y, 0);
  }

  TIFFClose(tif);

  return 1;
}

QxrdRasterData QxrdAcquisition::imageRaster(int iframe)
{
//   printf("QxrdAcquisition::imageRaster(%d)\n", iframe);
//   printf("  m_AcquiredImage.data() = %p\n", m_AcquiredImage.data());

  if (iframe >= 0 && iframe < m_CurrentFrame) {
    emit printMessage(tr("QxrdAcquisition::imageRaster(%1)=QxrdRasterData(%2,%3,%4,%5)")
                      .arg(iframe)
                      .arg((long)m_AcquiredImage.data(),8,16,QChar('0'))
                      .arg(iframe*m_NRows*m_NCols).arg(m_NRows).arg(m_NCols));

    return QxrdRasterData(&m_AcquiredImage, iframe*m_NRows*m_NCols, m_NRows, m_NCols);
  } else {
    return QxrdRasterData(&m_AcquiredImage, 0, m_NRows, m_NCols);
  }
}

void QxrdAcquisition::saveTestTIFF(QString name, int nbits, int isfloat)
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
