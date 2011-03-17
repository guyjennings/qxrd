#include "qcepimagedata.h"
#include <tiffio.h>

#include <QSettings>
#include <QFileInfo>

//static int allocCount = 0;

QcepImageDataBase::QcepImageDataBase(int width, int height)
  : QObject(),
    m_DataType(this, "dataType", UndefinedData),
    m_FileBase(this, "fileBase", ""),
    m_FileName(this, "fileName", ""),
    m_Title(this, "title", ""),
    m_ReadoutMode(this, "readoutMode", 0),
    m_ExposureTime(this, "exposureTime", 0),
    m_SummedExposures(this, "summedExposures", 0),
    m_ImageNumber(this, "imageNumber", 0),
    m_PhaseNumber(this, "phaseNumber", -1),
    m_NPhases(this, "nPhases", -1),
    m_DateTime(this, "dateTime", QDateTime::currentDateTime()),
    m_HBinning(this, "hBinning", 1),
    m_VBinning(this, "vBinning", 1),
    m_CameraGain(this, "cameraGain", 0),
    m_Triggered(this, "triggered", 0),
    m_UserComment1(this,"userComment1",""),
    m_UserComment2(this,"userComment2",""),
    m_UserComment3(this,"userComment3",""),
    m_UserComment4(this,"userComment4",""),
    m_ImageSaved(this,"imageSaved",0),
    m_Normalization(this, "normalization", QcepDoubleList()),
    m_Used(this, "used", true),
    m_Width(width),
    m_Height(height),
    m_Mutex(QMutex::Recursive)
{
//    printf("QcepImageDataBase::QcepImageDataBase allocate %d\n", allocCount++);
}

QcepImageDataBase::~QcepImageDataBase()
{
//    printf("QcepImageDataBase::QcepImageDataBase deallocate %d\n", allocCount--);
}

QMutex *QcepImageDataBase::mutex()
{
  return &m_Mutex;
}

void QcepImageDataBase::copyProperties(QcepImageDataBase *dest)
{
  dest -> set_DataType(get_DataType());
  dest -> set_FileBase(get_FileBase());
  dest -> set_FileName(get_FileName());
  dest -> set_Title(get_Title());
  dest -> set_ReadoutMode(get_ReadoutMode());
  dest -> set_ExposureTime(get_ExposureTime());
  dest -> set_SummedExposures(get_SummedExposures());
  dest -> set_ImageNumber(get_ImageNumber());
  dest -> set_PhaseNumber(get_PhaseNumber());
  dest -> set_NPhases(get_NPhases());
  dest -> set_DateTime(get_DateTime());
  dest -> set_HBinning(get_HBinning());
  dest -> set_VBinning(get_VBinning());
  dest -> set_CameraGain(get_CameraGain());
  dest -> set_Triggered(get_Triggered());
  dest -> set_UserComment1(get_UserComment1());
  dest -> set_UserComment2(get_UserComment2());
  dest -> set_UserComment3(get_UserComment3());
  dest -> set_UserComment4(get_UserComment4());
  dest -> set_ImageSaved(get_ImageSaved());
  dest -> set_Normalization(get_Normalization());
  dest -> set_Used(get_Used());

  QByteArray name;

  foreach(name, dynamicPropertyNames()) {
    dest -> setProperty(name.data(), property(name.data()));
  }
}

void QcepImageDataBase::copyPropertiesFrom(QSharedPointer<QcepImageDataBase> src)
{
  set_DataType(src -> get_DataType());
  set_FileBase(src -> get_FileBase());
  set_FileName(src -> get_FileName());
  set_Title(src -> get_Title());
  set_ReadoutMode(src -> get_ReadoutMode());
  set_ExposureTime(src -> get_ExposureTime());
  set_SummedExposures(src -> get_SummedExposures());
  set_ImageNumber(src -> get_ImageNumber());
  set_PhaseNumber(src -> get_PhaseNumber());
  set_NPhases(src -> get_NPhases());
  set_DateTime(src -> get_DateTime());
  set_HBinning(src -> get_HBinning());
  set_VBinning(src -> get_VBinning());
  set_CameraGain(src -> get_CameraGain());
  set_Triggered(src -> get_Triggered());
  set_UserComment1(src -> get_UserComment1());
  set_UserComment2(src -> get_UserComment2());
  set_UserComment3(src -> get_UserComment3());
  set_UserComment4(src -> get_UserComment4());
  set_ImageSaved(src -> get_ImageSaved());
  set_Normalization(src -> get_Normalization());
  set_Used(src -> get_Used());

  QByteArray name;

  foreach(name, src -> dynamicPropertyNames()) {
    setProperty(name.data(), src -> property(name.data()));
  }
}

void QcepImageDataBase::setTiffMetaData(TIFF *tif)
{
  TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, qPrintable(get_FileName()));
  TIFFSetField(tif, TIFFTAG_DATETIME, qPrintable(get_DateTime().toString("yyyy:MM:dd hh:mm:ss")));
}

void QcepImageDataBase::getTiffMetaData(TIFF * /*tif*/)
{
}

void QcepImageDataBase::loadMetaData()
{
//  printf("QcepImageDataBase::loadMetaData for file %s\n", qPrintable(get_FileName()));

  QTime tic;
  tic.start();

  {
    QMutexLocker lock(mutex());

    QSettings settings(get_FileName()+".metadata", QSettings::IniFormat);

    QcepProperty::readSettings(this, metaObject(), "metadata", settings);
  }
//
//  printf("QcepImageDataBase::loadMetaData for file %s took %d msec\n",  qPrintable(get_FileName()), tic.elapsed());
}

void QcepImageDataBase::saveMetaData()
{
  saveMetaData(get_FileName());
}

void QcepImageDataBase::saveMetaData(QString name)
{
//  printf("QcepImageDataBase::saveMetaData for file %s\n", qPrintable(name));

  QTime tic;
  tic.start();

//  printf("type 266 = %s\n", QMetaType::typeName(266));

  {
    QMutexLocker lock(mutex());

    QSettings settings(name+".metadata", QSettings::IniFormat);

    QcepProperty::writeSettings(this, &staticMetaObject/*metaObject()*/, "metadata", settings);

    settings.beginWriteArray("normalization");
    QcepDoubleList norm = get_Normalization();

    for (int i=0; i<norm.length(); i++) {
      settings.setArrayIndex(i);
      settings.setValue("val",norm[i]);
    }
    settings.endArray();
  }
//
//  printf("QcepImageDataBase::saveMetaData for file %s took %d msec\n",  qPrintable(name), tic.elapsed());
}

void QcepImageDataBase::setDefaultFileName(QString path)
{
  set_FileName(path);
  set_Title(QFileInfo(path).fileName());
}
