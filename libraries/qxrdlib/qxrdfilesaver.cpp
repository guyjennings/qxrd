#define _CRT_SECURE_NO_WARNINGS

#include "qxrddebug.h"
#include "qxrdfilesaver.h"
#include "qxrdacqcommon.h"
#include "qxrdprocessor.h"
#include "qcepintegrateddata.h"
#include "qxrdexperiment.h"
#include "qcepcenterfinder.h"
#include "qxrdintegrator.h"
#include "qcepimagedataformattiff.h"
#include <QDir>
#include <QThread>
#include "tiffio.h"
#include "qcepimagedata.h"
#include "tiffio.hxx"
#include <sstream>
#include "qcepbzlib.h"
#include "zlib.h"

QxrdFileSaver::QxrdFileSaver()
  : inherited("fileSaver"),
    m_Experiment(),
    m_Processor(),
    m_Acquisition()
{
#ifndef QT_NO_DEBUG
  printf("Constructing file saver\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaver::QxrdFileSaver(%p)\n", this);
  }
}

QxrdFileSaver::~QxrdFileSaver()
{
#ifndef QT_NO_DEBUG
  printf("Deleting file saver\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaver::~QxrdFileSaver(%p)\n", this);
  }
}

void QxrdFileSaver::haltFileSaver()
{
}

void QxrdFileSaver::setExperiment(QxrdExperimentWPtr expt)
{
  m_Experiment = expt;
}

QxrdExperimentWPtr QxrdFileSaver::experiment() const
{
  if (m_Experiment == NULL) {
    printMessage("experiment == NULL in QxrdFileSaver::experiment");
  }

  return m_Experiment;
}

void QxrdFileSaver::setProcessor(QxrdProcessorWPtr proc)
{
  m_Processor = proc;
}

void QxrdFileSaver::setAcquisition(QxrdAcqCommonWPtr acq)
{
  m_Acquisition = acq;
}

QxrdAcqCommonWPtr QxrdFileSaver::acquisition() const
{
  if (m_Acquisition == NULL) {
    printMessage("acquisition == NULL in QxrdFileSaver::acquisition");
  }

  return m_Acquisition;
}

void QxrdFileSaver::incBacklog()
{
  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    proc -> prop_SaverQueueLength()->incValue(1);
  }
}

void QxrdFileSaver::decBacklog()
{
  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    proc -> prop_SaverQueueLength()->incValue(-1);
  }
}

void QxrdFileSaver::mkPath(QString filePath)
{
  QFileInfo f(filePath);
  QDir dir = f.dir();

  if (!dir.exists()) {
    dir.mkpath(dir.absolutePath());
  }
}

QString QxrdFileSaver::uniqueFileName(QcepDataObjectPtr data,
                                      bool              canOverwrite)
{
  QString res;

  if (data) {
    res = uniqueFileName(data->get_FileDirectory(),
                         data,
                         canOverwrite,
                         data->get_FileExtension());
  }

  return res;
}

QString QxrdFileSaver::uniqueFileName(QcepDataObjectPtr data,
                                      bool              canOverwrite,
                                      QString           extension)
{
  QString res;

  if (data) {
    res = uniqueFileName(data->get_FileDirectory(),
                         data,
                         canOverwrite,
                         extension);
  }

  return res;
}

QString QxrdFileSaver::uniqueFileName(QString           dirName,
                                      QcepDataObjectPtr data,
                                      bool              canOverwrite)
{
  QString res;

  if (data) {
    res = uniqueFileName(dirName,
                         data,
                         canOverwrite,
                         data->get_FileExtension());
  }

  return res;
}

QString QxrdFileSaver::uniqueFileName(QString           dirName,
                                      QcepDataObjectPtr data,
                                      bool              canOverwrite,
                                      QString           extension)
{
  QString res = QString();

  if (data) {
    QDir dir(dirName);

    QString name = data->possibleFileName(extension);
    QString path = dir.filePath(name);

    QFileInfo f(path);

    if (f.exists()) {
      int i = data -> get_FileUniqueIndex()+1;

      for ( ; ; i++) {
        QString newName = data->possibleFileName(extension, i);
        QString newPath = dir.filePath(newName);
        QFileInfo f(newPath);

        if (!f.exists()) {
          data -> set_FileUniqueIndex(i);
          data -> set_FileName(newName);
          data -> set_FilePath(newPath);

          res = newPath;

          break;
        }
      }
    } else {
      data -> set_FileName(name);
      data -> set_FilePath(path);

      res = path;
    }
  }

  return res;
}

#define TIFFCHECK(a) if (res && ((a)==0)) { res = 0; }

void QxrdFileSaver::saveImageData(QcepImageDataBasePtr image,
                                  QcepMaskDataPtr      overflow,
                                  int                  canOverwrite)
{
  THREAD_CHECK;

  incBacklog();

  if (thread() != QThread::currentThread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveImageDataPrivate",
                                           Qt::QueuedConnection,
                                           Q_ARG(QcepImageDataBasePtr, image),
                                           Q_ARG(QcepMaskDataPtr, overflow),
                                           Q_ARG(int, canOverwrite)));
  } else {
    saveImageDataPrivate(image,
                         overflow,
                         canOverwrite);
  }
}

void QxrdFileSaver::saveImageDataPrivate(QcepImageDataBasePtr image,
                                         QcepMaskDataPtr      overflow,
                                         int                  canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    criticalMessage(tr("QxrdFileSaver::saveImageData: image == NULL"));
  } else {
    QcepDoubleImageDataPtr dimage = qSharedPointerDynamicCast<QcepDoubleImageData>(image);

    if (dimage) {
      saveDoubleDataPrivate(dimage, overflow, canOverwrite);
    } else {
      QcepUInt32ImageDataPtr i32image = qSharedPointerDynamicCast<QcepUInt32ImageData>(image);

      if (i32image) {
        saveRaw32DataPrivate(i32image, overflow, canOverwrite);
      } else {
        QcepUInt16ImageDataPtr i16image = qSharedPointerDynamicCast<QcepUInt16ImageData>(image);

        if (i16image) {
          saveRaw16DataPrivate(i16image, overflow, canOverwrite);
        } else {
          criticalMessage(tr("QxrdFileSaver::saveImageData: unknown image type"));

          decBacklog();
        }
      }
    }
  }
}

void QxrdFileSaver::saveDoubleData(QcepDoubleImageDataPtr image,
                                   QcepMaskDataPtr        overflow,
                                   int                    canOverwrite)
{
  incBacklog();

  if (thread() != QThread::currentThread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveDoubleDataPrivate",
                                           Qt::QueuedConnection,
                                           Q_ARG(QcepDoubleImageDataPtr,image),
                                           Q_ARG(QcepMaskDataPtr,overflow),
                                           Q_ARG(int,canOverwrite)));
  } else {
    saveDoubleDataPrivate(image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveDoubleDataPrivate(QcepDoubleImageDataPtr image,
                                          QcepMaskDataPtr        overflow,
                                          int                    canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    criticalMessage(tr("QxrdFileSaver::saveDoubleData: image == NULL"));
  } else {
    QTime tic;
    tic.start();

    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    QString name = uniqueFileName(image, canOverwrite);

    mkPath(name);

    int compress = 0;
    int level    = 0;

    std::ostringstream outStream;

    QxrdExperimentPtr expt(m_Experiment);

    if (expt && expt -> get_CompressOutputBZ2()) {
      compress = 1;
      level    = expt -> get_CompressLevelBZ2();
      name    += ".bz2";
    }

    printMessage(tr("Starting to save data in file \"%1\"").arg(name));

    TIFF* tif;

    if (compress == 0) {
      tif = qcepTIFFOpen(qPrintable(name),"w");
    } else {
      tif = qcepTIFFStreamOpen(qPrintable(name), &outStream);
    }

    int res = 1;

    if (tif) {
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP));

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(image->get_FileName())));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_DATETIME,     qPrintable(image->get_DateTime().toString("yyyy:MM:dd hh:mm:ss"))));

      QVector<float> buffvec(ncols);
      float* buffer = buffvec.data();

      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          buffer[x] = image->value(x,y);
        }

        TIFFCHECK(qcepTIFFWriteScanline(tif, buffer, y, 0));
      }

      qcepTIFFClose(tif);

      if (compress) {
        printMessage(tr("Started compressing data in file \"%1\" after %2 msec").
                                      arg(name).arg(tic.elapsed()));

        int bzerror;
        unsigned int nbytes_in, nbytes_out;

        FILE*   f   = fopen(qPrintable(name),"w");
        BZFILE* bzf = qcepBZ2_bzWriteOpen(&bzerror, f, level, 0, 0);

        const std::string& tmp = outStream.str();

        const char* b = tmp.c_str();
        int         n = tmp.length();

        qcepBZ2_bzWrite(&bzerror, bzf, (void*) b, n);
        qcepBZ2_bzWriteClose(&bzerror, bzf, 0, &nbytes_in, &nbytes_out);

        fclose(f);
      }

      image -> set_ObjectSaved(true);

      image -> saveMetaData(name);

      QxrdProcessorPtr proc(m_Processor);

      if (proc) {
        if (proc->get_SaveOverflowFiles()) {
          saveOverflowData(name, overflow);
        }

        proc -> updateEstimatedTime(proc -> prop_SaveSubtractedTime(), tic.elapsed());
        proc -> set_FileName(name);

        printMessage(tr("Saved data in file \"%1\" after %2 msec").
                                      arg(name).arg(tic.restart()));
      }
    } else {
      res = 0;
    }

    if (res == 0) {
      printMessage("Error saving file");
    }
  }

  decBacklog();
}

void QxrdFileSaver::saveInt32Data(QcepUInt32ImageDataPtr image,
                                  QcepMaskDataPtr        overflow,
                                  int                    canOverwrite)
{
  if (image == NULL) {
    criticalMessage(tr("QxrdFileSaver::saveInt32Data: image == NULL"));
  } else {
    saveRaw32Data(image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveInt16Data(QcepUInt16ImageDataPtr image,
                                  QcepMaskDataPtr        overflow,
                                  int                    canOverwrite)
{
  if (image == NULL) {
    criticalMessage(tr("QxrdFileSaver::saveInt16Data: image == NULL"));
  } else {
    saveRaw16Data(image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveMaskData(QcepMaskDataPtr image,
                                 int             canOverwrite)
{
  incBacklog();

  if (thread() != QThread::currentThread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveMaskDataPrivate",
                                           Qt::QueuedConnection,
                                           Q_ARG(QcepMaskDataPtr,image),
                                           Q_ARG(int,canOverwrite)));
  } else {
    saveMaskDataPrivate(image, canOverwrite);
  }
}

void QxrdFileSaver::saveMaskDataPrivate(QcepMaskDataPtr image,
                                        int             canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    criticalMessage(tr("QxrdFileSaver::saveMaskData: image == NULL"));
  } else {
    QTime tic;
    tic.start();

    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    QString name = uniqueFileName(image, canOverwrite);

    mkPath(name);

    int compress = 0;
    int level    = 0;

    std::ostringstream outStream;

    QxrdExperimentPtr expt(m_Experiment);

    if (expt && expt -> get_CompressOutputBZ2()) {
      compress = 1;
      level    = expt -> get_CompressLevelBZ2();
      name += ".bz2";
    }

    printMessage(tr("Starting to save data in file \"%1\"").arg(name));

    TIFF* tif;

    if (compress == 0) {
      tif = qcepTIFFOpen(qPrintable(name),"w");
    } else {
      tif = qcepTIFFStreamOpen(qPrintable(name), &outStream);
    }

    int res = 1;

    if (tif) {

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(image->get_FileName())));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_DATETIME,     qPrintable(image->get_DateTime().toString("yyyy:MM:dd hh:mm:ss"))));

      QVector<quint8> buffvec(ncols);
      quint8* buffer = buffvec.data();

      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          buffer[x] = image->value(x,y);
        }

        TIFFCHECK(qcepTIFFWriteScanline(tif, buffer, y, 0));
      }

      qcepTIFFClose(tif);

      if (compress) {
        printMessage(tr("Started compressing data in file \"%1\" after %2 msec").
                                      arg(name).arg(tic.elapsed()));

        int bzerror;
        unsigned int nbytes_in, nbytes_out;

        FILE*   f   = fopen(qPrintable(name),"w");
        BZFILE* bzf = qcepBZ2_bzWriteOpen(&bzerror, f, level, 0, 0);

        const std::string& tmp = outStream.str();

        const char* b = tmp.c_str();
        int         n = tmp.length();

        qcepBZ2_bzWrite(&bzerror, bzf, (void*) b, n);
        qcepBZ2_bzWriteClose(&bzerror, bzf, 0, &nbytes_in, &nbytes_out);

        fclose(f);
      }

      image -> set_ObjectSaved(true);

      image -> saveMetaData(name);

      printMessage(tr("Saved data in file \"%1\" after %2 msec").
                                    arg(name).arg(tic.restart()));
    } else {
      res = 0;
    }

    if (res == 0) {
      printMessage("Error saving file");
    }
  }

  decBacklog();
}

void QxrdFileSaver::saveRaw32Data(QcepUInt32ImageDataPtr image,
                                  QcepMaskDataPtr        overflow,
                                  int                    canOverwrite)
{
  incBacklog();

  if (thread() != QThread::currentThread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveRaw32DataPrivate",
                                           Qt::QueuedConnection,
                                           Q_ARG(QcepUInt32ImageDataPtr,image),
                                           Q_ARG(QcepMaskDataPtr,overflow),
                                           Q_ARG(int,canOverwrite)));
  } else {
    saveRaw32DataPrivate(image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveRaw32DataPrivate(QcepUInt32ImageDataPtr image,
                                         QcepMaskDataPtr        overflow,
                                         int                    canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    criticalMessage(tr("QxrdFileSaver::saveRaw32Data: image == NULL"));
  } else {
    QTime tic;
    tic.start();

    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    QString name = uniqueFileName(image, canOverwrite);

    mkPath(name);

    int compress = 0;
    int level    = 0;

    std::ostringstream outStream;

    QxrdExperimentPtr expt(m_Experiment);

    if (expt && expt -> get_CompressOutputBZ2()) {
      compress = 1;
      level    = expt -> get_CompressLevelBZ2();
      name    += ".bz2";
    }

    printMessage(tr("Starting to save data in file \"%1\"").arg(name));

    TIFF* tif;

    if (compress == 0) {
      tif = qcepTIFFOpen(qPrintable(name),"w");
    } else {
      tif = qcepTIFFStreamOpen(qPrintable(name), &outStream);
    }

    int res = 1;

    if (tif) {
      int nsum = image->get_SummedExposures();

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));

      if (nsum == 0) {
        TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8));
      } else if (nsum == 1) {
        TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16));
      } else {
        TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
      }

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(image->get_FileName())));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_DATETIME,     qPrintable(image->get_DateTime().toString("yyyy:MM:dd hh:mm:ss"))));

      if (nsum == 0) {
        QVector<quint8> buffvec(ncols);
        quint8* buffer = buffvec.data();

        for (int y=0; y<nrows; y++) {
          for (int x=0; x<ncols; x++) {
            buffer[x] = image->value(x,y);
          }

          TIFFCHECK(qcepTIFFWriteScanline(tif, buffer, y, 0));
        }
      } else if (nsum == 1) {
        QVector<quint16> buffvec(ncols);
        quint16* buffer = buffvec.data();

        for (int y=0; y<nrows; y++) {
          for (int x=0; x<ncols; x++) {
            buffer[x] = image->value(x,y);
          }

          TIFFCHECK(qcepTIFFWriteScanline(tif, buffer, y, 0));
        }
      } else {
        QVector<quint32> buffvec(ncols);
        quint32* buffer = buffvec.data();

        for (int y=0; y<nrows; y++) {
          for (int x=0; x<ncols; x++) {
            buffer[x] = image->value(x,y);
          }

          TIFFCHECK(qcepTIFFWriteScanline(tif, buffer, y, 0));
        }
      }

      qcepTIFFClose(tif);

      if (compress) {
        printMessage(tr("Started compressing data in file \"%1\" after %2 msec").
                                      arg(name).arg(tic.elapsed()));

        int bzerror;
        unsigned int nbytes_in, nbytes_out;

        FILE*   f   = fopen(qPrintable(name),"w");
        BZFILE* bzf = qcepBZ2_bzWriteOpen(&bzerror, f, level, 0, 0);

        const std::string& tmp = outStream.str();

        const char* b = tmp.c_str();
        int         n = tmp.length();

        qcepBZ2_bzWrite(&bzerror, bzf, (void*) b, n);
        qcepBZ2_bzWriteClose(&bzerror, bzf, 0, &nbytes_in, &nbytes_out);

        fclose(f);
      }

      image -> set_ObjectSaved(true);

      image -> saveMetaData(name);

      QxrdProcessorPtr proc(m_Processor);
      QxrdAcqCommonPtr acq(m_Acquisition);

      if (proc) {
        if (proc->get_SaveOverflowFiles()) {
          saveOverflowData(name, overflow);
        }

        if (acq) {
          proc -> updateEstimatedTime(acq -> prop_Raw32SaveTime(), tic.elapsed());
        }

        proc -> set_FileName(name);

        printMessage(tr("Saved 32 bit data in file \"%1\" after %2 msec").
                     arg(name).arg(tic.restart()));
      }
    } else {
      res = 0;
    }

    if (res == 0) {
      printMessage("Error saving file");
    }
  }

  decBacklog();
}

void QxrdFileSaver::saveRaw16Data(QcepUInt16ImageDataPtr image,
                                  QcepMaskDataPtr        overflow,
                                  int                    canOverwrite)
{
  incBacklog();

  if (thread() != QThread::currentThread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveRaw16DataPrivate",
                                           Qt::QueuedConnection,
                                           Q_ARG(QcepUInt16ImageDataPtr,image),
                                           Q_ARG(QcepMaskDataPtr,overflow),
                                           Q_ARG(int,canOverwrite)));
  } else {
    saveRaw16DataPrivate(image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveRaw16DataPrivate(QcepUInt16ImageDataPtr image,
                                         QcepMaskDataPtr        overflow,
                                         int                    canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    criticalMessage(tr("QxrdFileSaver::saveRaw16Data: image == NULL"));
  } else {
    QTime tic;
    tic.start();

    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    QString name = uniqueFileName(image, canOverwrite);

    mkPath(name);

    int compress = 0;
    int level    = 0;

    std::ostringstream outStream;

    QxrdExperimentPtr expt(m_Experiment);

    if (expt && expt -> get_CompressOutputBZ2()) {
      compress = 1;
      level    = expt -> get_CompressLevelBZ2();
      name += ".bz2";
    }

    printMessage(tr("Starting to save data in file \"%1\"").arg(name));

    TIFF* tif;

    if (compress == 0) {
      tif = qcepTIFFOpen(qPrintable(name),"w");
    } else {
      tif = qcepTIFFStreamOpen(qPrintable(name), &outStream);
    }

    int res = 1;

    if (tif) {

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(image->get_FileName())));
      TIFFCHECK(qcepTIFFSetField(tif, TIFFTAG_DATETIME,     qPrintable(image->get_DateTime().toString("yyyy:MM:dd hh:mm:ss"))));

      QVector<quint16> buffvec(ncols);
      quint16* buffer = buffvec.data();

      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          buffer[x] = image->value(x,y);
        }

        TIFFCHECK(qcepTIFFWriteScanline(tif, buffer, y, 0));
      }

      qcepTIFFClose(tif);

      if (compress) {
        printMessage(tr("Started compressing data in file \"%1\" after %2 msec").
                                      arg(name).arg(tic.elapsed()));

        int bzerror;
        unsigned int nbytes_in, nbytes_out;

        FILE*   f   = fopen(qPrintable(name),"w");
        BZFILE* bzf = qcepBZ2_bzWriteOpen(&bzerror, f, level, 0, 0);

        const std::string& tmp = outStream.str();

        const char* b = tmp.c_str();
        int         n = tmp.length();

        qcepBZ2_bzWrite(&bzerror, bzf, (void*) b, n);
        qcepBZ2_bzWriteClose(&bzerror, bzf, 0, &nbytes_in, &nbytes_out);

        fclose(f);
      }

      image -> set_ObjectSaved(true);

      image -> saveMetaData(name);

      QxrdProcessorPtr proc(m_Processor);
      QxrdAcqCommonPtr acq(m_Acquisition);

      if (proc) {
        if (proc->get_SaveOverflowFiles()) {
          saveOverflowData(name, overflow);
        }

        if (acq) {
          proc -> updateEstimatedTime(acq -> prop_Raw16SaveTime(), tic.elapsed());
        }

        proc -> set_FileName(name);

        printMessage(tr("Saved 16 bit data in file \"%1\" after %2 msec").
                                      arg(name).arg(tic.restart()));
      }
    } else {
      res = 0;
    }

    if (res == 0) {
      printMessage("Error saving file");
    }
  }

  decBacklog();
}

void QxrdFileSaver::saveTextData(QcepDoubleImageDataPtr image,
                                 QcepMaskDataPtr        overflow,
                                 int                    canOverwrite)
{
  incBacklog();

  if (thread() != QThread::currentThread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveTextDataPrivate",
                                           Qt::QueuedConnection,
                                           Q_ARG(QcepDoubleImageDataPtr,image),
                                           Q_ARG(QcepMaskDataPtr,overflow),
                                           Q_ARG(int,canOverwrite)));
  } else {
    saveTextDataPrivate(image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveTextDataPrivate(QcepDoubleImageDataPtr image,
                                        QcepMaskDataPtr        overflow,
                                        int                    canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    criticalMessage(tr("QxrdFileSaver::saveTextData: image == NULL"));
  } else {
    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    QString name = uniqueFileName(image, canOverwrite, ".txt");

    mkPath(name);

    FILE* file = fopen(qPrintable(name),"a");

    QxrdProcessorPtr proc(m_Processor);

    QString separator = "\t";

    if (proc) {
      separator = proc -> get_SaveAsTextSeparator();
    }

    char sep[10];
    strncpy(sep,qPrintable(separator),10);
    sep[9]=0;
    int nperline = 8;

    if (proc) {
      nperline = proc -> get_SaveAsTextPerLine();
    }

    for (int y=0; y<nrows; y++) {
      for (int x=0; x<ncols; x++) {
        if (nperline && ((x % nperline)==0)) {
          fputs("\n",file);
        } else if (x!=0) {
          fputs(sep,file);
        }
        fprintf(file,"%g",image->value(x,y));
      }
    }
    fprintf(file,"\n");

    fclose(file);

    image -> saveMetaData(name);

    if (proc) {
      if (proc->get_SaveOverflowFiles()) {
        saveOverflowData(name, overflow);
      }
    }
  }

  decBacklog();
}

void QxrdFileSaver::writeOutputScan(QString               dir,
                                    QcepIntegratedDataPtr data,
                                    QString               fileName)
{
  incBacklog();

  if (thread() != QThread::currentThread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "writeOutputScanPrivate",
                                           Qt::QueuedConnection,
                                           Q_ARG(QString,dir),
                                           Q_ARG(QcepIntegratedDataPtr,data),
                                           Q_ARG(QString,fileName)));
  } else {
    writeOutputScanPrivate(dir, data, fileName);
  }
}

void QxrdFileSaver::writeOutputScanPrivate(QString               dirName,
                                           QcepIntegratedDataPtr data,
                                           QString               fileName)
{
  THREAD_CHECK;

  if (data == NULL) {
    criticalMessage(tr("QxrdFileSaver::writeOutputScan: data == NULL"));
  } else {
    if (fileName.isNull()) {
      fileName    = data -> get_FileName();
    }

    QFileInfo fi(fileName);

    QString extension = ".avg";

    QxrdExperimentPtr exp(m_Experiment);

    if (exp) {
      extension = exp->get_ScanFileExtension();
    }

    QString name = uniqueFileName(dirName, data, false, extension);

    mkPath(name);

    FILE *f = fopen(qPrintable(name),"a");

    if (f == NULL) {
      printMessage(tr("Couldn't open file %1").arg(name));
    } else {
      writeOutputScanPrivate(f, data, fileName);

      fclose(f);

      printMessage(tr("Integrated data saved in %1").arg(name));

      return;
    }
  }

  decBacklog();
}

void QxrdFileSaver::writeOutputScan(FILE*                 logFile,
                                    QcepIntegratedDataPtr data,
                                    QString               fileName)
{
  incBacklog();

  if (thread() != QThread::currentThread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "writeOutputScanPrivate",
                                           Qt::QueuedConnection,
                                           Q_ARG(FILE*, logFile),
                                           Q_ARG(QcepIntegratedDataPtr,data),
                                           Q_ARG(QString,fileName)));
  } else {
    writeOutputScanPrivate(logFile, data, fileName);
  }
}

void QxrdFileSaver::writeOutputScanPrivate(FILE*                 logFile,
                                           QcepIntegratedDataPtr data,
                                           QString               fileName)
{
  THREAD_CHECK;

  if (data == NULL) {
    criticalMessage(tr("QxrdFileSaver::writeOutputScan: data == NULL"));
  } else if (logFile) {
    QTime tic;
    tic.start();
    int scanNumber = 0;

    if (fileName.isNull()) {
      fileName    = data -> get_FileName();
      scanNumber = data -> get_ScanNumber();
    }

    fprintf(logFile, "#S %d qxrd.integrate \"%s\" %g %g\n",
            scanNumber,
            qPrintable(fileName),
            data -> cx(),
            data -> cy());

    fprintf(logFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
    fprintf(logFile, "#N 2\n");
    fprintf(logFile, "#L x  y\n");

    int n = data->size();
    const double *x = data->x();
    const double *y = data->y();

    int negh = 0;
    QxrdExperimentPtr exp(m_Experiment);

    if (exp) {
      negh = exp->get_ScanDataNegative();
    }

    if (negh == 0) {
      for (int i=0; i<n; i++) {
        fprintf(logFile, "%E %E\n", x[i], y[i]);
      }
    } else if (negh == 1) {
      for (int i=0; i<n; i++) {
        if (y[i] >= 0) {
          fprintf(logFile, "%E %E\n", x[i], y[i]);
        } else {
          fprintf(logFile, "%E %E\n", x[i], 0.0);
        }
      }
    } else {
      for (int i=0; i<n; i++) {
        if (y[i] >= 0) {
          fprintf(logFile, "%E %E\n", x[i], y[i]);
        }
      }
    }

    fflush(logFile);

    QxrdProcessorPtr proc(m_Processor);

    if (proc) {
      proc -> updateEstimatedTime(proc -> prop_SaveIntegratedDataTime(), tic.restart());
    }
  }

  decBacklog();
}

void QxrdFileSaver::saveOverflowData(QString name, QcepMaskDataPtr overflow)
{
  if (overflow) {
    QString ovfname = name+".overflow";

    FILE *ovfile = fopen(qPrintable(ovfname),"w+");

    if (ovfile) {
      int novf = overflow ->countOverflowPixels();

      fprintf(ovfile, "#S %d overflow pixels for file %s\n", novf, qPrintable(name));
      fprintf(ovfile, "#N 3\n");
      fprintf(ovfile, "#L x  y  d\n");

      int ncols = overflow -> get_Width();
      int nrows = overflow -> get_Height();

      for (int row=0; row<nrows; row++) {
        for (int col=0; col<ncols; col++) {
          if (overflow->value(col,row)) {
            fprintf(ovfile, "%d\t%d\t1\n", col, row);
          }
        }
      }
    }

    fclose(ovfile);
  }
}
