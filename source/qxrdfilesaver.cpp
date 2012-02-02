#include "qxrdfilesaver.h"
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"

#include "tiffio.h"

QxrdFileSaver::QxrdFileSaver
    (QxrdAllocatorPtr allocator, QObject *parent)
  : QObject(parent),
    m_Processor(NULL),
    m_Allocator(allocator),
    m_Acquisition(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaver::QxrdFileSaver(%p)\n", this);
  }
}

QxrdFileSaver::~QxrdFileSaver()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaver::~QxrdFileSaver(%p)\n", this);
  }
}

void QxrdFileSaver::setProcessor(QxrdDataProcessorPtr proc)
{
  m_Processor = proc;
}

void QxrdFileSaver::setAcquisition(QxrdAcquisitionPtr acq)
{
  m_Acquisition = acq;
}

QxrdDataProcessorPtr QxrdFileSaver::processor() const
{
  if (m_Processor == NULL) {
    g_Application->printMessage("processor == NULL in QxrdFileSaver::processor\n");
  }

  return m_Processor;
}

QxrdAcquisitionPtr QxrdFileSaver::acquisition() const
{
  if (m_Acquisition == NULL) {
    g_Application->printMessage("acquisition == NULL in QxrdFileSaver::acquisition\n");
  }

  return m_Acquisition;
}

void QxrdFileSaver::mkPath(QString filePath)
{
  QFileInfo f(filePath);
  QDir dir = f.dir();

  if (!dir.exists()) {
    dir.mkpath(dir.absolutePath());
  }
}

QString QxrdFileSaver::uniqueFileName(QString name)
{
  THREAD_CHECK;

  QFileInfo f(name);

  if (f.exists()) {
    QDir dir = f.dir();
    QString base = f.baseName();
    QString suff = f.completeSuffix();

    int width = acquisition()->get_FileOverflowWidth();

    for (int i=1; ; i++) {
      QString newname = dir.filePath(base+QString().sprintf("-%0*d.",width,i)+suff);
      QFileInfo f(newname);

      if (!f.exists()) {
        return newname;
      }
    }
  } else {
    return name;
  }
}

#define TIFFCHECK(a) if (res && ((a)==0)) { res = 0; }

void QxrdFileSaver::saveData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveData",
                                           Q_ARG(QString,name),
                                           Q_ARG(QxrdDoubleImageDataPtr,image),
                                           Q_ARG(QxrdMaskDataPtr,overflow),
                                           Q_ARG(int,canOverwrite)))
  } else {
    QTime tic;
    tic.start();

    if (image == NULL) {
      g_Application->criticalMessage(tr("QxrdFileSaver::saveData: image == NULL"));
    } else {
      int nrows = image -> get_Height();
      int ncols = image -> get_Width();

      mkPath(name);

      if (canOverwrite == NoOverwrite) {
        name = uniqueFileName(name);
      }

      TIFF* tif = TIFFOpen(qPrintable(name),"w");
      int res = 1;

      if (tif) {
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

        TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP));

        image -> setTiffMetaData(tif);

        QVector<float> buffvec(ncols);
        float* buffer = buffvec.data();

        for (int y=0; y<nrows; y++) {
          for (int x=0; x<ncols; x++) {
            buffer[x] = image->value(x,y);
          }

          TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
        }

        TIFFClose(tif);

        image -> set_FileName(name);
        image -> set_ImageSaved(true);

        image -> saveMetaData();

        if (processor()->get_SaveOverflowFiles()) {
          saveOverflowData(name, overflow);
        }

        processor() -> updateEstimatedTime(processor() -> prop_SaveSubtractedTime(), tic.elapsed());
        processor() -> set_FileName(name);

        g_Application->printMessage(tr("Saved subtracted data in file \"%1\" after %2 msec").
                                    arg(name).arg(tic.restart()));
      } else {
        res = 0;
      }

      if (res == 0) {
        g_Application->printMessage("Error saving file");
      }
    }
  }
}

void QxrdFileSaver::saveData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (image == NULL) {
    g_Application->criticalMessage(tr("QxrdFileSaver::saveData: image == NULL"));
  } else {
    saveRawData(name, image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (image == NULL) {
    g_Application->criticalMessage(tr("QxrdFileSaver::saveData: image == NULL"));
  } else {
    saveRawData(name, image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveData(QString name, QxrdMaskDataPtr image, int canOverwrite)
{
  if (image == NULL) {
    g_Application->criticalMessage(tr("QxrdFileSaver::saveData: image == NULL"));
  } else if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveData",
                                           Q_ARG(QString,name),
                                           Q_ARG(QxrdMaskDataPtr,image),
                                           Q_ARG(int,canOverwrite)))
  } else {
    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    mkPath(name);

    if (canOverwrite == NoOverwrite) {
      name = uniqueFileName(name);
    }

    TIFF* tif = TIFFOpen(qPrintable(name),"w");
    int res = 1;

    if (tif) {

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

      image -> setTiffMetaData(tif);

      QVector<quint8> buffvec(ncols);
      quint8* buffer = buffvec.data();

      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          buffer[x] = image->value(x,y);
        }

        TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
      }

      TIFFClose(tif);

      image -> set_FileName(name);
      image -> set_ImageSaved(true);

      image -> saveMetaData();
    } else {
      res = 0;
    }

    if (res == 0) {
      g_Application->printMessage("Error saving file");
    }
  }
}

void QxrdFileSaver::saveRawData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (image == NULL) {
    g_Application->criticalMessage(tr("QxrdFileSaver::saveRawData: image == NULL"));
  } else if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveRawData",
                                           Q_ARG(QString,name),
                                           Q_ARG(QxrdInt32ImageDataPtr,image),
                                           Q_ARG(QxrdMaskDataPtr,overflow),
                                           Q_ARG(int,canOverwrite)))
  } else {
    QTime tic;
    tic.start();

    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    mkPath(name);

    if (canOverwrite == NoOverwrite) {
      name = uniqueFileName(name);
    }

    TIFF* tif = TIFFOpen(qPrintable(name),"w");
    int res = 1;

    if (tif) {
      int nsum = image->get_SummedExposures();

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));

      if (nsum == 0) {
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8));
      } else if (nsum == 1) {
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16));
      } else {
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
      }

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

      image -> setTiffMetaData(tif);

      if (nsum == 0) {
        QVector<quint8> buffvec(ncols);
        quint8* buffer = buffvec.data();

        for (int y=0; y<nrows; y++) {
          for (int x=0; x<ncols; x++) {
            buffer[x] = image->value(x,y);
          }

          TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
        }
      } else if (nsum == 1) {
        QVector<quint16> buffvec(ncols);
        quint16* buffer = buffvec.data();

        for (int y=0; y<nrows; y++) {
          for (int x=0; x<ncols; x++) {
            buffer[x] = image->value(x,y);
          }

          TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
        }
      } else {
        QVector<quint32> buffvec(ncols);
        quint32* buffer = buffvec.data();

        for (int y=0; y<nrows; y++) {
          for (int x=0; x<ncols; x++) {
            buffer[x] = image->value(x,y);
          }

          TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
        }
      }

      TIFFClose(tif);

      image -> set_ImageSaved(true);
      image -> saveMetaData(name);

      if (processor()->get_SaveOverflowFiles()) {
        saveOverflowData(name, overflow);
      }

      processor() -> updateEstimatedTime(m_Acquisition -> prop_Raw32SaveTime(), tic.elapsed());
      processor() -> set_FileName(name);

      g_Application->printMessage(tr("Saved raw data in file \"%1\" after %2 msec").
                        arg(name).arg(tic.restart()));
    } else {
      res = 0;
    }

    if (res == 0) {
      g_Application->printMessage("Error saving file");
    }
  }
}

void QxrdFileSaver::saveRawData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (image == NULL) {
    g_Application->criticalMessage(tr("QxrdFileSaver::saveRawData: image == NULL"));
  } else if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveRawData",
                                           Q_ARG(QString,name),
                                           Q_ARG(QxrdInt16ImageDataPtr,image),
                                           Q_ARG(QxrdMaskDataPtr,overflow),
                                           Q_ARG(int,canOverwrite)))
  } else {
    QTime tic;
    tic.start();

    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    mkPath(name);

    if (canOverwrite == NoOverwrite) {
      name = uniqueFileName(name);
    }

    TIFF* tif = TIFFOpen(qPrintable(name),"w");
    int res = 1;

    if (tif) {

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

      image -> setTiffMetaData(tif);

      QVector<quint16> buffvec(ncols);
      quint16* buffer = buffvec.data();

      for (int y=0; y<nrows; y++) {
        for (int x=0; x<ncols; x++) {
          buffer[x] = image->value(x,y);
        }

        TIFFCHECK(TIFFWriteScanline(tif, buffer, y, 0));
      }

      TIFFClose(tif);

      image -> set_FileName(name);
      image -> set_ImageSaved(true);

      image -> saveMetaData(name);

      if (processor()->get_SaveOverflowFiles()) {
        saveOverflowData(name, overflow);
      }

      processor() -> updateEstimatedTime(m_Acquisition -> prop_Raw16SaveTime(), tic.elapsed());
      processor() -> set_FileName(name);

      g_Application->printMessage(tr("Saved raw data in file \"%1\" after %2 msec").
                        arg(name).arg(tic.restart()));
    } else {
      res = 0;
    }

    if (res == 0) {
      g_Application->printMessage("Error saving file");
    }
  }
}

void QxrdFileSaver::saveTextData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (image == NULL) {
    g_Application->criticalMessage(tr("QxrdFileSaver::saveTextData: image == NULL"));
  } else if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "saveTextData",
                                           Q_ARG(QString,name),
                                           Q_ARG(QxrdDoubleImageDataPtr,image),
                                           Q_ARG(QxrdMaskDataPtr,overflow),
                                           Q_ARG(int,canOverwrite)))
  } else {
    int nrows = image -> get_Height();
    int ncols = image -> get_Width();

    QFileInfo f(name);
    QDir dir = f.dir();
    QString  base = f.completeBaseName();
    QString  suff = f.suffix();

    if (suff == "tif") {
      name = dir.filePath(base+".txt");
    }

    mkPath(name);

    if (canOverwrite == NoOverwrite) {
      name = uniqueFileName(name);
    }

    FILE* file = fopen(qPrintable(name),"a");
    QString separator = processor() -> get_SaveAsTextSeparator();
    char sep[10];
    strncpy(sep,qPrintable(separator),10);
    sep[9]=0;
    int nperline = processor() -> get_SaveAsTextPerLine();

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

    image -> set_FileName(name);
    image -> set_ImageSaved(true);

    image -> saveMetaData();

    if (processor()->get_SaveOverflowFiles()) {
      saveOverflowData(name, overflow);
    }

    processor() -> set_FileName(name);
  }
}

void QxrdFileSaver::writeOutputScan(QString dir, QxrdIntegratedDataPtr data, QString fileName)
{
  THREAD_CHECK;

  if (data == NULL) {
    g_Application->criticalMessage(tr("QxrdFileSaver::writeOutputScan: data == NULL"));
  } else if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "writeOutputScan",
                                           Q_ARG(QString,dir),
                                           Q_ARG(QxrdIntegratedDataPtr,data),
                                           Q_ARG(QString,fileName)))
  } else {
    mkPath(dir);

    if (fileName.isNull()) {
      QxrdDoubleImageDataPtr image = data -> get_Image();
      if (image == NULL) {
        g_Application->criticalMessage(tr("QxrdFileSaver::writeOutputScan: image == NULL"));
        return;
      }

      fileName    = image -> get_FileName();
    }

    QFileInfo fi(fileName);

    QString fileBase = fi.completeBaseName();

    QString name = QDir(dir).filePath(fileBase+".avg");

    name = uniqueFileName(name);

    FILE *f = fopen(qPrintable(name),"a");

    if (f == NULL) {
      g_Application->criticalMessage(tr("Couldn't open file %1").arg(name));
    } else {
      writeOutputScan(f, data, fileName);

      fclose(f);

      g_Application->printMessage(tr("Integrated data saved in %1").arg(name));
    }
  }
}

void QxrdFileSaver::writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data, QString fileName)
{
  if (data == NULL) {
    g_Application->criticalMessage(tr("QxrdFileSaver::writeOutputScan: data == NULL"));
  } else if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "writeOutputScan",
                                           Q_ARG(FILE*, logFile),
                                           Q_ARG(QxrdIntegratedDataPtr,data),
                                           Q_ARG(QString,fileName)))
  } else if (logFile) {
    QTime tic;
    tic.start();
    int imageNumber = 0;

    if (fileName.isNull()) {
      QxrdDoubleImageDataPtr image = data -> get_Image();
      if (image == NULL) {
        g_Application->criticalMessage(tr("QxrdFileSaver::writeOutputScan: image == NULL"));
        return;
      }

      fileName    = image -> get_FileName();
      imageNumber = image -> get_ImageNumber();
    }

    fprintf(logFile, "#S %d qxrd.integrate \"%s\" %g %g\n",
            imageNumber,
            qPrintable(fileName),
            data -> cx(),
            data -> cy());

    fprintf(logFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy")));
    fprintf(logFile, "#N 2\n");
    fprintf(logFile, "#L x  y\n");

    int n = data->size();
    const double *x = data->x();
    const double *y = data->y();

    for (int i=0; i<n; i++) {
      fprintf(logFile, "%g %g\n", x[i], y[i]);
    }

    fflush(logFile);

    processor() -> updateEstimatedTime(m_Processor -> prop_SaveIntegratedDataTime(), tic.restart());
  }
}

void QxrdFileSaver::saveOverflowData(QString name, QxrdMaskDataPtr overflow)
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
