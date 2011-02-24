#include "qxrdfilesaver.h"
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"

#include "tiffio.h"

QxrdFileSaver::QxrdFileSaver
    (QxrdAllocatorPtr allocator, QObject *parent)
  : QObject(parent),
    m_Processor(NULL),
    m_Allocator(allocator),
    m_Acquisition(NULL)
{
}

QxrdFileSaver::~QxrdFileSaver()
{
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
    printf("processor == NULL in QxrdFileSaver::processor\n");
  }

  return m_Processor;
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

    for (int i=1; ; i++) {
      QString newname = dir.filePath(base+QString().sprintf("-%05d.",i)+suff);
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
  QTime tic;
  tic.start();

  THREAD_CHECK;

  if (image == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::saveData: image == NULL"));
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

      emit printMessage(QDateTime::currentDateTime(),
                        tr("Saved subtracted data in file \"%1\" after %2 msec").
                        arg(name).arg(tic.restart()));
    } else {
      res = 0;
    }

    if (res == 0) {
      emit printMessage(QDateTime::currentDateTime(), "Error saving file");
    }
  }
}

void QxrdFileSaver::saveData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::saveData: image == NULL"));
  } else {
    saveRawData(name, image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::saveData: image == NULL"));
  } else {
    saveRawData(name, image, overflow, canOverwrite);
  }
}

void QxrdFileSaver::saveData(QString name, QxrdMaskDataPtr image, int canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::saveData: image == NULL"));
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
      emit printMessage(QDateTime::currentDateTime(), "Error saving file");
    }
  }
}

void QxrdFileSaver::saveRawData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  QTime tic;
  tic.start();

  THREAD_CHECK;

  if (image == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::saveRawData: image == NULL"));
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
      int nsum = image->get_SummedExposures();

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ncols));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nrows));
      TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1));

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG));

      if (nsum == 1) {
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16));
      } else {
        TIFFCHECK(TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32));
      }

      TIFFCHECK(TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT));

      image -> setTiffMetaData(tif);

      if (nsum == 1) {
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

      emit printMessage(QDateTime::currentDateTime(),
                        tr("Saved raw data in file \"%1\" after %2 msec").
                        arg(name).arg(tic.restart()));
    } else {
      res = 0;
    }

    if (res == 0) {
      emit printMessage(QDateTime::currentDateTime(), "Error saving file");
    }
  }
}

void QxrdFileSaver::saveRawData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  QTime tic;
  tic.start();

  THREAD_CHECK;

  if (image == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::saveRawData: image == NULL"));
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

      emit printMessage(QDateTime::currentDateTime(),
                        tr("Saved raw data in file \"%1\" after %2 msec").
                        arg(name).arg(tic.restart()));
    } else {
      res = 0;
    }

    if (res == 0) {
      emit printMessage(QDateTime::currentDateTime(), "Error saving file");
    }
  }
}

void QxrdFileSaver::saveTextData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  THREAD_CHECK;

  if (image == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::saveTextData: image == NULL"));
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

void QxrdFileSaver::writeOutputScan(QString dir, QxrdIntegratedDataPtr data)
{
  THREAD_CHECK;

  if (data == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::writeOutputScan: data == NULL"));
  } else {
    mkPath(dir);

    QxrdDoubleImageDataPtr image = data -> get_Image();

    if (image == NULL) {
      emit criticalMessage(QDateTime::currentDateTime(),
                           tr("QxrdFileSaver::writeOutputScan: image == NULL"));
    } else {
      QFileInfo fi(image->get_FileName());

      QString fileBase = fi.completeBaseName();

      QString name = QDir(dir).filePath(fileBase+".avg");

      name = uniqueFileName(name);

      FILE *f = fopen(qPrintable(name),"a");

      if (f == NULL) {
        emit criticalMessage(QDateTime::currentDateTime(),
                             tr("Couldn't open file %1").arg(name));
      } else {
        writeOutputScan(f, data);

        fclose(f);

        emit printMessage(QDateTime::currentDateTime(),
                          tr("Integrated data saved in %1").arg(name));
      }
    }
  }
}

void QxrdFileSaver::writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data)
{
  THREAD_CHECK;

  if (data == NULL) {
    emit criticalMessage(QDateTime::currentDateTime(),
                         tr("QxrdFileSaver::writeOutputScan: data == NULL"));
  } else if (logFile) {
    QTime tic;
    tic.start();

    QxrdDoubleImageDataPtr image = data -> get_Image();

    if (image == NULL) {
      emit criticalMessage(QDateTime::currentDateTime(),
                           tr("QxrdFileSaver::writeOutputScan: image == NULL"));
    } else {
      fprintf(logFile, "#S %d qxrd.integrate \"%s\" %g %g\n",
              image -> get_ImageNumber(),
              qPrintable(image -> get_FileName()),
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
